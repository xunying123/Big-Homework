#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
    template<typename T, class Compare = std::less<T>>
    class priority_queue {

    private:

        class node {
        public:
            T *value = nullptr;
            node *left = nullptr;
            node *right = nullptr;

            node()=default;

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
            node *head= nullptr;
            size_t size=0;

            tree() =default;

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
                return *(head->value);+
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
        /**
         * TODO constructors
         */

        priority_queue() {}

        priority_queue(const priority_queue &other) {
            heap = other.heap;
        }

        /**
         * TODO deconstructor
         */
        ~priority_queue() {}

        /**
         * TODO Assignment operator
         */
        priority_queue &operator=(const priority_queue &other) {
            if (this == &other)return *this;
            heap = other.heap;
            return *this;
        }

        /**
         * get the top of the queue.
         * @return a reference of the top element.
         * throw container_is_empty if empty() returns true;
         */
        const T &top() const {
            if(empty()) throw container_is_empty();
            return heap.top();
        }

        /**
         * TODO
         * push new element to the priority queue.
         */
        void push(const T &e) {
            if(!empty()) {
                Compare cmp;
                try {
                    bool a = cmp(e, this->top());
                } catch (...) {
                    return ;
                }
            }
            heap.push(e);
        }

        /**
         * TODO
         * delete the top element.
         * throw container_is_empty if empty() returns true;
         */
        void pop() {
            if (empty())throw container_is_empty();
            heap.pop();
        }

        /**
         * return the number of the elements.
         */
        size_t size() const {
            return heap.length();
        }

        /**
         * check if the container has at least an element.
         * @return true if it is empty, false if it has at least an element.
         */
        bool empty() const {
            return heap.length() == 0;
        }

        /**
         * merge two priority_queues with at least O(logn) complexity.
         * clear the other priority_queue.
         */
        void merge(priority_queue &other) {
            Compare cmp;
            try {
                bool a = cmp(other.top(), this->top());
            } catch (...) {
                throw exception();
            }
            heap.merge(other.heap);
        }
    };

}

#endif
