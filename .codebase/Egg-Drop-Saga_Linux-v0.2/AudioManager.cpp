#include "AudioManager.h"
#include <iostream>
#include <unistd.h>

// Music
static Mix_Music* currentMusic = nullptr;
Mix_Music* AudioManager::homeMusic = nullptr;
Mix_Music* AudioManager::gameMusic = nullptr;
Mix_Music* AudioManager::gameOverMusic = nullptr;

// SFX
Mix_Chunk* AudioManager::chickenDrop = nullptr;
Mix_Chunk* AudioManager::catchSound = nullptr;
Mix_Chunk* AudioManager::eggBreak = nullptr;
Mix_Chunk* AudioManager::lifeLost = nullptr;

bool AudioManager::init() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    Mix_AllocateChannels(16);

    //  Music lower
    Mix_VolumeMusic(50);   // 0–128

    std::cout << "SDL Audio Init Success\n";
    return true;
}

void AudioManager::load()
{
    const char* appDir = getenv("APPDIR");

    std::string basePath;

    if (appDir)
        basePath = std::string(appDir) + "/usr/bin/";
    else
        basePath = SDL_GetBasePath();  // fallback for normal run

    homeMusic = Mix_LoadMUS((basePath + "sounds/home_music.mp3").c_str());
    if (!homeMusic)
        std::cout << "Home music load error: " << Mix_GetError() << std::endl;

    gameMusic = Mix_LoadMUS((basePath + "sounds/gamemusic.mp3").c_str());
    gameOverMusic = Mix_LoadMUS((basePath + "sounds/gameover_music.mp3").c_str());

    chickenDrop = Mix_LoadWAV((basePath + "sounds/chickenpokpok.wav").c_str());
    catchSound = Mix_LoadWAV((basePath + "sounds/bucket-catch.wav").c_str());
    eggBreak = Mix_LoadWAV((basePath + "sounds/egg-broke.wav").c_str());
    lifeLost = Mix_LoadWAV((basePath + "sounds/life_lost.wav").c_str());

    // SFX louder than music
    Mix_VolumeChunk(chickenDrop, 100);
    Mix_VolumeChunk(catchSound, 110);
    Mix_VolumeChunk(eggBreak, 115);
    Mix_VolumeChunk(lifeLost, 100);
}

// -------- MUSIC --------

void AudioManager::playMusic(Mix_Music* music, int loops)
{
    if (currentMusic == music) return;  // already playing

    Mix_FadeOutMusic(500);
    Mix_PlayMusic(music, loops);

    Mix_PlayMusic(music, loops);
    currentMusic = music;
}

void AudioManager::playHomeMusic()
{
    playMusic(homeMusic, -1);
}

void AudioManager::playGameMusic()
{
    playMusic(gameMusic, -1);
}

void AudioManager::playGameOverMusic()
{
    playMusic(gameOverMusic, 0);
}

void AudioManager::stopMusic() {
    Mix_HaltMusic();
}

// -------- SFX --------

void AudioManager::playChickenDrop() {
    Mix_PlayChannel(-1, chickenDrop, 0);
}

void AudioManager::playCatch() {
    Mix_PlayChannel(-1, catchSound, 0);
}

void AudioManager::playEggBreak() {
    Mix_PlayChannel(-1, eggBreak, 0);
}

void AudioManager::playLifeLost() {
    Mix_PlayChannel(-1, lifeLost, 0);
}

void AudioManager::toggleMusic()
{
    if (Mix_PlayingMusic())
    {
        if (Mix_PausedMusic())
        {
            Mix_ResumeMusic();
            std::cout << "Music Resumed\n";
        }
        else
        {
            Mix_PauseMusic();
            std::cout << "Music Paused\n";
        }
    }
}

void AudioManager::cleanup() {
    Mix_FreeMusic(homeMusic);
    Mix_FreeMusic(gameMusic);
    Mix_FreeMusic(gameOverMusic);

    Mix_FreeChunk(chickenDrop);
    Mix_FreeChunk(catchSound);
    Mix_FreeChunk(eggBreak);
    Mix_FreeChunk(lifeLost);

    Mix_CloseAudio();
    SDL_Quit();
}
