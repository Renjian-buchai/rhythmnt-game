#include "chartParser.h"

#include <algorithm>
#include <iostream>
#include <sstream>

bool operator<(noteObject&& a, noteObject&& b) {
  bool ret;
  std::visit([&](auto&& thing1,
                 auto&& thing2) { ret = thing1.timing < thing2.timing; },
             a, b);
  return ret;
}

// TODO update parser

void parse(std::string chartFile,
 std::vector<noteObject>& Lane1,
           std::vector<noteObject>& Lane2,
 std::vector<noteObject>& Lane3,
           std::vector<noteObject>& Lane4, std::vector<timer>& timePoints, std::vector<movement>& movements) {
  std::vector<noteObject> noteObjects;

  std::ifstream input(chartFile);
  std::string buffer;

  while (std::getline(input, buffer) && buffer != "|||---|||") {
  }

  while (std::getline(input, buffer)) {
    std::stringstream inputStream(buffer);

    // Identifier
    inputStream >> buffer;
    switch (buffer[0]) {
      case 'N': {
        // Timing
        inputStream >> buffer;
        note note(std::stoull(buffer));

        inputStream >> buffer;
        if (buffer.back() != ';') {
          throw "lineTerminationError";
        }
        switch (buffer[0] - 48) {
          case 1:
            Lane1.push_back(note);
            break;

          case 2:
            Lane2.push_back(note);
            break;

          case 3:
            Lane3.push_back(note);
            break;

          case 4:
            Lane4.push_back(note);
            break;

          default:
            throw "noteLaneError";
            break;
        }
        break;
      }

      case 'L': {
        longNote ln;
        // Timing
        inputStream >> buffer;
        ln.timing = std::stoull(buffer);

        // DeltaTime
        inputStream >> buffer;
        std::cout << buffer;
        ln.deltaTime = std::stoull(buffer);

        inputStream >> buffer;
        if (buffer.back() != ';') {
          throw "Wtf";
        }
        switch (buffer[0] - 48) {
          case 1:
            Lane1.push_back(ln);
            break;

          case 2:
            Lane2.push_back(ln);
            break;

          case 3:
            Lane3.push_back(ln);
            break;

          case 4:
            Lane4.push_back(ln);
            break;

          default:
            throw "Wtf 2";
            break;
        }
        break;
      }

      case 'T':
        break;

      case 'M':
        break;

      default:
        break;
    }
  }

  input.close();

  auto cmp = [](noteObject& a, noteObject& b) { return a < b; };

  std::sort(Lane1.begin(), Lane1.end(), cmp);
  std::sort(Lane2.begin(), Lane2.end(), cmp);
  std::sort(Lane3.begin(), Lane3.end(), cmp);
  std::sort(Lane4.begin(), Lane4.end(), cmp);
  std::sort(timePoints.begin(), timePoints.end(),
            [](timer& a, timer& b) { return a.timing < b.timing; });
  std::sort(movements.begin(), movements.end(),
            [](movement& a, movement& b) { return a.timing < b.timing; });
}

note::note(int64_t _timing) : timing(_timing) {}

longNote::longNote(int64_t _timing, int64_t _deltaTime)
    : timing(_timing), deltaTime(_deltaTime) {}

timer::timer(uint32_t _bpm, int64_t _timing) : bpm(_bpm), timing(_timing) {}

movement::movement(uint8_t _position, int64_t _timing, int64_t _deltaTime, uint8_t _lane)
    : position(_position), timing(_timing), deltaTime(_deltaTime), lane(_lane) {}
