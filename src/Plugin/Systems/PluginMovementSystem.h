#ifndef PLUGIN_MOVEMENT_SYSTEM_H
#define PLUGIN_MOVEMENT_SYSTEM_H

#include "../SystemInstance.h"
#include <functional>
#include <string>
#include <unordered_map>

class PluginMovementSystem : public SystemInstance {
public:
  PluginMovementSystem();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
  getGUIElements() override;
  void onCollision(void *event);
  void onPlayerWallCollision(EntityType &player, EntityType &wall);
  ~PluginMovementSystem() override;
};

#endif
