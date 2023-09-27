#ifndef SJTU_BIGINTEGER
#define SJTU_BIGINTEGER

// Integer 1:
// 实现一个有符号的大整数类，只需支持简单的加减

// Integer 2:
// 实现一个有符号的大整数类，支持加减乘除，并重载相关运算符

// 请不要使用除了以下头文件之外的其它头文件
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

// 请不要使用 using namespace std;

namespace sjtu {
    class int2048 {
        // todo
    public:
        std::vector<unsigned long long> number_;
        bool sign_;

        // 构造函数
        inline int2048();

        inline int2048(long long x);

        inline int2048(const std::string &s);

        inline int2048(const int2048 &);

        // 以下给定函数的形式参数类型仅供参考，可自行选择使用常量引用或者不使用引用
        // 如果需要，可以自行增加其他所需的函数
        // ===================================
        // Integer1
        // ===================================

        // 读入一个大整数
        inline void read(const std::string &s);

        // 输出储存的大整数
        inline void print();

        // 加上一个大整数
        inline int2048 &add(const int2048 &p);

        // 输出两个大整数之和
        inline friend int2048 add(int2048 &p, const int2048 &q);

        // 减去一个大整数
        inline int2048 &minus(const int2048 &);

        // 输出两个大整数之差
        inline friend int2048 minus(int2048 &p, const int2048 &q);

        inline friend int2048 sum(const int2048 &p, const int2048 &q);

        inline friend int2048 sub(const int2048 &m, const int2048 &n);

        inline int2048 &clear_0();

        // ===================================
        // Integer2
        // ===================================

        inline int2048 &operator=(const int2048 &);

        inline int2048 &operator+=(int2048);

        inline friend int2048 operator+(int2048, const int2048 &);

        inline int2048 &operator-=(int2048);

        inline friend int2048 operator-(int2048, const int2048 &);

        inline int2048 &operator*=(const int2048 &);

        inline friend int2048 operator*(const int2048 &, const int2048 &);

        inline int2048 &operator/=(const int2048 &);

        inline friend int2048 operator/(int2048 &, const int2048 &);

        inline friend std::istream &operator>>(std::istream &, int2048 &);

        inline friend std::ostream &operator<<(std::ostream &, const int2048 &);

        inline friend bool operator==(const int2048 &p, const int2048 &q);

        inline friend bool operator!=(const int2048 &, const int2048 &);

        inline friend bool operator<(const int2048 &, const int2048 &);

        inline friend bool operator>(const int2048 &, const int2048 &);

        inline friend bool operator<=(const int2048 &, const int2048 &);

        inline friend bool operator>=(const int2048 &, const int2048 &);


    };

    inline sjtu::int2048::int2048() {
        number_.push_back(0);
        sign_ = false;
    }

    inline sjtu::int2048::int2048(long long x) {
        long long temp1_ = x;
        unsigned long long temp_;
        if (temp1_ < 0) {
            sign_ = true;
            temp1_ = 0 - temp1_;
        } else sign_ = false;
        temp_ = temp1_ % (unsigned long long) (1e9);
        number_.push_back(temp_);
        if (temp_ != temp1_) {
            temp_ = temp1_ / (unsigned long long) (1e9);
            if (temp_ >= 1e9) {
                temp1_ = temp_ % (unsigned long long) (1e9);
                number_.insert(number_.begin(), temp1_);
                temp_ = temp_ / (unsigned long long) (1e9);
                number_.insert(number_.begin(), temp_);
            } else {
                number_.insert(number_.begin(), temp_);
            }

        }
    }

    inline sjtu::int2048::int2048(const std::string &s) {
        unsigned long long l_ = s.length(), num_ = 0, count_ = 0, ratio = 1;
        if (s[0] == '-') {
            sign_ = true;
            if (s[1] == 0) sign_ = false;
            for (unsigned long long i = l_; i >= 2; i--) {
                count_++;
                num_ = num_ + (s[i - 1] - '0') * ratio;
                ratio *= 10;
                if (count_ == 9) {
                    number_.insert(number_.begin(), num_);
                    count_ = 0;
                    num_ = 0;
                    ratio = 1;
                }
            }
            if (count_ != 0) number_.insert(number_.begin(), num_);
        } else {
            sign_ = false;
            for (unsigned long long i = l_; i >= 1; i--) {
                count_++;
                num_ = num_ + (s[i - 1] - '0') * ratio;
                ratio *= 10;
                if (count_ == 9) {
                    number_.insert(number_.begin(), num_);
                    count_ = 0;
                    ratio = 1;
                    num_ = 0;
                }
            }
            if (count_ != 0) number_.insert(number_.begin(), num_);
        }
        clear_0();
    }

