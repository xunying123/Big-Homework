//
// Created by xun_y on 2023/6/25.
//

#ifndef UNTITLED1_RUN_H
#define UNTITLED1_RUN_H

#include <iostream>
#include<queue>
#include "predictor.h"
#include "memory.h"
#include "order.h"
#include "register.h"
#include "ROB.h"
#include <algorithm>

long long clock_time = 1;

void get_orders();

void order_run(u32);

void run() {
    int a[5]={1,2,3,4,5};
    while (true) {
        pcc = pc;
        k = reg_;
        std::random_shuffle(a,a+4);
        for(int i=0;i<5;i++) {
            switch (a[i]) {
                case 1:{
                    get_orders();
                    break;
                }
                case 2: {
                    if ((!rob_.full()) && (!ar.full()) && (!o_q.empty())) {
                        u32 n_order = o_q.pop();
                        order_run(n_order);
                    }
                    break;
                }
                case 3: {
                    l_s_b_.run(clock_time);
                    break;
                }
                case 4 :{
                    ar.run(clock_time);
                    break;
                }
                case 5: {
                    rob_.run(clock_time);
                    break;
                }
            }
        }
        if(cclo2!=-1) r_buss();
        if(cclo1!=-1) r_bus();
        if(cclo3!=-1) r_busss();
        clock_time++;
        pc = pcc;
        reg_ = k;
     //   reg_.out();
    }
}

void get_orders() {
    if ((!o_q.full()) && (pcc != -1)) {
        u32 temp = mem_.readfour(pcc);
        o_q.insert(temp);
    }
};

