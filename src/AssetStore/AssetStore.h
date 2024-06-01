#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>
#include <vector>

class AssetStore {
private:
  std::map<std::string, SDL_Texture *> textures;
  std::map<std::string, TTF_Font *> fonts;

public:
  AssetStore();
  ~AssetStore();
  SDL_Texture *GetTexture(const std::string &assetId);
  std::vector<std::string> GetTextureIds();
  void AddTexture(SDL_Renderer *renderer, const std::string &id,
                  const std::string path);
  TTF_Font *GetFont(const std::string &assetId);
  void AddFont(std::string id, const std::string &path, int fontSize);
  void ClearTextures();
  void ClearFonts();
  void Clear();
};

#endif
