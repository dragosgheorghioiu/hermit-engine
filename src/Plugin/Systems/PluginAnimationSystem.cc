#include "./PluginAnimationSystem.h"
#include "../../../libs/imgui/imgui.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

PluginAnimationSystem::PluginAnimationSystem() = default;
PluginAnimationSystem::~PluginAnimationSystem() = default;

void PluginAnimationSystem::update(std::vector<void *> params) {
  for (auto entity : getSystemEntities()) {
    AnimationComponent *animation = static_cast<AnimationComponent *>(
        entity.getComponent("AnimationComponent").instance);
    PluginSpriteComponent *sprite = static_cast<PluginSpriteComponent *>(
        entity.getComponent("SpriteComponent").instance);

    if (animation->animationIndex != animation->lastAnimationIndex) {
      animation->lastAnimationIndex = animation->animationIndex;
      animation->startTime = SDL_GetTicks();
      animation->indexCurrentFrame = 0;
    }

    if (!animation->isLooping[animation->animationIndex] &&
        animation->indexCurrentFrame >=
            animation->numFrames[animation->animationIndex] - 1) {
      continue;
    }

    animation->indexCurrentFrame =
        ((SDL_GetTicks() - animation->startTime) *
         animation->frameSpeed[animation->animationIndex] / 1000) %
        animation->numFrames[animation->animationIndex];
    sprite->srcRect.x =
        animation->originX + (sprite->width + animation->spaceBetweenFramesX) *
                                 animation->indexCurrentFrame;
    sprite->srcRect.y =
        animation->originY + (sprite->height + animation->spaceBetweenFramesY) *
                                 animation->animationIndex;
  }
}

const std::string PluginAnimationSystem::GetName() const {
  return "PluginAnimationSystem";
}

std::function<void(void *)>
PluginAnimationSystem::getCallback(std::string eventType) {
  return nullptr;
}

std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
PluginAnimationSystem::getGUIElements() {
  std::unordered_map<std::string, std::function<void(ImGuiContext *)>> elements;
  elements["Demo Window"] = [this](ImGuiContext *context) {
    demoWindow(context);
  };
  return elements;
}

void PluginAnimationSystem::demoWindow(ImGuiContext *context) {
  ImGui::SetCurrentContext(context);
  if (ImGui::Begin("Demo Window")) {
    ImGui::Text("This is a demo window");
  }
  ImGui::End();
}

extern "C" void *createInstance() { return new PluginAnimationSystem(); }
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginAnimationSystem *>(instance);
}
extern "C" const char *getName() { return "PluginAnimationSystem"; }
extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"SpriteComponent", "AnimationComponent",
                                     nullptr};
  return components;
}
