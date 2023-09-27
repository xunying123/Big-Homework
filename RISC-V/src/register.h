//
// Created by xun_y on 2023/6/25.
//

#ifndef UNTITLED1_REGISTER_H
#define UNTITLED1_REGISTER_H

#include<iostream>
#include<string>

using u8 = uint8_t;
using u32 = uint32_t;

class _register {
public:
    _register(int size_) {
        size=size_;
        for(int i=0;i<32;i++) {
            register_[i]=0;
            depenence[i]=0;
        }
        register_[0]=0;
        depenence[0]=0;
    }

    _register(const _register &oo) {
        for(int i=0;i<32;i++) {
            register_[i]=oo.register_[i];
            depenence[i]=oo.depenence[i];
        }
        register_[0]=0;
        depenence[0]=0;
    }

    _register& operator = (const _register &oo) {
        if(this==&oo) return *this;
        for(int i=0;i<32;i++) {
            register_[i]=oo.register_[i];
            depenence[i]=oo.depenence[i];
        }
        register_[0]=0;
        depenence[0]=0;
        return *this;
    }

    long long &ststus(int k) {
        return depenence[k];
    }

    u32 &data (int k) {
        return register_[k];
    }

    void out() {
        for(int i=0;i<32;i++) std::cout<<register_[i]<<' ';
        std::cout<<std::endl;
    }
    int size;
private:
    u32 register_[32];
    long long depenence[32];

};

_register reg_(32);
_register k(reg_);

#endif //UNTITLED1_REGISTER_H
