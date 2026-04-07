#include <iostream>
#include <array>
#include <string>

int main(){
    
    int array[100];
    array[0] = 4563;
    std::cout << "first element of this array" << array[0] << std::endl;
    
    std::array<std::string, 2> a3{"E", "\u018E"};
    for (const auto& s : a3)
        std::cout << s << ' ';
    std::cout << '\n';

    return 0;
}

