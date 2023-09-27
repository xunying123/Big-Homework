
#ifndef TICKET_SYSTEM_HASH_MAP_H
#define TICKET_SYSTEM_HASH_MAP_H

class _hash_map {
private:
    struct h_node {
        h_node *next;
        char key[21];
        int data;

        h_node() {
            next = nullptr;
        }

        h_node(char* k, int d) {
            next = nullptr;
            strcpy(key,k);
            data = d;
        }
    };

public:
    h_node arr[2003];

    ~_hash_map() {
        h_node *k, *d;
        for (int i = 0; i < 2003; i++) {
            k = arr[i].next;
            while (k != nullptr) {
                d = k->next;
                delete k;
                k = d;
            }
        }
    }

    void insert(char* k, int d) {
        int hh;
        hh=(k[0]+3*k[1]+9*k[2]+27*k[3]+81*k[4]+243*k[5]+729*k[6])%2003;
        h_node *h = &arr[hh % 2003];
        h_node *p = h;
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = new h_node(k, d);
    }

    void erase(char* k) {
        int hh;
        hh=(k[0]+3*k[1]+9*k[2]+27*k[3]+81*k[4]+243*k[5]+729*k[6])%2003;
        h_node *h = &arr[hh % 2003];
        h_node *p = h->next;
        while (p != nullptr) {
            if (strcmp(p->key , k)==0) {
                h->next = p->next;
                delete p;
                return;
            }
            h = p;
            p = p->next;
        }
    }

    int find(char* k) {
        int hh;
        hh=(k[0]+3*k[1]+9*k[2]+27*k[3]+81*k[4]+243*k[5]+729*k[6])%2003;
        h_node *h = (arr[hh % 2003].next);
        while (h != nullptr) {
            if (strcmp(h->key , k)==0) {
                return h->data;
            }
            h = h->next;
        }
        return -1;
    }
};

#endif //TICKET_SYSTEM_HASH_MAP_H
