#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL2/SDL.h>
#include <map>
#include <string>

class AssetStore {
private:
  std::map<std::string, SDL_Texture *> textures;

public:
  AssetStore();
  ~AssetStore();
  SDL_Texture *GetTexture(std::string assetId);
  void AddTexture(SDL_Renderer *renderer, std::string id,
                  const std::string path);
  void Clear();
};

#endif
