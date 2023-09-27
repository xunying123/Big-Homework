//
// Created by xun_y on 2023/6/25.
//

#ifndef UNTITLED1_MEMORY_H
#define UNTITLED1_MEMORY_H

#include "order.h"

class memory {
public:
    memory() {
        initial();
    }

    u32 readone(u32 loca) {
        u32 temp = memory_[loca];
        return temp;
    }

    u32 readtwo(u32 loca) {
        u32 temp = memory_[loca + 1];
        temp = temp << 8;
        temp = memory_[loca];
        return temp;
    }

    u32 readfour(u32 loca) {
        u32 temp = memory_[loca + 3];
        temp = temp << 8;
        temp += memory_[loca + 2];
        temp = temp << 8;
        temp += memory_[loca + 1];
        temp = temp << 8;
        temp += memory_[loca];
        return temp;
    }

    void write(u32 loca, u32 data) {
        memory_[loca] = (data << 24) >> 24;
        memory_[loca + 1] = (data << 16) >> 24;
        memory_[loca + 2] = (data << 8) >> 24;
        memory_[loca+3] = data >> 24;
    }

    u8 memory_[300000];
private:

    void initial() {
      //  freopen("in", "r", stdin);
     //   freopen("out", "w", stdout);
        std::string in_;
        int head_ = 0;
        while (std::cin >> in_) {
            if (in_[0] == '@') {
                head_ = 0;
                for (int i = 1; i < in_.length(); i++) {
                    int tt;
                    switch (in_[i]) {
                        case '0': {
                            tt = 0;
                            break;
                        }
                        case '1': {
                            tt = 1;
                            break;
                        }
                        case '2': {
                            tt = 2;
                            break;
                        }
                        case '3': {
                            tt = 3;
                            break;
                        }
                        case '4': {
                            tt = 4;
                            break;
                        }
                        case '5': {
                            tt = 5;
                            break;
                        }
                        case '6': {
                            tt = 6;
                            break;
                        }
                        case '7': {
                            tt = 7;
                            break;
                        }
                        case '8': {
                            tt = 8;
                            break;
                        }
                        case '9': {
                            tt = 9;
                            break;
                        }
                        case 'A': {
                            tt = 10;
                            break;
                        }
                        case 'B': {
                            tt = 11;
                            break;
                        }
                        case 'C': {
                            tt = 12;
                            break;
                        }
                        case 'D': {
                            tt = 13;
                            break;
                        }
                        case 'E': {
                            tt = 14;
                            break;
                        }
                        case 'F': {
                            tt = 15;
                            break;
                        }
                    }
                    head_ = head_ * 16 + tt;
                }
            } else {
                memory_[head_] = 0;
                for (int kk = 0; kk < 2; kk++) {
                    switch (in_[kk]) {
                        case '0': {
                            memory_[head_] *= 16;
                            memory_[head_] += 0;
                            break;
                        }
                        case '1': {
                            memory_[head_] *= 16;
                            memory_[head_] += 1;
                            break;
                        }
                        case '2': {
                            memory_[head_] *= 16;
                            memory_[head_] += 2;
                            break;
                        }
                        case '3': {
                            memory_[head_] *= 16;
                            memory_[head_] += 3;
                            break;
                        }
                        case '4': {
                            memory_[head_] *= 16;
                            memory_[head_] += 4;
                            break;
                        }
                        case '5': {
                            memory_[head_] *= 16;
                            memory_[head_] += 5;
                            break;
                        }
                        case '6': {
                            memory_[head_] *= 16;
                            memory_[head_] += 6;
                            break;
                        }
                        case '7': {
                            memory_[head_] *= 16;
                            memory_[head_] += 7;
                            break;
                        }
                        case '8': {
                            memory_[head_] *= 16;
                            memory_[head_] += 8;
                            break;
                        }
                        case '9': {
                            memory_[head_] *= 16;
                            memory_[head_] += 9;
                            break;
                        }
                        case 'A': {
                            memory_[head_] *= 16;
                            memory_[head_] += 10;
                            break;
                        }
                        case 'B': {
                            memory_[head_] *= 16;
                            memory_[head_] += 11;
                            break;
                        }
                        case 'C': {
                            memory_[head_] *= 16;
                            memory_[head_] += 12;
                            break;
                        }
                        case 'D': {
                            memory_[head_] *= 16;
                            memory_[head_] += 13;
                            break;
                        }
                        case 'E': {
                            memory_[head_] *= 16;
                            memory_[head_] += 14;
                            break;
                        }
                        case 'F': {
                            memory_[head_] *= 16;
                            memory_[head_] += 15;
                            break;
                        }
                    }
                }
                head_++;
            }
        }
    }


};

memory mem_;


#endif //UNTITLED1_MEMORY_H
