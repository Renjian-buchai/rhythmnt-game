#include <array>

#include "chartParser.h"
#include "game.h"

void game::update() {
  std::vector<noteObject> objects = parse("Charts/test/test.sl");

  std::array<SDL_Colour, 4> colours{
      SDL_Colour{0xac, 0xa9, 0xbb, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x77, 0x75, 0x86, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x34, 0x18, 0x18, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x64, 0x44, 0x42, SDL_ALPHA_OPAQUE}};
  SDL_Colour backgroundColour = {0x1f, 0x1e, 0x33, SDL_ALPHA_OPAQUE};

  int blockWidth = this->screen.w / 16;

  std::array<SDL_Rect, 4> lanes{
      SDL_Rect{blockWidth * 6, 0, blockWidth, screen.h},
      SDL_Rect{blockWidth * 7, 0, blockWidth, screen.h},
      SDL_Rect{blockWidth * 8, 0, blockWidth, screen.h},
      SDL_Rect{blockWidth * 9, 0, blockWidth, screen.h}};

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
    SDL_RenderFillRect(mainWindowRenderer, &note);

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
