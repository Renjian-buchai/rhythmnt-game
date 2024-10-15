#pragma once

#include <SDL.h>

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include "chartParser.h"

struct lanes {
  const std::array<SDL_Colour, 4> laneColours;
  std::array<std::vector<noteObject>, 4> laneTimings;
  std::array<std::vector<SDL_Rect>, 4> laneQueues;
  std::vector<timer> timingGroups;
  std::vector<movement> movementGroups;
  std::array<SDL_Rect, 4> positions;

  lanes(std::string chartfile, SDL_Rect gameplayScreen, const int blockWidth)
      : laneColours({SDL_Colour{0xac, 0xa9, 0xbb, SDL_ALPHA_OPAQUE},
                     SDL_Colour{0x77, 0x75, 0x86, SDL_ALPHA_OPAQUE},
                     SDL_Colour{0x34, 0x18, 0x18, SDL_ALPHA_OPAQUE},
                     SDL_Colour{0x64, 0x44, 0x42, SDL_ALPHA_OPAQUE}}),
        laneQueues({}),
        positions({SDL_Rect{gameplayScreen.x + blockWidth * 2, 0, blockWidth,
                            gameplayScreen.h},
                   SDL_Rect{gameplayScreen.x + blockWidth * 3, 0, blockWidth,
                            gameplayScreen.h},
                   SDL_Rect{gameplayScreen.x + blockWidth * 4, 0, blockWidth,
                            gameplayScreen.h},
                   SDL_Rect{gameplayScreen.x + blockWidth * 5, 0, blockWidth,
                            gameplayScreen.h}}) {
    parse(chartfile, laneTimings[0], laneTimings[1], laneTimings[2],
          laneTimings[3], timingGroups, movementGroups);

    std::cout << "\n" << gameplayScreen.x + blockWidth * 2 << " " << blockWidth;
  }

  void render(SDL_Renderer* renderer) const {
    for (int i = 0; i < 4; ++i) {
      SDL_SetRenderDrawColor(renderer, laneColours[i].r, laneColours[i].g,
                             laneColours[i].b, laneColours[i].a);
      SDL_RenderDrawRect(renderer, &(this->positions[i]));

      std::cout << SDL_GetError();
    }
  }
};
