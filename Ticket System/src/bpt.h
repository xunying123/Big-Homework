#ifndef TICKET_SYSTEM_BPT_H
#define TICKET_SYSTEM_BPT_H

#include<fstream>
#include<cstring>

#define MAX_CACHE 165
#define HASH_SIZE 733

template<typename T1, typename T2, typename Compare>
T1 llower_bound(T1 begin, T1 end, const T2& tofind, Compare comp)
{
    int dist = end - begin;
    while (dist > 3)
    {
        T1 mid = begin + (dist >> 1);
        if (comp(*mid, tofind))
            begin = mid + 1;
        else
            end = mid + 1;
        dist = end - begin;
    }
    while (begin != end && comp(*begin, tofind)) begin++;
    return begin;
}

template<typename T1, typename T2, typename Compare>
T1 uupper_bound(T1 begin, T1 end, const T2& tofind, Compare comp)
{
    int dist = end - begin;
    while (dist > 3)
    {
        T1 mid = begin + (dist >> 1);
        if (comp(tofind, *mid))
            end = mid + 1;
        else
            begin = mid + 1;
        dist = end - begin;
    }
    while (begin != end && !comp(tofind, *begin)) begin++;
    return begin;
}

template<typename T, int size>
class allocator
{
public:
    allocator()
    {
        space = (T*) malloc(size * sizeof(T));
    }
    ~allocator()
    {
        free(space);
    }

    T* new_space()
    {
        if (pool_cursor == -1)
            return space + data_cursor++;
        int* tmp = reinterpret_cast<int*>(space + pool_cursor);
        pool_cursor = *tmp;
        return  reinterpret_cast<T*>(tmp);
    }

    void delete_space(T* x)
    {
        int* tmp = reinterpret_cast<int*>(x);
        *tmp = pool_cursor;
        pool_cursor = x - space;
    }

    void clean()
    {
        data_cursor = 0;
        pool_cursor = -1;
    }

private:
    T* space;
    int data_cursor = 0;
    int pool_cursor = -1;
};

template<typename T, typename Header>
class Basefile
{
public:
    Basefile(const std::string& _name, const Header& _header)
    {
        name = _name;
        header = _header;
        data.open(name+".db");
        if (data.good())
        {
            data.seekg(0);
            data.read(reinterpret_cast <char *> (&data_cursor), sizeof(long));
            data.read(reinterpret_cast <char *> (&pool_cursor), sizeof(long));
            data.read(reinterpret_cast <char *> (&header), sizeof(Header));
        }
        else
        {
            data.open(name+".db", std::ios::out);
            data.close();
            data.open(name+".db");
            data.write(reinterpret_cast <char *> (&data_cursor), sizeof(long));
            data.write(reinterpret_cast <char *> (&pool_cursor), sizeof(long));
            data.write(reinterpret_cast <char *> (&header), sizeof(Header));
        }
    }

    ~Basefile()
    {
        data.seekp(0);
        data.write(reinterpret_cast <char *> (&data_cursor), sizeof(long));
        data.write(reinterpret_cast <char *> (&pool_cursor), sizeof(long));
        data.write(reinterpret_cast <char *> (&header), sizeof(Header));
        data.close();
    }

    long new_space()
    {
        long address;
        if (!pool_cursor)
        {
            address = data_cursor;
            data_cursor += sizeof(T);
            return address;
        }
        address = pool_cursor;
        data.seekg(pool_cursor);
        data.read(reinterpret_cast <char *> (&pool_cursor), sizeof(long));
        return address;
    }

    void delete_space(long address)
    {
        if (address == data_cursor - sizeof(T))
        {
            data_cursor = address;
            return;
        }
        std::swap(pool_cursor, address);
        data.seekp(pool_cursor);
        data.write(reinterpret_cast <char *> (&address), sizeof(long));
    }

    inline void read(long address, T& value)
    {
        data.seekg(address);
        data.read(reinterpret_cast <char *> (&value), sizeof(T));
    }

    inline void write(long address, const T& value)
    {
        data.seekp(address);
        data.write(reinterpret_cast <const char *> (&value), sizeof(T));
    }

    inline Header& head()
    {
        return header;
    }

    void clean()
    {
        data_cursor = 2*sizeof(long) + sizeof(Header);
        pool_cursor = 0;
        data.close();
        data.open(name+".db", std::ios::out);
        data.close();
        data.open(name+".db");
    }

private:
    std::fstream data;
    long data_cursor = 2*sizeof(long) + sizeof(Header);
    long pool_cursor = 0;
    Header header;
    std::string name;
};

template<typename T>
class Cache_List
{
public:
    struct Cache_Node
    {
        Cache_Node* pre;
        Cache_Node* next;
        long address;
        bool dirty;
        T data;
        Cache_Node(long a, const T& v, bool w, Cache_Node* p = nullptr, Cache_Node* n = nullptr):
                address(a), data(v), dirty(w), pre(p), next(n) {}
        Cache_Node() {}
    };

    Cache_List()
    {
        head = memory.new_space();
        end = memory.new_space();
        head->pre = end->next = nullptr;
        end->pre = head;
        head->next = end;
    }

    ~Cache_List() = default;

    Cache_Node* push_front(long address, const T& data, bool write)
    {
        Cache_Node* tmp = memory.new_space();
        tmp->address = address;
        tmp->data = data;
        tmp->dirty = write;
        tmp->pre = head;
        tmp->next = head->next;
        head->next = tmp;
        tmp->next->pre = tmp;
        Size++;
        return tmp;
    }

    void pop_back()
    {
        if (!Size) return;
        Cache_Node* topop = end->pre;
        end->pre = topop->pre;
        topop->pre->next = end;
        memory.delete_space(topop);
        Size--;
    }

    void erase(Cache_Node* toerase)
    {
        toerase->pre->next = toerase->next;
        toerase->next->pre = toerase->pre;
        memory.delete_space(toerase);
        Size--;
    }

    int size() const
    {
        return Size;
    }

    bool empty() const
    {
        return Size == 0;
    }

    void adjust_to_front(Cache_Node* p)
    {
        if (p->pre == head) return;
        p->pre->next = p->next;
        p->next->pre = p->pre;
        head->next->pre = p;
        p->next = head->next;
        p->pre = head;
        head->next = p;
    }

