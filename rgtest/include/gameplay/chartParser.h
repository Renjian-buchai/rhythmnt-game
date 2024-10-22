#pragma once
#include <cstdint>
#include <fstream>
#include <string>
#include <variant>
#include <vector>

struct note {
  int64_t timing = -1;

  note(int64_t timing);
  note() = default;
};

struct longNote {
  int64_t timing = -1;
  int64_t deltaTime = -1;

  longNote(int64_t timing, int64_t deltaTime);
  longNote() = default;
};

struct timer {
  int64_t timing = -1;
  uint32_t bpm = -1;

  timer(uint32_t bpm, int64_t timing);
  timer() = default;
};

struct movement {
  int64_t timing = -1;
  int64_t deltaTime = -1;
  uint8_t position = -1;
  uint8_t lane = -1; 

  movement(uint8_t position, int64_t timing, int64_t deltaTime, uint8_t lane);
  movement() = default;
};

using noteObject = std::variant<note, longNote, movement>;

void parse(std::string chartFile,
 std::vector<noteObject>& Lane1,
           std::vector<noteObject>& Lane2,
 std::vector<noteObject>& Lane3,
           std::vector<noteObject>& Lane4, std::vector<timer>& timePoints, std::vector<movement>& movements);
