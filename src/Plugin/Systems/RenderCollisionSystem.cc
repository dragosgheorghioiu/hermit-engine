#include "RenderCollisionSystem.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL2/SDL.h>
#include <iostream>

RenderCollisionSystem::RenderCollisionSystem() = default;
RenderCollisionSystem::~RenderCollisionSystem() = default;

void RenderCollisionSystem::update(std::vector<void *> params) {
  auto *renderer = static_cast<SDL_Renderer *>(params[0]);
  auto *camera = static_cast<SDL_Rect *>(params[1]);
  std::vector<EntityType> entities = getSystemEntities();

  for (auto &entity : entities) {
    auto *currentEnityBoxCollider = static_cast<BoxColliderComponent *>(
        entity.getComponent("BoxColliderComponent").instance);
    auto *currentEntityTransform = static_cast<PluginTransformComponent *>(
        entity.getComponent("TransformComponent").instance);

    SDL_Rect collider = {
        static_cast<int>(currentEntityTransform->position.x +
                         currentEnityBoxCollider->offset.x *
                             currentEntityTransform->scale.x -
                         camera->x),
        static_cast<int>(currentEntityTransform->position.y +
                         currentEnityBoxCollider->offset.y *
                             currentEntityTransform->scale.y -
                         camera->y),
        static_cast<int>(currentEnityBoxCollider->dimensions.x *
                         currentEntityTransform->scale.x),
        static_cast<int>(currentEnityBoxCollider->dimensions.y *
                         currentEntityTransform->scale.y)};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &collider);
  }
}

const std::string RenderCollisionSystem::GetName() const {
  return "RenderCollisionSystem";
}

std::function<void(void *)>
RenderCollisionSystem::getCallback(std::string eventType) {
  return nullptr;
}

std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
RenderCollisionSystem::getGUIElements() {
  return std::unordered_map<std::string, std::function<void(ImGuiContext *)>>();
}

extern "C" void *createInstance() { return new RenderCollisionSystem(); }

extern "C" void destroyInstance(void *instance) {
  delete static_cast<RenderCollisionSystem *>(instance);
}

extern "C" const char *getName() { return "RenderCollisionSystem"; }

extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"BoxColliderComponent",
                                     "TransformComponent", nullptr};
  return components;
}
