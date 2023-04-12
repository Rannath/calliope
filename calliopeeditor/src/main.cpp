#include <iostream>
#include <clp/engine.hpp>

int main(){
    std::cout << "Hello, Youtube!" << std::endl;
    clp::GetInfo();

    clp::Initialize();
    clp::Shutdown();
    return 0;
}