#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"
#include <cstring>
#include <climits>
#include <cstddef>

namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector 
{
public:
    T *head,*tail,*ending;
    int space,used;
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
        T* heading;
        long long pos=0;

        iterator() {
            pos=0;
        }

        iterator(const iterator &other) {
            heading=other.heading;
            pos=other.pos;
        }

        iterator &operator=(const iterator &other) {
            if(this==&other) return *this;
            heading=other.heading;
            pos=other.pos;
            return *this;
        }
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const 
		{
			iterator temp(*this);
            temp.pos=pos+n;
            return temp;
		}
		iterator operator-(const int &n) const 
		{
			iterator temp(*this);
            temp.pos=pos-n;
            return temp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
            if(heading==rhs.heading) {
                return abs(pos-rhs.pos);
            }else {
                throw invalid_iterator();
            }
		}
		iterator& operator+=(const int &n) 
		{
			pos+=n;
            return *this;
		}
		iterator& operator-=(const int &n) 
		{
            pos-=n;
            return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
            iterator temp(*this);
            pos++;
            return temp;
        }
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
            pos++;
            return *this;
        }
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            iterator temp(*this);
            pos--;
            return temp;
        }
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
            pos--;
            return *this;
        }
		/**
		 * TODO *it
		 */
		T& operator*() const{
            return heading[pos];
        }
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
            if((heading==rhs.heading)&&(pos==rhs.pos)) return true;
            else false;
        }
		bool operator==(const const_iterator &rhs) const {
            if((heading==rhs.heading)&&(pos==rhs.pos)) return true;
            else false;
        }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
            if((heading==rhs.heading)&&(pos==rhs.pos)) return false;
            else return true;
        }
		bool operator!=(const const_iterator &rhs) const {
            if((heading==rhs.heading)&&(pos==rhs.pos)) return false;
            else true;
        }
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		/*TODO*/

    public:
        T* heading;
        long long pos;

        const_iterator() {
            pos=0;
        }

        const_iterator(const const_iterator &other) {
            heading=other.heading;
            pos=other.pos;
        }

        const_iterator &operator=(const const_iterator &other) {
            if(this==&other) return *this;
            heading=other.heading;
            pos=other.pos;
            return *this;
        }
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        const_iterator operator+(const int &n) const
        {
            const_iterator temp(*this);
            temp.pos=pos+n;
            return temp;
        }
        const_iterator operator-(const int &n) const
        {
            const_iterator temp(*this);
            temp.pos=pos-n;
            return temp;
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const
        {
            if(heading==rhs.heading) {
                return abs(pos-rhs.pos);
            }else {
                throw invalid_iterator();
            }
        }
        const_iterator& operator+=(const int &n)
        {
            pos+=n;
            return *this;
        }
        const_iterator& operator-=(const int &n)
        {
            pos-=n;
            return *this;
        }
        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            const_iterator temp(*this);
            temp.pos=pos;
            pos++;
            return temp;
        }
        /**
         * TODO ++iter
         */
        const_iterator& operator++() {
            pos++;
            return *this;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            const_iterator temp(*this);
            temp.pos=pos;
            pos--;
            return temp;
        }
        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            pos++;
            return *this;
        }
        /**
         * TODO *it
         */
        T& operator*() const{
            return heading[pos];
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            if((heading==rhs.heading)&&(pos==rhs.pos)) return true;
            else false;
        }
        bool operator==(const const_iterator &rhs) const {
            if((heading==rhs.heading)&&(pos==rhs.pos)) return true;
            else false;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            if((heading==rhs.heading)&&(pos==rhs.pos)) return false;
            else true;
        }
        bool operator!=(const const_iterator &rhs) const {
            if((heading==rhs.heading)&&(pos==rhs.pos)) return false;
            else return true;
        }
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {
        head=(T*) malloc(1000* sizeof(T));
        tail=head;
        ending=&(head[999]);
        space=0;
        used=0;
    }
	vector(const vector &other) {
        long long k=1000;
        for(int i=0;i<other.space;i++) k=k*2;
        space=other.space;
        head=(T*) malloc(k* sizeof(T));
        for(int i=0;i<other.used;i++) {
            new(head + i) T(other.head[i]);
        }
        tail=&(head[used]);
        ending=&(head[k-1]);
        used=other.used;
    }
	/**
	 * TODO Destructor
	 */
	~vector() {
        for(int i=0;i<used;i++) head[i].~T();
       free(head);
    }
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
        if(this==&other) return *this;
        long long k=1000;
        for(int i=0;i<other.space;i++) k=k*2;
        space=other.space;
         T* head1=(T*) malloc(k* sizeof(T));
        for(int i=0;i<other.used;i++) {
            new(head1 + i) T(other.head[i]);
        }
        for(int i=0;i<used;i++) head[i].~T();
        free(head);
        head=head1;
        ending=&(head[k-1]);
        used=other.used;
        tail=&(head[used]);
        return *this;
    }
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
            if(pos+1>used) {
                index_out_of_bound a;
                throw a;
            }
            else {
                return head[pos];
            }
    }
	const T & at(const size_t &pos) const {
        if(pos+1>used) {
            throw index_out_of_bound();
        }
        else {
            T temp(head[pos]);
            return temp;
        }
    }
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
        if(pos+1>used) {
            throw index_out_of_bound();
        }
        else {
            return head[pos];
        }
    }
	const T & operator[](const size_t &pos) const {
        if(pos+1>used) {
            throw index_out_of_bound();
        }
        else {
            return head[pos];
        }
    }
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
        if(used==0) {
            throw container_is_empty();
        }
        else {
            return head[0];
        }
    }
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
        if(used==0) {
            throw container_is_empty();
        }
        else {
            return *(tail-1);
        }
    }
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
        iterator t;
        t.heading=head;
        t.pos=0;
        return t;
    }
	const_iterator cbegin() const {
        const_iterator t;
        t.heading=head;
        t.pos=0;
        return t;
    }
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
        iterator t;
        t.heading=head;
        t.pos=used;
        return t;
    }
	const_iterator cend() const {
        const_iterator t;
        t.heading=head;
        t.pos=used;
        return t;
    }
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
        if (used==0) {
            return true;
        }
        else {
            return false;
        }
    }
	/**
	 * returns the number of elements
	 */
	size_t size() const {
        return used;
    }
	/**
	 * clears the contents
	 */
	void clear() {
        if(used!=0) {
            tail=head+1;
            used=0;
        }
    }
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator post, const T &value) {
        if(post.pos>used) {
            space++;
            T* head1;
            int space1,used1;
            space1=space;
            used1=used;
            long long k=1000;
            for(int i=0;i<space;i++) k=k*2;
            k*=2;
            head1=(T*) malloc(k* sizeof(T));
            for(int i=0;i<post.pos;i++) {
                new(head1 + i) T(head[i]);
                head[i].~T();
            }
            new(head1 + post.pos) T(value);
            head[post.pos].~T();
            for(int i=post.pos+1;i<used+1;i++) {
                new(head1 + i) T(head[i-1]);
                head[i-1].~T();
            }
            free(head);
            head=head1;
            space=space1;
            used=used1+1;
            tail=&head[used];
            ending=&head[k-1];
            iterator temp=begin();
            temp.pos=post.pos;
            return temp;
        }
        else {
            T t(head[post.pos]);
            new (tail) T(head[post.pos]);
            head[post.pos]=value;
            for(int i=post.pos+1;i<used;i++) {
                *tail=head[i];
                head[i]=t;
                t=*tail;
            }
            tail++;
            used++;
            iterator temp=begin();
            temp.pos=post.pos;
            return temp;
        }
    }
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
        if(ind>used) {
            throw index_out_of_bound();
        }
        else {
            size_t t=ind;
            T te(value);
            return insert(t,te);
        }
    }
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator post) {
        head[post.pos].~T();
        for(int i=post.pos;i<used-1;i++) {
            head[i]=head[i+1];
        }
        tail--;
        used--;
        iterator temp=begin();
        temp.pos=post.pos;
        return temp;
    }
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
        if(ind>=used) {
            throw index_out_of_bound();
        }
        else {
            size_t t=ind;
            return erase(t);
        }
    }
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
        if(tail+1!=ending) {
            new(tail) T (value);
            tail++;
            used++;
        }
        else {
            space++;
            T* head1;
            int space1,used1;
            space1=space;
            used1=used;
            long long k=1000;
            for(int i=0;i<space;i++) k=k*2;
            k*=2;
            head1=(T*) malloc(k* sizeof(T));
            for(int i=0;i<used;i++) {
                new(head1 + i) T(head[i]);
                head[i].~T();
            }
            new(head1+used) T(value);
            free(head);
            head=head1;
            space=space1;
            used=used1+1;
            tail=&head[used];
            ending=&head[k-1];
        }
    }
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
        if(used==0) {
            throw container_is_empty();
        }
        else {
            head[used-1].~T();
            tail--;
            used--;
        }
    }
};


}

#endif
