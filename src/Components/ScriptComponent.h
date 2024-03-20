#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H

#include "sol/sol.hpp"

struct ScriptComponent {
  sol::function func;

  ScriptComponent(sol::function func = sol::lua_nil) : func(func) {}
};

#endif
