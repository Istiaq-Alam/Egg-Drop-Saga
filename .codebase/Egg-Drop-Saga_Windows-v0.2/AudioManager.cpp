#include "AudioManager.h"
#include <iostream>
#include <unistd.h>

// Music
static Mix_Music* currentMusic = nullptr;
Mix_Music* AudioManager::homeMusic = nullptr; //downloaded
Mix_Music* AudioManager::gameMusic = nullptr;
Mix_Music* AudioManager::gameOverMusic = nullptr; //downloaded

// SFX
Mix_Chunk* AudioManager::chickenDrop = nullptr; //need to edit
Mix_Chunk* AudioManager::catchSound = nullptr; //downloaded
Mix_Chunk* AudioManager::eggBreak = nullptr;   //downloaded
Mix_Chunk* AudioManager::lifeLost = nullptr;

bool AudioManager::init() {

    char cwd[1024];
getcwd(cwd, sizeof(cwd));
std::cout << "Current working directory: " << cwd << std::endl;
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    Mix_AllocateChannels(16);
    std::cout << "SDL Audio Init Success\n";
    return true;
}

void AudioManager::load() {

    homeMusic = Mix_LoadMUS("sounds/home_music.mp3");
    if (!homeMusic)
        std::cout << "Home music load error: " << Mix_GetError() << std::endl;

    gameMusic = Mix_LoadMUS("sounds/gamemusic.mp3");
    if (!gameMusic)
        std::cout << "Game music load error: " << Mix_GetError() << std::endl;

    gameOverMusic = Mix_LoadMUS("sounds/gameover_music.mp3");
    if (!gameOverMusic)
        std::cout << "GameOver music load error: " << Mix_GetError() << std::endl;

    chickenDrop = Mix_LoadWAV("sounds/chickenpokpok.wav");
    if (!chickenDrop)
        std::cout << "Chicken load error: " << Mix_GetError() << std::endl;

    catchSound = Mix_LoadWAV("sounds/bucket-catch.wav");
    if (!catchSound)
        std::cout << "Catch load error: " << Mix_GetError() << std::endl;

    eggBreak = Mix_LoadWAV("sounds/egg_break.wav");
    if (!eggBreak)
        std::cout << "EggBreak load error: " << Mix_GetError() << std::endl;

    lifeLost = Mix_LoadWAV("sounds/life_lost.wav");
    if (!lifeLost)
        std::cout << "LifeLost load error: " << Mix_GetError() << std::endl;
}

// -------- MUSIC --------

void AudioManager::playMusic(Mix_Music* music, int loops)
{
    if (currentMusic == music) return;  // already playing

    Mix_FadeOutMusic(500);
    Mix_FadeInMusic(music, loops, 500); //smooth music transitions

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
