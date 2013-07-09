#ifndef DATATYPES_H
#define DATATYPES_H

// Min Heap
template <typename T>
class BinaryHeap
{
    public:
        BinaryHeap();
        BinaryHeap(int maxSize);

        ~BinaryHeap();

        void setComparator(std::function<int(T, T)> comp);

        void insert(T object);

        T minValue();
        T popMin();

        bool isEmpty();

    private:
        void heapifyUp(int index);
        void siftDown();

        bool isFull();
        void doubleHeap();

        std::function<int(T, T)> compare;

        int numItems;
        int maxItems;

        T *items;
};

#endif
