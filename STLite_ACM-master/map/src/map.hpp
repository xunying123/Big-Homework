/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

    template<class Key, class T, class Compare = std::less<Key>>
    class map {

    public:
        /**
         * the internal type of data.
         * it should have a default constructor, a copy constructor.
         * You can use sjtu::map as value_type by typedef.
         */
        typedef pair<const Key, T> value_type;

        /**
         * see BidirectionalIterator at CppReference for help.
         *
         * if there is anything wrong throw invalid_iterator.
         *     like it = map.begin(); --it;
         *       or it = map.end(); ++end();
         */
        class iterator;

        class const_iterator;

    private:
        enum color {
            RED, BLACK
        };

        enum position {
            NONE, ONEL, ONER, TWOK
        };

        enum rotate {
            LEFT, RIGHT, STAY
        };

        class node {
        public:

            color co = RED;
            value_type *value = nullptr;
            node *parent = nullptr;
            node *left = nullptr;
            node *right = nullptr;

            node() = default;

            node(const value_type &other, color cc) {
                value = new value_type(other);
                co = cc;
            }

            node(value_type *other, color cc) {
                value = other;
                co = cc;
            }

            node(const node &other) {
                co = other.co;
                value = new value_type(*other.value);
            }

            node &operator=(const node &other) {
                if (this == &other) return *this;
                value = new value_type(*other.value);
                co = other.co;
            }

            ~node() {
                delete value;
            }

            Key getkey() const {
                return value->first;
            }

            void setcolor(color cc) {
                co = cc;
            }

            bool isred() const {
                return co == RED;
            }

            bool isleft() const {
                if (parent->left == this) return true;
                else return false;
            }

            bool isright() const {
                if (parent->right == this) return true;
                else return false;
            }

            bool ishead() const {
                if (parent == nullptr) return true;
                else return parent->parent == this && value == nullptr;
            }

            bool isroot() const {
                if (parent == nullptr) return false;
                else return parent->parent == this && value != nullptr;
            }

            int childnumber() const {
                int count = 0;
                if (left != nullptr) count++;
                if (right != nullptr) count++;
                return count;
            }

            int redchildnumber() const {
                int count = 0;
                if (left != nullptr && left->isred()) count++;
                if (right != nullptr && right->isred()) count++;
                return count;
            }

            position redchildposition() {
                switch (redchildnumber()) {
                    case 0:
                        return NONE;
                    case 1: {
                        if (isleft()) {
                            if (left != nullptr && left->isred()) return ONEL;
                            else return ONER;
                        } else {
                            if (right != nullptr && right->isred()) return ONEL;
                            else return ONER;
                        }
                    }
                    default:
                        return TWOK;
                }
            }

            rotate rotatetype() {
                if (isroot()) return STAY;
                else if (isleft()) return LEFT;
                else return RIGHT;
            }

            node *&rotating(rotate r) {
                switch (r) {
                    case LEFT:
                        return parent->left;
                    case RIGHT:
                        return parent->right;
                    default:
                        return parent->parent;
                }
            }

            node *&chaning() {
                if (isroot())return parent->parent;
                else if (isleft())return parent->left;
                else return parent->right;
            }

            void rleft() {
                rotate r = rotatetype();
                rotating(r) = right;
                right = right->left;
                if (right != nullptr) right->parent = this;
                rotating(r)->parent = parent;
                rotating(r)->left = this;
                parent = rotating(r);
            }

            void rright() {
                rotate r = rotatetype();
                rotating(r) = left;
                left = left->right;
                if (left != nullptr) left->parent = this;
                rotating(r)->parent = parent;
                rotating(r)->right = this;
                parent = rotating(r);
            }

            void swapc(node *other) {
                color c = other == nullptr ? BLACK : other->co;
                if (other != nullptr) other->setcolor(co);
                setcolor(c);
            }

            void swapp(node *other) {
                if (other->parent == this) other->swapp(this);
                else if (parent == other) {
                    bool l = isleft();
                    node *ll = left;
                    node *rr = right;
                    node *ss = sibling();
                    other->chaning() = this;
                    parent = other->parent;
                    other->parent = this;
                    if (ll != nullptr) ll->parent = other;
                    if (rr != nullptr) rr->parent = other;
                    if (ss != nullptr) ss->parent = this;
                    other->left = ll;
                    other->right = rr;
                    if (l) {
                        right = ss;
                        left = other;
                    } else {
                        left = ss;
                        right = other;
                    }
                } else {
                    chaning() = other;
                    other->chaning() = this;
                    if (left != nullptr)left->parent = other;
                    if (right != nullptr)right->parent = other;
                    node *ll = left;
                    node *rr = right;
                    node *pp = parent;
                    left = other->left;
                    right = other->right;
                    parent = other->parent;
                    if (other->left != nullptr)other->left->parent = this;
                    if (other->right != nullptr)other->right->parent = this;
                    other->left = ll;
                    other->right = rr;
                    other->parent = pp;
                }
            }

            node *sibling() const {
                if (isroot()) return nullptr;
                else if (isleft()) return parent->right;
                else return parent->left;
            }

            node *fmax() const {
                node *temp = const_cast<node *>(this);
                while (temp->right != nullptr) temp = temp->right;
                return temp;
            }

            node *fmin() const {
                node *temp = const_cast<node *>(this);
                while (temp->left != nullptr) temp = temp->left;
                return temp;
            }

            node *nexting() const {
                node *temp = const_cast<node *>(this);
                if (temp->right != nullptr) temp = temp->right->fmin();
                else {
                    while (temp->isright()) temp = temp->parent;
                    if (temp->right != temp->parent) temp = temp->parent;
                }
                return temp;
            }

            node *preing() const {
                node *temp = const_cast<node *>(this);
                if (temp->ishead()) temp = temp->right;
                else if (temp->left != nullptr) temp = temp->left->fmax();
                else {
                    while (temp->isleft()) temp = temp->parent;
                    if (temp->left != temp->parent) temp = temp->parent;
                }
                return temp;
            }
        };

        node *head = nullptr;
        int siz = 0;
        Compare cmp;

        void cl(node *temp) {
            if (temp == nullptr) return;
            cl(temp->left);
            cl(temp->right);
            delete temp;
        }

        node *makeatree(node *par, node *other) {
            if (other == nullptr) return nullptr;
            node *temp = new node(*other);
            temp->parent = par;
            temp->left = makeatree(temp, other->left);
            temp->right = makeatree(temp, other->right);
            return temp;
        }

        node *finding(const Key &other) const {
            node *temp = head->parent;
            while (temp != nullptr) {
                if ((!cmp(temp->getkey(), other)) && (!cmp(other, temp->getkey()))) return temp;
                temp = cmp(other, temp->getkey()) ? temp->left : temp->right;
            }
            return nullptr;
        }

        void insertbalance(node *temp) {
            if (temp->isleft()) {
                if (temp->parent->isleft()) {
                    temp->parent->parent->rright();
                    temp->parent->swapc(temp->parent->right);
                } else {
                    temp->parent->rright();
                    temp->parent->rleft();
                    temp->swapc(temp->left);
                }
            } else {
                if (temp->parent->isright()) {
                    temp->parent->parent->rleft();
                    temp->parent->swapc(temp->parent->left);
                } else {
                    temp->parent->rleft();
                    temp->parent->rright();
                    temp->swapc(temp->right);
                }
            }
        }

        node *findinsert(const value_type &value) {
            node *p = head;
            node *temp = head->parent;
            while (temp != nullptr) {
                if (temp->redchildnumber() == 2) {
                    temp->left->setcolor(BLACK);
                    temp->right->setcolor(BLACK);
                    if (!temp->isroot()) {
                        temp->setcolor(RED);
                        if (temp->parent->isred()) insertbalance(temp);
                    }
                }
                p = temp;
                temp = cmp(value.first, temp->getkey()) ? temp->left : temp->right;
            }
            return p;
        }

        iterator inserting(node *pos, const value_type &value) {
            siz++;
            if (pos == head) {
                head->parent = new node(value, BLACK);
                head->parent->parent = head;
                head->left = head->right = head->parent;
                return iterator(head->parent);
            }
            node *temp = new node(value, RED);
            if (cmp(value.first, pos->getkey())) {
                if (pos == head->left) head->left = temp;
                pos->left = temp;
            } else {
                if (pos == head->right) head->right = temp;
                pos->right = temp;
            }
            temp->parent = pos;
            if (temp->parent->isred()) insertbalance(temp);
            return iterator(temp);
        }

        void erasebalance(node *temp) {
            if (temp->isroot()) return;
            if (temp->sibling() != nullptr && temp->sibling()->isred()) {
                temp->parent->swapc(temp->sibling());
                if (temp->isleft()) temp->parent->rleft();
                else temp->parent->rright();
            }
            position pp;
            if (temp->sibling() == nullptr || ((pp = temp->sibling()->redchildposition()) == NONE)) {
                if (temp->parent->isred()) {
                    temp->parent->swapc(temp->sibling());
                    return;
                }
                if (temp->sibling() != nullptr) temp->sibling()->setcolor(RED);
                erasebalance(temp->parent);
            } else if (pp == ONEL) {
                temp->sibling()->setcolor(temp->parent->co);
                temp->parent->setcolor(BLACK);
                if (temp->isleft()) temp->parent->rleft();
                else temp->parent->rright();
                temp->parent->sibling()->setcolor(BLACK);
            } else {
                if (temp->isleft()) {
                    temp->sibling()->rright();
                    temp->parent->rleft();
                } else {
                    temp->sibling()->rleft();
                    temp->parent->rright();
                }
                temp->parent->parent->setcolor(temp->parent->co);
                temp->parent->setcolor(BLACK);
            }
        }

        void erasing(node *temp) {
            siz--;
            if (temp->childnumber() == 2) {
                node *n = temp->nexting();
                temp->swapp(n);
                temp->swapc(n);
            }
            if (temp->childnumber() == 1) {
                if (temp->left != nullptr) {
                    temp->left->parent = temp->parent;
                    temp->left->setcolor(BLACK);
                    temp->chaning() = temp->left;
                } else {
                    temp->right->parent = temp->parent;
                    temp->right->setcolor(BLACK);
                    temp->chaning() = temp->right;
                }
            } else {
                if (!temp->isred()) erasebalance(temp);
                temp->chaning() = nullptr;
            }
            delete temp;
        }

    public:


        class iterator {
            friend class map;

            friend class const_iterator;

        private:
            node *_node = nullptr;

            Key getkey() const {
                return _node->getkey();
            }
            /**
             * TODO add data members
             *   just add whatever you want.
             */
        public:

            bool invalid() const {
                return _node == nullptr;
            }

            iterator() = default;

            iterator(const iterator &other) {
                _node = other._node;
            }

            explicit iterator(node *other) {
                _node = other;
            }

            /**
             * TODO iter++
             */
            iterator operator++(int) {
                iterator temp = *this;
                ++(*this);
                return temp;
            }

            /**
             * TODO ++iter
             */
            iterator &operator++() {
                if (_node == nullptr) throw invalid_iterator();
                else if (_node->ishead()) {
                    _node = nullptr;
                    throw invalid_iterator();
                }
                _node = _node->nexting();
                return *this;
            }

            /**
             * TODO iter--
             */
            iterator operator--(int) {
                iterator temp = *this;
                --(*this);
                return temp;
            }

            /**
             * TODO --iter
             */
            iterator &operator--() {
                if (_node == nullptr) throw invalid_iterator();
                _node = _node->preing();
                if (_node->ishead()) {
                    _node = nullptr;
                    throw invalid_iterator();
                }
                return *this;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type &operator*() const {
                if (_node == nullptr || _node->ishead()) throw invalid_iterator();
                else return *(_node->value);
            }

            bool operator==(const iterator &rhs) const {
                return _node == rhs._node;
            }

            bool operator==(const const_iterator &rhs) const {
                return _node == rhs._node;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return _node != rhs._node;
            }

            bool operator!=(const const_iterator &rhs) const {
                return _node != rhs._node;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const {
                if (_node == nullptr || _node->ishead()) throw invalid_iterator();
                else return (_node->value);
            }
        };

        class const_iterator {
            friend class map;

            friend class iterator;
            // it should has similar member method as iterator.
            //  and it should be able to construct from an iterator.
        private:
            const node *_node = nullptr;

            Key getkey() const {
                return _node->getkey();
            }
            // data members.
        public:

            bool invalid() const {
                return _node == nullptr;
            }

            const_iterator() = default;

            const_iterator(const const_iterator &other) {
                _node = other._node;
            }

            const_iterator(const iterator &other) {
                _node = other._node;
            }

            explicit const_iterator(class node *other) {
                _node = other;
            }

            const_iterator &operator=(const iterator &other) {
                _node = other._node;
                return *this;
            }

            /**
            * TODO iter++
            */
            const_iterator operator++(int) {
                const_iterator temp = *this;
                ++(*this);
                return temp;
            }

            /**
             * TODO ++iter
             */
            const_iterator &operator++() {
                if (_node == nullptr) throw invalid_iterator();
                else if (_node->ishead()) {
                    _node = nullptr;
                    throw invalid_iterator();
                }
                _node = _node->nexting();
                return *this;
            }

            /**
             * TODO iter--
             */
            const_iterator operator--(int) {
                const_iterator temp = *this;
                --(*this);
                return temp;
            }

            /**
             * TODO --iter
             */
            const_iterator &operator--() {
                if (_node == nullptr) throw invalid_iterator();
                _node = _node->preing();
                if (_node->ishead()) {
                    _node = nullptr;
                    throw invalid_iterator();
                }
                return *this;
            }

            /**
             * a operator to check whether two iterators are same (pointing to the same memory).
             */
            value_type &operator*() const {
                if (_node == nullptr || _node->ishead()) throw invalid_iterator();
                else return *(_node->value);
            }

            bool operator==(const iterator &rhs) const {
                return _node == rhs._node;
            }

            bool operator==(const const_iterator &rhs) const {
                return _node == rhs._node;
            }

            /**
             * some other operator for iterator.
             */
            bool operator!=(const iterator &rhs) const {
                return _node != rhs._node;
            }

            bool operator!=(const const_iterator &rhs) const {
                return _node != rhs._node;
            }

            /**
             * for the support of it->first.
             * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
             */
            value_type *operator->() const {
                if (_node == nullptr || _node->ishead()) throw invalid_iterator();
                return _node->value;
            }
        };

        /**
         * TODO two constructors
         */
        map() {
            head = new node;
            head->parent = nullptr;
            head->left = head;
            head->right = head;
        }

        map(const map &other) {
            siz = other.siz;
            head = new node;
            head->parent = makeatree(head, other.head->parent);
            head->left = other.empty() ? head : finding(other.head->left->getkey());
            head->right = other.empty() ? head : finding(other.head->right->getkey());
        }

        /**
         * TODO assignment operator
         */
        map &operator=(const map &other) {
            if (this == &other) return *this;
            cl(head->parent);
            siz = other.siz;
            head->parent = makeatree(head, other.head->parent);
            head->left = other.empty() ? head : finding(other.head->left->getkey());
            head->right = other.empty() ? head : finding(other.head->right->getkey());
            return *this;
        }

        /**
         * TODO Destructors
         */
        ~map() {
            cl(head->parent);
            delete head;
        }

        /**
         * TODO
         * access specified element with bounds checking
         * Returns a reference to the mapped value of the element with key equivalent to key.
         * If no such element exists, an exception of type `index_out_of_bound'
         */
        T &at(const Key &key) {
            node *temp = finding(key);
            if (temp == nullptr) throw index_out_of_bound();
            return temp->value->second;
        }

        const T &at(const Key &key) const {
            node *pos = finding(key);
            if (pos == nullptr)throw index_out_of_bound();
            return pos->value->second;
        }

        /**
         * TODO
         * access specified element
         * Returns a reference to the value that is mapped to a key equivalent to key,
         *   performing an insertion if such key does not already exist.
         */
        T &operator[](const Key &key) {
            node *pos = finding(key);
            if (pos != nullptr) return pos->value->second;
            value_type v(key, T());
            iterator temp = inserting(findinsert(v), v);
            return temp._node->value->second;
        }

        /**
         * behave like at() throw index_out_of_bound if such key does not exist.
         */
        const T &operator[](const Key &key) const {
            return at(key);
        }

        /**
         * return a iterator to the beginning
         */
        iterator begin() {
            return iterator(head->left);
        }

        const_iterator cbegin() const {
            return const_iterator(head->left);
        }

        /**
         * return a iterator to the end
         * in fact, it returns past-the-end.
         */
        iterator end() {
            return iterator(head);
        }

        const_iterator cend() const {
            return const_iterator(head);
        }

        /**
         * checks whether the container is empty
         * return true if empty, otherwise false.
         */
        bool empty() const {
            return siz == 0;
        }

        /**
         * returns the number of elements.
         */
        size_t size() const {
            return siz;
        }

        /**
         * clears the contents
         */
        void clear() {
            cl(head->parent);
            siz = 0;
            head->left = head;
            head->right = head;
            head->parent = nullptr;

        }

        /**
         * insert an element.
         * return a pair, the first of the pair is
         *   the iterator to the new element (or the element that prevented the insertion),
         *   the second one is true if insert successfully, or false.
         */
        pair<iterator, bool> insert(const value_type &value) {
            node *temp = finding(value.first);
            if (temp != nullptr) return pair<iterator, bool>(iterator(temp), false);
            else return pair<iterator, bool>(iterator(inserting(findinsert(value), value)), true);
        }

        /**
         * erase the element at pos.
         *
         * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
         */
        void erase(iterator pos) {
            if (pos == end() || pos.invalid() || find(pos._node->getkey()) != pos) throw invalid_iterator();
            if (pos._node == head->left) head->left = head->left->nexting();
            if (pos._node == head->right) head->right = head->right->preing();
            erasing(pos._node);
        }

        /**
         * Returns the number of elements with key
         *   that compares equivalent to the specified argument,
         *   which is either 1 or 0
         *     since this container does not allow duplicates.
         * The default method of check the equivalence is !(a < b || b > a)
         */
        size_t count(const Key &key) const {
            if (finding(key) == nullptr) return 0;
            else return 1;
        }

        /**
         * Finds an element with key equivalent to key.
         * key value of the element to search for.
         * Iterator to an element with key equivalent to key.
         *   If no such element is found, past-the-end (see end()) iterator is returned.
         */
        iterator find(const Key &key) {
            node *temp = finding(key);
            if (temp == nullptr) return end();
            else return iterator(temp);
        }

        const_iterator find(const Key &key) const {
            node *temp = finding(key);
            if (temp == nullptr) return cend();
            else return const_iterator(temp);
        }
    };

}

#endif