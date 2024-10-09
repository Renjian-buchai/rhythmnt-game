#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <variant>
#include <vector>

struct note {
  uint64_t timing = -1;
  uint8_t lane = -1;

  note(uint8_t lane, uint64_t timing);
  note() = default; 
};

struct longNote {
  uint64_t timing = -1;
  uint64_t deltaTime = -1;
  uint8_t lane = -1;

  longNote(uint8_t lane, uint64_t timing, uint64_t deltaTime);
  longNote() = default; 
};

struct timer {
  uint64_t timing = -1;
  uint32_t bpm = -1;
  
  timer(uint32_t bpm, uint64_t timing);
  timer() = default; 
};

struct movement {
  uint64_t timing = -1;
  uint64_t deltaTime = -1;
  uint8_t lane = -1;
  uint8_t position = -1;

  movement(uint8_t lane, uint8_t position, uint64_t timing, uint64_t deltaTime); 
  movement() = default; 
};

using noteObject = std::variant<note, longNote, timer, movement>;

std::vector<noteObject> parse(std::string chartFile);
