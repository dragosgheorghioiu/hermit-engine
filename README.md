# Hermit Engine
## Description
A modular 2D game engine using a plugin architecture.

The goal of this project is to create a game engine that can be as simple or complex as needed.

## Features
- Scripting and prototyping using Lua
- Loading modules dynamically at runtime
- Easy to customize

## Dependencies
- [SDL2](https://www.libsdl.org)
    - SDL2Image
    - SDL2TTF
    - SDL2Mixer
- [Boost.cpp](https://www.boost.org/)
- [TOML11](https://github.com/ToruNiina/toml11)
- [Imgui](https://github.com/ocornut/imgui)
- [SOL2](https://github.com/ThePhD/sol2)
- [Lua](https://www.lua.org/)
- [glm](https://github.com/g-truc/glm)

## Build the project
```sh
mkdir build
cd build
cmake ..
make -j 8
```
