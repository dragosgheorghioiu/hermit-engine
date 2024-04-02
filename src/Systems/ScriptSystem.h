#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "../ECS/ECS.h"
#include "AssetStore/AssetStore.h"
#include <SDL_timer.h>
#include <glm.hpp>
#include <sol/state.hpp>

glm::vec2 GetEntityPosition(Entity entity);

void SetEntityPosition(Entity entity, float x, float y);

class ScriptSystem : public System {
public:
  ScriptSystem();
  void CreateLuaBindings(std::unique_ptr<Registry> &registry,
                         std::unique_ptr<AssetStore> &assetStore,
                         sol::state &lua);
  void Update(double deltaTime, int ellapsedTime);
};

#endif
