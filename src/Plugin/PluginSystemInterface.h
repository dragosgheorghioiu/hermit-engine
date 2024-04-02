#ifndef PLUGIN_SYSTEM_INTERFACE_H
#define PLUGIN_SYSTEM_INTERFACE_H

#include "ECS/ECS.h"

class PluginSystemInterface : public System {
public:
  template <typename... TArgs> void Update(TArgs &&...args);
  virtual ~PluginSystemInterface(){};
};

#endif
