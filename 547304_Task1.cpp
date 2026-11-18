#include <iostream>
using namespace std;

class MaxHeap {
private:
    int* arr;      // dynamic array holding the heap
    int size;      // number of elements currently stored
    int capacity;  // allocated length of arr

    // Doubles the array capacity when it is full.
    // Time complexity: O(n) for the copy, but O(1) amortized per insert.
    void grow() {
        capacity *= 2;
        int* bigger = new int[capacity];
        for (int i = 0; i < size; i++) bigger[i] = arr[i];
        delete[] arr;
        arr = bigger;
    }

    // Moves the element at index i up while it is larger than its parent.
    // Returns the number of swaps performed.
    // Time complexity: O(log n) (at most the height of the tree).
    int siftUp(int i) {
        int swaps = 0;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (arr[i] > arr[parent]) {
                swap(arr[i], arr[parent]);
                i = parent;
                swaps++;
            } else break;
        }
        return swaps;
    }

    // Moves the element at index i down within the first n elements,
    // swapping with its larger child until the max-heap property holds.
    // Time complexity: O(log n).
    void siftDown(int i, int n) {
        while (true) {
            int left = 2 * i + 1, right = 2 * i + 2, largest = i;
            if (left < n && arr[left] > arr[largest]) largest = left;
            if (right < n && arr[right] > arr[largest]) largest = right;
            if (largest == i) break;
            swap(arr[i], arr[largest]);
            i = largest;
        }
    }

public:
    // Creates an empty heap with the given starting capacity.
    // Time complexity: O(1).
    MaxHeap(int cap = 10) : size(0), capacity(cap) { arr = new int[capacity]; }

    // Frees the dynamic array. Time complexity: O(1).
    ~MaxHeap() { delete[] arr; }

    // Adds value at the end, sifts it up, then prints the value, the number
    // of swaps and the heap. Time complexity: O(log n) per insertion.
    void insert(int value) {
        if (size == capacity) grow();
        arr[size] = value;
        size++;
        int swaps = siftUp(size - 1);
        cout << "Inserted " << value << " | Swaps: " << swaps << " | Heap: ";
        print();
    }

    // Sorts the contents in ascending order in place, using only heap
    // operations: repeatedly swap the root (maximum) with the last element
    // of the shrinking heap, then sift the new root down.
    // Time complexity: O(n log n). After this the array is sorted, not a heap.
    void heapSort() {
        for (int end = size - 1; end > 0; end--) {
            swap(arr[0], arr[end]);
            siftDown(0, end);
        }
    }

    // Prints the array contents on one line. Time complexity: O(n).
    void print() const {
        for (int i = 0; i < size; i++) cout << arr[i] << (i + 1 < size ? " " : "");
        cout << endl;
    }
};

int main() {
    int input[] = {25, 40, 15, 60, 35, 10, 70, 50};
    int n = sizeof(input) / sizeof(input[0]);

    MaxHeap heap(4); // small capacity so resizing is exercised
    for (int i = 0; i < n; i++) heap.insert(input[i]);

    cout << "\nFinal max heap: ";
    heap.print();

    heap.heapSort();
    cout << "Sorted array:   ";
    heap.print();
    return 0;
}
