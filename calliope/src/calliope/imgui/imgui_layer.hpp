#pragma once

#include "calliope/core.hpp"
#include "calliope/layer.hpp"

#include "calliope/event/event.hpp"
#include "calliope/event/key_events.hpp"
#include "calliope/event/mouse_events.hpp"

namespace Calliope {
  class CALLIOPE_API ImGuiLayer : public Layer {
  public:
    ImGuiLayer(float width, float height);
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate() override;
    void OnEvent(Event& e) override;

    inline float& width() { return width_; }
    inline float& height() { return height_; }

  private:
    float time_ = 0.0f;
    float width_ = .0f, height_ = .0f;

    bool OnEvent(KeyPressedEvent& e);
    bool OnEvent(KeyReleasedEvent& e);
    bool OnEvent(MouseButtonPressedEvent& e);
    bool OnEvent(MouseButtonReleasedEvent& e);
    bool OnEvent(MouseMovedEvent& e);
  };
}