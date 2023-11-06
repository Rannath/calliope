#include "pch.hpp"
#include "layer.hpp"

namespace Calliope {
  Layer::Layer(const std::string& name) : debug_name_{ name } {}
  
  Layer::~Layer() { }
}