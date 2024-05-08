#ifndef DEMO_PLUGIN_H
#define DEMO_PLUGIN_H

#include "../SystemInstance.h"
#include <string>
#include <vector>

class DemoPlugin : public SystemInstance {
private:
  const char **requiredComponents;

public:
  DemoPlugin();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  void onPluginEvent(void *event);
  ~DemoPlugin() override;
};

#endif
