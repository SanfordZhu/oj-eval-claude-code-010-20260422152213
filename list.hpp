#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>
#include <vector>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T data;
        node *prev;
        node *next;

        node() : prev(nullptr), next(nullptr) {}
        node(const T &value) : data(value), prev(nullptr), next(nullptr) {}
        ~node() {}
    };

protected:
    /**
     * add data members for linked list as protected members
     */
    node *head;
    node *tail;
    size_t list_size;

    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        if (pos->prev) {
            pos->prev->next = cur;
        } else {
            head = cur;
        }
        pos->prev = cur;
        list_size++;
        return cur;
    }
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        if (pos->prev) {
            pos->prev->next = pos->next;
        } else {
            head = pos->next;
        }
        if (pos->next) {
            pos->next->prev = pos->prev;
        } else {
            tail = pos->prev;
        }
        list_size--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        /**
         * TODO add data members
         *   just add whatever you want.
         */
        node *ptr;
        list *container;

    public:
        iterator(node *p = nullptr, list *c = nullptr) : ptr(p), container(c) {}

        /**
         * iter++
         */
        iterator operator++(int) {
            if (!ptr) throw invalid_iterator();
            iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        /**
         * ++iter
         */
        iterator & operator++() {
            if (!ptr) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            if (!container) throw invalid_iterator();
            iterator temp = *this;
            if (ptr == nullptr) {
                if (container->empty()) throw invalid_iterator();
                ptr = container->tail;
            } else if (ptr == container->head) {
                throw invalid_iterator();
            } else {
                ptr = ptr->prev;
            }
            return temp;
        }
        /**
         * --iter
         */
        iterator & operator--() {
            if (!container) throw invalid_iterator();
            if (ptr == nullptr) {
                if (container->empty()) throw invalid_iterator();
                ptr = container->tail;
            } else if (ptr == container->head) {
                throw invalid_iterator();
            } else {
                ptr = ptr->prev;
            }
            return *this;
        }
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (!ptr) throw invalid_iterator();
            return ptr->data;
        }
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (!ptr) throw invalid_iterator();
            return &(ptr->data);
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        friend class list;
        friend class const_iterator;
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const node *ptr;
        const list *container;

    public:
        const_iterator(const node *p = nullptr, const list *c = nullptr) : ptr(p), container(c) {}
        const_iterator(const iterator &it) : ptr(it.ptr), container(it.container) {}

        const_iterator operator++(int) {
            if (!ptr) throw invalid_iterator();
            const_iterator temp = *this;
            ptr = ptr->next;
            return temp;
        }
        const_iterator & operator++() {
            if (!ptr) throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }
        const_iterator operator--(int) {
            if (!container) throw invalid_iterator();
            const_iterator temp = *this;
            if (ptr == nullptr) {
                if (container->empty()) throw invalid_iterator();
                ptr = container->tail;
            } else if (ptr == container->head) {
                throw invalid_iterator();
            } else {
                ptr = ptr->prev;
            }
            return temp;
        }
        const_iterator & operator--() {
            if (!container) throw invalid_iterator();
            if (ptr == nullptr) {
                if (container->empty()) throw invalid_iterator();
                ptr = container->tail;
            } else if (ptr == container->head) {
                throw invalid_iterator();
            } else {
                ptr = ptr->prev;
            }
            return *this;
        }
        const T & operator *() const {
            if (!ptr) throw invalid_iterator();
            return ptr->data;
        }
        const T * operator ->() const {
            if (!ptr) throw invalid_iterator();
            return &(ptr->data);
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
    };
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : head(nullptr), tail(nullptr), list_size(0) {}

    list(const list &other) : head(nullptr), tail(nullptr), list_size(0) {
        for (node *p = other.head; p; p = p->next) {
            push_back(p->data);
        }
    }
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
    }
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (node *p = other.head; p; p = p->next) {
            push_back(p->data);
        }
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) throw container_is_empty();
        return head->data;
    }
    const T & back() const {
        if (empty()) throw container_is_empty();
        return tail->data;
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head, this);
    }
    const_iterator cbegin() const {
        return const_iterator(head, this);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(nullptr, this);
    }
    const_iterator cend() const {
        return const_iterator(nullptr, this);
    }
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return list_size == 0;
    }
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return list_size;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        while (head) {
            node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        list_size = 0;
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        node *new_node = new node(value);
        node *pos_node = pos.ptr;

        if (!pos_node) {
            if (pos.container != this) throw invalid_iterator();
            if (tail) {
                insert_after(tail, new_node);
            } else {
                head = tail = new_node;
                list_size = 1;
            }
        } else {
            if (pos.container != this) throw invalid_iterator();
            insert_before(pos_node, new_node);
        }

        return iterator(new_node, this);
    }

    void insert_before(node *pos, node *new_node) {
        new_node->prev = pos->prev;
        new_node->next = pos;
        if (pos->prev) {
            pos->prev->next = new_node;
        } else {
            head = new_node;
        }
        pos->prev = new_node;
        list_size++;
    }

    void insert_after(node *pos, node *new_node) {
        new_node->prev = pos;
        new_node->next = pos->next;
        if (pos->next) {
            pos->next->prev = new_node;
        } else {
            tail = new_node;
        }
        pos->next = new_node;
        list_size++;
    }

    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty()) throw container_is_empty();
        if (!pos.ptr || pos.container != this) throw invalid_iterator();

        node *next_node = pos.ptr->next;
        erase_node(pos.ptr);
        return iterator(next_node, this);
    }

    void erase_node(node *pos) {
        if (pos->prev) {
            pos->prev->next = pos->next;
        } else {
            head = pos->next;
        }
        if (pos->next) {
            pos->next->prev = pos->prev;
        } else {
            tail = pos->prev;
        }
        delete pos;
        list_size--;
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *new_node = new node(value);
        if (tail) {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        } else {
            head = tail = new_node;
        }
        list_size++;
    }
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) throw container_is_empty();
        node *temp = tail;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
        list_size--;
    }
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *new_node = new node(value);
        if (head) {
            head->prev = new_node;
            new_node->next = head;
            head = new_node;
        } else {
            head = tail = new_node;
        }
        list_size++;
    }
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) throw container_is_empty();
        node *temp = head;
        head = head->next;
        if (head) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
        list_size--;
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (list_size <= 1) return;

        // Extract data to vector for sorting
        std::vector<T> temp;
        temp.reserve(list_size);
        for (node *p = head; p; p = p->next) {
            temp.push_back(p->data);
        }

        // Sort the vector
        sjtu::sort<T>(&temp[0], &temp[0] + temp.size(), [](const T &a, const T &b) { return a < b; });

        // Copy back to list
        size_t i = 0;
        for (node *p = head; p; p = p->next) {
            p->data = temp[i++];
        }
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;
        if (other.empty()) return;
        if (empty()) {
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;
            other.head = other.tail = nullptr;
            other.list_size = 0;
            return;
        }

        node *p1 = head;
        node *p2 = other.head;
        node *new_head = nullptr;
        node *new_tail = nullptr;
        size_t new_size = 0;

        while (p1 && p2) {
            if (p2->data < p1->data) {
                node *temp = p2;
                p2 = p2->next;
                if (!new_head) {
                    new_head = new_tail = temp;
                    temp->prev = temp->next = nullptr;
                } else {
                    new_tail->next = temp;
                    temp->prev = new_tail;
                    new_tail = temp;
                    new_tail->next = nullptr;
                }
                new_size++;
            } else {
                node *temp = p1;
                p1 = p1->next;
                if (!new_head) {
                    new_head = new_tail = temp;
                    temp->prev = temp->next = nullptr;
                } else {
                    new_tail->next = temp;
                    temp->prev = new_tail;
                    new_tail = temp;
                    new_tail->next = nullptr;
                }
                new_size++;
            }
        }

        while (p1) {
            node *temp = p1;
            p1 = p1->next;
            new_tail->next = temp;
            temp->prev = new_tail;
            new_tail = temp;
            new_tail->next = nullptr;
            new_size++;
        }

        while (p2) {
            node *temp = p2;
            p2 = p2->next;
            new_tail->next = temp;
            temp->prev = new_tail;
            new_tail = temp;
            new_tail->next = nullptr;
            new_size++;
        }

        head = new_head;
        tail = new_tail;
        list_size = new_size;

        other.head = other.tail = nullptr;
        other.list_size = 0;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (list_size <= 1) return;

        node *current = head;
        node *temp = nullptr;

        while (current) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        }

        if (temp) {
            tail = head;
            head = temp->prev;
        }
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (list_size <= 1) return;

        node *current = head;
        while (current && current->next) {
            if (current->data == current->next->data) {
                node *temp = current->next;
                current->next = temp->next;
                if (temp->next) {
                    temp->next->prev = current;
                } else {
                    tail = current;
                }
                delete temp;
                list_size--;
            } else {
                current = current->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
