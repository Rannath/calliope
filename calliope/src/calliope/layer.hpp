#pragma once
#include "core.hpp"
#include "event/event.hpp"

namespace Calliope {
  class CALLIOPE_API Layer {
  public:
    explicit Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnEvent(Event& e) {}

    inline const std::string& GatName() const { return debug_name_; }

  private:
    std::string debug_name_;
  };
}