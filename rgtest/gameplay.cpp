#include <SDL_opengl.h>

#include <array>

#include "conductor.h"
#include "game.h"
#include "lanes.h"

inline constexpr uint8_t blockCount = 14;

void game::update() {
  // Game logic initialisation
  uint16_t refreshRate;
  {
    SDL_DisplayMode display;
    SDL_GetWindowDisplayMode(mainWindow, &display);
    refreshRate = display.refresh_rate;
  }

  const int blockWidth = gameplayScreen.w / blockCount;
  int step = gameplayScreen.h / noteSpeed;
  const int spawnLocation = gameplayScreen.h -
                            gameplayScreen.h * refreshRate / noteSpeed +
                            blockWidth / 3;

  SDL_Event event;

  // Initialising lanes
  lanes lanes("./Charts/test/test.sl", gameplayScreen, blockWidth);

  // Next note info
  std::vector<SDL_Rect> lane1q;
  auto lane1Next = lanes.laneTimings[0].begin();
  int64_t lane1NextTime =
      std::visit([](auto&& nxt) -> int64_t { return nxt.timing; }, *lane1Next) -
      1000;

  // Rendering
  SDL_Colour backgroundColour = {0x1f, 0x1e, 0x33, SDL_ALPHA_OPAQUE};

  // High precision Timekeeping
  // If deltatime is >16 bits, we're fucked anyways
  uint64_t timeStart = SDL_GetTicks64(), timeEnd = SDL_GetTicks64();
  uint16_t deltaTime;

  // Music timekeeping
  static HSTREAM song =
      BASS_StreamCreateFile(0, "./Charts/test/test.ogg", 0, 0,
                            BASS_MUSIC_PRESCAN | BASS_MUSIC_AUTOFREE);

  conductor music(song);

  int64_t musicTimeStart = -3000;
  int64_t musicTimeEnd;

  // Sample note
  SDL_Rect note{gameplayScreen.x + blockWidth * 2, spawnLocation, blockWidth,
                blockWidth / 3};

  while (true) {
    /* Rendering */ {
      SDL_SetRenderDrawColor(mainWindowRenderer, 0xff, 0x00, 0x00, 0xff); 
      SDL_RenderClear(mainWindowRenderer);
      SDL_SetRenderDrawColor(mainWindowRenderer, backgroundColour.r,
                             backgroundColour.g, backgroundColour.b,
                             backgroundColour.a);
      SDL_RenderFillRect(mainWindowRenderer, &screen);

      lanes.render(mainWindowRenderer);

      SDL_SetRenderDrawColor(mainWindowRenderer, 0xff, 0xff, 0xff,
                             SDL_ALPHA_OPAQUE);
      for (uint8_t i = 1; i < blockCount; ++i) {
        SDL_RenderDrawLine(mainWindowRenderer,
                           gameplayScreen.x + blockWidth * i, 0,
                           gameplayScreen.x + blockWidth * i, gameplayScreen.h);
      }

      SDL_RenderDrawLine(mainWindowRenderer, gameplayScreen.x + blockWidth,
                         gameplayScreen.h - blockWidth,
                         gameplayScreen.x + (blockCount - 1) * blockWidth,
                         gameplayScreen.h - blockWidth);

      for (size_t i = 0; i < lane1q.size(); ++i) {
        SDL_RenderFillRect(mainWindowRenderer, &(lane1q[i]));
      }

      SDL_RenderPresent(mainWindowRenderer);
    }

    /* Event Processing */
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          this->state = gameState::QUIT;
          goto stateChange;

        default:
          break;
      }
    }

    /* Timekeeping */
    timeEnd = SDL_GetTicks64();
    deltaTime = static_cast<uint16_t>(timeEnd - timeStart);
    timeStart = timeEnd;

    music.Update();

    musicTimeEnd = music.time();

    if (lane1NextTime > musicTimeStart && lane1NextTime < musicTimeEnd &&
        lane1Next != lanes.laneTimings[0].end()) {
      lane1q.push_back(SDL_Rect{gameplayScreen.x + blockWidth * 2,
                                spawnLocation, blockWidth, blockWidth / 3});
      ++lane1Next;
      if (lane1Next == lanes.laneTimings[0].end()) {
        continue;
      }
      lane1NextTime =
          std::visit([](auto&& nxt) -> uint64_t { return nxt.timing; },
                     *lane1Next) -
          1000;
    }
    musicTimeStart = musicTimeEnd;

    for (size_t i = 0; i < lane1q.size();) {
      lane1q[i].y += gameplayScreen.h / noteSpeed * deltaTime / 6;
      if (lane1q[i].y >= gameplayScreen.h) {
        lane1q.erase(lane1q.begin() + i);
      } else {
        ++i;
      }
    }
  }

stateChange:
  return;
}
