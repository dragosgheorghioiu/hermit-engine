#ifndef DEMO_PLUGIN_H
#define DEMO_PLUGIN_H

#include "../PluginSystemInterface.h"
#include <vector>

class DemoPlugin : public PluginSystemInterface {
public:
  DemoPlugin();
  void Update(std::vector<void *>) override;
  const std::string GetName() const override;
  ~DemoPlugin();
};

#endif
