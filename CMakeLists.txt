cmake_minimum_required(VERSION 3.10)
project(game)

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

# Boost
find_package(Boost REQUIRED)

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

# Link SDL2 to imgui
target_link_libraries(imgui SDL2::SDL2)

# Add executable
set(
	SOURCES
	src/SceneLoader/SceneLoader.cpp
	src/AssetStore/AssetStore.cpp
	src/Game/Game.cpp
	src/ECS/ECS.cpp
	src/ECS/Entity.cpp
	src/ECS/Registry.cpp
	src/Main.cpp
	src/Logger/Logger.cpp
	src/Plugin/PluginLoader.cpp
	src/Plugin/PluginComponentFactory.cpp
)

##################################################### SHARED LIBRARIES OF SYSTEMS #####################################################

set(
	SYSTEMS_LIB
	# src/Systems/RenderGUISystem.cpp
	# src/Systems/CollisionSystem.cpp
	src/Systems/RenderSystem.cpp
	# src/Systems/ProjectileEmitSystem.cpp
	# src/Systems/ScriptSystem.cpp
	# src/Systems/ProjectileKillSystem.cpp
	# src/Systems/RenderCollisionSystem.cpp
	# src/Systems/CameraFollowSystem.cpp
	# src/Systems/KeyboardMovementSystem.cpp
	# src/Systems/AnimationSystem.cpp
	# src/Systems/MovementSystem.cpp
	# src/Systems/RenderTextLabelSystem.cpp
	# src/Systems/RenderHealthSystem.cpp
	# src/Systems/DamageSystem.cpp
)

add_library(
	systems
	SHARED
	${SYSTEMS_LIB}
)

target_include_directories(systems PUBLIC
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

target_link_libraries(
	systems
	sol2
	SDL2::SDL2
	SDL2_image::SDL2_image
	SDL2_ttf::SDL2_ttf
	SDL2_mixer::SDL2_mixer
	${LUA_LIBRARIES}
)
#######################################################################################################################################

set(
	COMPONENTS
	src/Components/AnimationComponent.h
	src/Components/KeyboardControlComponent.h
	src/Components/ScriptComponent.h
	src/Components/ProjectileEmitterComponent.h
	src/Components/ProjectileComponent.h
	src/Components/TextLabelComponent.h
	src/Components/CameraFollowComponent.h
	src/Components/SpriteComponent.h
	src/Components/FormatTextLabelComponent.h
	src/Components/RigidBodyComponent.h
	src/Components/HealthComponent.h
	src/Components/BoxColliderComponent.h
	src/Components/TransformComponent.h
)

set (
	HEADERS
	src/SceneLoader/SceneLoader.h
	src/AssetStore/AssetStore.h
	${COMPONENTS}
	src/EventBus/EventBus.h
	src/EventBus/Event.h
	src/Game/Game.h
	src/ECS/ECS.h
	src/ECS/Entity.h
	src/ECS/Registry.h
	src/ECS/ComponentInfoPool.h
	src/Events/KeyboardPressEvent.h
	src/Events/KeyboardReleaseEvent.h
	src/Events/CollisionEvent.h
	src/Logger/Logger.h
	src/Plugin/PluginComponentFactory.h
	src/Plugin/SystemInfo.h
	src/Plugin/PluginLoader.h
)

# add_library (
# 	demo
# 	SHARED
# 	IMPORTED
# )
# set_target_properties(
# 	demo
# 	PROPERTIES
# 	IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/demo.so
# )

add_executable(
	game
	${SOURCES}
	${HEADERS}
)

# Include directories
target_include_directories(game PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
	${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/libs/lua
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
	systems
	imgui
	sol2
	boost_filesystem
	boost_system
	SDL2::SDL2
	SDL2_image::SDL2_image
	SDL2_ttf::SDL2_ttf
	SDL2_mixer::SDL2_mixer
)
