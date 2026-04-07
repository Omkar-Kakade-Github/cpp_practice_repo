#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {

    // ============================================================
    // 1. PRIMITIVE TYPE SIZES
    // ============================================================

    cout << "=== Primitive Type Sizes ===" << endl;
    cout << "char:          " << sizeof(char)              << " byte(s)"  << endl;
    cout << "short:         " << sizeof(short)             << " byte(s)"  << endl;
    cout << "int:           " << sizeof(int)               << " byte(s)"  << endl;
    cout << "long:          " << sizeof(long)              << " byte(s)"  << endl;
    cout << "long long:     " << sizeof(long long)         << " byte(s)"  << endl;
    cout << "float:         " << sizeof(float)             << " byte(s)"  << endl;
    cout << "double:        " << sizeof(double)            << " byte(s)"  << endl;
    cout << "long double:   " << sizeof(long double)       << " byte(s)"  << endl;
    cout << "bool:          " << sizeof(bool)              << " byte(s)"  << endl;
    cout << "pointer (int*) " << sizeof(int*)              << " byte(s)"  << endl;


    // ============================================================
    // 2. VECTOR OBJECT SIZE (always 24 bytes on 64-bit systems)
    // - stores 3 things internally regardless of element type:
    //   * pointer to heap data  (8 bytes)
    //   * size                  (8 bytes)
    //   * capacity              (8 bytes)
    // ============================================================

    cout << "\n=== Vector Object Size (always 24 bytes) ===" << endl;
    cout << "vector<char>:        " << sizeof(vector<char>)        << " bytes" << endl;
    cout << "vector<short>:       " << sizeof(vector<short>)       << " bytes" << endl;
    cout << "vector<int>:         " << sizeof(vector<int>)         << " bytes" << endl;
    cout << "vector<long long>:   " << sizeof(vector<long long>)   << " bytes" << endl;
    cout << "vector<float>:       " << sizeof(vector<float>)       << " bytes" << endl;
    cout << "vector<double>:      " << sizeof(vector<double>)      << " bytes" << endl;
    cout << "vector<bool>:        " << sizeof(vector<bool>)        << " bytes" << endl;
    cout << "vector<string>:      " << sizeof(vector<string>)      << " bytes" << endl;


    // ============================================================
    // 3. ACTUAL HEAP MEMORY USED BY VECTOR ELEMENTS
    // = number of elements * sizeof(element type)
    // ============================================================

    cout << "\n=== Actual Heap Memory Used by Vector Elements ===" << endl;

    vector<char>      vc(5);
    vector<short>     vs(5);
    vector<int>       vi(5);
    vector<long long> vll(5);
    vector<float>     vf(5);
    vector<double>    vd(5);

    cout << "vector<char>(5):      " << 5 * sizeof(char)      << " bytes on heap" << endl;
    cout << "vector<short>(5):     " << 5 * sizeof(short)     << " bytes on heap" << endl;
    cout << "vector<int>(5):       " << 5 * sizeof(int)       << " bytes on heap" << endl;
    cout << "vector<long long>(5): " << 5 * sizeof(long long) << " bytes on heap" << endl;
    cout << "vector<float>(5):     " << 5 * sizeof(float)     << " bytes on heap" << endl;
    cout << "vector<double>(5):    " << 5 * sizeof(double)    << " bytes on heap" << endl;


    // ============================================================
    // 4. TOTAL MEMORY = vector object (24 bytes) + heap data
    // ============================================================

    cout << "\n=== Total Memory (object + heap data) for 5 elements ===" << endl;
    cout << "vector<int>(5):    " << sizeof(vector<int>)    + 5 * sizeof(int)    << " bytes total" << endl;
    cout << "vector<double>(5): " << sizeof(vector<double>) + 5 * sizeof(double) << " bytes total" << endl;

    return 0;
}
