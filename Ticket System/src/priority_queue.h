#ifndef TICKET_SYSTEM_PRIORITY_QUEUE_H
#define TICKET_SYSTEM_PRIORITY_QUEUE_H

#include <cstddef>
#include <functional>

template<typename T, class Compare = std::less<T>>
class priority_queue {

private:

    class node {
    public:
        T *value = nullptr;
        node *left = nullptr;
        node *right = nullptr;

        node() = default;

        node(const node &other) {
            value = new T(*(other.value));
        }

        explicit node(const T &temp) {
            value = new T(temp);
        }

        ~node() {
            delete value;
        }
    };

    class tree {
    public:
        node *head = nullptr;
        size_t size = 0;

        tree() = default;

        tree(const tree &other) {
            size = other.size;
            create(head, other.head);
        }

        ~tree() {
            dele(head);
        }

        tree &operator=(const tree &other) {
            if (this == &other) return *this;
            clear();
            head = nullptr;
            size = other.size;
            create(head, other.head);
            return *this;
        }

        void create(node *&first, node *second) {
            if (second == nullptr) return;
            first = new node(*(second->value));
            create(first->left, second->left);
            create(first->right, second->right);
        }

        void clear() {
            dele(head);
            size = 0;
        }

        void dele(node *temp) {
            if (temp == nullptr) return;
            dele(temp->left);
            dele(temp->right);
            delete temp;
        }

        void push(const T &temp) {
            node *tt = new node(temp);
            head = merging(head, tt);
            size++;
        }

        void pop() {
            node *temp = head;
            head = merging(head->left, head->right);
            delete temp;
            size--;
        }

        void merge(tree &other) {
            head = merging(head, other.head);
            size += other.size;
            other.head = nullptr;
            other.size = 0;
        }

        const T &top() const {
            return *(head->value);
        }

        size_t length() const {
            return size;
        }

        node *merging(node *first, node *second) {
            if (first == nullptr) return second;
            if (second == nullptr) return first;
            Compare cmp;
            if (cmp(*first->value, *second->value)) {
                node *temp = first;
                first = second;
                second = temp;
            }
            first->right = merging(first->right, second);
            node *temp = first->right;
            first->right = first->left;
            first->left = temp;
            return first;
        }

    };

    tree heap;

public:
    priority_queue() {}

    priority_queue(const priority_queue &other) {
        heap = other.heap;
    }

    ~priority_queue() {}

    priority_queue &operator=(const priority_queue &other) {
        if (this == &other)return *this;
        heap = other.heap;
        return *this;
    }

    const T &top() const {
        return heap.top();
    }

    void push(const T &e) {
        heap.push(e);
    }

    void pop() {
        if (empty()) return;
        heap.pop();
    }

    size_t size() const {
        return heap.length();
    }

    bool empty() const {
        return heap.length() == 0;
    }

    void merge(priority_queue &other) {
        heap.merge(other.heap);
    }
};


#endif //TICKET_SYSTEM_PRIORITY_QUEUE_H