    Cache_Node* front()
    {
        return head->next;
    }

    Cache_Node* back()
    {
        return end->pre;
    }

    void clean()
    {
        memory.clean();
        head = memory.new_space();
        end = memory.new_space();
        head->pre = end->next = nullptr;
        end->pre = head;
        head->next = end;
    }

private:
    int Size = 0;
    Cache_Node* head;
    Cache_Node* end;
    allocator<Cache_Node, MAX_CACHE+5> memory;
};

class Hashmap
{
public:
    Hashmap() = default;
    ~Hashmap() = default;

    long find(long key)
    {
        Node* res = array[key % HASH_SIZE].next;
        while (res != nullptr)
        {
            if (res->key == key)
                return res->data;
            res = res->next;
        }
        return -1;
    }

    void insert(long key, long data)
    {
        Node* pre = &array[key % HASH_SIZE];
        Node* next = pre->next;
        pre->next = memory.new_space();
        pre->next->key = key;
        pre->next->data = data;
        pre->next->next = next;
    }

    void erase(long key)
    {
        Node* pre = &array[key % HASH_SIZE];
        Node* toerase = pre->next;
        while (toerase != nullptr)
        {
            if (toerase->key == key)
            {
                pre->next = toerase->next;
                memory.delete_space(toerase);
                return;
            }
            pre = toerase;
            toerase = toerase->next;
        }
    }

    void clean()
    {
        memory.clean();
        for (int i = 0; i < HASH_SIZE; i++)
            array[i].next = nullptr;
    }

private:
    struct Node
    {
        long key;
        long data;
        Node* next;
        Node(): next(nullptr) {}
        Node(long k, long d): key(k), data(d), next(nullptr) {}
    };
    Node array[HASH_SIZE];
    allocator<Node, MAX_CACHE+5> memory;
};

template<typename T, typename Header>
class Myfile
{
public:
    Myfile(const std::string& name, const Header& _header): file(name, _header) {}
    ~Myfile()
    {
        auto tmp = list.front();
        while (tmp->next != nullptr)
        {
            if (tmp->dirty)
                file.write(tmp->address, tmp->data);
            tmp = tmp->next;
        }
    }

    inline Header& head()
    {
        return file.head();
    }

    const T* readonly(long address)
    {
        long found = node_map.find(address);
        if (found != -1)
        {
            auto tmp = reinterpret_cast<typename Cache_List<T>::Cache_Node*> (found);
            list.adjust_to_front(tmp);
            return &(tmp->data);
        }
        T value;
        file.read(address, value);
        auto ptr = list.push_front(address, value, false);
        node_map.insert(address, reinterpret_cast<long>(ptr));
        if (list.size() > MAX_CACHE) oversize();
        return &(ptr->data);
    }

    T* readwrite(long address)
    {
        long found = node_map.find(address);
        if (found != -1)
        {
            auto tmp = reinterpret_cast<typename Cache_List<T>::Cache_Node*> (found);
            list.adjust_to_front(tmp);
            tmp->dirty = true;
            return &(tmp->data);
        }
        T value;
        file.read(address, value);
        auto ptr = list.push_front(address, value, true);
        node_map.insert(address, reinterpret_cast<long>(ptr));
        if (list.size() > MAX_CACHE) oversize();
        return &(ptr->data);
    }

    void write(long address, const T& value)
    {
        node_map.insert(address, reinterpret_cast<long> (list.push_front(address, value, true)));
        if (list.size() > MAX_CACHE) oversize();
    }

    long new_space()
    {
        return file.new_space();
    }

    void delete_space(long address)
    {
        file.delete_space(address);
        long found = node_map.find(address);
        if (found != -1)
        {
            auto tmp = reinterpret_cast<typename Cache_List<T>::Cache_Node*> (found);
            list.erase(tmp);
            node_map.erase(address);
        }
    }

    void clean()
    {
        file.clean();
        list.clean();
        node_map.clean();
    }

private:
    Basefile<T, Header> file;
    Cache_List<T> list;
    Hashmap node_map;

    void oversize()
    {
        auto tmp = list.back();
        if (tmp->dirty)
            file.write(tmp->address, tmp->data);
        node_map.erase(tmp->address);
        list.pop_back();
    }
};

template<typename V>
class Datafile
{
public:
    Datafile(const std::string& name): file(name, pos)
    {
        pos = file.head();
        if (!pos)
        {
            Block tmp;
            pos = file.new_space();
            file.write(pos, tmp);
        }
    }
    ~Datafile()
    {
        file.head() = pos;
    }

    long new_space()
    {
        Block* tmp = file.readwrite(pos);
        if (tmp->size < MAXSIZE)
            return pos + (tmp->size++) * sizeof(V);
        Block new_block;
        new_block.size++;
        pos = file.new_space();
        file.write(pos, new_block);
        return pos;
    }

    void delete_space(long address)
    {
        long offset = (address - 3 * sizeof(long)) % sizeof(Block);
        long block_address = address - offset;
        Block* block = file.readwrite(block_address);
        if (--block->size) return;
        if (block_address != pos)
            file.delete_space(block_address);
    }

    void write(long address, const V& value)
    {
        long offset = (address - 3 * sizeof(long)) % sizeof(Block);
        Block* block = file.readwrite(address - offset);
        block->data[offset / sizeof(V)] = value;
    }

    const V* readonly(long address)
    {
        long offset = (address - 3 * sizeof(long)) % sizeof(Block);
        const Block* block = file.readonly(address - offset);
        return (block->data + offset / sizeof(V));
    }

    V* readwrite(long address)
    {
        long offset = (address - 3 * sizeof(long)) % sizeof(Block);
        Block* block = file.readwrite(address - offset);
        return (block->data + offset / sizeof(V));
    }

    void clean()
    {
        file.clean();
        pos = 0;
    }

private:
    constexpr static int MAXSIZE = std::max(4000/sizeof(V), 1UL);
    struct Block
    {
        V data[MAXSIZE];
        int size = 0;
    };
    long pos = 0;
    Myfile<Block, long> file;
};

template<typename T>
class vector {
public:
    T *head, *tail, *ending;
    int space, used;

