#ifndef RENDER_GUI_SYSTEM_H
#define RENDER_GUI_SYSTEM_H

#include "../AssetStore/AssetStore.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include <memory>

class RenderGUISystem : public System {
public:
  RenderGUISystem() = default;

  void Update(const std::unique_ptr<Registry> &registry,
              std::unique_ptr<AssetStore> &assetstore) {
    // setup imgui render window
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    // render imgui window

    if (ImGui::Begin("Spawn Enemy")) {
      ImGui::Text("Here is where you can spawn enemies");
      static int posx = 0;
      static int posy = 0;
      static int health = 99;
      static int scale = 1;
      static int speedX = 0;
      static int speedY = 0;
      static long unsigned int spriteIndex = 0;
      static const std::vector<std::string> sprites =
          assetstore->GetTextureIds();
      static int projectileRepeat = 0;
      static int projectileDuration = 0;
      ImGui::InputInt("Enemy health:", &health);
      ImGui::InputInt("Enemy x pos:", &posx);
      ImGui::InputInt("Enemy y pos:", &posy);
      ImGui::InputInt("Enemy scale:", &scale);
      ImGui::InputInt("Enemy speedX:", &speedX);
      if (ImGui::BeginCombo("Enemy sprite", sprites[spriteIndex].c_str())) {
        for (long unsigned int n = 0; n < sprites.size(); n++) {
          const bool is_selected = (spriteIndex == n);
          if (ImGui::Selectable(sprites[n].c_str(), is_selected))
            spriteIndex = n;
          if (is_selected)
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }

      ImGui::InputInt("Enemy speedY:", &speedY);
      if (ImGui::Button("Spawn Enemy")) {
        Entity enemy = registry->CreateEntity();
        enemy.Group("enemy");
        enemy.AddComponent<TransformComponent>(glm::vec2(posx, posy),
                                               glm::vec2(scale, scale));
        enemy.AddComponent<SpriteComponent>(32, 32, sprites[spriteIndex], 2);
        enemy.AddComponent<RigidBodyComponent>(glm::vec2(speedX, speedY));
        enemy.AddComponent<BoxColliderComponent>(glm::vec2(0, 0),
                                                 glm::vec2(32, 32));
        enemy.AddComponent<ProjectileEmitterComponent>(
            glm::vec2(75, 75), projectileRepeat, projectileDuration, RIGHT,
            false, 1);
        enemy.AddComponent<HealthComponent>(health);
      }
    }
    ImGui::End();

    // render imgui
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  }
};

#endif
