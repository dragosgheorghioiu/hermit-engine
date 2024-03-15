#ifndef RENDER_GUI_SYSTEM_H
#define RENDER_GUI_SYSTEM_H

#include "../ECS/ECS.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"

class RenderGUISystem : public System {
public:
  RenderGUISystem() = default;

  void Update() {
    // setup imgui render window
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    // render imgui window

    ImGui::SetNextWindowSize(ImVec2(550, 20), ImGuiCond_Once);
    if (ImGui::Begin("Spawn Enemy")) {
      ImGui::Text("Here is where you can spawn enemies");
    }
    ImGui::End();

    // render imgui
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  }
};

#endif
