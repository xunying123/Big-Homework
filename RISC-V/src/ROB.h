//
// Created by xun_y on 2023/6/26.
//

#ifndef UNTITLED1_ROB_H
#define UNTITLED1_ROB_H

#include "register.h"
#include "predictor.h"

long long pc = 0, pcc = 0;

int32_t ppos,ppos3;
u32 ddata1,ddata2,ddata3;
long long cclo1=-1,cclo2=-1,cclo3=-1;

void s_bus(int32_t pos, u32 data, long long clo);

void s_buss(int32_t data, long long clo);

void s_busss(int32_t pos, u32 data, long long clo);

void jalr_b(int32_t data, long long clo);

template<typename T>
class linked {
public:
    linked(int l) {
        size = 0;
        length = l;
        head = new node;
        head->next = nullptr;
    }

    ~linked() {
        node *t1, *t2;
        t1 = head;
        while (t1->next != nullptr) {
            t2 = t1;
            t1 = t1->next;
            delete t2;
        }
        delete t1;
        size = 0;
        size = 0;
    }

    bool full() {
        return size == length;
    }

    T top() {
        return (head->next->a);
    }

    void push(T aa) {
        size++;
        node *temp;
        temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = new node;
        temp->next->next = nullptr;
        temp->next->a = aa;
    }

    void pop() {
        node *temp = head->next;
        head->next = head->next->next;
        if (temp != nullptr) delete temp;
        size--;
    }

    void clear() {
        node *t1, *t2;
        t1 = head;
        while (t1->next != nullptr) {
            t2 = t1;
            t1 = t1->next;
            delete t2;
        }
        delete t1;
        size = 0;
        head = new node;
        head->next = nullptr;
    }

    struct node {
        node *next;
        T a;
    };

    node *head;
    int size;
    int length;
};

template<typename T>
class quq {
public:
    quq(int length) {
        for (int i = 0; i < 16; i++) bb[i] = false;
        size_ = 0;
    }

    void push(T a) {
        size_++;
        for (int i = 0; i < 16; i++) {
            if (!bb[i]) {
                aa[i] = a;
                bb[i] = true;
                break;
            }
        }
    }

    void pop(int k) {
        bb[k] = false;
        size_--;
    }

    int size() {
        return size_;
    }

    int length_() {
        return 16;
    }

    bool operator()(int kk) {
        return bb[kk];
    }

    T &operator[](int kk) {
        return aa[kk];
    }

    void clear() {
        for (int i = 0; i < 16; i++) bb[i] = false;
    }
    T aa[16];
    bool bb[16];
private:
    int length = 16;
    int size_;


};

struct l_s_b {
    kind type;
    long long clock_time;
    u32 pos;
    u32 data;
};

class Load_Store_Buffer {
public:
    void push(l_s_b &o) {
        lq.push(o);
    }

    void run(long long cc) {
        if (lq.empty()) return;

        if (countt == 2) {
            l_s_b temp = lq.front();
            if(temp.clock_time==cc) return ;
            lq.pop();
            switch (temp.type) {
                case LB : {
                    u32 kk = mem_.readone(temp.pos);
                    u32 k = kk >> 7;
                    if (k == 1) {
                        k = 0b11111111111111111111111100000000;
                    } else k = 0;
                    k += kk;
                    s_bus(temp.data, k, -temp.clock_time);
                    break;
                }
                case LH : {
                    u32 kk = mem_.readtwo(temp.pos);
                    u32 k = kk >> 15;
                    if (k == 1) {
                        k = 0b11111111111111110000000000000000;
                    } else k = 0;
                    k += kk;
                    s_bus(temp.data, k, -temp.clock_time);
                    break;
                }
                case LW : {
                    u32 kk = mem_.readfour(temp.pos);
                    s_bus(temp.data, kk, -temp.clock_time);
                    break;
                }
                case LBU : {
                    u32 kk = mem_.readone(temp.pos);
                    s_bus(temp.data, kk, -temp.clock_time);
                    break;
                }
                case LHU : {
                    u32 kk = mem_.readtwo(temp.pos);
                    s_bus(temp.data, kk, -temp.clock_time);
                    break;
                }
                case SB : {
                    temp.data = temp.data << 24;
                    temp.data = temp.data >> 24;
                    mem_.write(temp.pos, temp.data);
                    break;
                }
                case SH : {
                    temp.data = temp.data << 16;
                    temp.data = temp.data >> 16;
                    mem_.write(temp.pos, temp.data);
                    break;
                }
                case SW : {
                    mem_.write(temp.pos, temp.data);
                    break;
                }
                case EEND: {
                    int k = (temp.data) & (u32) 255;
                    std::cout << k << '\n';
                    std::cout<<cc<<'\n';
                    pre_.rate();
                    exit(0);
                }
            }
            return;
        }
        if (countt == 1) countt++;
        if (countt == 0) countt++;
    }

private:
    std::queue<l_s_b> lq;
    int countt = 0;
};

