#include "game.h"

game::game(SDL_Rect screen)
    : quit(false),
      state(gameState::SONGSELECT),
      mainWindow(SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen.w, screen.h,
                                  SDL_WINDOW_BORDERLESS)),
      mainWindowRenderer(SDL_CreateRenderer(
          mainWindow, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
      textures({}),
      screen(screen),
      font(TTF_OpenFont("./Resources/FiraCode-Regular.ttf", 5)),
      bassStreams({}) {
  if (mainWindow == nullptr) {
    std::cout << SDL_GetError();
    std::exit(1);
  }

  if (mainWindowRenderer == nullptr) {
    std::cout << SDL_GetError();
    SDL_DestroyWindow(mainWindow);
    std::exit(1);
  }

  if (font == nullptr) {
    std::cout << TTF_GetError();
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainWindowRenderer);
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

void game::songSelect() {
  SDL_RenderClear(mainWindowRenderer);
  // Background
  SDL_SetRenderDrawColor(mainWindowRenderer, 0x1f, 0x1e, 0x33,
                         SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(mainWindowRenderer, &screen);
}

void game::update() {
  SDL_SetRenderDrawColor(mainWindowRenderer, 15, 20, 25, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(mainWindowRenderer, &screen);
  SDL_RenderPresent(mainWindowRenderer);

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        this->quit = true;
        break;

      default:
        break;
    }
  }

  return;
}
