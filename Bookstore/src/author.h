
#ifndef MAIN_CPP_AUTHOR_H
#define MAIN_CPP_AUTHOR_H

#include <iostream>
#include <cstring>
#include <fstream>

class author {

public:
    char name_[62];
    author()=default;

    author(const std::string a) {
        int l=a.size();
        for(int i=0;i<l;i++) name_[i]=a[i];
        name_[l]='\0';
    }
    friend bool operator<(const author &obj1, const author &obj2) {
        return (strcmp(obj1.name_, obj2.name_) < 0);
    }

    friend bool operator==(const author &obj1, const author &obj2) {
        return (strcmp(obj1.name_, obj2.name_) == 0);
    }

    friend bool operator<=(const author &obj1, const author &obj2) {
        return (obj1 < obj2 || obj1 == obj2);
    }

    friend bool operator>(const author &obj1, const author &obj2) {
        return (!(obj1 <= obj2));
    }

    friend bool operator>=(const author &obj1, const author &obj2) {
        return (obj1 > obj2 || obj1 == obj2);
    }

    friend std::ostream &operator<<(std::ostream &x, const author &y) {
        x << y.name_;
        return x;
    }
};

class author_node{
public:
    author index;
    author ISBN;

    author_node() = default;

    author_node(const author &Name, const std::string Value) {
        index = Name;
        author k(Value);
        ISBN = k;
    }

    author_node(const author_node &x) {
        index = x.index;
        ISBN = x.ISBN;
    }

    ~author_node() = default;

    friend bool operator<(const author_node &a, const author_node &b) {
        if (!(a.index == b.index)) {
            return (a.index < b.index);
        }
        return a.ISBN < b.ISBN;
    }

    friend bool operator>(const author_node &a, const author_node &b) {
        if (!(a.index == b.index)) {
            return !(a.index < b.index);
        }
        return a.ISBN > b.ISBN;
    }

    friend bool operator==(const author_node &a, const author_node &b) {
        if (!(a.index == b.index)) {
            return false;
        }
        return a.ISBN == b.ISBN;
    }

    friend bool operator<=(const author_node &a, const author_node &b) {
        return !(a > b);
    }

    friend bool operator>=(const author_node &a, const author_node &b) {
        return !(a < b);
    }

    author_node &operator=(const author_node &p) {
        for(int i=0;i<62;i++) {
            index.name_[i]=p.index.name_[i];
            ISBN.name_[i]=p.ISBN.name_[i];
        }
        return *this;
    }
};

struct author_block {
    author_node sup;
    author_node inf;
    int Next = -1;
    int size = 0;
    author_node data[1000];
};

class author_store {
public:
    const int max = 800;
    const int min = 400;
    int block_number = -1;
    std::fstream file;

    author_store(){
        file.open("author");
        if (file){
            file.seekg(0);
            file.read(reinterpret_cast<char*>(&block_number),sizeof(int));
        }
        else {
            file.open("author",std::fstream::out);
            file.close();
            file.open("author");
            block_number = -1;
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&block_number),sizeof(int));
            file.seekp(sizeof(int));
            author_block tmp;
            file.write(reinterpret_cast<char*>(&tmp),sizeof(tmp));
        }
    }

    ~author_store(){
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&block_number),sizeof(block_number));
        file.close();
    }

    void insert(const author_node& data){
        static author_block now;
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
        static author_block nxt;
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

    void del(const author_node& data){
        static author_block now;
        readblock(0,now);
        int position_now = 0;
        while (position_now != -1){
            if (data >= now.inf && data <=now.sup){
                inner_deltete(position_now,data);
                return;
            }
            if (data < now.inf){
                return;
            }
            position_now = now.Next;
            readblock(position_now,now);
        }
    }

    void find(const author& index_,std::set<std::string> &sett){
        static author_block now;
        int i = 0;
        while (i != -1){
            readblock(i,now);
            if (now.size == 0){
                i = now.Next;
                continue;
            }
            if ((now.inf.index <= index_) && (now.sup.index >= index_)){
                read_data(i,now);
                for (int j = 0;j < now.size;j++){
                    if (now.data[j].index == index_){
                        std::string nn=now.data[j].ISBN.name_;
                        sett.insert(nn);
                    }
                }
            }
            else if(index_ < now.inf.index){
                break;
            }
            i = now.Next;
        }
    }

private:

    void readblock(int pos,author_block& data){
        if (pos < 0)
            return ;
        file.seekg(pos * sizeof(author_block) + sizeof(int));
        file.read(reinterpret_cast<char*>(&data.sup),sizeof(data.sup));
        file.read(reinterpret_cast<char*>(&data.inf),sizeof(data.inf));
        file.read(reinterpret_cast<char*>(&data.Next),sizeof(data.Next));
        file.read(reinterpret_cast<char*>(&data.size),sizeof(data.size));
    }

    void read_data(int pos,author_block& data){
        if (pos < 0)
            return ;
        file.seekg(pos * sizeof(author_block) + sizeof(int));
        file.read(reinterpret_cast<char*>(&data),sizeof(author_block));
    }

    void write_data(int pos, const author_block &data) {
        if (pos < 0)
            return ;
        file.seekp(pos * sizeof(author_block) + sizeof(int));
        file.write(reinterpret_cast<const char *>(&data), sizeof(author_block));
    }

    void inner_insert(int pos,const author_node& data){
        static author_block now;
        read_data(pos,now);
        int tmp = -1;
        for (int i = 0;i < now.size;i++){
            if (data < now.data[i]){
                tmp = i;
                break;
            }
            if (data == now.data[i])
                return;
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

    void inner_deltete(int pos,const author_node& data){
        static author_block now;
        read_data(pos,now);
        for (int i = 0;i < now.size;i++){
            if (now.data[i] > data){
                break;
            }
            if (now.data[i] == data){
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

    void split(int pos,author_block& data){
        block_number++;
        static author_block now;
        static author_block old_one;
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

    void combine(int pos,author_block& data){
        int next_one = data.Next;
        if (next_one == -1){
            write_data(pos,data);
            return;
        }
        static author_block nxt;
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



#endif //MAIN_CPP_AUTHOR_H
