#include "./AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL_image.h>

AssetStore::AssetStore() { Logger::Log("AssetStore created"); }
AssetStore::~AssetStore() { Logger::Log("AssetStore destroyed"); }

SDL_Texture *AssetStore::GetTexture(const std::string assetId) {
  if (textures.find(assetId) == textures.end()) {
    Logger::Err("ERROR: TEXTURE NOT FOUND: " + assetId);
    return nullptr;
  }
  return textures[assetId];
}
void AssetStore::AddTexture(SDL_Renderer *renderer, std::string id,
                            const std::string path) {
  SDL_Surface *surface = IMG_Load(path.c_str());
  if (!surface) {
    Logger::Err("ERROR LOADING IMAGE: " + path);
    return;
  }
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    Logger::Err("ERROR CREATING TEXTURE: " + path);
    return;
  }
  SDL_FreeSurface(surface);
  textures.emplace(id, texture);
  Logger::Log("Texture added: " + id);
}
void AssetStore::Clear() {
  for (auto &texture : textures) {
    SDL_DestroyTexture(texture.second);
  }
  textures.clear();
}
