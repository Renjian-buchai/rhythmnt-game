#pragma once

#pragma warning(push)
#pragma warning( \
    disable : 26812 26819 4081 4820 4514 26819 26495 5039 4668 4365 4820 4710)
#include <SDL.h>

#include <iostream>
#include <vector>
#pragma warning(pop)

class game {
 public:
  bool quit = false;
  SDL_Window* mainWindow = nullptr;
  SDL_Renderer* mainWindowRenderer = nullptr;
  std::vector<SDL_Texture*> textures{};

  game(SDL_Rect screen);
  ~game();

  void update();
};
