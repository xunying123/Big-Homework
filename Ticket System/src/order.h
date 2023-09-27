
#ifndef TICKET_SYSTEM_ORDER_H
#define TICKET_SYSTEM_ORDER_H

#include <cstring>
#include "bpt.h"
#include "tool.h"
#include "priority_queue.h"
#include "hash_map.h"

struct orders {
    int time_set;
    char train_id[21];
    char from[31];
    int leave_time; //八位
    char to[31];
    int arrive_time; //八位
    int status;
    int price;
    int num;
    int day;

    friend bool operator==(const orders &a, const orders &b) {
        return a.time_set == b.time_set && strcmp(a.train_id, b.train_id) == 0 && strcmp(a.from, b.from) == 0 &&
               strcmp(a.to, b.to) == 0 && a.leave_time == b.leave_time && a.arrive_time == b.arrive_time &&
               a.status == b.status && a.price == b.price && a.num == b.num && a.day == b.day;
    }

    friend bool operator<(const orders &a, const orders &b) {
        return a.time_set > b.time_set;
    }

};

struct waitings {
    char name[21];
    int time_set;
    char from[31];
    char to[31];
    int num;
    int l;
    int r;
    int p;
    int ll;
    int rr;
    long index;

    friend bool operator==(const waitings &a, const waitings &b) {
        return a.time_set == b.time_set && strcmp(a.from, b.from) == 0 &&
               strcmp(a.to, b.to) == 0 && a.num == b.num && a.index==b.index;
    }

    friend bool operator<(const waitings &a, const waitings &b) {
        return a.time_set < b.time_set;
    }
};

class time1 {
public:
    int t;
    char c[21];
    int l;
    int r;
    int p;
    int s;

    friend bool operator<(const time1 &a, const time1 &b) {
        if (a.t == b.t) {
            return (strcmp(a.c, b.c) > 0);
        }
        return a.t > b.t;
    }
};

struct price1 {
    int p;
    char c[21];
    int l;
    int r;
    int s;

    friend bool operator<(const price1 &a, const price1 &b) {
        if (a.p == b.p) {
            return !(strcmp(a.c, b.c) < 0);
        }
        return a.p > b.p;
    }
};

struct price2 {
    int p;
    int t;
    char c1[21];
    char c2[21];
    int t1;
    int t2;
    int a1;
    int a2;
    char s[31];
    int tt1;
    int tt2;
    int p1;
    int p2;

    friend bool operator<(const price2 &a, const price2 &b) {
        if (a.p != b.p) return a.p > b.p;
        if (a.t != b.t) return a.t > b.t;
        int k = strcmp(a.c1, b.c1);
        if (k != 0) return k > 0;
        return !(strcmp(a.c2, b.c2) < 0);
    }
};

struct time2 {
    int p;
    int t;
    char c1[21];
    char c2[21];
    int t1;
    int t2;
    int a1;
    int a2;
    char s[31];
    int tt1;
    int tt2;
    int p1;
    int p2;

    friend bool operator<(const time2 &a, const time2 &b) {
        if (a.t != b.t) return a.t > b.t;
        if (a.p != b.p) return a.p > b.p;
        int k = !(strcmp(a.c1, b.c1));
        if (k != 0) return k > 0;
        return !(strcmp(a.c2, b.c2) < 0);
    }
};

struct node {
    char c[31];
    int a;
    int aa;
};

Datafile<orders> order_index("index");
Multi_BPT<m_string<21>, long> order_data("order");
Multi_BPT<m_string<25>, waitings> waiting_data("waiting");

