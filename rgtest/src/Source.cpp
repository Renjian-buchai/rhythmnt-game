#include <SDL.h>
#include <SDL_ttf.h>
#include <bass.h>

#include <iostream>

#include "../../include/game.h"

SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainWindowRenderer;

int main(int argc [[maybe_unused]], char** argv [[maybe_unused]]) {
  if (int err = SDL_Init(SDL_INIT_EVERYTHING)) {
    std::cout << SDL_GetError();
    return err;
  }
  atexit(SDL_Quit);

  if (!BASS_Init(-1, 22050, BASS_DEVICE_STEREO, 0, NULL)) {
    std::cout << BASS_ErrorGetCode();
    return BASS_ErrorGetCode();
  }
  atexit(reinterpret_cast<void(__cdecl*)()>(BASS_Free));

  if (int err = TTF_Init()) {
    std::cout << TTF_GetError();
    return err;
  }
  atexit(TTF_Quit);

  SDL_Rect screen;
  if (int err = SDL_GetDisplayBounds(0, &screen)) {
    std::cout << SDL_GetError();
    return err;
  }

  game rg(screen);

  SDL_Renderer*& renderer = rg.mainWindowRenderer;

  while (true) {
    switch (rg.state) {
      case game::gameState::SONGSELECT:
        rg.songSelect();
        break;

      case game::gameState::GAMEPLAY:
        rg.update();
        break;

      case game::gameState::PAUSE:
        break;

      case game::gameState::QUIT:
        exit(0);

      default:
        break;
    }
  }

  return 0;
}
