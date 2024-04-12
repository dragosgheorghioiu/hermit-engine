#ifndef DEMO_PLUGIN_H
#define DEMO_PLUGIN_H

#include "../PluginSystemInterface.h"

class DemoPlugin : public PluginSystemInterface {
public:
  DemoPlugin();
  void Update() override;
  ~DemoPlugin();
};

#endif
