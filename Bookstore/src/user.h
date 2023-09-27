
#ifndef MAIN_CPP_USER_H
#define MAIN_CPP_USER_H

#include <iostream>
#include <cstring>
#include <fstream>

class User_id {

public:
    char name_[32];
    User_id()=default;

    User_id(const std::string a) {
        int l=a.size();
        for(int i=0;i<l;i++) name_[i]=a[i];
        name_[l]='\0';
    }
    friend bool operator<(const User_id &obj1, const User_id &obj2) {
        return (strcmp(obj1.name_, obj2.name_) < 0);
    }

    friend bool operator==(const User_id &obj1, const User_id &obj2) {
        return (strcmp(obj1.name_, obj2.name_) == 0);
    }

    friend bool operator<=(const User_id &obj1, const User_id &obj2) {
        return (obj1 < obj2 || obj1 == obj2);
    }

    friend bool operator>(const User_id &obj1, const User_id &obj2) {
        return (!(obj1 <= obj2));
    }

    friend bool operator>=(const User_id &obj1, const User_id &obj2) {
        return (obj1 > obj2 || obj1 == obj2);
    }

    friend std::ostream &operator<<(std::ostream &x, const User_id &y) {
        x << y.name_;
        return x;
    }
};

class user_node{
public:
    User_id id;
    std::string password;
    int Privilege;


    user_node() = default;

    user_node(const User_id &Name, const std::string pass,int rank) {
        id = Name;
        password = pass;
        Privilege=rank;
    }

    user_node(const user_node &x) {
        id = x.id;
        password = x.password;
        Privilege=x.Privilege;
    }

    ~user_node() = default;

    friend bool operator<(const user_node &a, const user_node &b) {
        return (a.id < b.id);
    }

    friend bool operator>(const user_node &a, const user_node &b) {
        return !(a.id < b.id);
    }

    friend bool operator==(const user_node &a, const user_node &b) {
        return a.id==b.id;
    }

    friend bool operator<=(const user_node &a, const user_node &b) {
        return !(a > b);
    }

    friend bool operator>=(const user_node &a, const user_node &b) {
        return !(a < b);
    }

    user_node &operator =(const user_node &p) {
        for(int i=0;i<32;i++) {
            id.name_[i]=p.id.name_[i];
        }
        std::string s=p.password;
        this->password=s;
        Privilege=p.Privilege;
        return *this;
    }
};

struct user_block {
    user_node sup;
    user_node inf;
    int Next = -1;
    int size = 0;
    user_node data[1000];
};

class user_store {
public:
    const int max = 800;
    const int min = 400;
    int block_number = -1;
    std::fstream file;

   user_store (){
        file.open("user");
        if (file){
            file.seekg(0);
            file.read(reinterpret_cast<char*>(&block_number),sizeof(int));
        }
        else {
            file.open("user",std::fstream::out);
            file.close();
            file.open("user");
            block_number = -1;
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&block_number),sizeof(int));
            file.seekp(sizeof(int));
            user_block tmp;
            file.write(reinterpret_cast<char*>(&tmp),sizeof(tmp));
            User_id k("root");
            user_node t(k,"sjtu",7);
            insert(t);
        }
    }

    ~user_store (){
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&block_number),sizeof(block_number));
        file.close();
    }

    void insert(const user_node& data){
        static user_block now;
        if (block_number == -1){
            ++block_number;
            now.size = 1;
            now.inf = data;
            now.sup = data;
            now.data[0] = data;
            write_data(0,now);
            return;
        }
        readblock(0,now);
        if (data < now.inf){
            inner_insert(0,data);
            return;
        }
        static user_block nxt;
        int position_now = 0;
        while (now.Next != -1){
            readblock(now.Next,nxt);
            if (data > now.inf && data < nxt.inf){
                inner_insert(position_now,data);
                return;
            }
            position_now = now.Next;
            read_data(position_now,now);
        }
        inner_insert(position_now,data);
    }

    void del(const User_id& index_){
        static user_block now;
        readblock(0,now);
        int position_now = 0;
        while (position_now != -1){
            if (index_ >= now.inf.id && index_ <=now.sup.id){
                inner_deltete(position_now,index_);
                return;
            }
            if (index_ < now.inf.id){
                return;
            }
            position_now = now.Next;
            readblock(position_now,now);
        }
    }

    user_node find(const User_id& index_){
        static user_block now;
        int i = 0;
        while (i != -1){
            readblock(i,now);
            if (now.size == 0){
                i = now.Next;
                continue;
            }
            if ((now.inf.id <= index_) && (now.sup.id >= index_)){
                read_data(i,now);
                for (int j = 0;j < now.size;j++){
                    if (now.data[j].id == index_){
                       return now.data[j];
                    }
                }
            }
            else if(index_ < now.inf.id){
                break;
            }
            i = now.Next;
        }
        User_id k("root");
        user_node t(k,"",7);
        return t;
    }