void order_run(u32 o) {
    if (o == 0x0FF00513) {
        r_o_b temp;
        temp.clock_time = clock_time;
        temp.type = EEND;
        temp.s_pos = 0;
        temp.status_ = 0;
        if (reg_.ststus(10) != 0) {
            temp.s_value = reg_.ststus(10);
            temp.status_ = 1;
        } else {
            temp.value = reg_.data(10);
        }
        rob_.push(temp);
        pcc += 4;
    }
    u32 sig = o & (u32) 127;
    if (sig == 55) {
        r_o_b temp;
        temp.clock_time = clock_time;
        temp.type = LUI;
        temp.status_ = 0;
        temp.s_value = 0;
        temp.value = o >> 12;
        temp.value = temp.value << 12;
        u32 pop = o >> 7;
        pop = pop & (u32) 31;
        temp.s_pos = 0;
        temp.pos = pop;
        k.ststus(pop) = clock_time;
        rob_.push(temp);
        pcc += 4;
     //   std::cout << "LUI" << std::endl;
    }
    if (sig == 23) {
        r_o_b tee;
        int32_t v1, v2;
        v1 = o >> 12;
        v1 = v1 << 12;
        u32 pop = o >> 7;
        pop = pop & (u32) 31;
        v2 = pc;
        k.ststus(pop) = clock_time;
        tee.type = AUIPC;
        tee.clock_time = clock_time;
        tee.status_ = 0;
        tee.s_value = 0;
        tee.s_pos = 0;
        tee.pos = pop;
        tee.value = (int32_t) (v1 + v2);
        rob_.push(tee);
        pcc += 4;
      //  std::cout << "AUIPC" << std::endl;
    }
    if (sig == 111) {
        r_o_b temp;
        temp.clock_time = clock_time;
        temp.type = JAL;
        temp.status_ = 0;
        temp.s_value = 0;
        int32_t a, b;
        a = o >> 31;
        if (a == 1) {
            a = (int32_t) 0b11111111111100000000000000000000;
        }
        b = o >> 12;
        b = b & (u32) 255;
        b = b << 12;
        a = a + b;
        b = o >> 20;
        b = b & (u32) 1;
        b = b << 11;
        a = a + b;
        b = o >> 21;
        b = b & (u32) 1023;
        b = b << 1;
        a = a + b;
        temp.value = pc + 4;
        pcc = (int32_t) pc + a;
        u32 pop = o >> 7 & (u32) 31;
        k.ststus(pop) = clock_time;
        temp.s_pos = 0;
        temp.pos = pop;
        rob_.push(temp);
    //    std::cout << "JAL" << std::endl;
    }
    if (sig == 103) {
        add_register temp;
        r_o_b tee;
        temp.clock_time = clock_time;
        temp.type = JALR;
        u32 pop = o >> 15;
        pop = pop & (u32) 31;
        if (reg_.ststus(pop) != 0) {
            temp.q1 = reg_.ststus(pop);
            temp.q2 = 0;
            temp.v1 = reg_.ststus(pop);
            temp.v2 = o >> 20;
        } else {
            temp.q1 = 0;
            temp.q2 = 0;
            temp.v1 = reg_.data(pop);
            u32 kkk = o >> 31;
            if (kkk == 1) kkk = 0b11111111111111111111000000000000;
            temp.v2 = kkk + (o >> 20);
        }
        tee.pc = pc + 4;
        pcc = -1;
        pop = (o >> 7) % 32;
        k.ststus(pop) = clock_time;
        ar.push(temp);
        tee.type = JALR;
        tee.clock_time = clock_time;
        tee.status_ = 1;
        tee.s_value = clock_time;
        tee.s_pos = 0;
        tee.pos = pop;
        rob_.push(tee);
      //  std::cout << "JALR" << std::endl;
    }
    if (sig == 99) {
        int32_t k, kk;
        k = o >> 31;
        if (k == 1) {
            kk = (int32_t) 0b11111111111111111111000000000000;
        } else kk = 0;
        kk += ((o >> 7) % 2) << 11;
        kk += ((o >> 25) % 64) << 5;
        kk += ((o >> 8) % 16) << 1;
        kk += pc;
        r_o_b temp;
        temp.clock_time = clock_time;
       /* if(pre_._predictor(o)) {
            pcc=kk;
            temp.pc=pc+4;
            temp.jump=1;
        }
        else {
            pcc+=4;
            temp.pc=kk;
            temp.jump=0;
        }*/
        pcc = -1;
        temp.pc=pc+4;
        temp.jump=kk;
        u32 pop1;
        pop1 = (o >> 15) % (u32) 32;
        if (reg_.ststus(pop1) == 0) {
            temp.s_pos = 0;
            temp.pos = reg_.data(pop1);
        } else {
            temp.s_pos = reg_.ststus(pop1);
        }
        u32 pop2 = (o >> 20) % (u32) 32;
        if (reg_.ststus(pop2) == 0) {
            temp.s_value = 0;
            temp.value = reg_.data(pop2);
        } else {
            temp.s_value = reg_.ststus(pop2);
        }
        if (temp.s_pos == 0 && temp.s_value == 0) temp.status_ = 0;
        else temp.status_ = 1;
        kk = o >> 12;
        temp.o = o;
        switch (kk % 8) {
            case 0: {
                temp.type = BEQ;
             //   std::cout << "BEQ" <<std::endl;
                break;
            }
            case 1: {
              //  std::cout << "BNE" << '\n';
                temp.type = BNE;
                break;
            }
            case 4: {
            //    std::cout << "BLT" << std::endl;

                temp.type = BLT;
                break;
            }
            case 5: {
              //  std::cout << "BGE" << std::endl;

                temp.type = BGE;
                break;
            }
            case 6: {
              //  std::cout << "BLTU" << std::endl;

                temp.type = BLTU;
                break;
            }
            case 7: {
              //  std::cout << "BGEU" <<std::endl;

                temp.type = BGEU;
                break;
            }
            default:
                std::cout << "warning" << std::endl;
        }
        rob_.push(temp);
    }
    if (sig == 3) {
        add_register temp;
        r_o_b tee;
        temp.clock_time = clock_time;
        temp.q1 = 0;
        u32 tt = o >> 31;
        if (tt == 1) {
            tt = 0b11111111111111111111000000000000;
        }
        tt += o >> 20;
        temp.v1 = tt;
        u32 pop1;
        pop1 = (o >> 15) & (u32) 31;
        if (reg_.ststus(pop1) == 0) {
            temp.q2 = 0;
            temp.v2 = reg_.data(pop1);
        } else {
            temp.q2 = reg_.ststus(pop1);
        }
        pop1 = (o >> 7) & (u32) 31;
        k.ststus(pop1) = -clock_time;
        tee.clock_time = clock_time;
        tee.status_ = 1;
        tee.s_pos = clock_time;
        tee.s_value = 0;
        u32 pop2 = (o >> 7) & (u32) 31;
        tee.value = pop2;
        pop1 = o >> 12;
        switch (pop1 % 8) {
            case 0: {
             //   std::cout << "LB" << std::endl;

                temp.type = LB;
                tee.type = LB;
                break;
            }
            case 1: {
              //  std::cout << "LH" << std::endl;

                temp.type = LH;
                tee.type = LH;
                break;
            }
            case 2: {
              //  std::cout << "LW" <<std::endl;

                temp.type = LW;
                tee.type = LW;
                break;
            }

            case 4: {
              //  std::cout << "LBU" << std::endl;

                temp.type = LBU;
                tee.type = LBU;
                break;
            }
            case 5: {
              //  std::cout << "LHU" << std::endl;

                temp.type = LHU;
                tee.type = LHU;
                break;
            }
            default:
                std::cout << "warning" << std::endl;
        }
        ar.push(temp);
        rob_.push(tee);
        pcc += 4;
    }
    if (sig == 19) {
        u32 popp = o >> 12;
        popp %= 8;
        if (popp == 1 || popp == 5) {
            add_register temp;
            r_o_b tee;
            temp.clock_time = clock_time;
            u32 tt = o >> 20;
            tt %= 32;
            temp.v1 = tt;
            temp.q1 = 0;
            u32 pop1;
            pop1 = (o >> 15) & (u32) 31;
            if (reg_.ststus(pop1) == 0) {
                temp.q2 = 0;
                temp.v2 = reg_.data(pop1);
            } else {
                temp.q2 = reg_.ststus(pop1);
            }
            tee.clock_time = clock_time;
            tee.status_ = 1;
            tee.value = clock_time;
            tee.s_value = clock_time;
            tee.s_pos = 0;
            u32 pop2 = (o >> 7) & (u32) 31;
            tee.pos = pop2;
            k.ststus(pop2) = clock_time;
            pop1 = o >> 12;
            switch (pop1 % 8) {
                case 1: {
                  //  std::cout << "SLLI" << std::endl;

                    temp.type = SLLI;
                    tee.type = SLLI;
                    break;
                }
                case 5 : {
                    popp = popp >> 25;
                    if (popp == 0) {
                      //  std::cout << "SRLI" << std::endl;

                        temp.type = SRLI;
                        tee.type = SRLI;
                        break;
                    } else {
                       // std::cout << "SRAI" << std::endl;

                        temp.type = SRAI;
                        tee.type = SRAI;
                        break;
                    }
                }
                default:
                    std::cout << "warning" << std::endl;
            }
            ar.push(temp);
            rob_.push(tee);
        } else {
            add_register temp;
            r_o_b tee;
            temp.clock_time = clock_time;
            temp.q1 = 0;
            u32 tt = o >> 31;
            if (tt == 1) {
                tt = 0b11111111111111111111000000000000;
            }
            tt += o >> 20;
            temp.v1 = tt;
            u32 pop1;
            pop1 = (o >> 15) & (u32) 31;
            if (reg_.ststus(pop1) == 0) {
                temp.q2 = 0;
                temp.v2 = reg_.data(pop1);

            } else {
                temp.q2 = reg_.ststus(pop1);
            }
            tee.clock_time = clock_time;
            tee.status_ = 1;
            tee.value = clock_time;
            tee.s_value = clock_time;
            tee.s_pos = 0;
            u32 pop2 = (o >> 7) & (u32) 31;
            tee.pos = pop2;
            k.ststus(pop2) = clock_time;
            pop1 = o >> 12;
            switch (pop1 % 8) {
                case 0 : {
                  //  std::cout << "ADDI" << std::endl;

                    temp.type = ADDI;
                    tee.type = ADDI;
                    break;
                }
                case 2 : {
                 //   std::cout << "SLTI" << std::endl;

                    temp.type = SLTI;
                    tee.type = SLTI;
                    break;
                }
                case 3 : {
                //    std::cout << "SLTIU" << std::endl;

                    temp.type = SLTIU;
                    tee.type = SLTIU;
                    break;
                }
                case 4 : {
                  //  std::cout << "XORI" << std::endl;

                    temp.type = XORI;
                    tee.type = XORI;
                    break;
                }
                case 6 : {
                 //     std::cout << "ORI" << std::endl;

                    temp.type = ORI;
                    tee.type = ORI;
                    break;
                }
                case 7 : {
                //    std::cout << "ANDI" << std::endl;

                    temp.type = ANI;
                    tee.type = ANI;
                    break;
                }
                default:
                    std::cout << "warning" << std::endl;
            }
            ar.push(temp);
            rob_.push(tee);
        };
        pcc += 4;
    }
    if (sig == 51) {
        add_register temp;
        r_o_b tee;
        temp.clock_time = clock_time;
        u32 pop1;
        pop1 = (o >> 15) & (u32) 31;
        if (reg_.ststus(pop1) == 0) {
            temp.q1 = 0;
            temp.v1 = reg_.data(pop1);
        } else {
            temp.q1 = reg_.ststus(pop1);
        }
        pop1 = (o >> 20) & (u32) 31;
        if (reg_.ststus(pop1) == 0) {
            temp.q2 = 0;
            temp.v2 = reg_.data(pop1);
        } else {
            temp.q2 = reg_.ststus(pop1);
        }
        tee.clock_time = clock_time;
        tee.status_ = 2;
        tee.s_pos = 0;
        pop1 = (o >> 7) & (u32) 31;
        tee.pos = pop1;
        k.ststus(pop1) = clock_time;
        tee.s_value = clock_time;
        tee.value = clock_time;
        pop1 = (o >> 12) % 8;
        if (pop1 == 0) {
            u32 kkk = o >> 25;
            if (kkk == 0) {
              //  std::cout << "ADD" << std::endl;

                temp.type = ADD;
                tee.type = ADD;
            } else {
              //  std::cout << "SUB" << std::endl;

                temp.type = SUB;
                tee.type = SUB;
            }
        } else if (pop1 == 1) {
         //   std::cout << "SLL" << std::endl;

            temp.type = SLL;
            tee.type = SLL;
        } else if (pop1 == 2) {
          //  std::cout << "SLT" << std::endl;

            temp.type = SLT;
            tee.type = SLT;
        } else if (pop1 == 3) {
         //  std::cout << "SLTU" <<std::endl;

            temp.type = SLTU;
            tee.type = SLTU;
        } else if (pop1 == 4) {
          //  std::cout << "XOR" << std::endl;

            temp.type = XOR;
            tee.type = XOR;
        } else if (pop1 == 5) {
            u32 kkk = o >> 25;
            if (kkk == 0) {
             //   std::cout << "SRL" << std::endl;

                temp.type = SRL;
                tee.type = SRL;
            } else {
             //   std::cout << "SRA" << std::endl;

                temp.type = SRA;
                tee.type = SRA;
            }
        } else if (pop1 == 6) {
         //  std::cout << "OR" << std::endl;

            temp.type = OR;
            tee.type = OR;
        } else if (pop1 == 7) {
        //    std::cout << "AND" << std::endl;

            temp.type = AND;
            tee.type = AND;
        } else std::cout << "warning" << std::endl;
        ar.push(temp);
        rob_.push(tee);
        pcc += 4;
    }
    if (sig == 35) {
        add_register temp;
        r_o_b tee;
        temp.clock_time = clock_time;
        temp.q1 = 0;
        u32 tt = o >> 31;
        if (tt == 1) {
            tt = 0b11111111111111111111000000000000;
        }
        tt += o >> 25 << 5;
        tt += (o >> 7) % 32;
        temp.v1 = tt;
        u32 pop1;
        pop1 = (o >> 15) & (u32) 31;
        if (reg_.ststus(pop1) == 0) {
            temp.q2 = 0;
            temp.v2 = reg_.data(pop1);
        } else {
            temp.q2 = reg_.ststus(pop1);
        }
        tee.clock_time = clock_time;
        tee.status_ = 1;
        tee.s_pos = clock_time;
        tee.pos = 0;
        pop1 = (o >> 20) % 32;
        if (reg_.ststus(pop1) == 0) {
            tee.s_value = 0;
            tee.value = reg_.data(pop1);
        } else {
            tee.s_value = reg_.ststus(pop1);
        }
        pop1 = (o >> 12) % 8;
        if (pop1 == 0) {
          //  std::cout << "SB" << std::endl;

            temp.type = SB;
            tee.type = SB;
        } else if (pop1 == 1) {
             //   std::cout << "SH" << std::endl;

            temp.type = SH;
            tee.type = SH;
        } else if (pop1 == 2) {
         //   std::cout << "SW" << std::endl;

            temp.type = SW;
            tee.type = SW;
        } else std::cout << "warning" << std::endl;
        ar.push(temp);
        rob_.push(tee);
        pcc += 4;
    }
}

#endif //UNTITLED1_RUN_H