    inline sjtu::int2048::int2048(const int2048 &p) {
        sign_ = p.sign_;
        for (auto iter = p.number_.begin(); iter != p.number_.end(); iter++) {
            number_.push_back(*iter);
        }
        clear_0();
    }

    inline void sjtu::int2048::read(const std::string &s) {
        number_.clear();
        unsigned long long l = s.length(), num = 0, count = 0, ratio = 1;
        if (s[0] == '-') {
            sign_ = true;
            if (s[1] == 0) sign_ = false;
            for (unsigned long long i = l; i >= 2; i--) {
                count++;
                num = num + (s[i - 1] - '0') * ratio;
                ratio *= 10;
                if (count == 9) {
                    number_.insert(number_.begin(), num);
                    count = 0;
                    num = 0;
                    ratio = 1;
                }
            }
            if (count != 0) number_.insert(number_.begin(), num);
        } else {
            sign_ = false;
            for (unsigned long long i = l; i >= 1; i--) {
                count++;
                num = num + (s[i - 1] - '0') * ratio;
                ratio *= 10;
                if (count == 9) {
                    number_.insert(number_.begin(), num);
                    count = 0;
                    num = 0;
                    ratio = 1;
                }
            }
            if (count != 0) number_.insert(number_.begin(), num);
        }
        clear_0();
    }

    // 输出储存的大整数
    inline void sjtu::int2048::print() {
        if (sign_) {
            auto iter = number_.begin();
            if (*iter != 0) std::cout << "-";
        }
        for (auto iter = number_.begin(); iter != number_.end(); iter++) {
            if (iter != number_.begin()) {
                if (*iter == 0) {
                    std::cout << "000000000";
                } else if (*iter < (unsigned long long) 1e8) {
                    unsigned long long temp_1 = *iter;
                    while (temp_1 < (unsigned long long) 1e8) {
                        std::cout << 0;
                        temp_1 *= 10;
                    }
                }
            }
            std::cout << *iter;
        }
    }

    // 加上一个大整数
    inline sjtu::int2048 &sjtu::int2048::add(int2048 const &p) {
        int2048 temp_(p);
        if ((sign_ && temp_.sign_) || ((!sign_) && (!temp_.sign_))) {
            *this = sum(*this, temp_);
        } else if (sign_ && (!temp_.sign_)) {
            sign_ = false;
            *this = sub(*this, temp_);
            sign_ = !sign_;
        } else if ((!sign_) && temp_.sign_) {
            temp_.sign_ = false;
            *this = sub(*this, temp_);
            temp_.sign_ = true;
        }
        clear_0();
        return *this;
    }

    // 输出两个大整数之和
    inline sjtu::int2048 add(int2048 &p, const int2048 &q) {
        int2048 temp_(0), temp_1(q);
        if ((p.sign_ && temp_1.sign_) || ((!p.sign_) && (!temp_1.sign_))) {
            temp_ = sum(p, temp_1);
        } else if (p.sign_ && (!temp_1.sign_)) {
            p.sign_ = false;
            temp_ = sub(p, temp_1);
            temp_.sign_ = !temp_.sign_;
            p.sign_ = true;
        } else if ((!p.sign_) && temp_1.sign_) {
            temp_1.sign_ = false;
            temp_ = sub(p, temp_1);
            temp_1.sign_ = true;
        }
        temp_.clear_0();
        return temp_;
    }

    // 减去一个大整数
    inline sjtu::int2048 &sjtu::int2048::minus(const int2048 &p) {
        int2048 temp_(p);
        if ((sign_ && temp_.sign_) || ((!sign_) && (!temp_.sign_))) {
            *this = sub(*this, temp_);
        } else if (sign_ && (!temp_.sign_)) {
            sign_ = false;
            *this = sum(*this, temp_);
            sign_ = true;
        } else if ((!sign_) && temp_.sign_) {
            temp_.sign_ = false;
            *this = sum(*this, temp_);
            temp_.sign_ = true;
        }
        clear_0();
        return *this;
    }

