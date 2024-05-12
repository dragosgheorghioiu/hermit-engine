#ifndef DEMO_PLUGIN_2_H
#define DEMO_PLUGIN_2_H

#include "../SystemInstance.h"
#include <string>
#include <vector>

class DemoPlugin2 : public SystemInstance {
private:
  const char **requiredComponents;

public:
  DemoPlugin2();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  std::unordered_map<std::string, std::function<void()>>
  getGUIElements() override;
  void onPluginEvent(void *event);
  ~DemoPlugin2() override;
};

#endif
