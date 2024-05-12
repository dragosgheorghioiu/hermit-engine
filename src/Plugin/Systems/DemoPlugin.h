#ifndef DEMO_PLUGIN_H
#define DEMO_PLUGIN_H

#include "../SystemInstance.h"
#include <string>
#include <vector>

class DemoPlugin : public SystemInstance {
public:
  DemoPlugin();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  std::unordered_map<std::string, std::function<void()>>
  getGUIElements() override;
  void onPluginEvent(void *event);
  ~DemoPlugin() override;
};

#endif