    class iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator_category = std::output_iterator_tag;
    private:
    public:
        T *heading;
        long long pos = 0;

        iterator() {
            pos = 0;
        }

        iterator(const iterator &other) {
            heading = other.heading;
            pos = other.pos;
        }

        iterator &operator=(const iterator &other) {
            if (this == &other) return *this;
            heading = other.heading;
            pos = other.pos;
            return *this;
        }

        iterator operator+(const int &n) const {
            iterator temp(*this);
            temp.pos = pos + n;
            return temp;
        }

        iterator operator-(const int &n) const {
            iterator temp(*this);
            temp.pos = pos - n;
            return temp;
        }

        int operator-(const iterator &rhs) const {
            if (heading == rhs.heading) {
                return abs(pos - rhs.pos);
            }
        }

        iterator &operator+=(const int &n) {
            pos += n;
            return *this;
        }

        iterator &operator-=(const int &n) {
            pos -= n;
            return *this;
        }

        iterator operator++(int) {
            iterator temp(*this);
            pos++;
            return temp;
        }

        iterator &operator++() {
            pos++;
            return *this;
        }

        iterator operator--(int) {
            iterator temp(*this);
            pos--;
            return temp;
        }

        iterator &operator--() {
            pos--;
            return *this;
        }

        T &operator*() const {
            return heading[pos];
        }

        bool operator==(const iterator &rhs) const {
            if ((heading == rhs.heading) && (pos == rhs.pos)) return true;
            else false;
        }

        bool operator!=(const iterator &rhs) const {
            if ((heading == rhs.heading) && (pos == rhs.pos)) return false;
            else return true;
        }
    };

    vector() {
        head = (T *) malloc(1000 * sizeof(T));
        tail = head;
        ending = &(head[999]);
        space = 0;
        used = 0;
    }

    vector(const vector &other) {
        long long k = 1000;
        for (int i = 0; i < other.space; i++) k = k * 2;
        space = other.space;
        head = (T *) malloc(k * sizeof(T));
        for (int i = 0; i < other.used; i++) {
            new(head + i) T(other.head[i]);
        }
        tail = &(head[used]);
        ending = &(head[k - 1]);
        used = other.used;
    }

    ~vector() {
        for (int i = 0; i < used; i++) head[i].~T();
        free(head);
    }

    vector &operator=(const vector &other) {
        if (this == &other) return *this;
        long long k = 1000;
        for (int i = 0; i < other.space; i++) k = k * 2;
        space = other.space;
        T *head1 = (T *) malloc(k * sizeof(T));
        for (int i = 0; i < other.used; i++) {
            new(head1 + i) T(other.head[i]);
        }
        for (int i = 0; i < used; i++) head[i].~T();
        free(head);
        head = head1;
        ending = &(head[k - 1]);
        used = other.used;
        tail = &(head[used]);
        return *this;
    }

    T &at(const size_t &pos) {
        if (pos + 1 > used) {
        } else {
            return head[pos];
        }
    }

    const T &at(const size_t &pos) const {
        if (pos + 1 > used) {
        } else {
            T temp(head[pos]);
            return temp;
        }
    }

    T &operator[](const size_t &pos) {
        if (pos + 1 > used) {
            return head[0];
        } else {
            return head[pos];
        }
    }

    const T &operator[](const size_t &pos) const {
        if (pos + 1 > used) {
        } else {
            return head[pos];
        }
    }

    const T &front() const {
        if (used == 0) {

        } else {
            return head[0];
        }
    }

    const T &back() const {
        if (used == 0) {
        } else {
            return *(tail - 1);
        }
    }

    iterator begin() {
        iterator t;
        t.heading = head;
        t.pos = 0;
        return t;
    }

    iterator end() {
        iterator t;
        t.heading = head;
        t.pos = used;
        return t;
    }

    bool empty() const {
        if (used == 0) {
            return true;
        } else {
            return false;
        }
    }

    size_t size() const {
        return used;
    }

    void clear() {
        if (used != 0) {
            tail = head + 1;
            used = 0;
        }
    }

    iterator insert(iterator post, const T &value) {
        if (post.pos > used) {
            space++;
            T *head1;
            int space1, used1;
            space1 = space;
            used1 = used;
            long long k = 1000;
            for (int i = 0; i < space; i++) k = k * 2;
            k *= 2;
            head1 = (T *) malloc(k * sizeof(T));
            for (int i = 0; i < post.pos; i++) {
                new(head1 + i) T(head[i]);
                head[i].~T();
            }
            new(head1 + post.pos) T(value);
            head[post.pos].~T();
            for (int i = post.pos + 1; i < used + 1; i++) {
                new(head1 + i) T(head[i - 1]);
                head[i - 1].~T();
            }
            free(head);
            head = head1;
            space = space1;
            used = used1 + 1;
            tail = &head[used];
            ending = &head[k - 1];
            iterator temp = begin();
            temp.pos = post.pos;
            return temp;
        } else {
            T t(head[post.pos]);
            new(tail) T(head[post.pos]);
            head[post.pos] = value;
            for (int i = post.pos + 1; i < used; i++) {
                *tail = head[i];
                head[i] = t;
                t = *tail;
            }
            tail++;
            used++;
            iterator temp = begin();
            temp.pos = post.pos;
            return temp;
        }
    }

    iterator insert(const size_t &ind, const T &value) {
        if (ind > used) {

        } else {
            size_t t = ind;
            T te(value);
            return insert(t, te);
        }
    }

    iterator erase(iterator post) {
        head[post.pos].~T();
        for (int i = post.pos; i < used - 1; i++) {
            head[i] = head[i + 1];
        }
        tail--;
        used--;
        iterator temp = begin();
        temp.pos = post.pos;
        return temp;
    }

    iterator erase(const size_t &ind) {
        if (ind >= used) {

        } else {
            size_t t = ind;
            return erase(t);
        }
    }

    void push_back(const T &value) {
        if (tail + 1 != ending) {
            new(tail) T(value);
            tail++;
            used++;
        } else {
            space++;
            T *head1;
            int space1, used1;
            space1 = space;
            used1 = used;
            long long k = 1000;
            for (int i = 0; i < space; i++) k = k * 2;
            k *= 2;
            head1 = (T *) malloc(k * sizeof(T));
            for (int i = 0; i < used; i++) {
                new(head1 + i) T(head[i]);
                head[i].~T();
            }
            new(head1 + used) T(value);
            free(head);
            head = head1;
            space = space1;
            used = used1 + 1;
            tail = &head[used];
            ending = &head[k - 1];
        }
    }

    void pop_back() {
        if (used == 0) {

        } else {
            head[used - 1].~T();
            tail--;
            used--;
        }
    }
};

