#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

// ============================================================
// 1. STACK MEMORY
// - Automatically managed, fixed size known at compile time
// - Fast, but limited (~8MB on Linux)
// - Local variables live here
// ============================================================

void stack_memory_demo() {
    int x = 7;              // stack — small, fixed size
    int arr[100];           // stack — size must be known at compile time

    // filling beyond declared size is undefined behavior (buffer overflow)
    // arr[200] = 5; // DO NOT DO THIS — corrupts adjacent memory silently

    cout << "x on stack: " << x << endl;
}


// ============================================================
// 2. HEAP MEMORY
// - Manually managed with new/delete
// - Size can be determined at runtime
// - Much larger than stack
// - Must free with delete to avoid memory leaks
// ============================================================

void heap_memory_demo() {
    int numberOfStudents = 5;

    // dynamically allocate array on heap at runtime
    int* studentids = new int[numberOfStudents];

    for (int i = 0; i < numberOfStudents; i++) {
        studentids[i] = i;  // [] already handles dereferencing
    }

    // *studentids        → same as studentids[0], value at first address
    // *(studentids + 1)  → same as studentids[1]
    cout << "First student id (dereferenced): " << *studentids << endl;
    cout << "Second student id (pointer arithmetic): " << *(studentids + 1) << endl;

    delete[] studentids;  // must free heap memory manually
}


// ============================================================
// 3. POINTERS vs REFERENCES
// ============================================================

void pointers_vs_references_demo() {
    int x = 7;

    // POINTER — stores memory address, needs * to dereference
    int* px = &x;
    cout << "x value: "          << x   << endl;
    cout << "x address (&x): "   << &x  << endl;
    cout << "px stores: "        << px  << endl;   // same as &x
    cout << "px dereferenced: "  << *px << endl;   // same as x

    *px = 10;  // modifies x through pointer
    cout << "x after *px = 10: " << x << endl;

    // REFERENCE — just an alias for an existing variable, no * needed
    int& rx = x;
    rx = 20;  // directly modifies x
    cout << "x after rx = 20: " << x << endl;
}


// ============================================================
// 4. std::vector — RAII (automatic heap management)
// - Calls new/delete internally
// - No manual memory management needed
// - Preferred over raw arrays for general use
// ============================================================

void vector_demo() {
    // vector handles heap allocation/deallocation automatically
    vector<int> v = {1, 2, 3, 4, 5};

    v.push_back(6);     // add to end
    v.pop_back();       // remove from end
    v.erase(v.begin()); // remove first element

    cout << "Vector size: "    << v.size()                                   << endl;
    cout << "Max element: "    << *max_element(v.begin(), v.end())           << endl;
    cout << "Min element: "    << *min_element(v.begin(), v.end())           << endl;
    cout << "Sum: "            << accumulate(v.begin(), v.end(), 0)          << endl;
    cout << "Front: "          << v.front()                                  << endl;
    cout << "Back: "           << v.back()                                   << endl;
}


// ============================================================
// 5. STACK vs HEAP — when to use which
//
//  Use STACK when:
//  - Size is small and known at compile time
//  - e.g. int x, int arr[10], local variables
//
//  Use HEAP when:
//  - Size is large (even if known at compile time)
//  - e.g. 512MB array — exceeds stack limit
//  - Size is only known at runtime
//
//  NOTE: Once allocated, accessing heap memory is just as fast
//  as stack memory. The slowness is only in new/delete calls.
// ============================================================

void when_to_use_heap_demo() {
    // even if we know size upfront, large arrays must go on heap
    int n = 1000000;

    // WRONG for large sizes — will stack overflow:
    // int arr[1000000];

    // CORRECT — heap via raw pointer
    int* arr = new int[n];
    delete[] arr;

    // BETTER — heap via vector (automatic cleanup)
    vector<int> vec(n);
}


// ============================================================
// 6. PERFORMANCE HIERARCHY (top = biggest impact)
//
//  1. Algorithm & Data Structure  — O(n) vs O(n²) matters most
//  2. Appropriate Types           — int vs long long, smaller = more cache
//  3. Stack vs Heap               — small/known → stack, large/dynamic → heap
//  4. Cache Locality              — sequential access > random access
//  5. Parallelism                 — OpenMP, MPI, CUDA
//  6. Hardware-specific           — SIMD, memory alignment, compiler flags (-O2, -O3)
// ============================================================


int main() {
    cout << "=== Stack Memory ===" << endl;
    stack_memory_demo();

    cout << "\n=== Heap Memory ===" << endl;
    heap_memory_demo();

    cout << "\n=== Pointers vs References ===" << endl;
    pointers_vs_references_demo();

    cout << "\n=== Vector (RAII) ===" << endl;
    vector_demo();

    cout << "\n=== Heap Allocation for Large Data ===" << endl;
    when_to_use_heap_demo();
    cout << "Large heap allocation succeeded." << endl;

    return 0;
}