void query_ticket(char *ordering)  {
    int i = 0, count = 0;
    int dd;
    bool con = true;
    std::string train_id;
    m_string<31> s1, t1;
    for (; i <= LENGTH && ordering[i] != '\0'; i++) {
        if (ordering[i] == ' ') {
            count++;
            if (count != 1) {
                switch (ordering[i + 2]) {
                    case 's': {
                        i += 4;
                        int c = 0;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            s1._str[c++] = ordering[i];
                        }
                        s1._str[c] = '\0';
                        i--;
                        break;
                    }
                    case 'p': {
                        i += 4;
                        if (ordering[i] == 't') con = true;
                        else con = false;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                        }
                        i--;
                        break;
                    }
                    case 't': {
                        i += 4;
                        int c = 0;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            t1._str[c++] = ordering[i];
                        }
                        t1._str[c] = '\0';
                        i--;
                        break;
                    }
                    case 'd': {
                        i += 4;
                        dd = 0;
                        dd += ordering[i++] - '0';
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        i++;
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        i--;
                        break;
                    }
                }
            }
        }
    }
    vector<node> tra;
    vector<stations> finding1, finding2;
    station_data.find(s1, finding1);
    station_data.find(t1, finding2);
    auto iter1 = finding1.begin();
    auto iter2 = finding2.begin();
    for (; iter1 != finding1.end() && iter2 != finding2.end();) {
        int boo = strcmp((*iter1).train_id, (*iter2).train_id);
        if (boo == 0) {
            if ((*iter1).leave_time >= (*iter2).arrive_time) {
                *iter1++;
                *iter2++;
                continue;
            }
            if ((*iter1).leave_time == -1) {
                *iter1++;
                *iter2++;
                continue;
            }
            node abc;
            strcpy(abc.c, (*iter1).train_id);
            abc.a = (*iter1).account;
            abc.aa = (*iter2).account;
            tra.push_back(abc);
            iter1++;
            iter2++;
        } else if (boo < 0) {
            iter1++;
        } else {
            iter2++;
        }
    }
    if (con) {
        priority_queue<time1> p;
        for (auto ite = tra.begin(); ite != tra.end(); ite++) {
            auto tt = train_data.readonly((*ite).c);
            if (!tt->status) continue;
            int ll, rr, d;
            d = tt->leave_time[(*ite).a] / 10000;
            ll = d + tt->day_time % 10000;
            rr = d + tt->day_time / 10000;
            ll = calendar(ll);
            rr = calendar(rr);
            if (ll >= dd && rr <= dd) {
                time1 mmm;
                mmm.t = cha(tt->leave_time[(*ite).a], tt->arrive_time[(*ite).aa]);
                strcpy(mmm.c, (*ite).c);
                mmm.l = tt->leave_time[(*ite).a];
                mmm.r = tt->arrive_time[(*ite).aa];
                mmm.p = 0;
                for (int j = (*ite).a; j < (*ite).aa; j++) {
                    mmm.p += tt->price[j];
                }
                std::string ss = mmm.c;
                int chh = back(dd, d);
                ss += (char) (chh / 1000 + '0');
                ss += (char) (chh / 100 % 10 + '0');
                ss += (char) (chh / 10 % 10 + '0');
                ss += (char) (chh % 10 + '0');
                int tic = 100000000;
                auto ff = ticket_data.readonly(ss);
                for (int j = (*ite).a; j < (*ite).aa; j++) {
                    if (ff->num[j] < tic) tic = ff->num[j];
                }
                mmm.s = tic;
                p.push(mmm);
            }
        }
        std::cout << p.size() << '\n';
        while (!p.empty()) {
            time1 mmm = p.top();
            std::cout << mmm.c << " " << s1._str << " ";
            out(dd / 100);
            std::cout << "-";
            out(dd % 100);
            std::cout << " ";
            out(mmm.l / 100 % 100);
            std::cout << ":";
            out(mmm.l % 100);
            std::cout << " -> " << t1._str << " ";
            int kk = calendar(dd + mmm.r / 10000 - mmm.l / 10000);
            out(kk / 100);
            std::cout << "-";
            out(kk % 100);
            std::cout << " ";
            out(mmm.r / 100 % 100);
            std::cout << ":";
            out(mmm.r % 100);
            std::cout << " " << mmm.p << " " << mmm.s << '\n';
            p.pop();
        }
    } else {
        priority_queue<price1> p;
        for (auto ite = tra.begin(); ite != tra.end(); ite++) {
            auto tt = train_data.readonly((*ite).c);
            int ll, rr, d;
            d = tt->leave_time[(*ite).a] / 10000;
            ll = d + tt->day_time % 10000;
            rr = d + tt->day_time / 10000;
            ll = calendar(ll);
            rr = calendar(rr);
            if (ll >= dd && rr <= dd) {
                price1 mmm;
                strcpy(mmm.c, (*ite).c);
                mmm.l = tt->leave_time[(*ite).a];
                mmm.r = tt->arrive_time[(*ite).aa];
                mmm.p = 0;
                for (int j = (*ite).a; j < (*ite).aa; j++) {
                    mmm.p += tt->price[j];
                }
                std::string ss = mmm.c;
                int chh = back(dd, d);
                ss += (char) (chh / 1000 + '0');
                ss += (char) (chh / 100 % 10 + '0');
                ss += (char) (chh / 10 % 10 + '0');
                ss += (char) (chh % 10 + '0');
                int tic = 100000000;
                auto ff = ticket_data.readonly(ss);
                for (int j = (*ite).a; j < (*ite).aa; j++) {
                    if (ff->num[j] < tic) tic = ff->num[j];
                }
                mmm.s = tic;
                p.push(mmm);
            }
        }
        std::cout << p.size() << '\n';
        while (!p.empty()) {
            price1 mmm = p.top();
            std::cout << mmm.c << " " << s1._str << " ";
            out(dd / 100);
            std::cout << "-";
            out(dd % 100);
            std::cout << " ";
            out(mmm.l / 100 % 100);
            std::cout << ":";
            out(mmm.l % 100);
            std::cout << " -> " << t1._str << " ";
            int kk = calendar(dd + mmm.r / 10000 - mmm.l / 10000);
            out(kk / 100);
            std::cout << "-";
            out(kk % 100);
            std::cout << " ";
            out(mmm.r / 100 % 100);
            std::cout << ":";
            out(mmm.r % 100);
            std::cout << " " << mmm.p << " " << mmm.s << '\n';
            p.pop();
        }
    }
}

