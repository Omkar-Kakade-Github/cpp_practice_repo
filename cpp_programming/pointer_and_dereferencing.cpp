#include <iostream>

using namespace std;

int main(){

    int x = 7;      // regular int variable storing 7
    int* px = &x;   // pointer to int, storing the ADDRESS of x
    
    cout << "Value of x         :" << x << endl;
    cout << "Address of x       :" << &x << endl;
    cout << "Value of px        :" << px << endl;
    cout << "Dereferencing of x :" << *px << endl;

    return 0;
} 
