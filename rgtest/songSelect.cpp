#include "game.h"

void game::songSelect() {
  SDL_Rect songListRect = {2, 2, screen.w / 3, screen.h / 10};
  SDL_Rect songListBorderRect = {0, 0, screen.w / 3 + 4, screen.h / 10 + 4};

  SDL_Colour textColour;
  textColour.a = SDL_ALPHA_OPAQUE;
  textColour.r = 0x00;
  textColour.g = 0x00;
  textColour.b = 0x00;

  SDL_Surface* songTitle =
      TTF_RenderText_Blended(font, "PRIMITIVE LIGHTS", textColour);

  textures.push_back(
      SDL_CreateTextureFromSurface(mainWindowRenderer, songTitle));

  while (true) {
    SDL_RenderClear(mainWindowRenderer);
    // Background
    SDL_SetRenderDrawColor(mainWindowRenderer, 0x1f, 0x1e, 0x33,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(mainWindowRenderer, &screen);

    SDL_SetRenderDrawColor(mainWindowRenderer, 0xff, 0xff, 0xff,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(mainWindowRenderer, &songListBorderRect);

    SDL_SetRenderDrawColor(mainWindowRenderer, 0x3d, 0x3b, 0x64,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(mainWindowRenderer, &songListRect);
    SDL_RenderCopy(mainWindowRenderer, textures[0], nullptr, &songListRect);
    SDL_RenderPresent(mainWindowRenderer);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          this->state = gameState::QUIT;
          goto stateChange;

        default:
          break;
      }
    }
  }

stateChange:
  return;
}
