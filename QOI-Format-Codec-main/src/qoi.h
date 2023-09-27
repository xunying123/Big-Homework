#ifndef QOI_FORMAT_CODEC_QOI_H_
#define QOI_FORMAT_CODEC_QOI_H_

#include "utils.h"

constexpr uint8_t QOI_OP_INDEX_TAG = 0x00;
constexpr uint8_t QOI_OP_DIFF_TAG = 0x40;
constexpr uint8_t QOI_OP_LUMA_TAG = 0x80;
constexpr uint8_t QOI_OP_RUN_TAG = 0xc0;
constexpr uint8_t QOI_OP_RGB_TAG = 0xfe;
constexpr uint8_t QOI_OP_RGBA_TAG = 0xff;
constexpr uint8_t QOI_PADDING[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 1u};
constexpr uint8_t QOI_MASK_2 = 0xc0;

/**
 * @brief encode the raw pixel data of an image to qoi format.
 *
 * @param[in] width image width in pixels
 * @param[in] height image height in pixels
 * @param[in] channels number of color channels, 3 = RGB, 4 = RGBA
 * @param[in] colorspace image color space, 0 = sRGB with linear alpha, 1 = all channels linear
 *
 * @return bool true if it is a valid qoi format image, false otherwise
 */
bool QoiEncode(uint32_t width, uint32_t height, uint8_t channels, uint8_t colorspace = 0);

/**
 * @brief decode the qoi format of an image to raw pixel data
 *
 * @param[out] width image width in pixels
 * @param[out] height image height in pixels
 * @param[out] channels number of color channels, 3 = RGB, 4 = RGBA
 * @param[out] colorspace image color space, 0 = sRGB with linear alpha, 1 = all channels linear
 *
 * @return bool true if it is a valid qoi format image, false otherwise
 */
bool QoiDecode(uint32_t &width, uint32_t &height, uint8_t &channels, uint8_t &colorspace);


bool QoiEncode(uint32_t width, uint32_t height, uint8_t channels, uint8_t colorspace) {

    // qoi-header part

    // write magic bytes "qoif"
    QoiWriteChar('q');
    QoiWriteChar('o');
    QoiWriteChar('i');
    QoiWriteChar('f');
    // write image width
    QoiWriteU32(width);
    // write image height
    QoiWriteU32(height);
    // write channel number
    QoiWriteU8(channels);
    // write color space specifier
    QoiWriteU8(colorspace);

    /* qoi-data part */
    int run = 0;
    int px_num = width * height;

    uint8_t history[64][4];
    memset(history, 0, sizeof(history));

    uint8_t r, g, b, a;
    a = 255u;
    uint8_t pre_r, pre_g, pre_b, pre_a;
    pre_r = 0u;
    pre_g = 0u;
    pre_b = 0u;
    pre_a = 255u;

    for (int i = 0; i < px_num; ++i) {
        r = QoiReadU8();
        g = QoiReadU8();
        b = QoiReadU8();
        if (channels == 4) a = QoiReadU8();

        int8_t dr, dg, db, da; //防止数据溢出，设立有符号变量
        uint8_t ddr, ddg, ddb, index;

        if ((r == pre_r) && (g == pre_g) && (b == pre_b) && (a == pre_a)) {
            run++;
            if (run == 62) { //若run大于62，输出的值会与QOI_OP_RGB_TAG，QOI_OP_RGBA_TAG相同
                QoiWriteU8(QOI_OP_RUN_TAG | (run - 1));
                run = 0;
            }
        } else {
            if (run != 0) {
                QoiWriteU8(QOI_OP_RUN_TAG | (run - 1));
                run = 0;
            }

            index = QoiColorHash(r, g, b, a);
            if ((history[index][0] == r) && (history[index][1] == g) && (history[index][2] == b) &&
                (history[index][3] == a)) {

                QoiWriteU8(QOI_OP_INDEX_TAG | index);
                pre_r = r;
                pre_g = g;
                pre_b = b;
                pre_a = a;
                continue;
            }

            history[index][0] = r; //重新覆盖history数组
            history[index][1] = g;
            history[index][2] = b;
            history[index][3] = a;

            dr = r - pre_r; //dr，dg等为有符号的类型，可以为负数
            dg = g - pre_g;
            db = b - pre_b;
            da = a - pre_a;

            if ((dr >= -2 && dr <= 1) && (dg >= -2 && dg <= 1) && (db >= -2 && db <= 1) && (da == 0)) {
                ddr = (2 + dr) << 4; //将负数加2，使之可以用2bite储存
                ddg = (dg + 2) << 2;
                ddb = db + 2;
                QoiWriteU8(QOI_OP_DIFF_TAG | ddr | ddg | ddb);
            } else {
                if (((dr - dg) >= -8 && (dr - dg) <= 7) && (dg >= -32 && dg <= 31) &&
                    ((db - dg) >= -8 && (db - dg) <= 7) && (da == 0)) {
                    ddr = (dr - dg + 8) << 4; //同上，将负数+32或+8，节省储存空间
                    ddb = db - dg + 8;
                    ddg = dg + 32;
                    QoiWriteU8(QOI_OP_LUMA_TAG | ddg);
                    QoiWriteU8(ddr | ddb);
                } else {
                    if (channels == 4) {
                        QoiWriteU8(QOI_OP_RGBA_TAG);
                        QoiWriteU8(r);
                        QoiWriteU8(g);
                        QoiWriteU8(b);
                        QoiWriteU8(a);
                    } else {
                        QoiWriteU8(QOI_OP_RGB_TAG);
                        QoiWriteU8(r);
                        QoiWriteU8(g);
                        QoiWriteU8(b);
                    }
                }
            }
        }

        pre_r = r;
        pre_g = g;
        pre_b = b;
        pre_a = a;
    }

    if (run != 0) { //run要归0
        QoiWriteU8(QOI_OP_RUN_TAG | (run - 1));
        run = 0;
    }
    // qoi-padding part
    for (int i = 0; i < sizeof(QOI_PADDING) / sizeof(QOI_PADDING[0]); ++i) {
        QoiWriteU8(QOI_PADDING[i]);
    }

    return true;
}