template<int length>
struct m_string {
    char _str[length];

    m_string() = default;

    m_string(const std::string &temp) {
        strcpy(_str, temp.c_str());
    }

    m_string(char *t) {
        strcpy(_str, t);
    }

    friend bool operator<(const m_string &a, const m_string &b) {
        return strcmp(a._str, b._str) < 0;
    }

    friend bool operator==(const m_string &a, const m_string &b) {
        return strcmp(a._str, b._str) == 0;
    }
};

template<typename K, typename V, class Comp_K = std::less<K>, class Comp_V = std::less<V>>
class Multi_BPT
{
public:
    Multi_BPT(const std::string& name): file(name, head)
    {
        head = file.head();
    }
    ~Multi_BPT()
    {
        file.head() = head;
    }

    void find(const K& key, vector<V>& res)
    {
        if (!head) return;
        const Node* tmp;
        long tofind = find_Node(key);
        tmp = file.readonly(tofind);
        const KVpair* begin = llower_bound(tmp->data, tmp->data+tmp->size, key, comp);
        int locat = begin - tmp->data;
        for (int i = locat; i < tmp->size; i++)
        {
            if (tmp->data[i].key == key)
                res.push_back(tmp->data[i].value);
            else return;
        }
        long next = tmp->ptr[1];
        while (next)
        {
            tmp = file.readonly(next);
            for (int i = 0; i < tmp->size; i++)
            {
                if (tmp->data[i].key == key)
                    res.push_back(tmp->data[i].value);
                else if (tmp->data[i].key < key) continue;
                else return;
            }
            next = tmp->ptr[1];
        }
    }

    void insert(const K& key, const V& value)
    {
        if (!head)
        {
            Node tmp;
            head = file.new_space();
            tmp.parent = 0;
            tmp.size = 1;
            tmp.data[0].key = key;
            tmp.data[0].value = value;
            tmp.ptr[0]  = tmp.ptr[1] = 0;
            file.write(head, tmp);
            return;
        }
        insert_leaf(find_Node(key, value), key, value);
    }

    void erase(const K& key, const V& value)
    {
        if (!head) return;
        erase_leaf(find_Node(key, value), key, value);
    }

    void clean()
    {
        file.clean();
        head = 0;
    }

private:
    constexpr static int DEGREE = 5500 / (sizeof(long) + sizeof(K) + sizeof(V));
    struct KVpair
    {
        K key;
        V value;
        KVpair() {}
        KVpair(const K& k, const V& v): key(k), value(v) {}
        friend bool operator==(const KVpair& a, const KVpair& b)
        {
            return a.key == b.key && a.value == b.value;
        }
    };
    struct Node
    {
        int size;
        long parent;
        KVpair data[DEGREE];
        long ptr[DEGREE+1]; // ptr[0] == 0 means leaf, whose ptr[1] points to next leaf
    };
    struct Comp
    {
        Comp_K comp_k;
        Comp_V comp_v;
        bool operator()(const KVpair& x, const K& key)
        {
            return comp_k(x.key, key);
        }
        bool operator()(const K& key, const KVpair& x)
        {
            return comp_k(key, x.key);
        }
        bool operator()(const KVpair& a, const KVpair& b)
        {
            if (!(a.key == b.key)) return comp_k(a.key, b.key);
            return comp_v(a.value, b.value);
        }
    } comp;
    long head = 0;
    Myfile<Node, long> file;

    long find_Node(const K& key, const V& value)
    {
        long res = head;
        const Node* tmp = file.readonly(head);
        KVpair tofind(key, value);
        while (tmp->ptr[0])
        {
            const KVpair* found = uupper_bound(tmp->data, tmp->data+tmp->size, tofind, comp);
            res = tmp->ptr[found - tmp->data];
            tmp = file.readonly(res);
        }
        return res;
    }

    long find_Node(const K& key)
    {
        long res = head;
        const Node* tmp = file.readonly(head);
        while (tmp->ptr[0])
        {
            const KVpair* found = llower_bound(tmp->data, tmp->data+tmp->size, key, comp);
            res = tmp->ptr[found - tmp->data];
            tmp = file.readonly(res);
        }
        return res;
    }

    void insert_leaf(long address, const K& key, const V& value)
    {
        Node& tmp = *file.readwrite(address);
        KVpair toinsert(key, value);
        KVpair* found = llower_bound(tmp.data, tmp.data+tmp.size, toinsert, comp);
        if (found != tmp.data+tmp.size && *found == toinsert) return; // remember to check out_of_bound!
        int locat = found - tmp.data;
        for (int i = tmp.size; i > locat; i--)
            tmp.data[i] = tmp.data[i-1];
        tmp.data[locat] = toinsert;
        tmp.size++;
        if (tmp.size < DEGREE)
            return;
        int carry = DEGREE / 2;
        long new_address = file.new_space();
        Node new_leaf;
        new_leaf.size = tmp.size - carry;
        tmp.size = carry;
        new_leaf.parent = tmp.parent;
        new_leaf.ptr[0] = 0;
        new_leaf.ptr[1] = tmp.ptr[1];
        tmp.ptr[1] = new_address;
        for (int i = 0; i < new_leaf.size; i++)
            new_leaf.data[i] = tmp.data[carry+i];
        file.write(new_address, new_leaf);
        insert_internal(tmp.parent, new_address, tmp.data[carry]);
    }