void query_transfer(char *ordering) {
    int i = 0, count = 0;
    int dd;
    bool con = true;
    m_string<31> s1, t1;
    for (; i <= LENGTH && ordering[i] != '\0'; i++) {
        if (ordering[i] == ' ') {
            count++;
            if (count != 1) {
                switch (ordering[i + 2]) {
                    case 's': {
                        i += 4;
                        int c = 0;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            s1._str[c++] = ordering[i];
                        }
                        s1._str[c] = '\0';
                        i--;
                        break;
                    }
                    case 'p': {
                        i += 4;
                        if (ordering[i] == 't') con = true;
                        else con = false;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                        }
                        i--;
                        break;
                    }
                    case 't': {
                        i += 4;
                        int c = 0;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            t1._str[c++] = ordering[i];
                        }
                        t1._str[c] = '\0';
                        i--;
                        break;
                    }
                    case 'd': {
                        i += 4;
                        dd = 0;
                        dd += ordering[i++] - '0';
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        i++;
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        i--;
                        break;
                    }
                }
            }
        }
    }
    vector<node> tra;
    vector<stations> finding1, finding2;
    vector<trains> tt;
    station_data.find(s1, finding1);
    station_data.find(t1, finding2);
    auto iter1 = finding1.begin();
    auto iter2 = finding2.begin();
    priority_queue<price2> p2;
    priority_queue<time2> t2;
    for (; iter1 != finding1.end(); iter1++) {
        auto finding3=train_data.readonly((*iter1).train_id);
        int l3, r3;
        l3 = finding3->leave_time[(*iter1).account] / 10000 + finding3->day_time / 10000;
        l3 = calendar(l3);
        r3 = finding3->leave_time[(*iter1).account] / 10000 + finding3->day_time % 10000;
        r3 = calendar(r3);
        if (dd < l3 || dd > r3) continue;
        for (int j = (*iter1).account + 1; j < finding3->station_number; j++) {
            if ((strcmp(finding3->station[j].station_name, t1._str) == 0)) continue;
            vector<stations> finding4;
            m_string<31> aaa;
            strcpy(aaa._str,finding3->station[j].station_name);
            station_data.find(aaa, finding4);
            auto iter4 = finding4.begin();
            for (; iter4 != finding4.end(); iter4++) {
                for (iter2 = finding2.begin(); iter2 != finding2.end(); iter2++) {
                    if (strcmp((*iter2).train_id, (*iter4).train_id) == 0) {
                        if (strcmp((*iter4).train_id, (*iter1).train_id) == 0) continue;
                        if ((*iter4).account >= (*iter2).account) continue;
                        int tim = 0;
                        int pric = 0, pric1 = 0, pric2 = 0;
                        for (int jj = (*iter1).account; jj < j; jj++) {
                            pric += finding3->price[jj];
                            pric1 += finding3->price[jj];
                        }
                        auto finding5=train_data.readonly((*iter4).train_id);
                   //     std::cout<<j<<" "<<(*iter1).train_id<<" "<<finding3->station[j].station_name<<" "<<(*iter4).train_id<<'\n';
                        int l2, r2;
                        l2 = finding5->leave_time[(*iter4).account] / 10000 + finding5->day_time / 10000;
                        l2 = calendar(l2);
                        l2 = l2 * 10000 + finding5->leave_time[(*iter4).account] % 10000;

                        r2 = finding5->leave_time[(*iter4).account] / 10000 + finding5->day_time % 10000;
                        r2 = calendar(r2);
                        r2 = r2 * 10000 + finding5->leave_time[(*iter4).account] % 10000;

                        int timing;
                        timing = dd - finding3->leave_time[(*iter1).account] / 10000 +
                                 finding3->arrive_time[j] / 10000;
                        timing = calendar(timing);
                        timing = timing * 10000 + finding3->arrive_time[j] % 10000;
                        if (r2 < timing) continue;
                        if (l2 <= timing) {
                            while (l2 < timing) {
                                int temmm;
                                temmm = l2 % 10000;
                                l2 = l2 / 10000;
                                l2++;
                                l2 = calendar(l2);
                                l2 = l2 * 10000 + temmm;

                            }
                            tim += cha(timing, l2);
                        } else {
                            tim += cha(timing, l2);
                        }
                        for (int jj = (*iter4).account; jj < (*iter2).account; jj++) {
                            pric += finding5->price[jj];
                            pric2 += finding5->price[jj];
                        }
                        tim += cha(finding3->leave_time[(*iter1).account], finding3->arrive_time[j]) +
                               cha(finding5->leave_time[(*iter4).account], finding5->arrive_time[(*iter2).account]);
                        int hh = 99999999, hhh = 99999999;
                        std::string ss1, ss2;
                        ss1 = (*iter1).train_id;
                        int fff = back(dd, finding3->leave_time[(*iter1).account] / 10000);
                        ss1 += (char) (fff / 1000 + '0');
                        ss1 += (char) (fff / 100 % 10 + '0');
                        ss1 += (char) (fff / 10 % 10 + '0');
                        ss1 += (char) (fff % 10 + '0');
                        ss2 = (*iter4).train_id;
                        fff = back(l2 / 10000, finding5->leave_time[(*iter4).account] / 10000);
                        ss2 += (char) (fff / 1000 + '0');
                        ss2 += (char) (fff / 100 % 10 + '0');
                        ss2 += (char) (fff / 10 % 10 + '0');
                        ss2 += (char) (fff % 10 + '0');
                        auto finding6=ticket_data.readonly(ss1);
                        auto finding7=ticket_data.readonly(ss2);
                        for (int h = (*iter1).account; h < j; h++) {
                            if (hh > finding6->num[h]) hh = finding6->num[h];
                        }
                        for (int h = (*iter4).account; h < (*iter2).account; h++) {
                            if (hhh > finding7->num[h]) hhh = finding7->num[h];
                        }
                        if (con) {
                            time2 temp1;
                            temp1.p = pric;
                            temp1.t = tim;
                            strcpy(temp1.c1, (*iter1).train_id);
                            strcpy(temp1.c2, (*iter4).train_id);
                            temp1.t1 = dd * 10000 + finding3->leave_time[(*iter1).account] % 10000;
                            temp1.t2 = l2;
                            temp1.a1 = dd - finding3->leave_time[(*iter1).account] / 10000 +
                                       finding3->arrive_time[j] / 10000;
                            temp1.a1 = calendar(temp1.a1);
                            temp1.a1 = temp1.a1 * 10000 + finding3->arrive_time[j] % 10000;
                            fff = calendar(fff + finding5->arrive_time[(*iter2).account] / 10000);
                            temp1.a2 = fff * 10000 + finding5->arrive_time[(*iter2).account] % 10000;
                            strcpy(temp1.s, finding3->station[j].station_name);
                            temp1.tt1 = hh;
                            temp1.tt2 = hhh;
                            temp1.p1 = pric1;
                            temp1.p2 = pric2;
                            t2.push(temp1);
                        } else {
                            price2 temp1;
                            temp1.p = pric;
                            temp1.t = tim;
                            strcpy(temp1.c1, (*iter1).train_id);
                            strcpy(temp1.c2, (*iter4).train_id);
                            strcpy(temp1.s, finding3->station[j].station_name);
                            temp1.t1 = dd * 10000 + finding3->leave_time[(*iter1).account] % 10000;
                            temp1.t2 = l2;
                            temp1.a1 = dd - finding3->leave_time[(*iter1).account] / 10000 +
                                       finding3->arrive_time[j] / 10000;
                            temp1.a1 = calendar(temp1.a1);
                            temp1.a1 = temp1.a1 * 10000 + finding3->arrive_time[j] % 10000;
                            fff = calendar(fff + finding5->arrive_time[(*iter2).account] / 10000);
                            temp1.a2 = fff * 10000 + finding5->arrive_time[(*iter2).account] % 10000;
                            temp1.tt1 = hh;
                            temp1.tt2 = hhh;
                            temp1.p1 = pric1;
                            temp1.p2 = pric2;
                            p2.push(temp1);
                        }
                    }
                }
            }
        }
    }
    if (t2.empty() && p2.empty()) {
        std::cout << 0 << '\n';
        return;
    }
    if (con) {
            time2 temp2 = t2.top();
            std::cout << temp2.c1 << " " << s1._str << " ";
            out(temp2.t1 / 1000000);
            std::cout << "-";
            out(temp2.t1 / 10000 % 100);
            std::cout << " ";
            out(temp2.t1 / 100 % 100);
            std::cout << ":";
            out(temp2.t1 % 100);
            std::cout << " -> " << temp2.s << " ";
            out(temp2.a1 / 1000000);
            std::cout << "-";
            out(temp2.a1 / 10000 % 100);
            std::cout << " ";
            out(temp2.a1 / 100 % 100);
            std::cout << ":";
            out(temp2.a1 % 100);
            std::cout << " " << temp2.p1 << " " << temp2.tt1 << '\n' << temp2.c2 << " " << temp2.s << " ";
            out(temp2.t2 / 1000000);
            std::cout << "-";
            out(temp2.t2 / 10000 % 100);
            std::cout << " ";
            out(temp2.t2 / 100 % 100);
            std::cout << ":";
            out(temp2.t2 % 100);
            std::cout << " -> " << t1._str << " ";
            out(temp2.a2 / 1000000);
            std::cout << "-";
            out(temp2.a2 / 10000 % 100);
            std::cout << " ";
            out(temp2.a2 / 100 % 100);
            std::cout << ":";
            out(temp2.a2 % 100);
            std::cout << " " << temp2.p2 << " " << temp2.tt2 << '\n';


    } else {
            price2 temp2 = p2.top();
            std::cout << temp2.c1 << " " << s1._str << " ";
            out(temp2.t1 / 1000000);
            std::cout << "-";
            out(temp2.t1 / 10000 % 100);
            std::cout << " ";
            out(temp2.t1 / 100 % 100);
            std::cout << ":";
            out(temp2.t1 % 100);
            std::cout << " -> " << temp2.s << " ";
            out(temp2.a1 / 1000000);
            std::cout << "-";
            out(temp2.a1 / 10000 % 100);
            std::cout << " ";
            out(temp2.a1 / 100 % 100);
            std::cout << ":";
            out(temp2.a1 % 100);
            std::cout << " " << temp2.p1 << " " << temp2.tt1 << '\n' << temp2.c2 << " " << temp2.s << " ";
            out(temp2.t2 / 1000000);
            std::cout << "-";
            out(temp2.t2 / 10000 % 100);
            std::cout << " ";
            out(temp2.t2 / 100 % 100);
            std::cout << ":";
            out(temp2.t2 % 100);
            std::cout << " -> " << t1._str << " ";
            out(temp2.a2 / 1000000);
            std::cout << "-";
            out(temp2.a2 / 10000 % 100);
            std::cout << " ";
            out(temp2.a2 / 100 % 100);
            std::cout << ":";
            out(temp2.a2 % 100);
            std::cout << " " << temp2.p2 << " " << temp2.tt2 << '\n';

    }
}

