#include <iostream>
#include <clp/engine.hpp>

int main(){
    std::cout << "Hello, Youtube!" << std::endl;
    clp::GetInfo();
    int a = clp::add(10, 5);
    int b = clp::sub(10, 5);
    std::cout << a << ", " << b << std::endl;
    return 0;
}