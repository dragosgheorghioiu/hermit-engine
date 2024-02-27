#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"

class AnimationSystem : public System {
public:
  AnimationSystem() {
    RequireComponent<AnimationComponent>();
    RequireComponent<SpriteComponent>();
  }
  void Update() {
    for (auto entity : GetSystemEntities()) {
      AnimationComponent &animation = entity.GetComponent<AnimationComponent>();
      SpriteComponent &sprite = entity.GetComponent<SpriteComponent>();

      animation.indexCurrentFrame = ((SDL_GetTicks() - animation.startTime) *
                                     animation.frameSpeed / 1000) %
                                    animation.numFrames;
      sprite.srcRect.x = sprite.width * animation.indexCurrentFrame;
    }
  }
};

#endif
