#include "AudioManager.h"

//Mix_Chunk* AudioManager::catchSound = nullptr;
//Mix_Chunk* AudioManager::missSound = nullptr;
//Mix_Chunk* AudioManager::gameOverSound = nullptr;

bool AudioManager::init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return false;

//    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        return false;

    return true;
}

void AudioManager::loadSounds() {
//    catchSound = Mix_LoadWAV("sounds/catch.wav");
  //  missSound = Mix_LoadWAV("sounds/miss.wav");
    //gameOverSound = Mix_LoadWAV("sounds/gameover.wav");
}

void AudioManager::playCatch() {
//    Mix_PlayChannel(-1, catchSound, 0);
}

void AudioManager::playMiss() {
//    Mix_PlayChannel(-1, missSound, 0);
}

void AudioManager::playGameOver() {
//    Mix_PlayChannel(-1, gameOverSound, 0);
}

void AudioManager::cleanup() {
//    Mix_FreeChunk(catchSound);
//    Mix_FreeChunk(missSound);
//    Mix_FreeChunk(gameOverSound);
//    Mix_CloseAudio();
    SDL_Quit();
}
