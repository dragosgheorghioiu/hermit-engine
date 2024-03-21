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
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <memory>

class RenderGUISystem : public System {
public:
  RenderGUISystem() = default;

  void Update(const std::unique_ptr<Registry> &registry,
              std::unique_ptr<AssetStore> &assetstore, const SDL_Rect &camera) {
    // setup imgui render window
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    // render imgui window

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    ImGuiModFlags flags = ImGuiWindowFlags_NoTitleBar |
                          ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                          ImGuiWindowFlags_AlwaysAutoResize |
                          ImGuiWindowFlags_NoDecoration;
    if (ImGui::Begin("Mouse Position", nullptr, flags)) {
      // show mouse position
      ImGui::Text("Mouse Position: (%.1f,%.1f)",
                  ImGui::GetIO().MousePos.x + camera.x,
                  ImGui::GetIO().MousePos.y + camera.y);
    }
    ImGui::End();

    if (ImGui::Begin("Spawn Enemy")) {
      ImGui::Text("Here is where you can spawn enemies");
      static int posx = 0;
      static int posy = 0;
      static int health = 20;
      static int scale = 1;
      static int speedX = 0;
      static int speedY = 0;
      static long unsigned int spriteIndex = 0;
      static const std::vector<std::string> sprites =
          assetstore->GetTextureIds();
      static float projectileRepeat = 1.0f;
      static float projectileDuration = 1.0f;
      static int projectileDamage = 0;
      static float projectileSpeed = 100.0;
      static float angle = 0.0f;

      if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::SliderInt("Health", &health, 1, 99);
      }
      ImGui::Spacing();

      if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
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
      }
      ImGui::Spacing();

      if (ImGui::CollapsingHeader("Transform",
                                  ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputInt("x pos", &posx);
        ImGui::InputInt("y pos", &posy);
        ImGui::SliderInt("scale", &scale, 1, 10);
        ImGui::InputInt("speedX (px/sec)", &speedX);
        ImGui::InputInt("speedY (px/sec)", &speedY);
      }
      ImGui::Spacing();

      if (ImGui::CollapsingHeader("Projectiles",
                                  ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputFloat("repeat (sec)", &projectileRepeat);
        ImGui::InputFloat("duration (sec)", &projectileDuration);
        ImGui::InputInt("damage", &projectileDamage);
        ImGui::InputFloat("velocity (px/sec)", &projectileSpeed);
        ImGui::SliderAngle("angle (deg)", &angle, 0.0f, 360.0f);
      }
      ImGui::Spacing();
      ImGui::Separator();
      ImGui::Spacing();

      if (ImGui::Button("Spawn Enemy")) {
        Entity enemy = registry->CreateEntity();
        enemy.Group("enemy");
        enemy.AddComponent<TransformComponent>(glm::vec2(posx, posy),
                                               glm::vec2(scale, scale));
        enemy.AddComponent<SpriteComponent>(32, 32, sprites[spriteIndex], 2);
        enemy.AddComponent<RigidBodyComponent>(glm::vec2(speedX, speedY));
        enemy.AddComponent<BoxColliderComponent>(glm::vec2(5, 5),
                                                 glm::vec2(25, 20));
        enemy.AddComponent<ProjectileEmitterComponent>(
            projectileSpeed, static_cast<int>(projectileRepeat * 1000),
            static_cast<int>(projectileDuration * 1000), false,
            projectileDamage, angle);
        enemy.AddComponent<HealthComponent>(health);
      }
    }
    ImGui::End();

    // ImGui::ShowDemoWindow();

    // render imgui
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  }
};

#endif
