#include "chartParser.h"

#include <sstream>

std::vector<noteObject> parse(std::string chartFile) {
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
        note note(0, 0);
        // Lane
        inputStream >> buffer;
        // Assuming input is valid, only the first character is important
        note.lane = buffer[0] - 48;
        if (note.lane > 4) {
          throw "Wtf";
        }

        // Timing
        inputStream >> buffer;
        if (buffer.back() != ';') {
          throw "Wtf";
        }
        note.timing = std::stoull(buffer);

        noteObjects.push_back(note);
        break;
      }

      case 'L': {
        longNote ln;
        // Lane
        inputStream >> buffer;
        ln.lane = buffer[0] - 48;
        if (ln.lane > 4) {
          throw "Wtf 2";
        }

        // Timing
        inputStream >> buffer;
        ln.timing = std::stoull(buffer);

        // DeltaTime
        inputStream >> buffer;
        if (buffer.back() != ';') {
          throw "Wtf";
        }
        buffer.pop_back();
        ln.deltaTime = std::stoull(buffer);

        noteObjects.push_back(ln);
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
  return noteObjects;
}

note::note(uint8_t _lane, uint64_t _timing) : lane(_lane), timing(_timing) {}

longNote::longNote(uint8_t _lane, uint64_t _timing, uint64_t _deltaTime)
    : lane(_lane), timing(_timing), deltaTime(_deltaTime) {}

timer::timer(uint32_t _bpm, uint64_t _timing) : bpm(_bpm), timing(_timing) {}

movement::movement(uint8_t _lane, uint8_t _position, uint64_t _timing,
                   uint64_t _deltaTime)
    : lane(_lane),
      position(_position),
      timing(_timing),
      deltaTime(_deltaTime) {}
