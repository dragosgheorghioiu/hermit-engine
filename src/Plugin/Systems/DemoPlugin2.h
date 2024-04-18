#ifndef DEMO_PLUGIN_2_H
#define DEMO_PLUGIN_2_H

#include "../PluginSystemInterface.h"

class DemoPlugin2 : public PluginSystemInterface {
private:
  int counter;

public:
  DemoPlugin2();
  void Update() override;
  const std::string GetName() const override;
  ~DemoPlugin2();
};

#endif
