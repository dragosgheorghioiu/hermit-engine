#include "RenderTextLabelSystem.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
RenderTextLabelSystem::RenderTextLabelSystem() {
  RequireComponent<TransformComponent>();
  RequireComponent<TextLabelComponent>();
}

void RenderTextLabelSystem::Update(SDL_Renderer *renderer,
                                   std::unique_ptr<AssetStore> &assetStore,
                                   SDL_Rect &camera) {
  for (auto &entity : GetSystemEntities()) {
    auto &textLabel = entity.GetComponent<TextLabelComponent>();
    auto &transform = entity.GetComponent<TransformComponent>();

    // render text on screen at the position of the entity
    TTF_Font *font = assetStore->GetFont(textLabel.font_id);
    SDL_Surface *surface =
        TTF_RenderText_Solid(font, textLabel.text.c_str(), textLabel.color);

    int posX = transform.position.x;
    int posY = transform.position.y;

    if (!textLabel.isFixed) {
      posX = transform.position.x - camera.x;
      posY = transform.position.y - camera.y;
    }

    int w, h;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    SDL_FreeSurface(surface);

    SDL_Rect textRect = {posX, posY, w, h};
    SDL_RenderCopy(renderer, texture, nullptr, &textRect);
    SDL_DestroyTexture(texture);
  }
}
