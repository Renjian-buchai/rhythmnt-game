#include "game.h"

game::game(SDL_Rect screen)
    : state(gameState::GAMEPLAY),
      noteSpeed(100),
      mainWindow(SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen.w, screen.h,
                                  SDL_WINDOW_BORDERLESS)),
      mainWindowRenderer(SDL_CreateRenderer(
          mainWindow, -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
      textures({}),
      screen(screen),
      font(TTF_OpenFont("./Resources/FiraCode-Regular.ttf", 30)),
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

  if (int err = SDL_RenderSetVSync(mainWindowRenderer, 1)) {
    std::cout << SDL_GetError();
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainWindowRenderer);
    std::exit(err);
  }

  if (font == nullptr) {
    std::cout << TTF_GetError();
    SDL_DestroyWindow(mainWindow);
    SDL_DestroyRenderer(mainWindowRenderer);
    std::exit(1);
  }

  aspectRatio = static_cast<double>(screen.w) / static_cast<double>(screen.h);

  if (aspectRatio * 10 > 16) {
    gameplayScreen.h = screen.h;
    gameplayScreen.w = static_cast<int>(screen.h * aspectRatio);
  } else {
    gameplayScreen.h = static_cast<int>(screen.w / aspectRatio);
    gameplayScreen.w = screen.w;
  }
  gameplayScreen.x = (screen.w - gameplayScreen.w) / 2;
  gameplayScreen.y = (screen.h - gameplayScreen.h) / 2;

  return;
}

game::~game() {
  SDL_DestroyRenderer(mainWindowRenderer);
  SDL_DestroyWindow(mainWindow);
  for (SDL_Texture* texture : textures) {
    SDL_DestroyTexture(texture);
  }
  TTF_CloseFont(font);

  return;
}
