#pragma warning(push)
#pragma warning( \
    disable : 26812 26819 4081 4820 4514 26819 26495 5039 4668 4365 4820 4710)
#include <SDL.h>
#include <bass.h>

#include <iostream>
#pragma warning(pop)

#include "game.h"

SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainWindowRenderer;

int main(int argc [[maybe_unused]], char** argv [[maybe_unused]]) {
  if (int err = SDL_Init(SDL_INIT_EVERYTHING)) {
    std::cout << SDL_GetError();
    return err;
  }

  SDL_Rect screen;
  if (int err = SDL_GetDisplayBounds(0, &screen)) {
    std::cout << SDL_GetError();
    SDL_Quit();
    return err;
  }

  game rg(screen);

  SDL_Renderer*& renderer = rg.mainWindowRenderer;

  SDL_SetRenderDrawColor(renderer, 15, 20, 25, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(renderer, &screen);
  SDL_RenderPresent(renderer);

  while (!rg.quit) {
    rg.update();
  }

  SDL_Quit();

  return 0;
}
