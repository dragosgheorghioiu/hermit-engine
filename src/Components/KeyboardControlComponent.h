#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include <glm.hpp>

struct KeyboardControlComponent {
  glm::vec2 upVelocity;
  glm::vec2 rightVelocity;
  glm::vec2 downVelocity;
  glm::vec2 leftVelocity;

  KeyboardControlComponent(glm::vec2 upVelocity = glm::vec2(0),
                           glm::vec2 rightVelocity = glm::vec2(0),
                           glm::vec2 downVelocity = glm::vec2(0),
                           glm::vec2 leftVelocity = glm::vec2(0))
      : upVelocity(upVelocity), rightVelocity(rightVelocity),
        downVelocity(downVelocity), leftVelocity(leftVelocity) {}
};

#endif
