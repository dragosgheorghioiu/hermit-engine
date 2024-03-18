#include "LevelLoader.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include "Game.h"
#include <fstream>
#include <sol/sol.hpp>

LevelLoader::LevelLoader() { Logger::Log("LevelLoader created"); }

LevelLoader::~LevelLoader() { Logger::Log("LevelLoader destroyed"); }

void LevelLoader::LoadLevel(sol::state &lua, int level,
                            std::unique_ptr<Registry> &registry,
                            std::unique_ptr<AssetStore> &assetStore,
                            SDL_Renderer *renderer) {
  // Load level from file
  // Create entities
  // Create components
  // Add components to entities
  // Add entities to the game

  std::string levelPath =
      "assets/scripts/Level" + std::to_string(level) + ".lua";
  sol::load_result result = lua.load_file(levelPath);

  // check if script is valid
  if (!result.valid()) {
    sol::error err = result;
    Logger::Err("LevelLoader::LoadLevel: Error loading level file: " +
                levelPath + " - " + err.what());
    return;
  }

  // execute the script
  lua.script_file(levelPath);
  Logger::Log("LevelLoader::LoadLevel: " + levelPath);

  // Get the level table
  sol::table levelTable = lua["Level"];

  // Read the level assets
  sol::table assetsTable = levelTable["assets"];

  int i = 0;
  while (true) {
    sol::optional<sol::table> existsAsset = assetsTable[i];
    if (existsAsset == sol::nullopt) {
      break;
    }
    std::string assetType = assetsTable[i]["type"];
    if (assetType == "texture") {
      std::string assetId = assetsTable[i]["id"];
      std::string assetPath = assetsTable[i]["file"];
      assetStore->AddTexture(renderer, assetId, assetPath);
    } else if (assetType == "font") {
      std::string assetId = assetsTable[i]["id"];
      std::string assetPath = assetsTable[i]["file"];
      int assetFontSize = assetsTable[i]["fontSize"];
      assetStore->AddFont(assetId, assetPath, assetFontSize);
    }
    i++;
  }

  // Read the level map
  sol::table mapTable = levelTable["tilemap"];
  std::string map = mapTable["map"];
  std::string mapTextureId = mapTable["texture"];
  int tileSize = mapTable["tileSize"];
  double tileScale = mapTable["scale"];
  int mapColumns = mapTable["mapCols"];
  int mapRows = mapTable["mapRows"];

  std::fstream mapFile;
  mapFile.open(map);
  if (!mapFile.is_open()) {
    Logger::Err("ERROR LOADING TILEMAP");
    return;
  }

  for (int row = 0; row < mapRows; row++) {
    for (int column = 0; column < mapColumns; column++) {
      char ch;
      mapFile.get(ch);
      int sourceRectY = atoi(&ch) * tileSize;
      mapFile.get(ch);
      int sourceRectX = atoi(&ch) * tileSize;
      mapFile.ignore();

      Entity tile = registry->CreateEntity();
      tile.Group("tile");
      tile.AddComponent<TransformComponent>(
          glm::vec2(column * tileSize * tileScale, row * tileSize * tileScale),
          glm::vec2(tileScale, tileScale));
      tile.AddComponent<SpriteComponent>(tileSize, tileSize, mapTextureId, 0,
                                         false, sourceRectX, sourceRectY);
    }
  }
  mapFile.close();

  Game::mapWidth = mapColumns * tileSize * tileScale;
  Game::mapHeight = mapRows * tileSize * tileScale;

  // load entities
  sol::table entitiesTable = levelTable["entities"];
  i = 0;
  while (true) {
    sol::optional<sol::table> existsEntity = entitiesTable[i];
    if (existsEntity == sol::nullopt) {
      break;
    }
    sol::table entityTable = entitiesTable[i];
    Entity entity = registry->CreateEntity();

    sol::optional<std::string> tag = entityTable["tag"];
    if (tag != sol::nullopt) {
      entity.Tag(tag.value());
    }

    sol::optional<std::string> group = entityTable["group"];
    if (group != sol::nullopt) {
      entity.Group(group.value());
    }

    sol::optional<sol::table> components = entityTable["components"];
    if (components == sol::nullopt) {
      break;
    }
    sol::table componentsTable = components.value();

    // load components
    // TransformComponent
    sol::optional<sol::table> transform = componentsTable["transform"];
    if (transform != sol::nullopt) {
      sol::table transformTable = transform.value();
      glm::vec2 position = glm::vec2(transformTable["position"]["x"],
                                     transformTable["position"]["y"]);
      glm::vec2 scale =
          glm::vec2(transformTable["scale"]["x"], transformTable["scale"]["y"]);
      int rotation = transformTable["rotation"];
      entity.AddComponent<TransformComponent>(position, scale, rotation);
    }

    // // SpriteComponent
    sol::optional<sol::table> sprite = componentsTable["sprite"];
    if (sprite != sol::nullopt) {
      sol::table spriteTable = sprite.value();
      int width = spriteTable["width"];
      int height = spriteTable["height"];
      std::string id = spriteTable["id"];
      unsigned int zIndex = spriteTable["zIndex"].get_or(0);
      bool isFixed = spriteTable["isFixed"].get_or(false);
      int srcRectX = spriteTable["srcRect"]["x"].get_or(0);
      int srcRectY = spriteTable["srcRect"]["y"].get_or(0);
      entity.AddComponent<SpriteComponent>(width, height, id, zIndex, isFixed,
                                           srcRectX, srcRectY);
    }

    // // AnimationComponent
    sol::optional<sol::table> animation = componentsTable["animation"];
    if (animation != sol::nullopt) {
      sol::table animationTable = animation.value();
      int speed = animationTable["speed"];
      int frames = animationTable["frames"];
      bool loop = animationTable["loop"];
      entity.AddComponent<AnimationComponent>(frames, speed, loop);
    }

    // RigidBodyComponent
    sol::optional<sol::table> rigidBody = componentsTable["rigidbody"];
    if (rigidBody != sol::nullopt) {
      sol::table rigidBodyTable = rigidBody.value();
      glm::vec2 velocity = glm::vec2(rigidBodyTable["velocity"]["x"],
                                     rigidBodyTable["velocity"]["y"]);
      entity.AddComponent<RigidBodyComponent>(velocity);
    }

    // KeyboardControlComponent
    sol::optional<sol::table> keyboardControl =
        componentsTable["keyboardcontrol"];
    if (keyboardControl != sol::nullopt) {
      sol::table keyboardControlTable = keyboardControl.value();
      glm::vec2 up = glm::vec2(keyboardControlTable["up"]["x"],
                               keyboardControlTable["up"]["y"]);
      glm::vec2 right = glm::vec2(keyboardControlTable["right"]["x"],
                                  keyboardControlTable["right"]["y"]);
      glm::vec2 down = glm::vec2(keyboardControlTable["down"]["x"],
                                 keyboardControlTable["down"]["y"]);
      glm::vec2 left = glm::vec2(keyboardControlTable["left"]["x"],
                                 keyboardControlTable["left"]["y"]);
      entity.AddComponent<KeyboardControlComponent>(up, right, down, left);
    }

    // CameraFollowComponent
    sol::optional<sol::table> cameraFollow = componentsTable["camerafollow"];
    if (cameraFollow != sol::nullopt) {
      entity.AddComponent<CameraFollowComponent>();
    }

    // HealthComponent
    sol::optional<sol::table> health = componentsTable["health"];
    if (health != sol::nullopt) {
      sol::table healthTable = health.value();
      int healthValue = healthTable["max"];
      entity.AddComponent<HealthComponent>(healthValue);
    }

    // BoxColliderComponent
    sol::optional<sol::table> boxCollider = componentsTable["boxcollider"];
    if (boxCollider != sol::nullopt) {
      sol::table boxColliderTable = boxCollider.value();
      glm::vec2 offset = glm::vec2(boxColliderTable["offset"]["x"],
                                   boxColliderTable["offset"]["y"]);
      glm::vec2 dimensions = glm::vec2(boxColliderTable["dimensions"]["x"],
                                       boxColliderTable["dimensions"]["y"]);
      entity.AddComponent<BoxColliderComponent>(offset, dimensions);
    }

    // ProjectileEmitterComponent
    sol::optional<sol::table> projectileEmitter =
        componentsTable["projectileemitter"];
    if (projectileEmitter != sol::nullopt) {
      sol::table projectileEmitterTable = projectileEmitter.value();
      float velocity = projectileEmitterTable["speed"];
      int repeat = projectileEmitterTable["repeat"];
      int duration = projectileEmitterTable["duration"];
      bool isFriendly = projectileEmitterTable["isFriendly"];
      int damage = projectileEmitterTable["damage"];
      float angle = projectileEmitterTable["angle"];
      entity.AddComponent<ProjectileEmitterComponent>(
          velocity, repeat, duration, isFriendly, damage, angle);
    }

    // TextLabelComponent
    sol::optional<sol::table> textLabel = componentsTable["textlabel"];
    if (textLabel != sol::nullopt) {
      sol::table textLabelTable = textLabel.value();
      std::string text = textLabelTable["text"];
      std::string fontId = textLabelTable["font"];
      SDL_Color color = {
          textLabelTable["color"]["r"], textLabelTable["color"]["g"],
          textLabelTable["color"]["b"], textLabelTable["color"]["a"]};
      bool isFixed = textLabelTable["isFixed"];
      entity.AddComponent<TextLabelComponent>(text, color, fontId, isFixed);
    }
    i++;
  }
  //
  //   Entity label = registry->CreateEntity();
  //   label.Tag("label");
  //   label.AddComponent<TransformComponent>(
  //       glm::vec2(Game::windowWidth / 2 - 200, 10), glm::vec2(1.0, 1.0));
  //   label.AddComponent<TextLabelComponent>("CHOPPA", SDL_Color{255, 255,
  //   255},
  //                                          "bigblue48", false);
}
