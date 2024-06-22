cmake_minimum_required(VERSION 3.10)
project(game)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
 set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
 set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O0")


# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Boost
find_package(Boost REQUIRED)

# Lua
find_package(Lua REQUIRED)


# Add executable
set(
	SOURCES
	src/SceneLoader/SceneLoader.cpp
	src/AssetStore/AssetStore.cpp
	src/Game/Game.cpp
	src/ECS/Entity.cpp
	src/ECS/Registry.cpp
	src/Main.cpp
	src/Logger/Logger.cpp
	src/Plugin/PluginLoader.cpp
	src/Plugin/PluginComponentFactory.cpp
	src/Plugin/PluginEventFactoryList.cpp
	libs/imgui/imgui.cpp
	libs/imgui/imgui_draw.cpp
	libs/imgui/imgui_widgets.cpp
	libs/imgui/imgui_demo.cpp
	libs/imgui/imgui_impl_sdl2.cpp
	libs/imgui/imgui_impl_sdlrenderer2.cpp
	libs/imgui/imgui_tables.cpp
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libs/sol2
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libs/toml
)

set (
	HEADERS
	src/SceneLoader/SceneLoader.h
	src/AssetStore/AssetStore.h
	src/Game/Game.h
	src/ECS/Entity.h
	src/ECS/Registry.h
	src/ECS/ComponentInfoPool.h
	src/Logger/Logger.h
	src/Plugin/PluginComponentFactory.h
	src/Plugin/PluginEventFactoryList.h
	src/Plugin/SystemInfo.h
	src/Plugin/SystemInstance.h
	src/Plugin/PluginLoader.h

	libs/imgui/imgui.h
	libs/imgui/imconfig.h
	libs/imgui/imgui_internal.h
	libs/imgui/imstb_rectpack.h
	libs/imgui/imstb_textedit.h
	libs/imgui/imstb_truetype.h
	libs/imgui/imgui_impl_sdl2.h
	libs/imgui/imgui_impl_sdlrenderer2.h
)

add_executable(
	game
	${SOURCES}
	${HEADERS}
)

# Include directories
target_include_directories(game PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/libs/sol2
	${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
	${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/libs/toml
	${SDL2_INCLUDE_DIRS}
	${SDL2IMAGE_INCLUDE_DIRS}
	${SDL2TTF_INCLUDE_DIRS}
	${SDL2MIXER_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(
	game
	sol2
	boost_filesystem
	SDL2::SDL2
	SDL2_image::SDL2_image
	SDL2_ttf::SDL2_ttf
	SDL2_mixer::SDL2_mixer
	${LUA_LIBRARIES}
)
