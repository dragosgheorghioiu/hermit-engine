#include "PluginRenderSystem.h"
#include "../../AssetStore/AssetStore.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL2/SDL_render.h>
#include <algorithm>

PluginRenderSystem::PluginRenderSystem() = default;
PluginRenderSystem::~PluginRenderSystem() = default;

void PluginRenderSystem::update(std::vector<void *> params) {
  SDL_Renderer *renderer = static_cast<SDL_Renderer *>(params[0]);
  AssetStore *assetStore = static_cast<AssetStore *>(params[1]);
  SDL_Rect *camera = static_cast<SDL_Rect *>(params[2]);

  std::vector<EntityType> entities = getSystemEntities();

  entities.erase(
      std::remove_if(
          entities.begin(), entities.end(),
          [&camera](EntityType &entity) {
            PluginTransformComponent *transform =
                static_cast<PluginTransformComponent *>(
                    entity.getComponent("TransformComponent").instance);
            PluginSpriteComponent *sprite =
                static_cast<PluginSpriteComponent *>(
                    entity.getComponent("SpriteComponent").instance);
            if (sprite->isFixed)
              return false;
            if (transform->position.x + sprite->width * transform->scale.x <
                    camera->x ||
                transform->position.y + sprite->height * transform->scale.y <
                    camera->y ||
                transform->position.x > camera->x + camera->w ||
                transform->position.y > camera->y + camera->h) {
              return true;
            }
            return false;
          }),
      entities.end());

  std::sort(entities.begin(), entities.end(), [](EntityType &a, EntityType &b) {
    PluginSpriteComponent *spriteA = static_cast<PluginSpriteComponent *>(
        a.getComponent("SpriteComponent").instance);
    PluginSpriteComponent *spriteB = static_cast<PluginSpriteComponent *>(
        b.getComponent("SpriteComponent").instance);
    return spriteA->zIndex < spriteB->zIndex;
  });

  for (auto entity : entities) {
    PluginTransformComponent *transform =
        static_cast<PluginTransformComponent *>(
            entity.getComponent("TransformComponent").instance);
    PluginSpriteComponent *sprite = static_cast<PluginSpriteComponent *>(
        entity.getComponent("SpriteComponent").instance);

    SDL_Rect srcRect = sprite->srcRect;
    SDL_Rect dstRect = {
        static_cast<int>(transform->position.x - !sprite->isFixed * camera->x),
        static_cast<int>(transform->position.y - !sprite->isFixed * camera->y),
        static_cast<int>(sprite->width * transform->scale.x),
        static_cast<int>(sprite->height * transform->scale.y)};

    SDL_RenderCopyEx(renderer, assetStore->GetTexture(sprite->id), &srcRect,
                     &dstRect, transform->rotation, nullptr, sprite->flip);
  }
}

const std::string PluginRenderSystem::GetName() const { return "RenderSystem"; }

std::function<void(void *)>
PluginRenderSystem::getCallback(std::string eventType) {
  return nullptr;
}

std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
PluginRenderSystem::getGUIElements() {
  return std::unordered_map<std::string, std::function<void(ImGuiContext *)>>();
}

extern "C" void *createInstance() { return new PluginRenderSystem(); }
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginRenderSystem *>(instance);
}

extern "C" const char *getName() { return "PluginRenderSystem"; }

extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"SpriteComponent", "TransformComponent",
                                     nullptr};
  return components;
}