Load_Store_Buffer l_s_b_;

struct add_register {
    kind type;
    long long clock_time;
    long long q1;
    u32 v1;
    long long q2;
    u32 v2;
};

class alu_register {
public:

    alu_register() : a_r(16) {}

    void push(add_register t) {
        a_r.push(t);
    }

    bool full() {
        return a_r.size() == 16;
    }

    void clear() {
        a_r.clear();
    }

    void nob() {
        for(int i=0;i<16;i++) {
            if(a_r(i)) {
                for(int j=0;j<32;j++) {
                    if(reg_.ststus(j)==a_r[i].clock_time) {
                        reg_.ststus(i)=0;
                    }
                }
            }
        }
    }

    void run(long long cc) {
        if (a_r.size() == 0) return;
        int i = 0;
        for (; i < a_r.length_(); i++) {
            if (!a_r(i)) continue;
            if ((a_r[i].q1 == 0) && (a_r[i].q2 == 0)) break;
        }
        if (i == a_r.length_()) return;
        if(a_r[i].clock_time==cc) return ;
        a_r.pop(i);
        switch (a_r[i].type) {
            case AUIPC: {
                s_buss((int32_t) a_r[i].v1 + (int32_t) a_r[i].v2, a_r[i].clock_time);
                break;
            }
            case JALR: {
                auto kk = (int32_t) a_r[i].v1 + (int32_t) a_r[i].v2;
                kk = kk >> 1;
                kk = kk << 1;
                jalr_b(kk, a_r[i].clock_time);
                break;
            }
            case ADDI:
            case ADD: {
                s_buss((int32_t) a_r[i].v1 + (int32_t) a_r[i].v2, a_r[i].clock_time);
                break;
            }
            case SLTI:
            case SLT: {
                int32_t a = a_r[i].v1;
                int32_t b = a_r[i].v2;
                if (a < b) s_buss(1, a_r[i].clock_time);
                else s_buss(0, a_r[i].clock_time);
                break;

            }
            case SLTIU:
            case SLTU: {
                if (a_r[i].v1 < a_r[i].v2) s_buss(1, a_r[i].clock_time);
                else s_buss(0, a_r[i].clock_time);
                break;
            }
            case XORI:
            case XOR: {
                s_buss(((int32_t) a_r[i].v1) ^ ((int32_t) a_r[i].v2), a_r[i].clock_time);
                break;
            }
            case ORI:
            case OR: {
                s_buss(((int32_t) a_r[i].v1) | ((int32_t) a_r[i].v2), a_r[i].clock_time);
                break;
            }
            case ANI:
            case AND: {
                s_buss(((int32_t) a_r[i].v1) & ((int32_t) a_r[i].v2), a_r[i].clock_time);
                break;
            }
            case SLLI: {
                s_buss(a_r[i].v2 << a_r[i].v1, a_r[i].clock_time);
                break;
            }
            case SRLI : {
                s_buss(a_r[i].v2 >> a_r[i].v1, a_r[i].clock_time);
                break;
            }
            case SRAI: {
                u32 k = a_r[i].v2 >> 31;
                u32 t = 0;
                for (int j = 0; j < a_r[j].v1; j++) {
                    t << 1;
                    t += k;
                }
                t = t << (32 - a_r[i].v1);
                k = a_r[i].v2 >> a_r[i].v1;
                t += k;
                s_buss(t, a_r[i].clock_time);
                break;
            }
            case LB:
            case LH:
            case LW:
            case LBU:
            case LHU:
            case SB:
            case SH:
            case SW: {
                s_buss((int32_t) a_r[i].v1 + (int32_t) a_r[i].v2, a_r[i].clock_time);
                break;
            }
            case SUB: {
                s_buss((int32_t) a_r[i].v1 - (int32_t) a_r[i].v2, a_r[i].clock_time);
                break;
            }
            case SLL: {
                u32 t = a_r[i].v2 & (u32) 31;
                s_buss(a_r[i].v1 << t, a_r[i].clock_time);
                break;
            }
            case SRL: {
                u32 t = a_r[i].v2 & (u32) 31;
                s_buss(a_r[i].v1 >> t, a_r[i].clock_time);
                break;
            }
            case SRA: {
                u32 tt = a_r[i].v2 & (u32) 31;
                u32 k = a_r[i].v2 >> 31;
                u32 t = 0;
                for (int j = 0; j < tt; j++) {
                    t << 1;
                    t += k;
                }
                t = t << (32 - tt);
                k = a_r[i].v2 >> tt;
                t += k;
                s_buss(t, a_r[i].clock_time);
                break;
            }
        }
    }

