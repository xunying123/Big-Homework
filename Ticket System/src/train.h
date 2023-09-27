
#ifndef TICKET_SYSTEM_TRAIN_H
#define TICKET_SYSTEM_TRAIN_H

#include <cstring>
#include "bpt.h"
#include "tool.h"

struct stations {
    char train_id[21];
    int leave_time;
    int arrive_time;
    int price;
    int account;

    friend bool operator==(const stations &a, const stations &b) {
        return a.account == b.account && a.leave_time == b.leave_time && a.arrive_time == b.arrive_time &&
               a.price == b.price && strcmp(a.train_id, b.train_id) == 0;
    }

    friend bool operator<(const stations &a, const stations &b) {
        if (strcmp(a.train_id, b.train_id) == 0) {
            return a.account < b.account;
        }
        return strcmp(a.train_id, b.train_id) < 0;
    }

};

struct tickets {
    int num[100];

    friend bool operator==(const tickets &a, const tickets &b) {
        for (int i = 0; i < 100; i++) {
            if (a.num[i] != b.num[i]) return false;
        }
        return true;
    }

    friend bool operator<(const tickets &a, const tickets &b) {
        return a.num[0] < b.num[0];
    }
};

struct _stations {
    char station_name[31];

    _stations()=default;

    _stations(const _stations &other) {
        strcpy(station_name,other.station_name);
    }

    _stations& operator=(const _stations &other) {
        if(this==&other) return *this;
        strcpy(station_name,other.station_name);
        return *this;
    }
};

struct trains {
    int station_number;
    _stations station[100];
    int seat;
    int price[100];
    int day_time;
    char type;
    int leave_time[100];
    int arrive_time[100];
    bool status;

    friend bool operator==(const trains &a, const trains &b) {
        return a.station_number == b.station_number;
    }

    friend bool operator<(const trains &a, const trains &b) {
        return a.station_number < b.station_number;
    }

};

Multi_BPT<m_string<31>, stations> station_data("station");
BPT<m_string<25>, tickets> ticket_data("ticket");
BPT<m_string<21>, trains> train_data("train");