    void insert_internal(long this_address, long right_address, const KVpair& toinsert)
    {
        if (!this_address)
        {
            Node new_node;
            long new_head = file.new_space();
            new_node.size = 1;
            new_node.parent = 0;
            new_node.data[0] = toinsert;
            new_node.ptr[0] = head;
            new_node.ptr[1] = right_address;
            Node* tmp = file.readwrite(head);
            tmp->parent = new_head;
            tmp = file.readwrite(right_address);
            tmp->parent = new_head;
            head = new_head;
            file.write(new_head, new_node);
            return;
        }
        Node& this_node = *file.readwrite(this_address);
        KVpair* found = llower_bound(this_node.data, this_node.data+this_node.size, toinsert, comp);
        int locat = found - this_node.data;
        if (this_node.size < DEGREE)
        {
            for (int i = this_node.size - 1; i >= locat; i--)
            {
                this_node.data[i+1] = this_node.data[i];
                this_node.ptr[i+2] = this_node.ptr[i+1];
            }
            this_node.data[locat] = toinsert;
            this_node.ptr[locat+1] = right_address;
            this_node.size++;
            return;
        }
        // split
        int carry = DEGREE / 2;
        KVpair tocarry = this_node.data[carry];
        long new_address = file.new_space();
        Node new_node;
        new_node.size = this_node.size - carry - 1;
        this_node.size = carry;
        new_node.parent = this_node.parent;
        for (int i = 0; i < new_node.size; i++)
        {
            new_node.data[i] = this_node.data[carry+1+i];
            new_node.ptr[i] = this_node.ptr[carry+1+i];
        }
        new_node.ptr[new_node.size] = this_node.ptr[DEGREE];
        // insert
        if (locat <= carry)
        {
            for (int i = this_node.size - 1; i >= locat; i--)
            {
                this_node.data[i+1] = this_node.data[i];
                this_node.ptr[i+2] = this_node.ptr[i+1];
            }
            this_node.data[locat] = toinsert;
            this_node.ptr[locat+1] = right_address;
            this_node.size++;
        }
        else
        {
            locat -= carry + 1;
            for (int i = new_node.size - 1; i >= locat; i--)
            {
                new_node.data[i+1] = new_node.data[i];
                new_node.ptr[i+2] = new_node.ptr[i+1];
            }
            new_node.data[locat] = toinsert;
            new_node.ptr[locat+1] = right_address;
            new_node.size++;
        }
        for (int i = 0; i <= new_node.size; i++)
        {
            Node* tmp = file.readwrite(new_node.ptr[i]);
            tmp->parent = new_address;
        }
        file.write(new_address, new_node);
        insert_internal(this_node.parent, new_address, tocarry);
    }

    void erase_leaf(long address, const K& key, const V& value)
    {

        KVpair toerase(key, value);
        Node &tmp = *file.readwrite(address);
        KVpair* found = llower_bound(tmp.data, tmp.data+tmp.size, toerase, comp);
        if (!(*found == toerase)) return;
        int locat = found - tmp.data;
        if (locat == tmp.size) return;
        for (int i = locat; i < tmp.size-1; i++)
            tmp.data[i] = tmp.data[i+1];
        tmp.size--;
        if (tmp.size >= DEGREE/2) return;
        erase_leaf_rebalance(address, tmp);
    }

    void erase_leaf_rebalance(long address, Node& this_node)
    {
        if (!this_node.parent)
        {
            if (this_node.size) return;
            file.delete_space(address);
            head = 0;
            return;
        }
        Node &parent_node = *file.readwrite(this_node.parent);
        KVpair* this_key = uupper_bound(parent_node.data, parent_node.data+parent_node.size, this_node.data[0], comp) - 1;
        int locat = this_key - parent_node.data;
        // borrow from right sibling
        // ptr[1] IS NOT ALWAYS RIGHT SIBLING!
        long right;
        if (locat == parent_node.size - 1)
            right = 0;
        else
            right = parent_node.ptr[locat+2];
        Node* right_node;
        if (right)
        {
            right_node = file.readwrite(right);
            if (right_node->size > DEGREE / 2)
            {
                this_node.data[this_node.size] = right_node->data[0];
                this_node.size++;
                for (int i = 1; i < right_node->size; i++)
                    right_node->data[i-1] = right_node->data[i];
                right_node->size--;
                *(this_key+1) = right_node->data[0];
                return;
            }
        }
        // borrow from left sibling
        Node* left_node;
        long left;
        if (locat >= 0)
        {
            left = parent_node.ptr[locat];
            left_node = file.readwrite(left);
            if (left_node->size > DEGREE / 2)
            {
                for (int i = this_node.size; i > 0; i--)
                    this_node.data[i] = this_node.data[i-1];
                left_node->size--;
                this_node.data[0] = left_node->data[left_node->size];
                this_node.size++;
                *this_key = this_node.data[0];
                return;
            }
        }
        // merge with siblings
        if (right)
        {
            for (int i = 0; i < right_node->size; i++)
                this_node.data[this_node.size+i] = right_node->data[i];
            this_node.size += right_node->size;
            this_node.ptr[1] = right_node->ptr[1];
            for (int i = locat+1; i < parent_node.size-1; i++)
            {
                parent_node.data[i] = parent_node.data[i+1];
                parent_node.ptr[i+1] = parent_node.ptr[i+2];
            }
            parent_node.size--;
            file.delete_space(right);
        }
        else
        {
            for (int i = 0; i < this_node.size; i++)
                left_node->data[left_node->size+i] = this_node.data[i];
            left_node->size += this_node.size;
            left_node->ptr[1] = this_node.ptr[1];
            for (int i = locat; i < parent_node.size-1; i++)
            {
                parent_node.data[i] = parent_node.data[i+1];
                parent_node.ptr[i+1] = parent_node.ptr[i+2];
            }
            parent_node.size--;
            file.delete_space(address);
        }
        if (parent_node.size >= DEGREE / 2) return;
        erase_internal_rebalance(this_node.parent, parent_node);
    }

