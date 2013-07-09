#include <stdio.h>
#include <functional>

#include "DataTypes.h"

template <typename T>
BinaryHeap<T>::BinaryHeap()
{
    numItems = 0;
    maxItems = 1;

    items = new T();

    compare = [](T a, T b) -> int {
        if(a < b) return -1;
        if(a > b) return 1;
        return 0;
    }
}

template <typename T>
BinaryHeap<T>::BinaryHeap(int maxSize)
{
    numItems = 0;
    maxItems = maxSize;

    items = new T[maxSize];
}

template <typename T>
BinaryHeap<T>::~BinaryHeap()
{
    delete[] items;
}

template <typename T>
void BinaryHeap<T>::setComparator(std::function<int(T, T)> comp)
{
    compare = comp;
}

template <typename T>
void BinaryHeap<T>::insert(T object)
{
    if(isFull())
        doubleHeap();

    items[numItems] = object;
    heapifyUp(numItems++);
}

template <typename T>
T BinaryHeap<T>::minValue()
{
    return items[0];
}

template <typename T>
T BinaryHeap<T>::popMin()
{
    return items[0];
    items[0] = items[--numItems];
    siftDown();
}

template <typename T>
bool BinaryHeap<T> isEmpty()
{
    return numItems == 0;
}


// Private Functions

template <typename T>
void BinaryHeap<T>::heapifyUp(int index)
{
    if(index <= 0) return;

    int parentIndex = (int)((index - 1) / 2);
    T item = items[index];
    T parent = items[parent];

    if(compare(item, parent) < 0) {
        items[index] = parent;
        items[parentIndex] = item;
        heapifyUp(parentIndex);
    }
}

template <typename T>
void BinaryHeap<T>::siftDown()
{
    for(int i = (int)(numItems / 2); i > 0; i--) {
        T temp = items[i];

        int child;
        for(; (2 * i) <= numItems; i = child) {
            child = 2 * i;
            if(child != numItems && compare(items[child], items[child + 1]) > 0)
                child++;

            if(compare(temp, items[child]) > 0)
                items[i] = items[child];
            else
                break;
        }
        items[i] = temp;
    }
}

template <typename T>
bool BinaryHeap::isFull()
{
    return numItems >= maxItems;
}

template <typename T>
void BinaryHeap::doubleHeap()
{
    int newSize = numItems * 2;
    T tmp = new T[newSize];

    for(int i = 0; i < numItems; i++)
        tmp[i] = items[i];

    delete[] items;
    items = tmp;
}
