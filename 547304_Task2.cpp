#include <iostream>
using namespace std;

// One heap class used as a max heap (isMax = true) or min heap (isMax = false).
class Heap {
private:
    int* arr;
    int size, capacity;
    bool isMax;

    // True if a should be closer to the root than b. O(1).
    bool before(int a, int b) const { return isMax ? a > b : a < b; }

    // Doubles capacity when full. O(n) for the copy, O(1) amortized per push.
    void grow() {
        capacity *= 2;
        int* bigger = new int[capacity];
        for (int i = 0; i < size; i++) bigger[i] = arr[i];
        delete[] arr;
        arr = bigger;
    }

    // Moves element i up until the heap order holds. O(log n).
    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (before(arr[i], arr[p])) { swap(arr[i], arr[p]); i = p; }
            else break;
        }
    }

    // Moves element i down, swapping with the better child. O(log n).
    void siftDown(int i) {
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, best = i;
            if (l < size && before(arr[l], arr[best])) best = l;
            if (r < size && before(arr[r], arr[best])) best = r;
            if (best == i) break;
            swap(arr[i], arr[best]);
            i = best;
        }
    }

public:
    // Creates an empty heap. O(1).
    Heap(bool max, int cap = 4) : size(0), capacity(cap), isMax(max) { arr = new int[capacity]; }
    // Frees memory. O(1).
    ~Heap() { delete[] arr; }

    // Inserts a value and sifts it up. O(log n).
    void push(int v) {
        if (size == capacity) grow();
        arr[size++] = v;
        siftUp(size - 1);
    }

    // Removes and returns the root, then sifts the replacement down. O(log n).
    int pop() {
        int top = arr[0];
        arr[0] = arr[--size];
        if (size > 0) siftDown(0);
        return top;
    }

    // Returns the root without removing it. O(1).
    int top() const { return arr[0]; }
    // Returns the number of stored elements. O(1).
    int count() const { return size; }
    // True if empty. O(1).
    bool empty() const { return size == 0; }
};

class MedianMonitor {
private:
    Heap lower; // max heap: smaller half
    Heap upper; // min heap: larger half
    double threshold;

public:
    MedianMonitor(double t) : lower(true), upper(false), threshold(t) {}

    // Inserts x into the correct heap, then rebalances so sizes differ by at
    // most one with the max heap holding the extra element.
    // Time complexity: O(log n).
    void addWait(int x) {
        if (lower.empty() || x <= lower.top()) lower.push(x);
        else upper.push(x);

        if (lower.count() > upper.count() + 1) upper.push(lower.pop());
        else if (upper.count() > lower.count()) lower.push(upper.pop());
    }

    // Returns the median: max-heap root if it has one extra element,
    // otherwise the average of both roots. Time complexity: O(1).
    double getMedian() const {
        if (lower.count() > upper.count()) return lower.top();
        return (lower.top() + upper.top()) / 2.0;
    }

    // Adds a wait and prints the report line. Time complexity: O(log n).
    void process(int arrival, int x) {
        addWait(x);
        double med = getMedian();
        cout << "Arrival " << arrival << " | Wait " << x
             << " | MaxRoot " << lower.top() << " | MinRoot ";
        if (upper.empty()) cout << "none"; else cout << upper.top();
        cout << " | Median " << med << " | "
             << (med > threshold ? "ALERT: call extra staff" : "Normal") << endl;
    }
};

int main() {
    int waits[] = {45, 20, 60, 15, 30, 90, 10, 55};
    int n = sizeof(waits) / sizeof(waits[0]);

    MedianMonitor monitor(35);
    for (int i = 0; i < n; i++) monitor.process(i + 1, waits[i]);
    return 0;
}