void add_train(char *order) {
    int i = 0, count = 0;
    std::string train_id;
    trains temp;
    int t[100], s[100];
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'i': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            train_id += order[i];
                        }
                        i--;
                        break;
                    }
                    case 'n': {
                        i += 4;
                        temp.station_number = 0;
                        while (order[i] != ' ' && order[i] != '\0') {
                            temp.station_number *= 10;
                            temp.station_number += order[i++] - '0';
                        }
                        i--;
                        break;
                    }
                    case 'm': {
                        i += 4;
                        temp.seat = 0;
                        while (order[i] != ' ' && order[i] != '\0') {
                            temp.seat *= 10;
                            temp.seat += order[i++] - '0';
                        }
                        i--;
                        break;
                    }
                    case 's': {
                        i += 4;
                        int j = 0, c = 0;
                        for (; i <= LENGTH && order[i] != '\0' && order[i] != ' '; i++) {
                            if (order[i] == '|') {
                                temp.station[j++].station_name[c] = '\0';
                                c = 0;
                                std::string sss = temp.station[j - 1].station_name;
                                continue;
                            }
                            temp.station[j].station_name[c++] = order[i];
                        }
                        temp.station[j++].station_name[c] = '\0';
                        i--;
                        break;
                    }
                    case 'p': {
                        i += 4;
                        int j = 0;
                        temp.price[0] = 0;
                        for (; i <= LENGTH && order[i] != '\0' && order[i] != ' '; i++) {
                            if (order[i] == '|') {
                                j++;
                                if (j != 100) temp.price[j] = 0;
                                continue;
                            }
                            temp.price[j] *= 10;
                            temp.price[j] += order[i] - '0';
                        }
                        i--;
                        break;
                    }
                    case 'x': {
                        i += 4;
                        temp.leave_time[0] = 0;
                        temp.leave_time[0] += order[i++] - '0';
                        temp.leave_time[0] *= 10;
                        temp.leave_time[0] += order[i++] - '0';
                        i++;
                        temp.leave_time[0] *= 10;
                        temp.leave_time[0] += order[i++] - '0';
                        temp.leave_time[0] *= 10;
                        temp.leave_time[0] += order[i++] - '0';
                        i--;
                        break;
                    }
                    case 't': {
                        i += 4;
                        int j = 0;
                        t[0] = 0;
                        for (; i <= LENGTH && order[i] != '\0' && order[i] != ' '; i++) {
                            if (order[i] == '|') {
                                j++;
                                if (j < 100) t[j] = 0;
                                continue;
                            }
                            t[j] *= 10;
                            t[j] += order[i] - '0';
                        }
                        i--;
                        break;
                    }
                    case 'o': {
                        i += 4;
                        if (order[i] == '_') break;
                        int j = 0;
                        s[0] = 0;
                        for (; i <= LENGTH && order[i] != '\0' && order[i] != ' '; i++) {
                            if (order[i] == '|') {
                                j++;
                                if (j < 100) s[j] = 0;
                                continue;
                            }
                            s[j] *= 10;
                            s[j] += order[i] - '0';
                        }
                        i--;
                        break;
                    }
                    case 'd': {
                        i += 4;
                        temp.day_time = 0;
                        temp.day_time += order[i++] - '0';
                        temp.day_time *= 10;
                        temp.day_time += order[i++] - '0';
                        i++;
                        temp.day_time *= 10;
                        temp.day_time += order[i++] - '0';
                        temp.day_time *= 10;
                        temp.day_time += order[i++] - '0';
                        i++;
                        temp.day_time *= 10;
                        temp.day_time += order[i++] - '0';
                        temp.day_time *= 10;
                        temp.day_time += order[i++] - '0';
                        i++;
                        temp.day_time *= 10;
                        temp.day_time += order[i++] - '0';
                        temp.day_time *= 10;
                        temp.day_time += order[i++] - '0';
                        i--;
                        break;
                    }
                    case 'y': {
                        i += 4;
                        temp.type = order[i];
                        break;
                    }

                }
            }
        }
    }
    temp.status = false;
    temp.arrive_time[0] = -1;
    for (int ii = 1; ii < temp.station_number; ii++) {
        int d1, m1, f1;
        d1 = temp.leave_time[ii - 1] / 10000;
        m1 = temp.leave_time[ii - 1] / 100 % 100;
        f1 = temp.leave_time[ii - 1] % 100 + t[ii - 1];
        if (f1 >= 60) {
            m1 += f1 / 60;
            f1 %= 60;
        }
        if (m1 >= 24) {
            d1 += m1 / 24;
            m1 %= 24;
        }
        temp.arrive_time[ii] = d1 * 10000 + m1 * 100 + f1;
        d1 = temp.arrive_time[ii] / 10000;
        m1 = temp.arrive_time[ii] / 100 % 100;
        f1 = temp.arrive_time[ii] % 100 + s[ii - 1];
        if (f1 >= 60) {
            m1 += f1 / 60;
            f1 %= 60;
        }
        if (m1 >= 24) {
            d1 += m1 / 24;
            m1 %= 24;
        }
        temp.leave_time[ii] = d1 * 10000 + m1 * 100 + f1;
    }
    temp.leave_time[temp.station_number - 1] = -1;
    temp.price[temp.station_number - 1] = -1;
    auto finding=train_data.readonly(train_id);
    if (finding== nullptr) {
        train_data.insert(train_id, temp);
        std::cout << 0 << '\n';
        return;
    }
    std::cout << "-1" << '\n';
}

void delete_train(char *order) {
    int i = 0, count = 0;
    std::string train_id;
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'i': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            train_id += order[i];
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    auto finding=train_data.readonly(train_id);
    if ((finding!= nullptr) && (!finding[0].status)) {
        train_data.erase(train_id);
        std::cout << 0 << '\n';
        return;
    }
    std::cout << "-1" << '\n';
}

void release_train(char *order) {
    int i = 0, count = 0;
    std::string train_id;
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'i': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            train_id += order[i];
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    auto finding=train_data.readwrite(train_id);
    if ((finding!= nullptr) && (!finding[0].status)) {
        finding->status = true;
        std::cout << 0 << '\n';
        for (int j = 0; j < finding[0].station_number; j++) {
            m_string<31> abc;
            strcpy(abc._str, finding[0].station[j].station_name);
            std::string sss = finding[0].station[j].station_name;
            stations bcd;
            m_string<21> ttt(train_id);
            strcpy(bcd.train_id, ttt._str);
            bcd.leave_time = finding[0].leave_time[j];
            bcd.arrive_time = finding[0].arrive_time[j];
            bcd.price = finding[0].price[j];
            bcd.account = j;
            station_data.insert(abc, bcd);
        }
        int m1, d1, m2, d2;
        m1 = finding[0].day_time / 1000000;
        d1 = finding[0].day_time / 10000 % 100;
        m2 = finding[0].day_time / 100 % 100;
        d2 = finding[0].day_time % 100;
        tickets aaa;
        int ddd = m1 * 100 + d1;
        for (int j = 0; j < finding[0].station_number; j++) {
            aaa.num[j] = finding[0].seat;
        }
        std::string _tr = train_id;
        _tr += (char) (ddd / 1000 + '0');
        _tr += (char) (ddd / 100 % 10 + '0');
        _tr += (char) (ddd / 10 % 10 + '0');
        _tr += (char) (ddd % 10 + '0');
        ticket_data.insert(_tr, aaa);
        while ((d1 != d2) || (m1 != m2)) {
            d1++;
            ddd = calendar(m1 * 100 + d1);
            d1 = ddd % 100;
            m1 = ddd / 100;
            _tr = train_id;
            _tr += (char) (ddd / 1000 + '0');
            _tr += (char) (ddd / 100 % 10 + '0');
            _tr += (char) (ddd / 10 % 10 + '0');
            _tr += (char) (ddd % 10 + '0');
            ticket_data.insert(_tr, aaa);
        }
        return;
    }
    std::cout << "-1" << '\n';
}

