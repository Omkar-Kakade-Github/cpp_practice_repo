#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// ============================================================
// 1. BASIC unique_ptr — automatic cleanup, no manual delete
// ============================================================

void basic_unique_ptr_demo() {
    cout << "=== Basic unique_ptr ===" << endl;

    // raw pointer - manual delete required
    int* raw = new int(6);
    cout << "raw: " << *raw << endl;
    delete raw; // important!
    
    // unique ptr - automatic delete when out of scope
    unique_ptr<int> uptr = make_unique<int>(3);
    cout << "uptr: " << *uptr << endl;

    // no delete required
}
// ============================================================
// 2. unique_ptr CANNOT be copied — only one owner at a time
// ============================================================
 
void ownership_demo() {
    cout << "\n=== Ownership ===" << endl;
    
    unique_ptr<int> uptr = make_unique<int>(42);

    // unique_ptr<int> uptr2 = uptr;  // ERROR — cannot copy, only one owner
    // ownership can be TRANSFERRED with move

    unique_ptr<int> uptr_2 = move(uptr);
    cout << "uptr_2: " << *uptr_2 << endl;

    if (uptr == nullptr) {
        cout << "uptr holds no value after ownership got transfered" << endl;
    }

}

// ============================================================
// 3. unique_ptr FOR ARRAYS
// ============================================================
 
void unique_ptr_for_array() {
    cout << "\n=== unique_ptr for arrays ===" << endl;

    // raw array - must use delete
    int* raw_arr = new int[3]{4,2,0};
    cout << "raw_arr has been created but now needs to manually destroyed" << endl;
    delete[] raw_arr;

    // unique_pte array - automatically gets deleted
    unique_ptr<int[]> uptr_arr = make_unique<int[]>(5);
    for (int i = 0; i < 5; i++) {
        uptr_arr[i] = i + 3;
    }

    cout << "uptr_arr[4] : " << uptr_arr[4] << "\nBut now no need to delete uptr_arr" << endl;

}

// ============================================================
// 4. unique_ptr WITH CLASSES
// most common real-world use case
// ============================================================
 
class Engine {
    public:
        int horsepower;

        Engine(int hp) : horsepower(hp) {
            cout << "Engine created with " << hp << "hp" << endl;
        }

        ~Engine() {
            cout << "Destructor called. Engine destroyed!" << endl;
        }

        void start(int time) {
            cout << "Engine started for :"<< time << "s" << endl;
            cout << "Speed after " << time << " s is " << horsepower * time  << " km/h" << endl;
        }
};

void class_unique_ptr_demo() {
    cout << "\n=== unique_ptr with classes ===" << endl;
 
    unique_ptr<Engine> engine = make_unique<Engine>(500);
    engine->start(3);
    cout << "Horsepower: " << engine->horsepower << endl;
 
    // engine automatically destroyed and memory freed here    
}


// ============================================================
// 5. RETURNING unique_ptr FROM FUNCTIONS
// safe way to return heap allocated objects
// ============================================================
 
unique_ptr<Engine> createEngine(int hp) {
    return make_unique<Engine>(hp);  // ownership transferred to caller
}
 
void return_unique_ptr_demo() {
    cout << "\n=== Returning unique_ptr ===" << endl;
 
    unique_ptr<Engine> myEngine = createEngine(300);
    myEngine->start(5);
    // automatically destroyed when myEngine goes out of scope
}
 
 
// ============================================================
// 6. unique_ptr IN VECTORS
// store heap objects in a container safely
// ============================================================
 
void vector_unique_ptr_demo() {
    cout << "\n=== vector of unique_ptrs ===" << endl;
 
    vector<unique_ptr<Engine>> garage;
    garage.push_back(make_unique<Engine>(200));
    garage.push_back(make_unique<Engine>(400));
    garage.push_back(make_unique<Engine>(600));
 
    for (auto& e : garage) {
        e->start(10);
        cout << "HP: " << e->horsepower << endl;
    }
    // all engines automatically destroyed when garage goes out of scope
}
 
 
// ============================================================
// 7. HPC USE CASE — large heap arrays without manual delete
// ============================================================
 
void hpc_use_case_demo() {
    cout << "\n=== HPC use case ===" << endl;
 
    int n = 1000000;
 
    // raw pointer — easy to forget delete, causes memory leak
    int* raw = new int[n];
    // ... do work ...
    delete[] raw;  // must remember
 
    // unique_ptr — automatic cleanup, same performance as raw pointer
    unique_ptr<int[]> safe = make_unique<int[]>(n);
    // ... do work ...
    // automatically freed — no leak possible
 
    cout << "Large array allocated and freed safely" << endl;
}
 
 
// ============================================================
// SUMMARY
//
// unique_ptr<T>      — owns a single heap object, auto deletes
// unique_ptr<T[]>    — owns a heap array, auto calls delete[]
// make_unique<T>()   — preferred way to create unique_ptr (no raw new)
// move()             — transfers ownership between unique_ptrs
//
// Cannot copy — only one owner at a time
// Same performance as raw pointer — zero overhead
// Use in HPC to avoid memory leaks in large allocations
// ============================================================
 
 
int main() {
    basic_unique_ptr_demo();
    ownership_demo();
    unique_ptr_for_array();
    class_unique_ptr_demo();
    return_unique_ptr_demo();
    vector_unique_ptr_demo();
    hpc_use_case_demo();
 
    return 0;
}

