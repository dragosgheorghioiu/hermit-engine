#ifndef DEMO_PLUGIN_H
#define DEMO_PLUGIN_H

#include "../PluginSystemInterface.h"

class DemoPlugin : public PluginSystemInterface {
private:
  int counter;

public:
  DemoPlugin();
  void Update() override;
  const std::string GetName() const override;
  ~DemoPlugin();
};

#endif
