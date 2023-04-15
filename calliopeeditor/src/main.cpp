#include <iostream>
#include <clp/engine.hpp>

int main(){
  auto& engine = clp::Engine::Instance();
  engine.Run();
  return 0;
}