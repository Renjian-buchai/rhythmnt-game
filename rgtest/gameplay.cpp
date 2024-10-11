#include <SDL_opengl.h>

#include <array>

#include "chartParser.h"
#include "conductor.h"
#include "game.h"

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

  // Deserialising charts
  std::vector<noteObject> lane1Timings;
  std::vector<noteObject> lane2Timings;
  std::vector<noteObject> lane3Timings;
  std::vector<noteObject> lane4Timings;
  std::vector<movement> movementGroups;
  std::vector<timer> timingGroups;
  parse("./Charts/test/test.sl", lane1Timings, lane2Timings, lane3Timings,
        lane4Timings, timingGroups, movementGroups);

  // Next note info
  std::vector<SDL_Rect> lane1q;
  auto lane1Next = lane1Timings.begin();
  int64_t lane1NextTime =
      std::visit([](auto&& nxt) -> int64_t { return nxt.timing; }, *lane1Next) -
      1000;

  // Rendering
  const static std::array<SDL_Colour, 4> colours{
      SDL_Colour{0xac, 0xa9, 0xbb, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x77, 0x75, 0x86, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x34, 0x18, 0x18, SDL_ALPHA_OPAQUE},
      SDL_Colour{0x64, 0x44, 0x42, SDL_ALPHA_OPAQUE}};
  SDL_Colour backgroundColour = {0x1f, 0x1e, 0x33, SDL_ALPHA_OPAQUE};

  const static std::array<SDL_Rect, 4> lanes{
      SDL_Rect{gameplayScreen.x + blockWidth * 2, 0, blockWidth,
               gameplayScreen.h},
      SDL_Rect{gameplayScreen.x + blockWidth * 3, 0, blockWidth,
               gameplayScreen.h},
      SDL_Rect{gameplayScreen.x + blockWidth * 4, 0, blockWidth,
               gameplayScreen.h},
      SDL_Rect{gameplayScreen.x + blockWidth * 5, 0, blockWidth,
               gameplayScreen.h}};

  // High precision Timekeeping
  // If deltatime is >16 bits, we're fucked anyways
  uint64_t timeStart = SDL_GetTicks64(), timeEnd;
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
      for (uint8_t i = 1; i < blockCount; ++i) {
        SDL_RenderDrawLine(mainWindowRenderer,
                           gameplayScreen.x + blockWidth * i, 0,
                           gameplayScreen.x + blockWidth * i, gameplayScreen.h);
      }

      SDL_RenderDrawLine(mainWindowRenderer, gameplayScreen.x + blockWidth,
                         gameplayScreen.h - blockWidth,
                         gameplayScreen.x + (blockCount - 1) * blockWidth,
                         gameplayScreen.h - blockWidth);

      for (auto it = lane1q.begin(); it != lane1q.end(); ++it) {
        SDL_RenderFillRect(mainWindowRenderer, &(*it));
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
    std::cout << musicTimeEnd << "\n";
    if (lane1NextTime > musicTimeStart && lane1NextTime < musicTimeEnd) {
      lane1q.push_back(SDL_Rect{gameplayScreen.x + blockWidth * 2,
                                spawnLocation, blockWidth, blockWidth / 3});
      lane1NextTime =
          std::visit([](auto&& nxt) -> uint64_t { return nxt.timing; },
                     *lane1Next) -
          1000;
      ++lane1Next;

      std::cout << "Summoned\n";
    }
    musicTimeStart = musicTimeEnd;

    for (size_t i = 0; i < lane1q.size();) {
      std::cout << "Moving, " << lane1q[i].y;
      lane1q[i].y += step * deltaTime / 6;
      std::cout << " To " << lane1q[i].y;

      if (lane1q[i].y >= gameplayScreen.h) {
        lane1q.erase(lane1q.begin() + i);
      } else {
        ++i;
      }
    }

#if 0  // To check time from top to bottom 
    if (note.y > gameplayScreen.h) {
      note.y = spawnLocation;
      static uint64_t restartTime = 0;
      static uint64_t lastRestartTime = restartTime;
      restartTime = SDL_GetTicks64();

      std::cout << restartTime - lastRestartTime << " " << restartTime << "\n";
    }
#endif
  }

stateChange:
  return;
}