    void erase_internal_rebalance(long address, Node& this_node)
    {
        Node* son;
        if (!this_node.parent)
        {
            if (this_node.size)
                return;
            son = file.readwrite(this_node.ptr[0]);
            son->parent = 0;
            head = this_node.ptr[0];
            file.delete_space(address);
            return;
        }
        Node &parent_node = *file.readwrite(this_node.parent);
        KVpair* this_key = uupper_bound(parent_node.data, parent_node.data+parent_node.size, this_node.data[0], comp) - 1;
        int locat = this_key - parent_node.data;
        long right;
        if (locat == parent_node.size - 1)
            right = 0;
        else
            right = parent_node.ptr[locat+2];
        // borrow from right sibling
        Node* right_node;
        if (right)
        {
            right_node = file.readwrite(right);
            if (right_node->size > DEGREE / 2)
            {
                this_node.size++;
                son = file.readwrite(right_node->ptr[0]);
                this_node.ptr[this_node.size] = right_node->ptr[0];
                son->parent = address;
                this_node.data[this_node.size-1] = *(this_key+1); // NOT this_node.data[this_node.size-1] = son->data[0]!!!
                *(this_key+1) = right_node->data[0]; // THIS LINE SHOULD BE DONE BEFORE MOVING!
                for (int i = 1; i < right_node->size; i++)
                {
                    right_node->data[i-1] = right_node->data[i];
                    right_node->ptr[i-1] = right_node->ptr[i];
                }
                right_node->ptr[right_node->size-1] = right_node->ptr[right_node->size];
                right_node->size--;
                return;
            }
        }
        // borrow from left sibling
        Node* left_node;
        long left;
        if (locat >= 0)
        {
            left = parent_node.ptr[locat];
            left_node = file.readwrite(left);
            if (left_node->size > DEGREE / 2)
            {
                this_node.ptr[this_node.size+1] = this_node.ptr[this_node.size];
                for (int i = this_node.size; i > 0; i--)
                {
                    this_node.data[i] = this_node.data[i-1];
                    this_node.ptr[i] = this_node.ptr[i-1];
                }
                son = file.readwrite(left_node->ptr[left_node->size]);
                this_node.ptr[0] = left_node->ptr[left_node->size];
                son->parent = address;
                this_node.size++;
                left_node->size--;
                this_node.data[0] = *this_key; // NOT this_node.data[0] = son->data[0]!
                *this_key = left_node->data[left_node->size]; // SIGNIFICANT STEP!
                return;
            }
        }
        // merge with siblings
        if (right)
        {
            this_node.ptr[this_node.size+1] = right_node->ptr[0];
            son = file.readwrite(right_node->ptr[0]);
            son->parent = address;
            for (int i = 0; i < right_node->size; i++)
            {
                this_node.data[this_node.size+1+i] = right_node->data[i];
                this_node.ptr[this_node.size+i+2] = right_node->ptr[i+1];
                son = file.readwrite(right_node->ptr[i+1]);
                son->parent = address;
            }
            this_node.data[this_node.size] = parent_node.data[locat+1];
            this_node.size += right_node->size + 1;
            for (int i = locat + 1; i < parent_node.size - 1; i++)
            {
                parent_node.data[i] = parent_node.data[i+1];
                parent_node.ptr[i+1] = parent_node.ptr[i+2];
            }
            parent_node.size--;
            file.delete_space(right);
        }
        else
        {
            left_node->ptr[left_node->size+1] = this_node.ptr[0];
            son = file.readwrite(this_node.ptr[0]);
            son->parent = left;
            for (int i = 0; i < this_node.size; i++)
            {
                left_node->data[left_node->size+1+i] = this_node.data[i];
                left_node->ptr[left_node->size+i+2] = this_node.ptr[i+1];
                son = file.readwrite(this_node.ptr[i+1]);
                son->parent = left;
            }
            left_node->data[left_node->size] = parent_node.data[locat];
            left_node->size += this_node.size + 1;
            for (int i = locat; i < parent_node.size - 1; i++)
            {
                parent_node.data[i] = parent_node.data[i+1];
                parent_node.ptr[i+1] = parent_node.ptr[i+2];
            }
            parent_node.size--;
            file.delete_space(address);
        }
        if (parent_node.size >= DEGREE / 2) return;
        erase_internal_rebalance(this_node.parent, parent_node);
    }
};

template<typename K, typename V, class Comp = std::less<K>>
class BPT
{
public:
    BPT(const std::string& name): file(name + "_index", head), data(name + "_data")
    {
        head = file.head();
    }
    ~BPT()
    {
        file.head() = head;
    }

    const V* readonly(const K& key)
    {
        if (!head) return nullptr;
        const Node* tmp;
        long tofind = find_Node(key);
        tmp = file.readonly(tofind);
        const K* found = llower_bound(tmp->key, tmp->key+tmp->size, key, comp);
        int locat = found - tmp->key;
        if (locat == tmp->size || !(*found == key)) return nullptr;
        return data.readonly(tmp->ptr[locat]);
    }

    V* readwrite(const K& key)
    {
        if (!head) return nullptr;
        const Node* tmp;
        long tofind = find_Node(key);
        tmp = file.readonly(tofind);
        const K* found = llower_bound(tmp->key, tmp->key+tmp->size, key, comp);
        int locat = found - tmp->key;
        if (locat == tmp->size || !(*found == key)) return nullptr;
        return data.readwrite(tmp->ptr[locat]);
    }

    void insert(const K& key, const V& value)
    {
        if (!head)
        {
            Node tmp;
            head = file.new_space();
            tmp.parent = 0;
            tmp.isleaf = true;
            tmp.size = 1;
            tmp.key[0] = key;
            tmp.ptr[0] = data.new_space();
            data.write(tmp.ptr[0], value);
            file.write(head, tmp);
            return;
        }
        insert_leaf(find_Node(key), key, value);
    }

    void erase(const K& key)
    {
        if (!head) return;
        erase_leaf(find_Node(key), key);
    }

    bool empty() const
    {
        if (!head) return true;
        return false;
    }

    void clean()
    {
        file.clean();
        data.clean();
        head = 0;
    }

private:
    constexpr static int DEGREE = 5500 / (sizeof(long) + sizeof(K));
    struct Node
    {
        int size;
        bool isleaf;
        long parent;
        K key[DEGREE];
        long ptr[DEGREE+1]; // leaf's ptr[DEGREE] points to next leaf
    };
    long head = 0;
    Comp comp;
    Myfile<Node, long> file;
    Datafile<V> data;

    long find_Node(const K& key)
    {
        long res = head;
        const Node* tmp = file.readonly(head);
        while (!tmp->isleaf)
        {
            const K* found = uupper_bound(tmp->key, tmp->key+tmp->size, key, comp);
            res = tmp->ptr[found - tmp->key];
            tmp = file.readonly(res);
        }
        return res;
    }

