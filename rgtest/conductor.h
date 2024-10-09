#pragma once

#include <bass.h>

struct conductor { 
  double mixerTime = 0; 
  double startTime = 0;
  HSTREAM handle; 

  conductor(HSTREAM handle); 

  void Update();
  void Pause();
  void Resume();
 
  double currentTime(); 
};
