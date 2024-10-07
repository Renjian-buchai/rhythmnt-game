#pragma warning(push)
#pragma warning( \
    disable : 26812 26819 4081 4820 4514 26819 26495 5039 4668 4365 4820 4710)
#include <SDL.h>
#include <SDL_ttf.h>
#include <bass.h>

#include <iostream>
#pragma warning(pop)

#include "game.h"

SDL_Window* mainWindow = nullptr;
SDL_Renderer* mainWindowRenderer;

int main(int argc [[maybe_unused]], char** argv [[maybe_unused]]) {
  if (int err = SDL_Init(SDL_INIT_EVERYTHING)) {
    std::cout << SDL_GetError();
    return err;
  }
  atexit(SDL_Quit); 

  if (!BASS_Init(-1, 22050, BASS_DEVICE_STEREO, 0, NULL)) {
    std::cout << BASS_ErrorGetCode();
    return BASS_ErrorGetCode();
  }
  atexit(reinterpret_cast<void(__cdecl*)()>(BASS_Free)); 

  if (int err = TTF_Init()) {
    std::cout << TTF_GetError();
    return err;
  }
  atexit(TTF_Quit); 

  SDL_Rect screen;
  if (int err = SDL_GetDisplayBounds(0, &screen)) {
    std::cout << SDL_GetError();
    return err;
  }

  game rg(screen);

  SDL_Renderer*& renderer = rg.mainWindowRenderer;

  HSTREAM audioStream = BASS_StreamCreateFile(
      false,
      "C:/Users/tyiwe/Music/Arcaea Sound Collection - Memories of Serenity/"
      "24 PRIMITIVE LIGHTS (Long Ver.).mp3",
      0, 0, BASS_STREAM_PRESCAN);

  BASS_ChannelPlay(audioStream, false);

  while (!rg.quit) {
    rg.update();
  }

  return 0;
}