void buy_ticket(char *ordering) {
    std::string new_user, train_id;
    int i = 0, count = 0;
    int dd, n = 0;
    m_string<31> f1, t1;
    bool con = false;
    for (; i <= LENGTH && ordering[i] != '\0'; i++) {
        if (ordering[i] == ' ') {
            count++;
            if (count != 1) {
                switch (ordering[i + 2]) {
                    case 'u': {
                        i += 4;
                        for (; ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            new_user += ordering[i];
                        }
                        i--;
                        break;
                    }
                    case 'i': {
                        i += 4;
                        for (; ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            train_id += ordering[i];
                        }
                        i--;
                        break;
                    }
                    case 'n': {
                        i += 4;
                        while (ordering[i] != ' ' && ordering[i] != '\0') {
                            n *= 10;
                            n += ordering[i++] - '0';
                        }
                        i--;
                        break;
                    }
                    case 't': {
                        i += 4;
                        int c = 0;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            t1._str[c++] = ordering[i];
                        }
                        t1._str[c] = '\0';
                        i--;
                        break;
                    }
                    case 'd': {
                        i += 4;
                        dd = 0;
                        dd += ordering[i++] - '0';
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        i++;
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        dd *= 10;
                        dd += ordering[i++] - '0';
                        i--;
                        break;
                    }
                    case 'f': {
                        i += 4;
                        int c = 0;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            f1._str[c++] = ordering[i];
                        }
                        f1._str[c] = '\0';
                        i--;
                        break;
                    }
                    case 'q': {
                        i += 4;
                        if (ordering[i] == 't') con = true;
                        else con = false;
                        for (; i <= LENGTH && ordering[i] != '\0' && ordering[i] != ' '; i++) {
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    vector<stations> finding3, finding4;
    char s[21];
    strcpy(s, new_user.c_str());
    int pp = log_in.find(s);
    if (pp != -1) {
        auto finding2=train_data.readonly(train_id);
        if (finding2== nullptr) {
            std::cout << "-1" << '\n';
            return;
        }
        if (n > finding2->seat) {
            std::cout << "-1" << '\n';
            return;
        }
        if (!finding2->status) {
            std::cout << "-1" << '\n';
            return;
        }
        station_data.find(f1, finding3);
        station_data.find(t1, finding4);
        if ((!finding3.empty()) && (!finding4.empty())) {
            int i3 = 0, i4 = 0;
            for (; i3 < finding3.size(); i3++) {
                if (strcmp(finding3[i3].train_id, train_id.c_str()) == 0) break;
            }
            for (; i4 < finding4.size(); i4++) {
                if (strcmp(finding4[i4].train_id, train_id.c_str()) == 0) break;
            }
            if ((i3 == finding3.size()) || (i4 == finding4.size())) {
                std::cout << "-1" << '\n';
                return;
            }
            if (finding3[i3].account >= finding4[i4].account) {
                std::cout << "-1" << '\n';
                return;
            }
            int d = finding2->leave_time[finding3[i3].account] / 10000;
            std::string ss = train_id;
            int chh = back(dd, d);
            if (chh >= finding2->day_time / 10000 && chh <= finding2->day_time % 10000) {
                ss += (char) (chh / 1000 + '0');
                ss += (char) (chh / 100 % 10 + '0');
                ss += (char) (chh / 10 % 10 + '0');
                ss += (char) (chh % 10 + '0');
                auto finding5=ticket_data.readwrite(ss);
                int tic = 100000000;
                for (int j = finding3[i3].account; j < finding4[i4].account; j++) {
                    if (tic > finding5->num[j]) tic = finding5->num[j];
                }
                if (tic >= n) {
                    int p = 0;
                    for (int j = finding3[i3].account; j < finding4[i4].account; j++) {
                        p += finding2->price[j];
                    }
                    std::cout << n * p << '\n';
                    for (int j = finding3[i3].account; j < finding4[i4].account; j++) {
                        finding5->num[j] -= n;
                    }
                    orders oo;
                    oo.time_set = 0;
                    for (int ii = 1; ii < LENGTH; ii++) {
                        if (ordering[ii] == ' ') break;
                        oo.time_set *= 10;
                        oo.time_set += ordering[ii] - '0';
                    }
                    strcpy(oo.train_id, train_id.c_str());
                    strcpy(oo.from, f1._str);
                    strcpy(oo.to, t1._str);
                    int ll, rr;
                    ll = finding2->leave_time[finding3[i3].account];
                    rr = finding2->arrive_time[finding4[i4].account];
                    oo.leave_time = dd * 10000 + ll % 10000;
                    int rrr = calendar(chh + rr / 10000);
                    oo.arrive_time = rrr * 10000 + rr % 10000;
                    oo.status = 1;
                    oo.num = n;
                    oo.price = p;
                    oo.day = chh;
                    long address=order_index.new_space();
                    order_index.write(address,oo);
                    order_data.insert(new_user, -address);
                    return;
                } else if (con) {
                    std::cout << "queue" << '\n';
                    waitings ttt;
                    strcpy(ttt.from, f1._str);
                    strcpy(ttt.to, t1._str);
                    strcpy(ttt.name, new_user.c_str());
                    ttt.num = n;
                    ttt.time_set = 0;
                    for (int ii = 1; ii < LENGTH; ii++) {
                        if (ordering[ii] == ' ') break;
                        ttt.time_set *= 10;
                        ttt.time_set += ordering[ii] - '0';
                    }
                    ttt.l = finding3[i3].account;
                    ttt.r = finding4[i4].account;
                    int p = 0;
                    for (int j = finding3[i3].account; j < finding4[i4].account; j++) {
                        p += finding2->price[j];
                    }
                    ttt.p = p;
                    orders oo;
                    oo.time_set = 0;
                    for (int ii = 1; ii < LENGTH; ii++) {
                        if (ordering[ii] == ' ') break;
                        oo.time_set *= 10;
                        oo.time_set += ordering[ii] - '0';
                    }
                    strcpy(oo.train_id, train_id.c_str());
                    strcpy(oo.from, f1._str);
                    strcpy(oo.to, t1._str);
                    int ll, rr;
                    ll = finding2->leave_time[finding3[i3].account];
                    rr = finding2->arrive_time[finding4[i4].account];
                    oo.leave_time = dd * 10000 + ll % 10000;
                    int rrr = calendar(chh + rr / 10000);
                    oo.arrive_time = rrr * 10000 + rr % 10000;
                    oo.status = 0;
                    oo.num = n;
                    oo.price = p;
                    oo.day = chh;
                    long address=order_index.new_space();
                    order_index.write(address,oo);
                    order_data.insert(new_user, -address);
                    ttt.ll = oo.leave_time;
                    ttt.rr = oo.arrive_time;
                    ttt.index=-address;
                    waiting_data.insert(ss, ttt);
                    return;
                }
            }
        }
    }
    std::cout << "-1" << '\n';
}

void query_order(char *ordering) {
    std::string new_user;
    int i = 0, count = 0;
    for (; i <= LENGTH && ordering[i] != '\0'; i++) {
        if (ordering[i] == ' ') {
            count++;
            if (count != 1) {
                switch (ordering[i + 2]) {
                    case 'u': {
                        i += 4;
                        for (; ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            new_user += ordering[i];
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    char s[21];
    strcpy(s, new_user.c_str());
    int pp = log_in.find(s);
    if (pp == -1) {
        std::cout << "-1" << '\n';
        return;
    }
    vector<long> finding;
    order_data.find(new_user, finding);
    std::cout << finding.size() << '\n';
    if(finding.empty()) return ;
    for (auto iter = finding.begin(); iter != finding.end(); iter++) {
        auto oo=order_index.readonly(-(*iter));
        std::cout << "[";
        if (oo->status == 1) std::cout << "success] ";
        if (oo->status == 0) std::cout << "pending] ";
        if (oo->status == -1) std::cout << "refunded] ";
        std::cout << oo->train_id << " " << oo->from << " ";
        out(oo->leave_time / 1000000);
        std::cout << "-";
        out(oo->leave_time / 10000 % 100);
        std::cout << " ";
        out(oo->leave_time / 100 % 100);
        std::cout << ":";
        out(oo->leave_time % 100);
        std::cout << " -> " << oo->to << " ";
        out(oo->arrive_time / 1000000);
        std::cout << "-";
        out(oo->arrive_time / 10000 % 100);
        std::cout << " ";
        out(oo->arrive_time / 100 % 100);
        std::cout << ":";
        out(oo->arrive_time % 100);
        std::cout << " " << oo->price << " " << oo->num << '\n';
    }
}

void refund_ticket(char *ordering) {
    std::string new_user;
    int i = 0, count = 0;
    int n = 1;
    for (; i <= LENGTH && ordering[i] != '\0'; i++) {
        if (ordering[i] == ' ') {
            count++;
            if (count != 1) {
                switch (ordering[i + 2]) {
                    case 'u': {
                        i += 4;
                        for (; ordering[i] != '\0' && ordering[i] != ' '; i++) {
                            new_user += ordering[i];
                        }
                        i--;
                        break;
                    }
                    case 'n': {
                        i += 4;
                        n = 0;
                        while (ordering[i] != ' ' && ordering[i] != '\0') {
                            n *= 10;
                            n += ordering[i++] - '0';
                        }
                        i--;
                        break;
                    }
                }
            }
        }
    }
    char s[21];
    strcpy(s, new_user.c_str());
    int pp = log_in.find(s);
    if (pp == -1) {
        std::cout << "-1" << '\n';
        return;
    }
    vector<long> finding;
    order_data.find(new_user, finding);
    if (finding.empty()) {
        std::cout << "-1" << '\n';
        return;
    }
    if (finding.size() < n) {
        std::cout << "-1" << '\n';
        return;
    }
    auto oo=order_index.readwrite(-finding[n-1]);
    if (oo->status == -1) {
        std::cout << "-1" << '\n';
        return;
    }
    vector<waitings> finding1;
    int i2 = 0, i3 = 0;
    std::string ss = oo->train_id;
    ss += (char) (oo->day / 1000 + '0');
    ss += (char) (oo->day / 100 % 10 + '0');
    ss += (char) (oo->day / 10 % 10 + '0');
    ss += (char) (oo->day % 10 + '0');
    waiting_data.find(ss, finding1);
    if (oo->status == 0) {
        oo->status = -1;
        auto iter = finding1.begin();
        for (; iter != finding1.end(); iter++) {
            if ((*iter).time_set == oo->time_set) break;
        }
        waiting_data.erase(ss, (*iter));
        std::cout << 0 << '\n';
        return;
    }
    oo->status = -1;
    vector<stations> finding2, finding3;
    station_data.find(oo->from, finding2);
    station_data.find(oo->to, finding3);
    for (; i2 < finding2.size(); i2++) {
        if (strcmp(finding2[i2].train_id, oo->train_id) == 0) break;
    }
    for (; i3 < finding3.size(); i3++) {
        if (strcmp(finding3[i3].train_id, oo->train_id) == 0) break;
    }
    auto finding4=ticket_data.readwrite(ss);
    for (int j = finding2[i2].account; j < finding3[i3].account; j++) {
        finding4->num[j] += oo->num;
    }
    for (auto iter = finding1.begin(); iter != finding1.end(); iter++) {
        bool f = true;
        for (int jj = (*iter).l; jj < (*iter).r; jj++) {
            if (finding4->num[jj] >= (*iter).num) continue;
            else {
                f = false;
                break;
            }
        }
        if (!f) continue;
        for (int jj = (*iter).l; jj < (*iter).r; jj++) {
            finding4->num[jj] -= (*iter).num;
        }
        waiting_data.erase(ss, (*iter));
        auto temp=order_index.readwrite(-(*iter).index);
        temp->status = 1;
    }
    std::cout << 0 << '\n';
}

#endif //TICKET_SYSTEM_ORDER_H
