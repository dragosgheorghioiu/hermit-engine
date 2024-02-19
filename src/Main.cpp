#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <iostream>
#include <sol/sol.hpp>

int main() {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string,
                     sol::lib::table, sol::lib::math);
  SDL_Init(SDL_INIT_EVERYTHING);
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