    void insert_leaf(long address, const K& key, const V& value)
    {
        Node& tmp = *file.readwrite(address);
        K* found = llower_bound(tmp.key, tmp.key+tmp.size, key, comp);
        if (found != tmp.key+tmp.size && *found == key) return; // remember to check out_of_bound!
        int locat = found - tmp.key;
        for (int i = tmp.size; i > locat; i--)
        {
            tmp.key[i] = tmp.key[i-1];
            tmp.ptr[i] = tmp.ptr[i-1];
        }
        tmp.key[locat] = key;
        tmp.ptr[locat] = data.new_space();
        data.write(tmp.ptr[locat], value);
        tmp.size++;
        if (tmp.size < DEGREE)
            return;
        int carry = DEGREE / 2;
        long new_address = file.new_space();
        Node new_leaf;
        new_leaf.isleaf = true;
        new_leaf.size = tmp.size - carry;
        tmp.size = carry;
        new_leaf.parent = tmp.parent;
        new_leaf.ptr[DEGREE] = tmp.ptr[DEGREE];
        tmp.ptr[DEGREE] = new_address;
        for (int i = 0; i < new_leaf.size; i++)
        {
            new_leaf.key[i] = tmp.key[carry+i];
            new_leaf.ptr[i] = tmp.ptr[carry+i];
        }
        file.write(new_address, new_leaf);
        insert_internal(tmp.parent, new_address, tmp.key[carry]);
    }

    void insert_internal(long this_address, long right_address, const K& toinsert)
    {
        if (!this_address)
        {
            Node new_node;
            long new_head = file.new_space();
            new_node.isleaf = false;
            new_node.size = 1;
            new_node.parent = 0;
            new_node.key[0] = toinsert;
            new_node.ptr[0] = head;
            new_node.ptr[1] = right_address;
            Node* tmp = file.readwrite(head);
            tmp->parent = new_head;
            tmp = file.readwrite(right_address);
            tmp->parent = new_head;
            head = new_head;
            file.write(new_head, new_node);
            return;
        }
        Node& this_node = *file.readwrite(this_address);
        K* found = llower_bound(this_node.key, this_node.key+this_node.size, toinsert, comp);
        int locat = found - this_node.key;
        if (this_node.size < DEGREE)
        {
            for (int i = this_node.size - 1; i >= locat; i--)
            {
                this_node.key[i+1] = this_node.key[i];
                this_node.ptr[i+2] = this_node.ptr[i+1];
            }
            this_node.key[locat] = toinsert;
            this_node.ptr[locat+1] = right_address;
            this_node.size++;
            return;
        }
        // split
        int carry = DEGREE / 2;
        K tocarry = this_node.key[carry];
        long new_address = file.new_space();
        Node new_node;
        new_node.isleaf = false;
        new_node.size = this_node.size - carry - 1;
        this_node.size = carry;
        new_node.parent = this_node.parent;
        for (int i = 0; i < new_node.size; i++)
        {
            new_node.key[i] = this_node.key[carry+1+i];
            new_node.ptr[i] = this_node.ptr[carry+1+i];
        }
        new_node.ptr[new_node.size] = this_node.ptr[DEGREE];
        // insert
        if (locat <= carry)
        {
            for (int i = this_node.size - 1; i >= locat; i--)
            {
                this_node.key[i+1] = this_node.key[i];
                this_node.ptr[i+2] = this_node.ptr[i+1];
            }
            this_node.key[locat] = toinsert;
            this_node.ptr[locat+1] = right_address;
            this_node.size++;
        }
        else
        {
            locat -= carry + 1;
            for (int i = new_node.size - 1; i >= locat; i--)
            {
                new_node.key[i+1] = new_node.key[i];
                new_node.ptr[i+2] = new_node.ptr[i+1];
            }
            new_node.key[locat] = toinsert;
            new_node.ptr[locat+1] = right_address;
            new_node.size++;
        }
        for (int i = 0; i <= new_node.size; i++)
        {
            Node* tmp = file.readwrite(new_node.ptr[i]);
            tmp->parent = new_address;
        }
        file.write(new_address, new_node);
        insert_internal(this_node.parent, new_address, tocarry);
    }

    void erase_leaf(long address, const K& key)
    {

        Node &tmp = *file.readwrite(address);
        K* found = llower_bound(tmp.key, tmp.key+tmp.size, key, comp);
        if (!(*found == key)) return;
        int locat = found - tmp.key;
        if (locat == tmp.size) return;
        data.delete_space(tmp.ptr[locat]);
        for (int i = locat; i < tmp.size-1; i++)
        {
            tmp.key[i] = tmp.key[i+1];
            tmp.ptr[i] = tmp.ptr[i+1];
        }
        tmp.size--;
        if (tmp.size >= DEGREE/2) return;
        erase_leaf_rebalance(address, tmp);
    }

