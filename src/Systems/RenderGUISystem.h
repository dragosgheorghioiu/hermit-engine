#ifndef RENDER_GUI_SYSTEM_H
#define RENDER_GUI_SYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../ECS/ECS.h"
#include <memory>
#include <sol/state.hpp>

void RenderImGuiSpawnEnemy(Registry *registry, AssetStore *assetstore);
void CreateImGuiDemoWindow();
void ReloadScript(int level, sol::state &lua);
void CreateLuaScriptReloadButton(sol::state &lua);

class RenderGUISystem : public System {
public:
  RenderGUISystem();
  void Update(const std::unique_ptr<Registry> &registry,
              std::unique_ptr<AssetStore> &assetstore, const SDL_Rect &camera,
              sol::state &lua);
};

#endif
