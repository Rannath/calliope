#include <calliope.hpp>

class Sandbox : public Calliope::Application {
public:
  Sandbox() {}
  ~Sandbox() override {}
};

namespace Calliope {
  AppPtr CreateApplication() { return std::make_unique<Sandbox>(); }
}
