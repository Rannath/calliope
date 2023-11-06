#pragma once

#include "core.hpp"
#include "layer.hpp"

#include "pch.hpp"

namespace Calliope {
  class CALLIOPE_API LayerStack {
  public:
    using Store = std::vector<Layer*>;
    using Iter = Store::iterator;

    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PopLayer(Layer* layer);
    void PushOverlay(Layer* layer);
    void PopOverlay(Layer* layer);

    Iter begin() { return layers_.begin(); }
    Iter end() { return layers_.end(); }

  private:
    Store layers_;
    Iter insert_;
  };
}