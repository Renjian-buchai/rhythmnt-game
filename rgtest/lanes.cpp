#include "lanes.h"

lanes::lanes(std::string chartfile, SDL_Rect gameplayScreen,
             const int blockWidth)
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
                          gameplayScreen.h}}),
      nextSpawn({-1}) {
  parse(chartfile, laneTimings[0], laneTimings[1], laneTimings[2],
        laneTimings[3], timingGroups, movementGroups);

  for (auto it = laneTimings.begin(); it != laneTimings.end(); ++it) {
    // What is this atrocity
    nextSpawn[it - laneTimings.begin()] =
        it->size() != 0
            ? std::visit([](auto&& nxt) -> int64_t { return nxt.timing; },
                         it->front()) -
                  1000
            // Very safe to say that these notes will never be spawned
            : -1000000;
  }

  return;
}

void lanes::render(SDL_Renderer* renderer) const {
  for (int i = 0; i < 4; ++i) {
    SDL_SetRenderDrawColor(renderer, this->laneColours[i].r,
                           this->laneColours[i].g, this->laneColours[i].b,
                           this->laneColours[i].a);
    SDL_RenderFillRect(renderer, &(this->positions[i]));
  }
}
