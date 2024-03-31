cmake_minimum_required(VERSION 3.10)
project(grapevine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# lua
find_library(LUA_LIBRARIES lua)

# Make imgui dynamic library
set(IMGUI_SOURCES
	libs/imgui/imgui.cpp
	libs/imgui/imgui_draw.cpp
	libs/imgui/imgui_widgets.cpp
	libs/imgui/imgui_demo.cpp
	libs/imgui/imgui_tables.cpp
	libs/imgui/imgui_impl_sdlrenderer2.cpp
	libs/imgui/imgui_impl_sdl2.cpp
)

add_library(imgui STATIC
	${IMGUI_SOURCES}
)

# Link SDL2
target_link_libraries(imgui SDL2::SDL2)

# Add executable
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)
add_executable(grapevine
	${SOURCES}
	${HEADERS}
)

# Include directories
target_include_directories(grapevine PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
	${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/libs/lua
	${CMAKE_CURRENT_SOURCE_DIR}/libs/toml
	${SDL2_INCLUDE_DIRS}
	${SDL2IMAGE_INCLUDE_DIRS}
	${SDL2TTF_INCLUDE_DIRS}
	${SDL2MIXER_INCLUDE_DIRS}
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libs/sol2
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libs/toml
)

# Link libraries
target_link_libraries(
	grapevine
	imgui
	sol2
	SDL2::SDL2
	SDL2_image::SDL2_image
	SDL2_ttf::SDL2_ttf
	SDL2_mixer::SDL2_mixer
	${LUA_LIBRARIES}
)
