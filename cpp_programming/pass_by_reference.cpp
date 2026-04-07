#include <iostream>
#include <algorithm>

void SetValue(int arg) {
    std::cout << "arg's addr is :" << &arg << std::endl;
}

void PassByReference(int& alias){
    std::cout << "alias addr is :" << &alias << std::endl;
    alias = 9999;
}

int main(){

    int x = 35;
    std::cout << "the value of x is : " << x << std::endl;
    std::cout << "the addr of x is  : " << &x << std::endl;

    SetValue(x);
    PassByReference(x);

    std::cout << "the value of x is : " << x << std::endl;
    std::cout << "the addr of x is  : " << &x << std::endl;
}




