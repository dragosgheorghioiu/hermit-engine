#ifndef PLUGIN_ANIMATION_SYSTEM_H
#define PLUGIN_ANIMATION_SYSTEM_H

#include "../SystemInstance.h"
#include <string>
#include <vector>

class PluginAnimationSystem : public SystemInstance {
public:
  PluginAnimationSystem();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
  getGUIElements() override;
  void demoWindow(ImGuiContext *context);
  ~PluginAnimationSystem() override;
};

#endif
