#include "AnimationSystem.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

AnimationSystem::AnimationSystem() {
  RequireComponent<AnimationComponent>();
  RequireComponent<SpriteComponent>();
}

void AnimationSystem::Update() {
  for (auto entity : GetSystemEntities()) {
    AnimationComponent &animation = entity.GetComponent<AnimationComponent>();
    SpriteComponent &sprite = entity.GetComponent<SpriteComponent>();

    animation.indexCurrentFrame =
        ((SDL_GetTicks() - animation.startTime) * animation.frameSpeed / 1000) %
        animation.numFrames;
    sprite.srcRect.x = sprite.width * animation.indexCurrentFrame;
  }
}