    void erase_leaf_rebalance(long address, Node& this_node)
    {
        if (!this_node.parent)
        {
            if (this_node.size) return;
            file.delete_space(address);
            head = 0;
            return;
        }
        Node &parent_node = *file.readwrite(this_node.parent);
        K* this_key = uupper_bound(parent_node.key, parent_node.key+parent_node.size, this_node.key[0], comp) - 1;
        int locat = this_key - parent_node.key;
        // borrow from right sibling
        long right;
        if (locat == parent_node.size - 1)
            right = 0;
        else
            right = parent_node.ptr[locat+2];
        Node* right_node;
        if (right)
        {
            right_node = file.readwrite(right);
            if (right_node->size > DEGREE / 2)
            {
                this_node.key[this_node.size] = right_node->key[0];
                this_node.ptr[this_node.size] = right_node->ptr[0];
                this_node.size++;
                for (int i = 1; i < right_node->size; i++)
                {
                    right_node->key[i-1] = right_node->key[i];
                    right_node->ptr[i-1] = right_node->ptr[i];
                }
                right_node->size--;
                *(this_key+1) = right_node->key[0];
                return;
            }
        }
        // borrow from left sibling
        Node* left_node;
        long left;
        if (locat >= 0)
        {
            left = parent_node.ptr[locat];
            left_node = file.readwrite(left);
            if (left_node->size > DEGREE / 2)
            {
                for (int i = this_node.size; i > 0; i--)
                {
                    this_node.key[i] = this_node.key[i-1];
                    this_node.ptr[i] = this_node.ptr[i-1];
                }
                left_node->size--;
                this_node.key[0] = left_node->key[left_node->size];
                this_node.ptr[0] = left_node->ptr[left_node->size];
                this_node.size++;
                *this_key = this_node.key[0];
                return;
            }
        }
        // merge with siblings
        if (right)
        {
            for (int i = 0; i < right_node->size; i++)
            {
                this_node.key[this_node.size+i] = right_node->key[i];
                this_node.ptr[this_node.size+i] = right_node->ptr[i];
            }
            this_node.size += right_node->size;
            this_node.ptr[DEGREE] = right_node->ptr[DEGREE];
            for (int i = locat+1; i < parent_node.size-1; i++)
            {
                parent_node.key[i] = parent_node.key[i+1];
                parent_node.ptr[i+1] = parent_node.ptr[i+2];
            }
            parent_node.size--;
            file.delete_space(right);
        }
        else
        {
            for (int i = 0; i < this_node.size; i++)
            {
                left_node->key[left_node->size+i] = this_node.key[i];
                left_node->ptr[left_node->size+i] = this_node.ptr[i];
            }
            left_node->size += this_node.size;
            left_node->ptr[DEGREE] = this_node.ptr[DEGREE];
            for (int i = locat; i < parent_node.size-1; i++)
            {
                parent_node.key[i] = parent_node.key[i+1];
                parent_node.ptr[i+1] = parent_node.ptr[i+2];
            }
            parent_node.size--;
            file.delete_space(address);
        }
        if (parent_node.size >= DEGREE / 2) return;
        erase_internal_rebalance(this_node.parent, parent_node);
    }

    void erase_internal_rebalance(long address, Node& this_node)
    {
        Node* son;
        if (!this_node.parent)
        {
            if (this_node.size)
                return;
            son = file.readwrite(this_node.ptr[0]);
            son->parent = 0;
            head = this_node.ptr[0];
            file.delete_space(address);
            return;
        }
        Node &parent_node = *file.readwrite(this_node.parent);
        K* this_key = uupper_bound(parent_node.key, parent_node.key+parent_node.size, this_node.key[0], comp) - 1;
        int locat = this_key - parent_node.key;
        long right;
        if (locat == parent_node.size - 1)
            right = 0;
        else
            right = parent_node.ptr[locat+2];
        // borrow from right sibling
        Node* right_node;
        if (right)
        {
            right_node = file.readwrite(right);
            if (right_node->size > DEGREE / 2)
            {
                this_node.size++;
                son = file.readwrite(right_node->ptr[0]);
                this_node.ptr[this_node.size] = right_node->ptr[0];
                son->parent = address;
                this_node.key[this_node.size-1] = *(this_key+1); // NOT this_node.key[this_node.size-1] = son->key[0]!!!
                *(this_key+1) = right_node->key[0]; // THIS LINE SHOULD BE DONE BEFORE MOVING!
                for (int i = 1; i < right_node->size; i++)
                {
                    right_node->key[i-1] = right_node->key[i];
                    right_node->ptr[i-1] = right_node->ptr[i];
                }
                right_node->ptr[right_node->size-1] = right_node->ptr[right_node->size];
                right_node->size--;
                return;
            }
        }
        // borrow from left sibling
        Node* left_node;
        long left;
        if (locat >= 0)
        {
            left = parent_node.ptr[locat];
            left_node = file.readwrite(left);
            if (left_node->size > DEGREE / 2)
            {
                this_node.ptr[this_node.size+1] = this_node.ptr[this_node.size];
                for (int i = this_node.size; i > 0; i--)
                {
                    this_node.key[i] = this_node.key[i-1];
                    this_node.ptr[i] = this_node.ptr[i-1];
                }
                son = file.readwrite(left_node->ptr[left_node->size]);
                this_node.ptr[0] = left_node->ptr[left_node->size];
                son->parent = address;
                this_node.size++;
                left_node->size--;
                this_node.key[0] = *this_key; // NOT this_node.key[0] = son->key[0]!
                *this_key = left_node->key[left_node->size]; // SIGNIFICANT STEP!
                return;
            }
        }
        // merge with siblings
        if (right)
        {
            this_node.ptr[this_node.size+1] = right_node->ptr[0];
            son = file.readwrite(right_node->ptr[0]);
            son->parent = address;
            for (int i = 0; i < right_node->size; i++)
            {
                this_node.key[this_node.size+1+i] = right_node->key[i];
                this_node.ptr[this_node.size+i+2] = right_node->ptr[i+1];
                son = file.readwrite(right_node->ptr[i+1]);
                son->parent = address;
            }
            this_node.key[this_node.size] = parent_node.key[locat+1];
            this_node.size += right_node->size + 1;
            for (int i = locat + 1; i < parent_node.size - 1; i++)
            {
                parent_node.key[i] = parent_node.key[i+1];
                parent_node.ptr[i+1] = parent_node.ptr[i+2];
            }
            parent_node.size--;
            file.delete_space(right);
        }
        else
        {
            left_node->ptr[left_node->size+1] = this_node.ptr[0];
            son = file.readwrite(this_node.ptr[0]);
            son->parent = left;
            for (int i = 0; i < this_node.size; i++)
            {
                left_node->key[left_node->size+1+i] = this_node.key[i];
                left_node->ptr[left_node->size+i+2] = this_node.ptr[i+1];
                son = file.readwrite(this_node.ptr[i+1]);
                son->parent = left;
            }
            left_node->key[left_node->size] = parent_node.key[locat];
            left_node->size += this_node.size + 1;
            for (int i = locat; i < parent_node.size - 1; i++)
            {
                parent_node.key[i] = parent_node.key[i+1];
                parent_node.ptr[i+1] = parent_node.ptr[i+2];
            }
            parent_node.size--;
            file.delete_space(address);
        }
        if (parent_node.size >= DEGREE / 2) return;
        erase_internal_rebalance(this_node.parent, parent_node);
    }
};

#endif //TICKET_SYSTEM_BPT_H