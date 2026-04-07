#include <iostream>

int main() {

    const float x = 42.546832;
    std::cout << x << std::endl;
    std::cout <<"The size of this number is "<< sizeof(x) << std::endl;

    int y{};
    std::cout << "Please enter a number" << std::endl;
    std::cin >> y;
    std::cout << "You just entered this stupid ass number!! ----->" << y << '\n';
   
    return 0;
}
