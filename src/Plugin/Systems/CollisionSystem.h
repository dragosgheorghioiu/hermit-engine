#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../SystemInstance.h"
#include <string>
#include <vector>

class CollisionSystem : public SystemInstance {
public:
  CollisionSystem();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  std::unordered_map<std::string, std::function<void()>>
  getGUIElements() override;
  ~CollisionSystem() override;
};

#endif