    // 输出两个大整数之差
    inline sjtu::int2048 minus(int2048 &p, const int2048 &q) {
        int2048 temp_(0), temp_1(q);
        if ((p.sign_ && temp_1.sign_) || ((!p.sign_) && (!temp_1.sign_))) {
            temp_ = sub(p, temp_1);
        } else if (p.sign_ && (!temp_1.sign_)) {
            p.sign_ = false;
            temp_ = sum(p, temp_1);
            p.sign_ = true;
            temp_.sign_ = true;
        } else if ((!p.sign_) && temp_1.sign_) {
            temp_1.sign_ = false;
            temp_ = sum(p, temp_1);
            temp_1.sign_ = true;
        }
        temp_.clear_0();
        return temp_;
    }

    inline sjtu::int2048 sum(const int2048 &p, const int2048 &q) {
        int2048 temp1_(0);
        temp1_.number_.clear();
        unsigned long long temp_;
        int p_size, q_size, size_, status_ = 0;
        p_size = p.number_.size();
        q_size = q.number_.size();
        size_ = std::min(p_size, q_size);
        auto iter1 = p.number_.end();
        auto iter2 = q.number_.end();
        temp1_.sign_ = p.sign_;
        while (size_ != 0) {
            iter1--;
            iter2--;
            size_--;
            temp_ = *iter1 + *iter2;
            if (status_ == 1) {
                status_ = 0;
                temp_++;
            }
            if (temp_ >= (unsigned long long) 1e9) {
                status_ = 1;
                temp_ -= 1e9;
            }
            temp1_.number_.insert(temp1_.number_.begin(), temp_);
        }
        if (p_size == q_size) {
            if (status_ == 1) temp1_.number_.insert(temp1_.number_.begin(), 1);
        } else {
            do {
                if (iter1 != p.number_.begin()) {
                    iter1--;
                    temp_ = *iter1;
                    if (status_ == 1) {
                        status_ = 0;
                        temp_++;
                    }
                    if (temp_ >= (unsigned long long) 1e9) {
                        status_ = 1;
                        temp_ -= 1e9;
                    }
                    temp1_.number_.insert(temp1_.number_.begin(), temp_);
                }
                if (iter2 != q.number_.begin()) {
                    iter2--;
                    temp_ = *iter2;
                    if (status_ == 1) {
                        status_ = 0;
                        temp_++;
                    }
                    if (temp_ >= (unsigned long long) 1e9) {
                        status_ = 1;
                        temp_ -= 1e9;
                    }
                    temp1_.number_.insert(temp1_.number_.begin(), temp_);
                }
            } while ((iter1 != p.number_.begin()) || (iter2 != q.number_.begin()));
        }
        return temp1_;
    }