bool QoiDecode(uint32_t &width, uint32_t &height, uint8_t &channels, uint8_t &colorspace) {

    char c1 = QoiReadChar();
    char c2 = QoiReadChar();
    char c3 = QoiReadChar();
    char c4 = QoiReadChar();
    if (c1 != 'q' || c2 != 'o' || c3 != 'i' || c4 != 'f') {
        return false;
    }

    // read image width
    width = QoiReadU32();
    // read image height
    height = QoiReadU32();
    // read channel number
    channels = QoiReadU8();
    // read color space specifier
    colorspace = QoiReadU8();

    int run = 0;
    int px_num = width * height;

    uint8_t history[64][4];
    memset(history, 0, sizeof(history));

    uint8_t r, g, b, a, tag, read, index; //read为输入的值，tag为计算得到的标签，index为hash值
    uint8_t dr, dg, db;
    int8_t ddr, ddb, ddg;
    uint8_t pre_r, pre_g, pre_b, pre_a;
    pre_r = 0u;
    pre_g = 0u;
    pre_b = 0u;
    pre_a = 255u;
    a = 255u;

    for (int i = 0; i < px_num; ++i) {

        read = QoiReadU8();
        tag = read & QOI_MASK_2;

        if (tag == QOI_OP_RUN_TAG) { //read与RGB与RGBA的开头2比特相同
            if ((read == QOI_OP_RGB_TAG) || (read == QOI_OP_RGBA_TAG)) {
                r = QoiReadU8();
                g = QoiReadU8();
                b = QoiReadU8();
                if (channels == 4) a = QoiReadU8();
            } else {
                run = read - tag + 1;
                r = pre_r;
                g = pre_g;
                b = pre_b;
                a = pre_a;
                index = QoiColorHash(r, g, b, a);
                history[index][0] = r;
                history[index][1] = g;
                history[index][2] = b;
                history[index][3] = a;
                i = run + i - 1; //run相当于跳过，i也要随之改变
                for (int j = 1; j <= run; j++) {
                    QoiWriteU8(r);
                    QoiWriteU8(g);
                    QoiWriteU8(b);
                    if (channels == 4) QoiWriteU8(a);
                }
                continue;
            }
        }

        if (tag == QOI_OP_INDEX_TAG) {
            index = read - tag;
            r = history[index][0];
            g = history[index][1];
            b = history[index][2];
            a = history[index][3];
        }

        if (tag == QOI_OP_DIFF_TAG) {
            db = read & 3u;
            dg = (read & 12u) >> 2;
            dr = (read & 48u) >> 4;
            r = dr + pre_r - 2;
            g = dg + pre_g - 2;
            b = db + pre_b - 2;
        }

        if (tag == QOI_OP_LUMA_TAG) { //为了防止数据范围溢出，定义int8_t变量
            dg = read & 63u;
            ddg = dg - 32u;
            read = QoiReadU8();
            db = read & 15u;
            dr = read >> 4;
            ddr = dr - 8u;
            r = pre_r + ddr + ddg;
            ddb = db - 8u;
            b = pre_b + ddb + ddg;
            g = pre_g + ddg;
        }

        index = QoiColorHash(r, g, b, a);
        history[index][0] = r;
        history[index][1] = g;
        history[index][2] = b;
        history[index][3] = a;

        QoiWriteU8(r);
        QoiWriteU8(g);
        QoiWriteU8(b);
        if (channels == 4) QoiWriteU8(a);

        pre_r = r;
        pre_g = g;
        pre_b = b;
        pre_a = a;
    }

    bool valid = true;
    for (int i = 0; i < sizeof(QOI_PADDING) / sizeof(QOI_PADDING[0]); ++i) {
        if (QoiReadU8() != QOI_PADDING[i]) valid = false;
    }

    return valid;
}

#endif // QOI_FORMAT_CODEC_QOI_H_