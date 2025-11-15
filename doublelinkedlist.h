#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H
#include <stdexcept>
using namespace std;
template <typename T>
struct Node {
    T data;
    Node* prev;
    Node* next;
    Node(const T& d) : data(d), prev(nullptr), next(nullptr) {}
};
template <typename T>
class DoubleLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;
public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~DoubleLinkedList();

    bool isEmpty() const;
    void clear();

    void append(const T& value);
    void removeAt(int index);

    T& operator()(int index) const;
    int getSize() const;
};
template <typename T>
DoubleLinkedList<T>::~DoubleLinkedList() {
    Node<T>* cur = head;
    while (cur) {
        Node<T>* next = cur->next;
        delete cur;
        cur = next;
    }
}
template <typename T>
bool DoubleLinkedList<T>::isEmpty() const{
    return size == 0;
}
template <typename T>
void DoubleLinkedList<T>::clear()
{
    Node<T>* cur = head;
    while(cur)
    {
        Node<T>* next = cur->next;
        delete cur;
        cur = next;
    }
    head = tail = nullptr;
    size = 0;
}
template <typename T>
void DoubleLinkedList<T>::append(const T& value) {
    Node<T>* node = new Node<T>(value);
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    size++;
}
template <typename T>
void DoubleLinkedList<T>::removeAt(int index) {
    if (index < 0 || index >= size) return;
    Node<T>* cur = head;
    for (int i = 0; i < index; ++i) cur = cur->next;

    if (cur->prev) cur->prev->next = cur->next;
    else head = cur->next;
    if (cur->next) cur->next->prev = cur->prev;
    else tail = cur->prev;

    delete cur;
    size--;
}
template <typename T>
T& DoubleLinkedList<T>::operator()(int index) const {
    if(index < 0 || index >= size)
    throw out_of_range("index out of range");
    Node<T>* cur = head;
     for (int i = 0; i < index; ++i)
        cur = cur->next;
    return cur->data;
}
template <typename T>
int DoubleLinkedList<T>::getSize() const {
    return size;
}
#endif
