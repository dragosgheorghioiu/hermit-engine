#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "../Components/ScriptComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include <glm.hpp>

glm::vec2 GetEntityPosition(Entity entity) {
  auto &transform = entity.GetComponent<TransformComponent>();
  return transform.position;
}

void SetEntityPosition(Entity entity, int x, int y) {
  if (!entity.HasComponent<TransformComponent>()) {
    Logger::Err("Entity does not have TransformComponent");
    return;
  }
  std::cout << "Setting position to " << x << ", " << y << std::endl;
  auto &transform = entity.GetComponent<TransformComponent>();
  transform.position.x = x;
  transform.position.y = y;
}

class ScriptSystem : public System {
public:
  ScriptSystem() { RequireComponent<ScriptComponent>(); }

  void CreateLuaBindings(sol::state &lua) {
    lua.new_usertype<Entity>("Entity", "get_id", &Entity::GetId, "destroy",
                             &Entity::Kill, "has_tag", &Entity::HasTag,
                             "belongs_to_group", &Entity::BelongsGroup);

    lua.set_function("get_position", GetEntityPosition);
    lua.set_function("set_position", SetEntityPosition);
  }

  void Update(double deltaTime, int ellapsedTime) {
    for (auto &entity : GetSystemEntities()) {
      const auto &script = entity.GetComponent<ScriptComponent>();
      script.func(entity, deltaTime, ellapsedTime);
    }
  }
};

#endif
