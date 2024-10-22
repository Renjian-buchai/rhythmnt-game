#pragma once

#include <SDL.h>

#include <array>
#include <string>
#include <vector>

#include "chartParser.h"

struct lanes {
  const std::array<SDL_Colour, 4> laneColours;
  std::array<std::vector<SDL_Rect>, 4> laneQueues;
  std::array<std::vector<noteObject>, 4> laneTimings;
  std::vector<timer> timingGroups;
  std::vector<movement> movementGroups;
  std::array<SDL_Rect, 4> positions;

  // Logic
  std::array<int64_t, 4> nextSpawn; 

  lanes(std::string chartfile, SDL_Rect gameplayScreen, const int blockWidth);

  void render(SDL_Renderer* renderer) const;
};