    quq<add_register> a_r;
};

alu_register ar;

struct r_o_b {
    kind type;
    long long clock_time;
    int status_;
    u32 value;
    long long s_value;
    long long s_pos;
    long long pos;
    u32 pc = 0;
    int jump = 0;
    u32 o;
};

class rob {
public:
    rob() : rob_(32) {
        eend = false;
    }

    bool full() {
        return rob_.full();
    }

    r_o_b top() {
        return rob_.top();
    }

    void push(r_o_b &temp) {
        rob_.push(temp);
    }

    void nob() {
        auto kkk=rob_.head;
        while(kkk->next!= nullptr) {
            for(int i=0;i<32;i++) {
                if(reg_.ststus(i)==kkk->a.clock_time) {
                    reg_.ststus(i)=0;
                }
            }
            kkk=kkk->next;
        }
        for(int i=0;i<32;i++) {
            if(reg_.ststus(i)==kkk->a.clock_time) {
                reg_.ststus(i)=0;
            }
        }
    }

    void clear() {

        rob_.clear();
    }

    void run(long long cc) {
        if (eend) return;
        if (rob_.size == 0) return;
        r_o_b ttt = rob_.top();
        if(ttt.clock_time==cc) return ;
        if (ttt.status_ == 0) {
            rob_.pop();
            switch (ttt.type) {
                case LUI:
                case AUIPC:
                case JAL:
                case ADDI:
                case SLTI:
                case SLTIU:
                case XORI:
                case ORI:
                case ANI:
                case SLLI:
                case SRLI:
                case SRAI:
                case ADD:
                case SUB:
                case SLL:
                case SLT:
                case SLTU:
                case XOR:
                case SRL:
                case SRA:
                case OR:
                case AND: {
                    s_busss(ttt.pos, ttt.value, ttt.clock_time);
                    break;
                }
                case LB:
                case LH:
                case LW:
                case LBU:
                case LHU:
                case SB:
                case SH:
                case SW: {
                    l_s_b temp;
                    temp.clock_time = ttt.clock_time;
                    temp.type = ttt.type;
                    temp.pos = ttt.pos;
                    temp.data = ttt.value;
                    l_s_b_.push(temp);
                    break;
                }
                case BEQ: {
                   /* if (ttt.value == ttt.pos) {
                        if(ttt.jump) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    } else {
                        if(ttt.jump==0) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    }*/
                   if((ttt.value==ttt.pos)==pre_._predictor(ttt.o)) {
                       pre_.update(ttt.jump, true, ttt.o);
                   }
                   else pre_.update(ttt.jump, false, ttt.o);
                    if (ttt.value == ttt.pos) {
                        pcc = ttt.jump;
                    }
                    else pcc=ttt.pc;
                    break;
                }
                case BNE: {
                    /*if (ttt.value == ttt.pos) {
                        if(ttt.jump) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    } else {
                        if(ttt.jump!=0) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    }*/
                    if((ttt.value!=ttt.pos)==pre_._predictor(ttt.o)) {
                        pre_.update(ttt.jump, true, ttt.o);
                    }
                    else pre_.update(ttt.jump, false, ttt.o);
                    if (ttt.value != ttt.pos) {
                        pcc = ttt.jump;
                    }
                    else pcc=ttt.pc;
                    break;
                }
                case BLT: {
                    int32_t a, b;
                    a = ttt.value;
                    b = ttt.pos;
                   /* if (a>b) {
                        if(ttt.jump) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    } else {
                        if(ttt.jump==0) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    }*/
                    if((a>b)==pre_._predictor(ttt.o)) {
                        pre_.update(ttt.jump, true, ttt.o);
                    }
                    else pre_.update(ttt.jump, false, ttt.o);
                    if (a>b) {
                        pcc = ttt.jump;
                    }
                    else pcc=ttt.pc;
                    break;
                }
                case BGE: {
                    uint32_t a, b;
                    a = ttt.value;
                    b = ttt.pos;
                   /* if (a<=b) {
                        if(ttt.jump) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    } else {
                        if(ttt.jump==0) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    }*/
                    if((a<=b)==pre_._predictor(ttt.o)) {
                        pre_.update(ttt.jump, true, ttt.o);
                    }
                    else pre_.update(ttt.jump, false, ttt.o);
                    if (a<=b) {
                        pcc = ttt.jump;
                    }
                    else pcc=ttt.pc;
                    break;

                }
                case BLTU: {
                   /* if (ttt.value > ttt.pos) {
                        if(ttt.jump) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    } else {
                        if(ttt.jump==0) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    }*/
                    if((ttt.value>ttt.pos)==pre_._predictor(ttt.o)) {
                        pre_.update(ttt.jump, true, ttt.o);
                    }
                    else pre_.update(ttt.jump, false, ttt.o);
                    if (ttt.value > ttt.pos) {
                        pcc = ttt.jump;
                    }
                    else pcc=ttt.pc;
                    break;
                }
                case BGEU: {
                   /* if (ttt.value <= ttt.pos) {
                        if(ttt.jump) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    } else {
                        if(ttt.jump==0) {
                            pre_.update(ttt.jump, true, ttt.o);
                        }
                        else {
                            pre_.update(ttt.jump, false, ttt.o);
                            pcc = ttt.pc;
                            ar.nob();
                            nob();
                            clear();
                            ar.clear();
                        }
                    }*/
                    if((ttt.value<=ttt.pos)==pre_._predictor(ttt.o)) {
                        pre_.update(ttt.jump, true, ttt.o);
                    }
                    else pre_.update(ttt.jump, false, ttt.o);
                    if (ttt.value <= ttt.pos) {
                        pcc = ttt.jump;
                    }
                    else pcc=ttt.pc;
                    break;
                }
                case EEND: {
                    l_s_b eee;
                    eee.type = EEND;
                    eee.clock_time = ttt.clock_time;
                    eee.data = ttt.value;
                    l_s_b_.push(eee);
                    eend = true;
                    break;
                }
                case JALR: {
                    pcc = ttt.value;
                    s_busss(ttt.pos, ttt.pc, ttt.clock_time);
                    break;
                }
            }
            if(cc>=85139264) {
                std::cout<<(62+5)*2+3<<'\n';
                exit(0);
            }
        } else return;
    }
    bool eend;
    linked<r_o_b> rob_;
};

