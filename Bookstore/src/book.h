

#ifndef MAIN_CPP_BOOK_H
#define MAIN_CPP_BOOK_H

#include <iostream>
#include <cstring>
#include <fstream>

class bookbook {
public:
    char name_[62];
    bookbook() = default;

    bookbook(const std::string a) {
        int l = a.size();
        for (int i = 0; i < l; i++) name_[i] = a[i];
        name_[l] = '\0';
    }

    friend bool operator<(const bookbook &obj1, const bookbook &obj2) {
        return (strcmp(obj1.name_, obj2.name_) < 0);
    }

    friend bool operator==(const bookbook &obj1, const bookbook &obj2) {
        return (strcmp(obj1.name_, obj2.name_) == 0);
    }

    friend bool operator<=(const bookbook &obj1, const bookbook &obj2) {
        return (obj1 < obj2 || obj1 == obj2);
    }

    friend bool operator>(const bookbook &obj1, const bookbook &obj2) {
        return (!(obj1 <= obj2));
    }

    friend bool operator>=(const bookbook &obj1, const bookbook &obj2) {
        return (obj1 > obj2 || obj1 == obj2);
    }

    friend std::ostream &operator<<(std::ostream &x, const bookbook &y) {
        x << y.name_;
        return x;
    }
};

class book_node {
public:
    bookbook index;
    std::string Bookname;
    std::string Author;
    std::string Keyword;
    int Quantity;
    double Price;

    book_node() = default;

    book_node(const bookbook &Name, const std::string bn, const std::string au, int qu, double pr) {
        index = Name;
        Bookname = bn;
        Author = au;
        Quantity = qu;
        Price = pr;
    }

    book_node(const book_node &x) {
        index = x.index;
        Bookname = x.Bookname;
        Author = x.Author;
        Keyword = x.Keyword;
        Quantity = x.Quantity;
        Price = x.Price;
    }

    ~book_node() = default;

    friend bool operator<(const book_node &a, const book_node &b) {
        return (a.index < b.index);
    }

    friend bool operator>(const book_node &a, const book_node &b) {
        return !(a.index < b.index);
    }

    friend bool operator==(const book_node &a, const book_node &b) {
        return false;
    }

    friend bool operator<=(const book_node &a, const book_node &b) {
        return !(a > b);
    }

    friend bool operator>=(const book_node &a, const book_node &b) {
        return !(a < b);
    }

    book_node &operator=(const book_node &p) {
        for(int i=0;i<62;i++) {
            index.name_[i]=p.index.name_[i];
        }
        Bookname=p.Bookname;
        Author=p.Author;
        Keyword=p.Keyword;
        Quantity=p.Quantity;
        Price=p.Price;
        return *this;
    }
};

struct book_block {
    book_node sup;
    book_node inf;
    int Next = -1;
    int size = 0;
    book_node data[1000];
};

class book_store {
public:
    const int max = 800;
    const int min = 400;
    int block_number = -1;
    std::fstream file;

    book_store() {
        file.open("book");
        if (file) {
            file.seekg(0);
            file.read(reinterpret_cast<char *>(&block_number), sizeof(int));
        } else {
            file.open("book", std::fstream::out);
            file.close();
            file.open("book");
            block_number = -1;
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&block_number), sizeof(int));
            file.seekp(sizeof(int));
            book_block tmp;
            file.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        }
    }

    ~book_store() {
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&block_number), sizeof(block_number));
        file.close();
    }

    void insert(const book_node &data) {
        static book_block now;
        if (block_number == -1) {
            ++block_number;
            now.size = 1;
            now.inf = data;
            now.sup = data;
            now.data[0] = data;
            write_data(0, now);
            return;
        }
        readblock(0, now);
        if (data < now.inf) {
            inner_insert(0, data);
            return;
        }
        static book_block nxt;
        int position_now = 0;
        while (now.Next != -1) {
            readblock(now.Next, nxt);
            if (data > now.inf && data < nxt.inf) {
                inner_insert(position_now, data);
                return;
            }
            position_now = now.Next;
            read_data(position_now, now);
        }
        inner_insert(position_now, data);
    }

    void del(const bookbook &index_) {
        static book_block now;
        readblock(0, now);
        int position_now = 0;
        while (position_now != -1) {
            if (index_ >= now.inf.index && index_ <= now.sup.index) {
                inner_deltete(position_now, index_);
                return;
            }
            if (index_ < now.inf.index) {
                return;
            }
            position_now = now.Next;
            readblock(position_now, now);
        }
    }

    book_node find(const bookbook &index_) {
        static book_block now;
        int i = 0;
        while (i != -1) {
            readblock(i, now);
            if (now.size == 0) {
                i = now.Next;
                continue;
            }
            if ((now.inf.index <= index_) && (now.sup.index >= index_)) {
                read_data(i, now);
                for (int j = 0; j < now.size; j++) {
                    if (now.data[j].index == index_) {
                        return now.data[j];
                    }
                }
            } else if (index_ < now.inf.index) {
                break;
            }
            i = now.Next;
        }
        bookbook t(index_);
        book_node k(t,"","",-1,-1);
        return k;
    }

    void book_out () {
        static book_block now;
        int i = 0;
        while(i!=-1) {
            readblock(i, now);
            if (now.size == 0) {
                i = now.Next;
                continue;
            }
            read_data(i, now);
            for (int j = 0; j < now.size; j++) {
                std::cout << now.data[j].index.name_ << '\t' << now.data[j].Bookname << "\t" << now.data[j].Author << "\t"
                          << now.data[j].Keyword << '\t' << now.data[j].Price << '\t' << now.data[j].Quantity << std::endl;
            }
            i = now.Next;
        }
    }
