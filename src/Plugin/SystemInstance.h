#ifndef SYSTEM_INSTANCE_H
#define SYSTEM_INSTANCE_H

#include "../ECS/Entity.h"
#include <bitset>
#include <vector>

const int MAX_PLUGINS_COMPONENTS = 32;

typedef std::bitset<MAX_PLUGINS_COMPONENTS> Signature;

class SystemInstance {
private:
  Signature componentSignature;
  std::vector<EntityType> entities;

public:
  SystemInstance() = default;
  virtual ~SystemInstance() = default;
  virtual void update(std::vector<void *> params) = 0;
  // fucntion that returns a function pointer to the callback function
  virtual std::function<void(void *)> getCallback(std::string eventType) = 0;

  void addEntityToSystem(const EntityType &entity) {
    entities.push_back(entity);
  }

  void changeComponentSignature(const int &componentId) {
    componentSignature.set(componentId);
  }

  void removeEntityFromSystem(const EntityType &entity) {
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
                       [&entity](EntityType other) { return entity == other; }),
        entities.end());
  }
  std::vector<EntityType> getSystemEntities() const { return entities; }
  const Signature &getComponentSignature() const { return componentSignature; }
};

#endif