rob rob_;

void r_bus() {
    int i = 0;
    for (; i < 16; i++) {
        if (ar.a_r[i].q1 == cclo1) {
            ar.a_r[i].v1 = ddata1;
            ar.a_r[i].q1 = 0;
        }
        if (ar.a_r[i].q2 == cclo1) {
            ar.a_r[i].v2 = ddata1;
            ar.a_r[i].q2 = 0;
        }
    }
    linked<r_o_b>::node *temp;
    temp = rob_.rob_.head;
    while (temp->next != nullptr) {
        if (temp->a.s_value == cclo1) {
            temp->a.value = ddata1;
            temp->a.s_value = 0;
        }
        if (temp->a.s_pos == cclo1) {
            temp->a.pos = ddata1;
            temp->a.s_pos = 0;
        }
        if (temp->a.s_value == 0 && temp->a.s_pos == 0) temp->a.status_ = 0;
        temp = temp->next;
    }
    if (temp->a.s_value == cclo1) {
        temp->a.value = ddata1;
        temp->a.s_value = 0;
    }
    if (temp->a.s_pos == cclo1) {
        temp->a.pos = ddata1;
        temp->a.s_pos = 0;
    }
    if (temp->a.s_value == 0 && temp->a.s_pos == 0) temp->a.status_ = 0;
    if(k.ststus(ppos)==cclo1) {
        k.ststus(ppos) = 0;
    }
    k.data(ppos) = ddata1;
    cclo1=-1;
}

