#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include "glm/glm.hpp"

struct BoxColliderComponent {
  glm::vec2 offset;
  glm::vec2 dimensions;
  BoxColliderComponent(glm::vec2 offset = glm::vec2(0.0, 0.0),
                       glm::vec2 dimensions = glm::vec2(0.0, 0.0))
      : offset(offset), dimensions(dimensions) {}
};

#endif
