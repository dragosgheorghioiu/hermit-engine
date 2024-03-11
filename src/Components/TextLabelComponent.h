#ifndef TEXT_LABEL_COMPONENT_H
#define TEXT_LABEL_COMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

struct TextLabelComponent {
  std::string text;
  SDL_Color color;
  std::string font_id;
  bool isFixed;

  TextLabelComponent(const std::string &text = "",
                     const SDL_Color &color = {0, 0, 0, 255},
                     const std::string &font = "", bool isFixed = true)
      : text(text), color(color), font_id(font), isFixed(isFixed) {}
};

#endif
