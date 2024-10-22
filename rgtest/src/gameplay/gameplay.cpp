#include <SDL.h>

#include <algorithm>
#include <array>

#include "../../include/game.h"
#include "../../include/gameplay/conductor.h"
#include "../../include/gameplay/lanes.h"

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
  int64_t musicTimeCurrent;

  // Sample note
  SDL_Rect note{gameplayScreen.x + blockWidth * 2, spawnLocation, blockWidth,
                blockWidth / 3};

  while (true) {
    SDL_SetRenderDrawColor(mainWindowRenderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderClear(mainWindowRenderer);

    /* Rendering logic */ {
      // Background
      SDL_SetRenderDrawColor(mainWindowRenderer, backgroundColour.r,
                             backgroundColour.g, backgroundColour.b,
                             backgroundColour.a);
      SDL_RenderFillRect(mainWindowRenderer, &screen);

      // Lanes
      lanes.render(mainWindowRenderer);

      // Lane borders
      SDL_SetRenderDrawColor(mainWindowRenderer, 0xff, 0xff, 0xff,
                             SDL_ALPHA_OPAQUE);
      for (uint8_t i = 1; i < blockCount; ++i) {
        SDL_RenderDrawLine(mainWindowRenderer,
                           gameplayScreen.x + blockWidth * i, 0,
                           gameplayScreen.x + blockWidth * i, gameplayScreen.h);
      }

      // Judgement line
      SDL_SetRenderDrawColor(mainWindowRenderer, 0xff, 0xff, 0xff,
                             SDL_ALPHA_OPAQUE);
      SDL_RenderDrawLine(mainWindowRenderer, gameplayScreen.x + blockWidth,
                         gameplayScreen.h - blockWidth,
                         gameplayScreen.x + (blockCount - 1) * blockWidth,
                         gameplayScreen.h - blockWidth);

      // Notes (Currently lane 1 only)
      for (size_t i = 0; i < lanes.laneQueues[0].size(); ++i) {
        SDL_RenderFillRect(mainWindowRenderer, &(lanes.laneQueues[0][i]));
      }
    }

    SDL_RenderPresent(mainWindowRenderer);

    /* Event Processing */
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          state = gameState::QUIT;
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

    musicTimeCurrent = music.time();

    // Spawning
    lanes.spawn(musicTimeStart, musicTimeCurrent, spawnLocation, gameplayScreen);

    musicTimeStart = musicTimeCurrent;

    for (size_t i = 0; i < lanes.laneQueues[0].size();) {
      lanes.laneQueues[0][i].y += gameplayScreen.h / noteSpeed * deltaTime / 6;
      if (lanes.laneQueues[0][i].y >= gameplayScreen.h) {
        lanes.laneQueues[0].erase(lanes.laneQueues[0].begin() + i);
      } else {
        ++i;
      }
    }
  }

stateChange:
  return;
}