    inline sjtu::int2048 sub(const int2048 &m, const int2048 &n) {
        int2048 temp1_(0), p(m), q(n);
        bool status_sign;
        std::vector<unsigned long long> gg;
        temp1_.number_.clear();
        temp1_.sign_ = q.sign_;
        long long temp_;
        int p_size, q_size, size_, status_ = 0;
        p_size = p.number_.size();
        q_size = q.number_.size();
        size_ = std::min(p_size, q_size);
        auto iter1 = p.number_.end();
        auto iter2 = q.number_.end();
        status_sign = p.sign_;
        p.sign_ = q.sign_ = false;
        if (p < q) {
            while (size_ != 0) {
                iter1--;
                iter2--;
                size_--;
                temp_ = *iter2 - *iter1;
                if (status_ == 1) {
                    status_ = 0;
                    temp_--;
                }
                if (temp_ < 0) {
                    status_ = 1;
                    temp_ += 1e9;
                }
                gg.push_back(temp_);
            }
            while (iter2 != q.number_.begin()) {
                iter2--;
                temp_ = *iter2;
                if (status_ == 1) {
                    status_ = 0;
                    temp_--;
                }
                if (temp_ < 0) {
                    status_ = 1;
                    temp_ += 1e9;
                }
                if (!((iter2 == q.number_.begin()) && temp_ == 0)) {
                    gg.push_back(temp_);
                }
            }
            auto iter1 = gg.end();
            while (iter1 != gg.begin()) {
                iter1--;
                temp1_.number_.push_back(*iter1);
            }
            temp1_.sign_ = !status_sign;
            return temp1_;
        }
        if (p > q) {
            while (size_ != 0) {
                iter1--;
                iter2--;
                size_--;
                temp_ = *iter1 - *iter2;
                if (status_ == 1) {
                    status_ = 0;
                    temp_--;
                }
                if (temp_ < 0) {
                    status_ = 1;
                    temp_ += 1e9;
                }
                gg.push_back(temp_);
            }
            while (iter1 != p.number_.begin()) {
                iter1--;
                temp_ = *iter1;
                if (status_ == 1) {
                    status_ = 0;
                    temp_--;
                }
                if (temp_ < 0) {
                    status_ = 1;
                    temp_ += 1e9;
                }
                if (!((iter1 == p.number_.begin()) && temp_ == 0)) {
                    gg.push_back(temp_);
                }
            }
            auto iter1 = gg.end();
            while (iter1 != gg.begin()) {
                iter1--;
                temp1_.number_.push_back(*iter1);
            }
            temp1_.sign_ = status_sign;
            return temp1_;
        }
        if (p == q) {
            temp1_.number_.push_back(0);
            p.sign_ = q.sign_ = status_sign;
        }

        return temp1_;
    }

    inline sjtu::int2048 &sjtu::int2048::clear_0() {
        auto iter_ = number_.begin();
        if (number_.empty()) return *this;
        while (1) {
            if ((*iter_ == 0) && (number_.size() != 1)) {
                number_.erase(number_.begin(), number_.begin() + 1);
                iter_ = number_.begin();
            } else break;
        }
        return *this;
    }

    inline sjtu::int2048 &sjtu::int2048::operator=(const int2048 &p) {
        sign_ = p.sign_;
        number_.clear();
        for (auto iter = p.number_.begin(); iter != p.number_.end(); iter++) {
            number_.push_back(*iter);
        }
        return *this;
    }

    inline sjtu::int2048 &sjtu::int2048::operator+=(int2048 p) {
        add(p);
        return *this;
    }

    inline sjtu::int2048 operator+(int2048 p, const int2048 &q) {
        int2048 temp_(0);
        temp_ = add(p, q);
        return temp_;
    }

    inline sjtu::int2048 &sjtu::int2048::operator-=(int2048 p) {
        minus(p);
        return *this;
    }

    inline sjtu::int2048 operator-(int2048 p, const int2048 &q) {
        int2048 temp_(0);
        temp_ = minus(p, q);
        return temp_;
    }