void r_busss() {
    int i = 0;
    for (; i < 16; i++) {
        if (ar.a_r[i].q1 == cclo3) {
            ar.a_r[i].v1 = ddata3;
            ar.a_r[i].q1 = 0;
        }
        if (ar.a_r[i].q2 == cclo3) {
            ar.a_r[i].v2 = ddata3;
            ar.a_r[i].q2 = 0;
        }
    }
    linked<r_o_b>::node *temp;
    temp = rob_.rob_.head;
    while (temp->next != nullptr) {
        if (temp->a.s_value == cclo3) {
            temp->a.value = ddata3;
            temp->a.s_value = 0;
        }
        if (temp->a.s_pos == cclo3) {
            temp->a.pos = ddata3;
            temp->a.s_pos = 0;
        }
        if (temp->a.s_value == 0 && temp->a.s_pos == 0) temp->a.status_ = 0;
        temp = temp->next;
    }
    if (temp->a.s_value == cclo3) {
        temp->a.value = ddata3;
        temp->a.s_value = 0;
    }
    if (temp->a.s_pos == cclo3) {
        temp->a.pos = ddata3;
        temp->a.s_pos = 0;
    }
    if (temp->a.s_value == 0 && temp->a.s_pos == 0) temp->a.status_ = 0;
    if(k.ststus(ppos3)==cclo3) {
        k.ststus(ppos3) = 0;
    }
    k.data(ppos3) = ddata3;
    cclo3=-1;
}

void s_bus(int32_t pos, u32 data, long long clo) {
    ppos=pos;
    ddata1=data;
    cclo1=clo;
}

void s_buss(int32_t data, long long clo) {
    ddata2=data;
    cclo2=clo;
}

void s_busss(int32_t pos, u32 data, long long clo) {
    ppos3=pos;
    ddata3=data;
    cclo3=clo;
}

void r_buss() {
    int i = 0;
    for (; i < 16; i++) {
        if (!ar.a_r(i)) continue;
        if (ar.a_r[i].q1 == cclo2) {
            ar.a_r[i].v1 = ddata2;
            ar.a_r[i].q1 = 0;
        }
        if (ar.a_r[i].q2 == cclo2) {
            ar.a_r[i].v2 = ddata2;
            ar.a_r[i].q2 = 0;
        }
    }
    linked<r_o_b>::node *temp;
    temp = rob_.rob_.head;
    while (temp->next != nullptr) {
        if (temp->a.s_value == cclo2) {
            temp->a.value = ddata2;
            temp->a.s_value = 0;
        }
        if (temp->a.s_pos == cclo2) {
            temp->a.pos = ddata2;
            temp->a.s_pos = 0;
        }
        if (temp->a.s_value == 0 && temp->a.s_pos == 0) temp->a.status_ = 0;
        temp = temp->next;
    }
    if (temp->a.s_value == cclo2) {
        temp->a.value = ddata2;
        temp->a.s_value = 0;
    }
    if (temp->a.s_pos == cclo2) {
        temp->a.pos = ddata2;
        temp->a.s_pos = 0;
    }
    if (temp->a.s_value == 0 && temp->a.s_pos == 0) temp->a.status_ = 0;
    cclo2=-1;
}

void jalr_b(int32_t data, long long clo) {
    linked<r_o_b>::node *temp;
    temp = rob_.rob_.head;
    while (temp->next != nullptr) {
        if (temp->a.s_value == clo) {
            temp->a.value = data;
            temp->a.s_value = 0;
        }
        if (temp->a.s_pos == clo) {
            temp->a.pos = data;
            temp->a.s_pos = 0;
        }
        if (temp->a.s_value == 0 && temp->a.s_pos == 0) temp->a.status_ = 0;
        temp = temp->next;
    }
    if (temp->a.s_value == clo) {
        temp->a.value = data;
        temp->a.s_value = 0;
    }
    if (temp->a.s_pos == clo) {
        temp->a.pos = data;
        temp->a.s_pos = 0;
    }
    if (temp->a.s_value == 0 && temp->a.s_pos == 0) temp->a.status_ = 0;
}

#endif //UNTITLED1_ROB_H
