#include "game.h"

game::game(SDL_Rect screen)
    : quit(false),
      mainWindow(SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen.w, screen.h,
                                  SDL_WINDOW_BORDERLESS)),
      mainWindowRenderer(SDL_CreateRenderer(
          mainWindow, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
      textures({}) {
  if (mainWindow == nullptr) {
    std::cout << SDL_GetError();
    SDL_Quit();
    std::exit(1);
  }

  if (mainWindowRenderer == nullptr) {
    std::cout << SDL_GetError();
    SDL_DestroyWindow(mainWindow);
    SDL_Quit();
    std::exit(1);
  }

  return;
}

game::~game() {
  SDL_DestroyRenderer(mainWindowRenderer);
  SDL_DestroyWindow(mainWindow);
  for (SDL_Texture* texture : textures) {
    SDL_DestroyTexture(texture);
  }

  return;
}

void game::update() {
  SDL_Event event;
  while (SDL_WaitEventTimeout(&event, 1000)) {
    switch (event.type) {
      case SDL_QUIT:
        this->quit = true;
        break;

      default:
        break;
    }
  }

  std::cout << "Updated"; 
  

  return;
}
