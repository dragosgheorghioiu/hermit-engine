#ifndef CAMERA_FOLLOW_SYSTEM_H
#define CAMERA_FOLLOW_SYSTEM_H

#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../Game/Game.h"
#include <SDL2/SDL_rect.h>

class CameraFollowSystem : public System {
public:
  CameraFollowSystem() {
    RequireComponent<CameraFollowComponent>();
    RequireComponent<TransformComponent>();
  }

  void Update(SDL_Rect &camera) {
    for (Entity entity : GetSystemEntities()) {
      const TransformComponent &transform =
          entity.GetComponent<TransformComponent>();

      if (transform.position.x + static_cast<int>(camera.w / 2) <
          Game::mapWidth) {
        camera.x =
            transform.position.x - static_cast<int>(Game::windowWidth / 2);
      }
      if (transform.position.y + static_cast<int>(camera.h / 2) <
          Game::mapHeight) {
        camera.y =
            transform.position.y - static_cast<int>(Game::windowHeight / 2);
      }

      camera.x = camera.x < 0 ? 0 : camera.x;
      camera.y = camera.y < 0 ? 0 : camera.y;
      camera.x = camera.x > camera.w ? camera.w : camera.x;
      camera.y = camera.y > camera.h ? camera.h : camera.y;
    }
  }
};

#endif
