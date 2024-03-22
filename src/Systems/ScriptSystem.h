#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "../Components/ScriptComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "Systems/RenderGUISystem.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <SDL_timer.h>
#include <glm.hpp>

glm::vec2 GetEntityPosition(Entity entity) {
  auto &transform = entity.GetComponent<TransformComponent>();
  return transform.position;
}

void SetEntityPosition(Entity entity, float x, float y) {
  if (!entity.HasComponent<TransformComponent>()) {
    Logger::Err("Entity does not have TransformComponent");
    return;
  }
  auto &transform = entity.GetComponent<TransformComponent>();
  transform.position.x = x;
  transform.position.y = y;
}

// create imgui demo window
void CreateImGuiDemoWindow() { ImGui::ShowDemoWindow(); }

class ScriptSystem : public System {
public:
  ScriptSystem() { RequireComponent<ScriptComponent>(); }

  void CreateLuaBindings(std::unique_ptr<Registry> &registry,
                         std::unique_ptr<AssetStore> &assetStore,
                         sol::state &lua) {
    lua.new_usertype<Entity>("entity", "get_id", &Entity::GetId, "destroy",
                             &Entity::Kill, "has_tag", &Entity::HasTag,
                             "belongs_to_group", &Entity::BelongsGroup);

    lua.set_function("get_position", GetEntityPosition);
    lua.set_function("set_position", SetEntityPosition);
    lua.set_function("create_imgui_demo_window", CreateImGuiDemoWindow);
    lua.set_function("create_imgui_spawn_enemy", RenderImGuiSpawnEnemy);

    lua.set("registry", registry.get());
    lua.set("assetstore", assetStore.get());
  }

  void Update(double deltaTime, int ellapsedTime) {
    for (auto &entity : GetSystemEntities()) {
      const auto &script = entity.GetComponent<ScriptComponent>();
      script.func(entity, deltaTime, ellapsedTime);
    }
  }
};

#endif