    inline sjtu::int2048 operator*(const int2048 &p, const int2048 &q) {
        if (p.number_.size() == 1) {
            int2048 ans(0);
            std::vector<unsigned long long> g;
            ans.number_.clear();
            unsigned long long temp_1, adding_ = 0;
            auto iter = q.number_.end();
            while (iter != q.number_.begin()) {
                iter--;
                temp_1 = (p.number_[0]) * (*iter);
                adding_ += temp_1;
                unsigned long long hh = adding_;
                if (hh >= 1e9) {
                    adding_ = hh / (unsigned long long) 1e9;
                    hh %= (unsigned long long) 1e9;
                    g.push_back(hh);
                } else {
                    adding_ = 0;
                    g.push_back(hh);
                }
            }
            if (adding_ != 0) g.push_back(adding_);
            auto iter1 = g.end();
            while (iter1 != g.begin()) {
                iter1--;
                ans.number_.push_back(*iter1);
            }
            ans.clear_0();
            if (p.sign_ != q.sign_) ans.sign_ = true;
            return ans;
        }
        if (q.number_.size() == 1) {
            int2048 ans(0);
            std::vector<unsigned long long> g;
            ans.number_.clear();
            unsigned long long temp_1, adding_ = 0;
            auto iter = p.number_.end();
            while (iter != p.number_.begin()) {
                iter--;
                temp_1 = (q.number_[0]) * (*iter);
                adding_ += temp_1;
                unsigned long long hh = adding_;
                if (hh >= 1e9) {
                    adding_ = hh / (unsigned long long) 1e9;
                    hh %= (unsigned long long) 1e9;
                    g.push_back(hh);
                } else {
                    g.push_back(hh);
                    adding_ = 0;
                }
            }
            if (adding_ != 0) g.push_back(adding_);
            auto iter1 = g.end();
            while (iter1 != g.begin()) {
                iter1--;
                ans.number_.push_back(*iter1);
            }
            ans.clear_0();
            if (p.sign_ != q.sign_) ans.sign_ = true;
            return ans;
        }
        if (p >= q) {
            int i = 0;
            int2048 ans(0);
            ans.number_.clear();
            unsigned long long temp_1, adding_;
            auto iter2 = q.number_.end();
            while (iter2 != q.number_.begin()) {
                int d = ans.number_.size() - i;
                iter2--;
                adding_ = 0;
                auto iter1 = p.number_.end();
                while (iter1 != p.number_.begin()) {
                    iter1--;
                    temp_1 = (*iter1) * (*iter2);
                    adding_ += temp_1;
                    if (d == 0) {
                        ans.number_.insert(ans.number_.begin(), 1, adding_);
                        if (ans.number_[0] >= 1e9) {
                            adding_ = ans.number_[0] / (unsigned long long) 1e9;
                            ans.number_[0] %= (unsigned long long) 1e9;
                        } else adding_ = 0;
                    } else {
                        d--;
                        ans.number_[d] += adding_;
                        if (ans.number_[d] >= 1e9) {
                            adding_ = ans.number_[d] / (unsigned long long) 1e9;
                            ans.number_[d] %= (unsigned long long) 1e9;
                        } else adding_ = 0;
                    }
                }
                if (adding_ != 0) ans.number_.insert(ans.number_.begin(), 1, adding_);
                i++;
            }
            if (p.sign_ != q.sign_) ans.sign_ = true;
            ans.clear_0();
            return ans;
        }
        if (p < q) {
            int i = 0;
            int2048 ans(0);
            ans.number_.clear();
            unsigned long long temp_1, adding_;
            auto iter2 = p.number_.end();
            while (iter2 != p.number_.begin()) {
                int d = ans.number_.size() - i;
                iter2--;
                adding_ = 0;
                auto iter1 = q.number_.end();
                while (iter1 != q.number_.begin()) {
                    iter1--;
                    temp_1 = (*iter1) * (*iter2);
                    adding_ += temp_1;
                    if (d == 0) {
                        ans.number_.insert(ans.number_.begin(), 1, adding_);
                        if (ans.number_[0] >= 1e9) {
                            adding_ = ans.number_[0] / (unsigned long long) 1e9;
                            ans.number_[0] %= (unsigned long long) 1e9;
                        } else adding_ = 0;
                    } else {
                        d--;
                        ans.number_[d] += adding_;
                        if (ans.number_[d] >= 1e8) {
                            adding_ = ans.number_[d] / (unsigned long long) 1e9;
                            ans.number_[d] %= (unsigned long long) 1e9;
                        } else adding_ = 0;
                    }
                }
                if (adding_ != 0) ans.number_.insert(ans.number_.begin(), 1, adding_);
                i++;
            }
            if (p.sign_ != q.sign_) ans.sign_ = true;
            ans.clear_0();
            return ans;
        }
    }

    inline sjtu::int2048 &sjtu::int2048::operator*=(const int2048 &p) {
        *this = (*this) * p;
        return *this;
    }

    inline sjtu::int2048 &sjtu::int2048::operator/=(const int2048 &p) {
        *this = (*this) / p;
        return *this;
    }

