#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include "../ECS/ECS.h"

class AnimationSystem : public System {
public:
  AnimationSystem();
  void Update();
};

#endif
