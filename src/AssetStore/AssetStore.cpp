#include "./AssetStore.h"
#include "../Logger/Logger.h"
#include <SDL2/SDL_image.h>

AssetStore::AssetStore() { Logger::Log("AssetStore created"); }
AssetStore::~AssetStore() {
  Clear();
  Logger::Log("AssetStore destroyed");
}

SDL_Texture *AssetStore::GetTexture(const std::string &assetId) {
  if (textures.find(assetId) == textures.end()) {
    Logger::Err("ERROR: TEXTURE NOT FOUND: " + assetId);
    return nullptr;
  }
  return textures[assetId];
}
void AssetStore::AddTexture(SDL_Renderer *renderer, const std::string &id,
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

TTF_Font *AssetStore::GetFont(const std::string &assetId) {
  if (fonts.find(assetId) == fonts.end()) {
    Logger::Err("ERROR: FONT NOT FOUND: " + assetId);
    return nullptr;
  }
  return fonts[assetId];
}

void AssetStore::AddFont(std::string id, const std::string &path,
                         int fontSize) {
  TTF_Font *font = TTF_OpenFont(path.c_str(), fontSize);
  if (!font) {
    Logger::Err("ERROR LOADING FONT: " + path);
    return;
  }
  fonts.emplace(id, font);
  Logger::Log("Font added: " + id);
}

void AssetStore::ClearTextures() {
  for (auto &texture : textures) {
    SDL_DestroyTexture(texture.second);
  }
  textures.clear();
}
void AssetStore::ClearFonts() {
  for (auto &font : fonts) {
    TTF_CloseFont(font.second);
  }
  fonts.clear();
}

void AssetStore::Clear() {
  ClearTextures();
  ClearFonts();
}