    inline sjtu::int2048 operator/(int2048 &p, const int2048 &q) {
        int2048 divided(0);
        divided.number_.clear();
        unsigned long long length1, length2;
        int2048 ans(0);
        auto iter = p.number_.begin();
        length1 = p.number_.size();
        length2 = q.number_.size();
        if (length1 < length2) {
            if (p.sign_ != q.sign_) ans.sign_ = true;
            return ans;
        } else if ((length1 == length2) && (p.number_[0] < q.number_[0])) {
            if (p.sign_ != q.sign_) ans.sign_ = true;
            return ans;
        } else {
            for (int i = 0; i < length2; i++) {
                divided.number_.push_back(p.number_[i]);
                iter++;
            }
        }
        iter--;
        int x = 0;
        while (iter != p.number_.end()) {
            if (x != 0) {
                divided.number_.push_back(*iter);
            }
            iter++;
            x = 1;
            if (divided < q) {
                ans.number_.push_back(0);
                continue;
            }
            if (divided == q) {
                ans.number_.push_back(1);
                divided = 0;
                continue;
            }
            if (divided > q) {
                unsigned long long left, right, mid;
                if (divided.number_[0] > q.number_[0]) {
                    left = divided.number_[0] / (q.number_[0] + 1);
                    right = (divided.number_[0]) / (q.number_[0]) + 1;
                } else {
                    unsigned long long t;
                    t = divided.number_[0] * (unsigned long long) 1e9 + divided.number_[1];
                    left = t / (q.number_[0] + 1);
                    right = (t) / (q.number_[0]) + 1;
                }
                while (1) {
                    if (right - left == 1) {
                        ans.number_.push_back(left);
                        divided = divided - left * q;
                        break;
                    }
                    mid = (left + right) / 2;
                    int2048 t = divided - mid * q;
                    if (t < 0) {
                        right = mid;
                        continue;
                    }
                    if (t == 0) {
                        ans.number_.push_back(mid);
                        divided = 0;
                        break;
                    }
                    if (t > 0) {
                        left = mid;
                    }
                }
            }
        }
        if (p.sign_ != q.sign_) ans.sign_ = true;
        ans.clear_0();
        return ans;
    }

    inline std::ostream &operator<<(std::ostream &out, const int2048 &p) {
        if (p.sign_) {
            auto iter = p.number_.begin();
            if (*iter != 0) out << "-";
        }
        for (auto iter = p.number_.begin(); iter != p.number_.end(); iter++) {
            if (iter != p.number_.begin()) {
                if (*iter == 0) {
                    out << "00000000";
                } else if (*iter < (unsigned long long) 1e8) {
                    unsigned long long temp_1 = *iter;
                    while (temp_1 < (unsigned long long) 1e8) {
                        out << '0';
                        temp_1 *= 10;
                    }
                }
            }
            out << *iter;
        }
        return out;
    }

    inline std::istream &operator>>(std::istream &in, int2048 &p) {
        std::string s;
        in >> s;
        int2048 a(s);
        p = a;
        return in;
    }

    inline bool operator==(const int2048 &p, const int2048 &q) {
        if (p.sign_ == q.sign_) {
            int p_size, q_size;
            p_size = p.number_.size();
            q_size = q.number_.size();
            if (p_size != q_size) return false;
            if (p_size == q_size) {
                auto iter1 = p.number_.begin();
                auto iter2 = q.number_.begin();
                while (p_size != 0) {
                    p_size--;
                    if (*iter1 != *iter2) return false;
                    iter1++;
                    iter2++;
                }
                return true;
            }
        } else return false;
        return true;
    }

    inline bool operator!=(const int2048 &p, const int2048 &q) {
        return !(p == q);
    }

    inline bool operator<(const int2048 &p, const int2048 &q) {
        int p_size, q_size;
        bool status_;
        if (p == q) return false;
        if ((!p.sign_) && q.sign_) return false;
        if (p.sign_ && (!q.sign_)) return true;
        p_size = p.number_.size();
        q_size = q.number_.size();
        if (p_size < q_size) status_ = 1;
        if (p_size > q_size) status_ = 0;
        if (p_size == q_size) {
            auto iter1 = p.number_.begin();
            auto iter2 = q.number_.begin();
            while (p_size != 0) {
                p_size--;
                if (*iter1 < *iter2) {
                    status_ = 1;
                    break;
                }
                if (*iter1 > *iter2) {
                    status_ = 0;
                    break;
                }
                iter1++;
                iter2++;
            }
        }
        if (p.sign_) return !status_;
        else return status_;
    }

    inline bool operator>(const int2048 &p, const int2048 &q) {
        if ((p == q) || (p < q)) return false;
        else return true;
    }

    inline bool operator<=(const int2048 &p, const int2048 &q) {
        if ((p < q) || (p == q)) return true;
        else return false;
    }

    inline bool operator>=(const int2048 &p, const int2048 &q) {
        if ((p > q) || (p == q)) return true;
        else return false;
    }
} // namespace sjtu

#endif