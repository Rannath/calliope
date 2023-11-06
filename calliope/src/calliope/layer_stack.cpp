#include "pch.hpp"
#include "layer_stack.hpp"

namespace Calliope {
  LayerStack::LayerStack() {
    insert_ = layers_.begin();
  }

  LayerStack::~LayerStack() {
    for (auto* layer : layers_)
      delete layer;
  }

  void LayerStack::PushLayer(Layer* layer) {
    insert_ = layers_.emplace(insert_, layer);
  }

  void LayerStack::PopLayer(Layer* layer) {
    auto it = std::find(layers_.begin(), layers_.end(), layer);
    if (it != layers_.end()) {
      layers_.erase(it);
      insert_--;
    }
  }

  void LayerStack::PushOverlay(Layer* layer) {
    layers_.emplace_back(layer);
  }

  void LayerStack::PopOverlay(Layer* layer) {
    auto it = std::find(layers_.begin(), layers_.end(), layer);
    if (it != layers_.end()) {
      layers_.erase(it);
    }
  }
}
