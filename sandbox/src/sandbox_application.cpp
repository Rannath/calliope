#include <calliope.hpp>

class ExampleLayer : public Calliope::Layer {
public:
  ExampleLayer()
    :Layer("Example") {}

  void OnUpdate() override {
    Calliope::ClientLogger->Info("ExampleLayer::Update");
  }

  void OnEvent(Calliope::Event& e) override {
    Calliope::ClientLogger->Trace(e.ToString());
  }
};


class Sandbox : public Calliope::Application {
public:
  Sandbox() {
    PushLayer(new ExampleLayer());
  }
  ~Sandbox() override { }

private:
};

namespace Calliope {
  AppPtr CreateApplication() {
    return std::make_unique<Sandbox>();
  }
}
