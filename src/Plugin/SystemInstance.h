#ifndef SYSTEM_INSTANCE_H
#define SYSTEM_INSTANCE_H

#include "../../libs/imgui/imgui.h"
#include "../ECS/Entity.h"
#include <algorithm>
#include <bitset>
#include <sol/state.hpp>
#include <vector>

const int MAX_PLUGINS_COMPONENTS = 32;

typedef std::bitset<MAX_PLUGINS_COMPONENTS> Signature;

class SystemInstance {
protected:
  sol::state *lua;

private:
  Signature componentSignature;
  std::vector<EntityType> entities;

public:
  SystemInstance() = default;
  virtual ~SystemInstance() = default;
  virtual void update(std::vector<void *> params) = 0;
  virtual std::function<void(void *)> getCallback(std::string eventType) = 0;
  virtual std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
  getGUIElements() = 0;

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
  void setLua(sol::state *lua) { this->lua = lua; }
};

#endif
