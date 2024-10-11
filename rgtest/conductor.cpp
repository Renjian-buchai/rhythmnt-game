#include "conductor.h"

#include <iostream>

conductor::conductor(HSTREAM handle)
    : mixerTime(BASS_ChannelBytes2Seconds(
          handle, BASS_ChannelGetPosition(handle, BASS_POS_BYTE))),
      startTime(mixerTime + 3),
      currentTime(mixerTime - startTime),
      handle(handle) {
  BASS_ChannelSetAttribute(handle, BASS_ATTRIB_VOL, 0.0f);
  BASS_ChannelPlay(handle, false);
}

void conductor::Update() {
  currentTime = mixerTime - startTime;
  static bool playing = false;

  if (currentTime >= 0) {
    if (playing == false) {
      startTime -= 3;
      playing = true;
      BASS_ChannelSetAttribute(handle, BASS_ATTRIB_VOL, 1.0f);
      BASS_ChannelSetPosition(handle, BASS_ChannelSeconds2Bytes(handle, 0.0f),
                              BASS_POS_BYTE);
    }
  }

  mixerTime = BASS_ChannelBytes2Seconds(
      handle, BASS_ChannelGetPosition(handle, BASS_POS_BYTE));
}

void conductor::Pause() { BASS_ChannelPause(handle); }

void conductor::Resume() { BASS_ChannelPlay(handle, false); }

int64_t conductor::time() { return static_cast<int64_t>(currentTime * 1000); }
