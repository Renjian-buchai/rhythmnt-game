#pragma once

#include <bass.h>

#include <cstdint>

struct conductor {
  double mixerTime = 0;
  double startTime = 0;
  double currentTime = 0;
  HSTREAM handle;

  conductor(HSTREAM handle);

  void Update();
  void Pause();
  void Resume();

  int64_t time();
};
