//
// Created by xun_y on 2023/6/25.
//

#ifndef UNTITLED1_ORDER_H
#define UNTITLED1_ORDER_H

#include "register.h"
#include<queue>


enum kind {
    LUI,
    AUIPC,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANI,
    SLLI,
    SRLI,
    SRAI,
    SB,
    SH,
    SW,
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    EEND
};

class order_queue {
public:
    bool full() {
        if (q_order.size() == 1) {
            return true;
        } else return false;
    }

    void insert(u32 order) {
        q_order.push(order);
    }

    u32 pop() {
        u32 temp = q_order.front();
        q_order.pop();
        return temp;
    }

    int size() {
        return q_order.size();
    }

    bool empty () {
        return q_order.empty();
    }

private:
    std::queue<u32> q_order;
};

order_queue o_q;

#endif //UNTITLED1_ORDER_H
