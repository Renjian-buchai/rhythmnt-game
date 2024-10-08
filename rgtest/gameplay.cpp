#include <array>

#include "chartParser.h"
#include "game.h"

inline const uint8_t blockCount = 16; 

void game::update() {
  double aspectRatio =
      static_cast<double>(screen.w) / static_cast<double>(screen.h);
  SDL_Rect gameplayScreen;
  gameplayScreen.h = aspectRatio * 10 > 16 ? screen.h : screen.w * 10 / 16;
  gameplayScreen.w = aspectRatio * 10 > 16 ? screen.h * 16 / 10 : screen.w;
  gameplayScreen.x = (screen.w - gameplayScreen.w) / 2;
  gameplayScreen.y = (screen.h - gameplayScreen.h) / 2;

  const static std::array<SDL_Colour, 4> colours{
      SDL_Colour{0xac, 0xa9, 0xbb, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x77, 0x75, 0x86, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x34, 0x18, 0x18, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x64, 0x44, 0x42, SDL_ALPHA_OPAQUE}};
  SDL_Colour backgroundColour = {0x1f, 0x1e, 0x33, SDL_ALPHA_OPAQUE};

  const int blockWidth = gameplayScreen.w / blockCount;

  const static std::array<SDL_Rect, 4> lanes{
      SDL_Rect{gameplayScreen.x + blockWidth * 2, 0, blockWidth,
               gameplayScreen.h},
      SDL_Rect{gameplayScreen.x + blockWidth * 3, 0, blockWidth,
               gameplayScreen.h},
      SDL_Rect{gameplayScreen.x + blockWidth * 4, 0, blockWidth,
               gameplayScreen.h},
      SDL_Rect{gameplayScreen.x + blockWidth * 5, 0, blockWidth,
               gameplayScreen.h}};

  SDL_Rect note{lanes[0].x + blockWidth / 100, 0, blockWidth * 98 / 100,
                screen.h / 20};

  int step = screen.h / this->noteSpeed;

  while (true) {
    SDL_RenderClear(mainWindowRenderer);
    SDL_SetRenderDrawColor(mainWindowRenderer, backgroundColour.r,
                           backgroundColour.g, backgroundColour.b,
                           backgroundColour.a);
    SDL_RenderFillRect(mainWindowRenderer, &screen);


    for (uint8_t i = 0; i < 4; ++i) {
      SDL_SetRenderDrawColor(mainWindowRenderer, colours[i].r, colours[i].g,
                             colours[i].b, colours[i].a);
      SDL_RenderFillRect(mainWindowRenderer, &lanes[i]);
    }

    SDL_SetRenderDrawColor(mainWindowRenderer, 0xff, 0xff, 0xff,
                           SDL_ALPHA_OPAQUE);
    for (uint8_t i = 1; i < blockCount ; ++i) {
      SDL_RenderDrawLine(mainWindowRenderer, gameplayScreen.x + blockWidth * i, 0, gameplayScreen.x +  blockWidth * i,
                         gameplayScreen.h);
    }

    // SDL_SetRenderDrawColor(mainWindowRenderer, 0xff, 0xff, 0xff,
    //                       SDL_ALPHA_OPAQUE);
    // SDL_RenderFillRect(mainWindowRenderer, &note);

    SDL_RenderPresent(mainWindowRenderer);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          this->state = gameState::QUIT;
          goto stateChange;

        default:
          break;
      }
    }

    note.y += step;
  }

stateChange:
  return;
}
