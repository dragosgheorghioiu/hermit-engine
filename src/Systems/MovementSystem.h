#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

class MovementSystem : public System {
public:
  MovementSystem() {
    // require tranform component
  }
  void Update() {
    // update component position every frame
    //
    for (auto enity : GetEntities()) {
      // update entity position based on its velocity
    }
  }
};

#endif
