//
// Created by xun_y on 2023/6/25.
//

#ifndef UNTITLED1_PREDICTOR_H
#define UNTITLED1_PREDICTOR_H

#include "register.h"

class predictor {
public:

    predictor() {
        right = wrong = 0;
        predictor_ = new int[512];

        for (int i = 0; i < 512; i++) {
            predictor_[i] = 2;
        }
    }

    ~predictor() {
        delete[]predictor_;
    }

    bool _predictor(u32 order) {
        u32 temp = order >> 23;
        if (predictor_[temp] > 1) return true;
        else return false;
    }

    void update(bool answer, bool real, u32 order) {
        u32 temp = order >> 23;
        if (real) {
            right++;
            if (predictor_[temp] != 3) predictor_[temp]++;
        } else {
            wrong++;
            if (predictor_[temp] != 0) predictor_[temp]--;
        }
    }

    void rate() const {
        std::cout << right << " " << wrong << '\n';
    }


private:

    int *predictor_;
    int right;
    int wrong;

};

predictor pre_;

#endif //UNTITLED1_PREDICTOR_H