private:

    void readblock(int pos, book_block &data) {
        if (pos < 0)
            return;
        file.seekg(pos * sizeof(book_block) + sizeof(int));
        file.read(reinterpret_cast<char *>(&data.sup), sizeof(data.sup));
        file.read(reinterpret_cast<char *>(&data.inf), sizeof(data.inf));
        file.read(reinterpret_cast<char *>(&data.Next), sizeof(data.Next));
        file.read(reinterpret_cast<char *>(&data.size), sizeof(data.size));
    }

    void read_data(int pos, book_block &data) {
        if (pos < 0)
            return;
        file.seekg(pos * sizeof(book_block) + sizeof(int));
        file.read(reinterpret_cast<char *>(&data), sizeof(book_block));
    }

    void write_data(int pos, const book_block &data) {
        if (pos < 0)
            return;
        file.seekp(pos * sizeof(book_block) + sizeof(int));
        file.write(reinterpret_cast<const char *>(&data), sizeof(book_block));
    }

    void inner_insert(int pos, const book_node &data) {
        static book_block now;
        read_data(pos, now);
        int tmp = -1;
        for (int i = 0; i < now.size; i++) {
            if (data < now.data[i]) {
                tmp = i;
                break;
            }
            if (data == now.data[i])
                return;
        }
        now.size++;
        if (tmp == -1) {
            now.data[now.size - 1] = data;
        } else {
            for (int i = now.size - 1; i > tmp; --i) {
                now.data[i] = now.data[i - 1];
            }
            now.data[tmp] = data;
        }
        if (now.size > max) {
            split(pos, now);
        } else {
            now.inf = now.data[0];
            now.sup = now.data[now.size - 1];
            write_data(pos, now);
        }
    }

    void inner_deltete(int pos, const bookbook &data) {
        static book_block now;
        read_data(pos, now);
        for (int i = 0; i < now.size; i++) {
            if (now.data[i].index > data) {
                break;
            }
            if (now.data[i].index == data) {
                for (int j = i + 1; j < now.size; ++j) {
                    now.data[j - 1] = now.data[j];
                }
                now.size--;
                if (now.size == 0) {
                    write_data(pos, now);
                    return;
                }
                now.inf = now.data[0];
                now.sup = now.data[now.size - 1];
                if (now.size < min) {
                    combine(pos, now);
                } else {
                    write_data(pos, now);
                }
                return;
            }
        }
    }

    void split(int pos, book_block &data) {
        block_number++;
        static book_block now;
        static book_block old_one;
        for (int i = min; i < data.size; ++i) {
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
        write_data(pos, old_one);
        write_data(block_number, now);
    }

    void combine(int pos, book_block &data) {
        int next_one = data.Next;
        if (next_one == -1) {
            write_data(pos, data);
            return;
        }
        static book_block nxt;
        read_data(next_one, nxt);
        if (data.size + nxt.size <= max) {
            for (int i = data.size; i < data.size + nxt.size; ++i) {
                data.data[i] = nxt.data[i - data.size];
            }
            data.size += nxt.size;
            data.Next = nxt.Next;
            data.inf = data.data[0];
            data.sup = data.data[data.size - 1];
        }
        write_data(pos, data);
    }
};


#endif //MAIN_CPP_BOOK_H
