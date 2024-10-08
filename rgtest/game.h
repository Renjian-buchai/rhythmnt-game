#pragma once

#pragma warning(push)
#pragma warning( \
    disable : 26812 26819 4081 4820 4514 26819 26495 5039 4668 4365 4820 4710)
#include <SDL.h>
#include <SDL_ttf.h>
#include <bass.h>

#include <iostream>
#include <vector>
#pragma warning(pop)

class game {
 public:
  enum class gameState : uint8_t { QUIT, SONGSELECT, GAMEPLAY, PAUSE };

  // Game logic
  gameState state = gameState::GAMEPLAY;
  uint8_t noteSpeed = 100; 

  // SDL rendering, event handling
  const SDL_Rect screen;
  SDL_Window* mainWindow = nullptr;
  SDL_Renderer* mainWindowRenderer = nullptr;
  std::vector<SDL_Texture*> textures{};

  // SDL fonts
  TTF_Font* font = nullptr;

  // Audio management
  std::vector<HSTREAM> bassStreams{};

  game(SDL_Rect screen);
  ~game();

  void songSelect();
  void update();
};
