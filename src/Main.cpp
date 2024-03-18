#include "./Game/Game.h"
#include <sol/sol.hpp>

int main(int argc, char *argv[]) {
  Game game;

  game.Init();
  game.Run();
  game.Destroy();

  return 0;
}