private:

    void readblock(int pos,user_block& data){
        if (pos < 0)
            return ;
        file.seekg(pos * sizeof(user_block) + sizeof(int));
        file.read(reinterpret_cast<char*>(&data.sup),sizeof(data.sup));
        file.read(reinterpret_cast<char*>(&data.inf),sizeof(data.inf));
        file.read(reinterpret_cast<char*>(&data.Next),sizeof(data.Next));
        file.read(reinterpret_cast<char*>(&data.size),sizeof(data.size));
    }

    void read_data(int pos,user_block& data){
        if (pos < 0)
            return ;
        file.seekg(pos * sizeof(user_block) + sizeof(int));
        file.read(reinterpret_cast<char*>(&data),sizeof(user_block));
    }

    void write_data(int pos, const user_block &data) {
        if (pos < 0)
            return ;
        file.seekp(pos * sizeof(user_block) + sizeof(int));
        file.write(reinterpret_cast<const char *>(&data), sizeof(user_block));
    }

    void inner_insert(int pos,const user_node& data){
        static user_block now;
        read_data(pos,now);
        int tmp = -1;
        for (int i = 0;i < now.size;i++){
            if (data < now.data[i]){
                tmp = i;
                break;
            }
        }
        now.size++;
        if (tmp == -1){
            now.data[now.size - 1] = data;
        }
        else {
            for (int i = now.size - 1;i > tmp;--i){
                now.data[i] = now.data[i - 1];
            }
            now.data[tmp] = data;
        }
        if (now.size > max){
            split(pos,now);
        }
        else {
            now.inf = now.data[0];
            now.sup = now.data[now.size - 1];
            write_data(pos,now);
        }
    }

    void inner_deltete(int pos,const User_id& index_){
        static user_block now;
        read_data(pos,now);
        for (int i = 0;i < now.size;i++){
            if (now.data[i].id > index_){
                break;
            }
            if (now.data[i].id == index_){
                for (int j = i + 1;j < now.size;++j){
                    now.data[j - 1] = now.data[j];
                }
                now.size--;
                if (now.size == 0){
                    write_data(pos,now);
                    return;
                }
                now.inf = now.data[0];
                now.sup = now.data[now.size - 1];
                if (now.size < min){
                    combine(pos,now);
                }
                else {
                    write_data(pos,now);
                }
                return;
            }
        }
    }

    void split(int pos,user_block& data){
        block_number++;
        static user_block now;
        static user_block old_one;
        for (int i = min;i < data.size;++i){
            now.data[i - min] = data.data[i];
            old_one.data[i - min] = data.data[i - min];
        }
        now.size = min + 1;
        old_one.size = min;
        now.Next = data.Next;
        old_one.Next = block_number;
        now.inf = now.data[0];
        now.sup = now.data[now.size - 1];
        old_one.inf = old_one.data[0];
        old_one.sup = old_one.data[old_one.size - 1];
        write_data(pos,old_one);
        write_data(block_number,now);
    }

    void combine(int pos,user_block& data){
        int next_one = data.Next;
        if (next_one == -1){
            write_data(pos,data);
            return;
        }
        static user_block nxt;
        read_data(next_one,nxt);
        if (data.size + nxt.size <= max){
            for (int i = data.size;i < data.size + nxt.size;++i){
                data.data[i] = nxt.data[i - data.size];
            }
            data.size += nxt.size;
            data.Next = nxt.Next;
            data.inf = data.data[0];
            data.sup = data.data[data.size - 1];
        }
        write_data(pos,data);
    }
};


#endif //MAIN_CPP_USER_H
