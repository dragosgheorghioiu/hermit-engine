#ifndef CAMERA_FOLLOW_SYSTEM_H
#define CAMERA_FOLLOW_SYSTEM_H

#include "../ECS/ECS.h"
#include <SDL2/SDL_rect.h>

class CameraFollowSystem : public System {
public:
  CameraFollowSystem();
  void Update(SDL_Rect &camera);
};

#endif
