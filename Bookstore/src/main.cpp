#include<vector>
#include<cmath>
#include<set>
#include"user.h"
#include"keyword.h"
#include "author.h"
#include "bookname.h"
#include "book.h"

struct logging {
    user_node userr;
    std::string selecting;
};

int main() {
    user_store user;
    key_store key;
    author_store author;
    bookname_store bookname;
    book_store book;
    std::vector<logging> online;
    std::string order;
    std::cin >> order;
    while (!std::cin.eof()) {
        if (order == "su") {
            std::string userid, password;
            std::cin >> userid;
            user_node temp = user.find(userid);
            User_id tem("root");
            if (temp.id == tem) {
                std::cin >> password;
                std::cin >> order;
                if (temp.password == "") {
                    std::cout << "Invalid" << std::endl;
                    continue;
                } else {
                    if (temp.password == password) {
                        logging p;
                        p.userr = temp;
                        p.selecting = "";
                        online.push_back(p);
                        continue;
                    } else std::cout << "Invalid" << std::endl;
                }
            } else {
                logging now;
                now = online.back();
                if (now.userr.Privilege > temp.Privilege) {
                    logging p;
                    p.userr = temp;
                    p.selecting = "";
                    online.push_back(p);
                    std::cin >> order;
                    continue;
                }
                std::cin >> password;
                User_id pass(password);
                if (pass == temp.password) {
                    logging pp;
                    pp.userr = temp;
                    pp.selecting = "";
                    online.push_back(pp);
                } else std::cout << "Invalid" << std::endl;
                std::cin >> order;
                continue;
            }
        }
        if (order == "logout") {
            if (!online.empty()) {
                online.pop_back();
            } else std::cout << "Invalid" << std::endl;
            std::cin >> order;
            continue;
        }
        if (order == "register") {
            std::string userid, password, username;
            std::cin >> userid >> password >> username;
            user_node temp = user.find(userid);
            User_id tem("root");
            if (temp.id == tem) {
                User_id k(userid);
                user_node t(k, password, 1);
                user.insert(t);
            } else std::cout << "Invalid" << std::endl;
            std::cin >> order;
            continue;
        }
        if (order == "passwd") {
            std::string userid, password, uword;
            std::cin >> userid;
            user_node temp = user.find(userid);
            User_id tem("root");
            if (temp.password == "") {
                std::cin >> password >> uword;
                std::cin >> order;
                std::cout << "Invalid" << std::endl;
                continue;
            } else {
                logging now;
                now = online.back();
                if (now.userr.Privilege == 7) {
                    std::cin >> password;
                    user.del(temp.id);
                    temp.password = password;
                    user.insert(temp);
                } else {
                    std::cin >> uword >> password;
                    User_id pass(uword);
                    if (pass == temp.password) {
                        user.del(temp.id);
                        temp.password = password;
                        user.insert(temp);
                    } else std::cout << "Invalid" << std::endl;
                }
                std::cin >> order;
                continue;
            }
        }
        if (order == "useradd") {
            int k;
            std::string userid, password, username;
            std::cin >> userid >> password >> k >> username;
            logging now;
            now = online.back();
            if (now.userr.Privilege >= k) {
                user_node temp = user.find(userid);
                User_id tem("root");
                if (temp.password == "") {
                    User_id m(userid);
                    user_node t(m, password, k);
                    user.insert(t);
                } else std::cout << "Invalid" << std::endl;
            } else std::cout << "Invalid" << std::endl;
            std::cin >> order;
            continue;
        }
        if (order == "delete") {
            std::string userid;
            std::cin >> userid;
            bool has = 0;
            User_id t(userid);
            user_node tempp = user.find(userid);
            if (tempp.password == "") {
                std::cout << "Invalid" << std::endl;
                std::cin >> order;
                continue;
            }
            for (auto itea = online.begin(); itea != online.end(); itea++) {
                if ((itea->userr).id == t) has = 1;
            }
            if (!has) {
                user.del(userid);
            } else std::cout << "Invalid" << std::endl;
            std::cin >> order;
            continue;
        }
        if (order == "show") {
            std::cin >> order;
            if (order == "finance") {

            }
            if (order[0] == '-') {
                int j;
                if (order[1] == 'I') {
                    j = 6;
                    if(order.size()==6) {
                        std::cout << "Invalid" << std::endl;
                        std::cin >> order;
                        continue;
                    }
                }
                if (order[1] == 'n') {
                    j = 7;
                    if(order.size()==6) {
                        std::cout << "Invalid" << std::endl;
                        std::cin >> order;
                        continue;
                    }
                }
                if (order[1] == 'a') {
                    j = 10;
                    if(order.size()==8) {
                        std::cout << "Invalid" << std::endl;
                        std::cin >> order;
                        continue;
                    }
                }
                if (order[1] == 'k') {
                    j = 11;
                    if(order.size()==9) {
                        std::cout << "Invalid" << std::endl;
                        std::cin >> order;
                        continue;
                    }
                }
                std::string nn = "";
                for (int g = j; g < order.size(); g++) {
                    if (order[g] != '"')
                        nn += order[g];
                }
                std::set<std::string> sett;
                if (order[1] == 'n') bookname.find(nn, sett);
                if (order[1] == 'a') author.find(nn, sett);
                if (order[1] == 'k') {
                    for(int a=0;a<nn.size();a++) {
                        if(nn[a]=='|') {
                            std::cout << "Invalid" << std::endl;
                            std::cin >> order;
                            continue;
                        }
                    }
                    key.find(nn, sett);
                }
                if (order[1] == 'I') {
                    book_node te = book.find(nn);
                    std::cout << te.index.name_ << '\t' << te.Bookname << "\t" << te.Author << "\t"
                              << te.Keyword << '\t' << te.Price << '\t' << te.Quantity << std::endl;
                } else {
                    if (!sett.empty()) {
                        for (auto itea = sett.begin(); itea != sett.end(); itea++) {
                            book_node te = book.find(*itea);
                            std::cout << te.index.name_ << '\t' << te.Bookname << "\t" << te.Author << "\t"
                                      << te.Keyword << '\t' << te.Price << '\t' << te.Quantity << std::endl;
                        }
                    }
                }

            } else book.book_out();
            std::cin >> order;
            continue;
        }
        if (order == "buy") {
            std::string is, qu;
            std::cin >> is >> qu;
            bookbook x(is);
            book_node k = book.find(x);
            if(k.Bookname=="") {
                std::cout << "Invalid" << std::endl;
                std::cin >> order;
                continue;
            }
            int Quan = 0;
            bool sign = 1;
            for (int j = 0; j < qu.size(); j++) {
                if (qu[j] >= '0' && qu[j] <= '9') Quan = Quan * 10 + qu[j] - '0';
                else {
                    sign = 0;
                    break;
                }
            }
            if (sign) {
                k.Quantity -= Quan;
                std::cout << Quan * k.Price << std::endl;
                book.del(is);
                book.insert(k);
            }else std::cout << "Invalid" << std::endl;
            std::cin >> order;
            continue;
        }
        if (order == "select") {
            std::string is;
            std::cin >> is;
            bookbook x(is);
            book_node k = book.find(x);
            if (k.Bookname == "" && k.Author == "") {
                book.del(is);
                book.insert(k);
                logging t = online.back();
                online.pop_back();
                t.selecting = is;
                online.push_back(t);
            }
            std::cin >> order;
            continue;
        }
        if (order == "modify") {
            std::cin >> order;
            if (order[0] == '-') {
                if (!online.empty()) {
                    logging t;
                    t = online.back();
                    if (t.selecting != "") {
                        bookbook x(t.selecting);
                        book_node k = book.find(x);
                        book.del(k.index);
                        while (order[0] == '-') {
                            int j;
                            if (order[1] == 'I') {
                                j = 6;
                                if(order.size()==6) {
                                    std::cout << "Invalid" << std::endl;
                                    std::cin >> order;
                                    continue;
                                }
                            }
                            if (order[1] == 'n') {
                                j = 7;
                                if(order.size()==6) {
                                    std::cout << "Invalid" << std::endl;
                                    std::cin >> order;
                                    continue;
                                }
                            }
                            if (order[1] == 'a') {
                                j = 10;
                                if(order.size()==8) {
                                    std::cout << "Invalid" << std::endl;
                                    std::cin >> order;
                                    continue;
                                }
                            }
                            if (order[1] == 'k') {
                                j = 11;
                                if(order.size()==9) {
                                    std::cout << "Invalid" << std::endl;
                                    std::cin >> order;
                                    continue;
                                }
                            }
                            if (order[1] == 'p') {
                                int Quan = 0;
                                int count = 0;
                                bool sign = 1;
                                for (int g = 7; g < order.size(); g++) {
                                    if (order[g] >= '0' && order[g] <= '9') {
                                        Quan = Quan * 10 + order[g] - '0';
                                        if (count != 0) count++;
                                    } else if (order[g] == '.') {
                                        count = 1;
                                    } else {
                                        sign = 0;
                                        break;
                                    }
                                }
                                if (sign) {
                                    double kk = Quan / pow(10, count - 1);
                                    k.Price = kk;
                                }
                            } else {
                                std::string nn = "";
                                for (int g = j; g < order.size(); g++) {
                                    if (order[g] != '"')
                                        nn += order[g];
                                }
                                if (j == 6) {
                                    bookbook tt(nn);
                                    if (!(k.index == tt)) {
                                        k.index = tt;
                                    }else std::cout << "Invalid" << std::endl;
                                }
                                if (j == 7) {
                                    std::string gg = k.index.name_;
                                    bookname_node tt(k.Bookname, gg);
                                    bookname.del(tt);
                                    k.Bookname = nn;
                                    tt.index = nn;
                                    bookname.insert(tt);
                                }
                                if (j == 10) {
                                    std::string gg = k.index.name_;
                                    author_node tt(k.Author, gg);
                                    author.del(tt);
                                    k.Author = nn;
                                    tt.index = nn;
                                    author.insert(tt);
                                }
                                if (j == 11) {
                                    std::string rr = "";
                                    for (int g = 0; g < k.Keyword.size(); g++) {
                                        if (k.Keyword[g] != '|') rr += k.Keyword[g];
                                        else {
                                            std::string gg = k.index.name_;
                                            key_node temp(rr, gg);
                                            key.del(temp);
                                            rr = "";
                                        }
                                    }
                                    std::string ggg = k.index.name_;
                                    key_node tttemp(rr, ggg);
                                    key.del(tttemp);
                                    rr = "";
                                    for (int g = 0; g < nn.size(); g++) {
                                        if (nn[g] != '|') rr += nn[g];
                                        else {
                                            std::string gg = k.index.name_;
                                            key_node temp(rr, gg);
                                            key.insert(temp);
                                            rr = "";
                                        }
                                    }
                                    std::string gg = k.index.name_;
                                    key_node temp(rr, gg);
                                    key.del(temp);
                                    rr = "";
                                }
                            }
                            std::cin >> order;
                        }
                        book.insert(k);
                        continue;
                    }else std::cout << "Invalid" << std::endl;
                }else std::cout << "Invalid" << std::endl;
                std::cin >> order;
                continue;

            }
        }
        if (order == "import") {
            if (!online.empty()) {
                logging t;
                t = online.back();
                if (t.selecting != "") {
                    bookbook x(t.selecting);
                    book_node k = book.find(x);
                    std::string quan;
                    std::cin >> quan;
                    if (quan[0] != '-') {
                        int Quan = 0;
                        bool sign = 1;
                        for (int j = 0; j < quan.size(); j++) {
                            if (quan[j] >= '0' && quan[j] <= '9') Quan = Quan * 10 + quan[j] - '0';
                            else {
                                sign = 0;
                                break;
                            }
                        }
                        double cost;
                        std::cin >> cost;
                        if (sign && cost > 0) {
                            k.Quantity += Quan;
                            book.del(x);
                            book.insert(k);
                        }else std::cout << "Invalid" << std::endl;
                    }
                }else std::cout << "Invalid" << std::endl;
            }
            std::cin >> order;
            continue;
        }
        if (order == "quit") {
            online.clear();
            break;
        }
        if (order == "exit") {
            online.clear();
            break;
        }
        if (order == "log") {

        }
    }
    return 0;
}

