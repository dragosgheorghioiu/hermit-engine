#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardPressEvent.h"
#include "../Events/KeyboardReleaseEvent.h"
#include <SDL2/SDL_keyboard.h>

class KeyboardMovementSystem : public System {
private:
  void updateAngle(Entity &entity, const double &angle);

public:
  KeyboardMovementSystem();

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus);

  void onKeyboardInput(KeyPressEvent &e);

  void onKeyboardRelease(KeyReleaseEvent &e);

  void Update();
};

#endif
