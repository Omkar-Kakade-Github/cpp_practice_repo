#include <iostream>
#include <vector>

using namespace std;

// ============================================================
// 1. LVALUE
// - has a persistent memory address
// - can appear on LEFT side of =
// - "lives" beyond a single expression
// ============================================================

void lvalue_demo() {
    cout << "=== lvalue ===" << endl;

    int x = 7;       // x is an lvalue — has address, persists
    int y = x;       // y is an lvalue — has address, persists

    cout << "x: "  << x  << " address: " << &x << endl;
    cout << "y: "  << y  << " address: " << &y << endl;

    x = 10;  // fine — x is an lvalue, can be assigned to
    cout << "x after reassignment: " << x << endl;
}


// ============================================================
// 2. RVALUE
// - a temporary value with NO persistent address
// - can only appear on RIGHT side of =
// - "dies" after the expression
// ============================================================

void rvalue_demo() {
    cout << "\n=== rvalue ===" << endl;

    int x = 7;         // 7 is an rvalue — temporary literal
    int y = x + 1;     // (x + 1) is an rvalue — temporary result

    // 7 = x;          // ERROR — can't assign to an rvalue
    // (x + 1) = 5;    // ERROR — can't assign to a temporary

    cout << "y: " << y << endl;
}


// ============================================================
// 3. LVALUE REFERENCE (&)
// - alias for an lvalue
// - CANNOT bind to an rvalue
// ============================================================

void lvalue_reference_demo() {
    cout << "\n=== lvalue reference ===" << endl;

    int x = 7;
    int& rx = x;    // fine — x is an lvalue

    // int& rx2 = 7;       // ERROR — 7 is an rvalue
    // int& rx3 = x + 1;   // ERROR — (x+1) is an rvalue

    rx = 20;  // modifies x directly
    cout << "x after rx = 20: " << x << endl;
    cout << "same address? " << (&x == &rx) << endl;  // prints 1 (true)
}


// ============================================================
// 4. RVALUE REFERENCE (&&)
// - binds to rvalues (temporaries) only
// - extends the lifetime of the temporary
// - core tool for move semantics
// ============================================================

void rvalue_reference_demo() {
    cout << "\n=== rvalue reference ===" << endl;

    int&& rrx = 7;       // fine — 7 is an rvalue
    int&& rry = 3 + 4;   // fine — (3+4) is a temporary rvalue

    // int x = 7;
    // int&& rrz = x;    // ERROR — x is an lvalue

    cout << "rrx: " << rrx << endl;
    cout << "rry: " << rry << endl;

    rrx = 99;  // can modify the bound temporary
    cout << "rrx after modification: " << rrx << endl;
}


// ============================================================
// 5. MOVE SEMANTICS — the real use case for rvalue references
// - instead of copying a temporary, we STEAL its resources
// - avoids expensive deep copies
// ============================================================

void move_semantics_demo() {
    cout << "\n=== move semantics ===" << endl;

    // without move — expensive copy
    vector<int> v1 = {1, 2, 3, 4, 5};
    vector<int> v2 = v1;  // COPY — allocates new memory, copies all elements
    cout << "v1 size after copy: " << v1.size() << endl;  // v1 still intact
    cout << "v2 size after copy: " << v2.size() << endl;

    // with move — cheap steal
    vector<int> v3 = {1, 2, 3, 4, 5};
    vector<int> v4 = move(v3);  // MOVE — steals v3's heap memory, no copy
    cout << "v3 size after move: " << v3.size() << endl;  // v3 is now empty!
    cout << "v4 size after move: " << v4.size() << endl;  // v4 has the data
}


// ============================================================
// SUMMARY
//
// lvalue        — has address, persists beyond expression    e.g. int x = 7
// rvalue        — temporary, no persistent address           e.g. 7, x+1
// lvalue ref &  — alias for lvalue, cannot bind to rvalue    e.g. int& rx = x
// rvalue ref && — binds to rvalue, used for move semantics   e.g. int&& rx = 7
//
// KEY INSIGHT:
// rvalue references exist mainly to enable MOVE SEMANTICS —
// stealing resources from temporaries instead of copying them.
// ============================================================


int main() {
    lvalue_demo();
    rvalue_demo();
    lvalue_reference_demo();
    rvalue_reference_demo();
    move_semantics_demo();

    return 0;
}
