#ifndef PLUGIN_RENDER_SYSTEM_H
#define PLUGIN_RENDER_SYSTEM_H

#include "../SystemInstance.h"

class PluginRenderSystem : public SystemInstance {
public:
  PluginRenderSystem();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
  getGUIElements() override;
  ~PluginRenderSystem() override;
};

#endif
