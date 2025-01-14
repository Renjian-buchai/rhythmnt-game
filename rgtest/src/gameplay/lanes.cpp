#include "../../include/gameplay/lanes.h"

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
      nextSpawnTiming({-1}),
      nextNote() {
  parse(chartfile, laneTimings[0], laneTimings[1], laneTimings[2],
        laneTimings[3], timingGroups, movementGroups);

  for (auto it = laneTimings.begin(); it != laneTimings.end(); ++it) {
    size_t index = it - laneTimings.begin();

    // What is this atrocity
    nextSpawnTiming[index] =
        it->size() != 0
            ? std::visit([](auto&& nxt) -> int64_t { return nxt.timing; },
                         it->front()) -
                  1000
            // Very safe to say that these notes will never be spawned
            : -1000000;

    nextNote[index] = it->begin();
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

#include <iostream>

void lanes::spawn(int64_t musicTimeStart, int64_t musicTimeCurrent,
                  int spawnLocation, SDL_Rect& screen) {
  for (uint8_t i = 0; i < 4; ++i) {
    if (this->nextSpawnTiming[i] > musicTimeStart &&
        this->nextSpawnTiming[i] < musicTimeCurrent &&
        this->nextNote[i] != this->laneTimings[i].end()) {
      this->laneQueues[i].push_back(SDL_Rect{
          positions[i].x, spawnLocation, positions[i].w, positions[i].w / 3});
      ++this->nextNote[i];

      if (this->nextNote[i] != this->laneTimings[i].end()) {
        this->nextSpawnTiming[i] =
            std::visit([](auto&& nxt) -> uint64_t { return nxt.timing; },
                       *this->nextNote[i]) -
            1000;
      }
    }
  }

  return;
}