void query_train(char *order) {
    int i = 0, count = 0;
    std::string train_id;
    int ddd = 0;
    for (; i <= LENGTH && order[i] != '\0'; i++) {
        if (order[i] == ' ') {
            count++;
            if (count != 1) {
                switch (order[i + 2]) {
                    case 'i': {
                        i += 4;
                        for (; order[i] != '\0' && order[i] != ' '; i++) {
                            train_id += order[i];
                        }
                        i--;
                        break;
                    }
                    case 'd': {
                        i += 4;
                        ddd = 0;
                        ddd += order[i++] - '0';
                        ddd *= 10;
                        ddd += order[i++] - '0';
                        i++;
                        ddd *= 10;
                        ddd += order[i++] - '0';
                        ddd *= 10;
                        ddd += order[i++] - '0';
                        i--;
                        break;
                    }
                }
            }
        }
    }
    auto finding1=train_data.readonly(train_id);
    if (finding1== nullptr) {
        std::cout << "-1" << '\n';
        return;
    }
    if (ddd < finding1->day_time / 10000 || ddd > finding1->day_time % 10000) {
        std::cout << "-1" << '\n';
        return;
    }
    std::cout << train_id << " " << finding1->type << '\n';
    std::string _tr = train_id;
    _tr += (char) (ddd / 1000 + '0');
    _tr += (char) (ddd / 100 % 10 + '0');
    _tr += (char) (ddd / 10 % 10 + '0');
    _tr += (char) (ddd % 10 + '0');
    auto finding2=ticket_data.readonly(_tr);
    int pp = 0;
    for (int j = 0; j < finding1->station_number; j++) {
        std::cout << finding1->station[j].station_name << " ";
        if (j == 0) std::cout << "xx-xx ";
        else if (finding1->arrive_time[j] >= 10000) {
            int tem = ddd + finding1->arrive_time[j] / 10000;
            tem = calendar(tem);
            out(tem / 100);
            std::cout << "-";
            out(tem % 100);
            std::cout << " ";
        } else {
            out(ddd / 100);
            std::cout << "-";
            out(ddd % 100);
            std::cout << " ";
        }
        if (j == 0) std::cout << "xx:xx -> ";
        else {
            out(finding1->arrive_time[j] / 100 % 100);
            std::cout << ":";
            out(finding1->arrive_time[j] % 100);
            std::cout << " -> ";
        }
        if (j == finding1->station_number - 1) std::cout << "xx-xx ";
        else if (finding1->leave_time[j] >= 10000) {
            int tem = ddd + finding1->leave_time[j] / 10000;
            tem = calendar(tem);
            out(tem / 100);
            std::cout << "-";
            out(tem % 100);
            std::cout << " ";
        } else {
            out(ddd / 100);
            std::cout << "-";
            out(ddd % 100);
            std::cout << " ";
        }
        if (j == finding1->station_number - 1) std::cout << "xx:xx ";
        else {
            out(finding1->leave_time[j] / 100 % 100);
            std::cout << ":";
            out(finding1->leave_time[j] % 100);
            std::cout << " ";
        }
        if (j != 0) pp += finding1->price[j - 1];
        std::cout << pp << " ";
        if (j == finding1->station_number - 1) std::cout << "x" << '\n';
        else if (finding1->status) {
            std::cout << finding2->num[j] << '\n';
        } else {
            std::cout<<finding1->seat<<'\n';
        }
    }
}

#endif //TICKET_SYSTEM_TRAIN_H
