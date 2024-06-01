#include "RenderHealthSystem.h"
#include "../Components/HealthComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

RenderHealthSystem::RenderHealthSystem() {
  RequireComponent<TransformComponent>();
  RequireComponent<SpriteComponent>();
  RequireComponent<HealthComponent>();
}

void RenderHealthSystem::Update(SDL_Renderer *renderer,
                                std::unique_ptr<AssetStore> &assetStore,
                                SDL_Rect &camera) {
  for (auto entity : GetSystemEntities()) {
    const auto &transform = entity.GetComponent<TransformComponent>();
    const auto &sprite = entity.GetComponent<SpriteComponent>();
    const auto &health = entity.GetComponent<HealthComponent>();

    SDL_Color color = {255, 0, 0, 255};
    if (health.currentHealth >= health.maxHealth * 2 / 3) {
      // green
      color = {0, 255, 0, 255};
    } else if (health.currentHealth > health.maxHealth / 3) {
      // yellow
      color = {255, 255, 0, 255};
    } else {
      // red
      color = {255, 0, 0, 255};
    }

    int width = 50;
    int height = 10;
    double healthPositionX =
        transform.position.x + (sprite.width * transform.scale.x) - camera.x;
    double healthPositionY = transform.position.y - 10 - camera.y;

    // draw outline
    SDL_Rect outline = {static_cast<int>(healthPositionX - 3),
                        static_cast<int>(healthPositionY - 3), width + 6,
                        height + 21};
    SDL_SetRenderDrawColor(renderer, 24, 24, 24, 255);
    SDL_RenderFillRect(renderer, &outline);

    // draw health bar
    SDL_Rect healthBar = {
        static_cast<int>(healthPositionX), static_cast<int>(healthPositionY),
        static_cast<int>(width * health.currentHealth / health.maxHealth),
        height};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &healthBar);

    // write how much health is left under the health bar
    std::string healthText = "HP: " + std::to_string(health.currentHealth);
    SDL_Surface *surface = TTF_RenderText_Solid(
        assetStore->GetFont("bigblue12"), healthText.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect textRect = {static_cast<int>(healthPositionX),
                         static_cast<int>(healthPositionY + height + 5), w, h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
  }
}
