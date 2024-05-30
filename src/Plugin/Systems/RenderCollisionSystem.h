
#ifndef RENDERCOLLISIONSYSTEM_H
#define RENDERCOLLISIONSYSTEM_H

#include "../SystemInstance.h"

class RenderCollisionSystem : public SystemInstance {
public:
  RenderCollisionSystem();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
  getGUIElements() override;
  ~RenderCollisionSystem() override;
};

#endif
