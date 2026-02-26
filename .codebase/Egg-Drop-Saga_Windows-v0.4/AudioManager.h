#ifdef _WIN32
#define SDL_MAIN_HANDLED
#endif

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL.h>
#include <SDL2/SDL_mixer.h>

class AudioManager {
public:
    static bool init();
    static void load();

    // Music
    static void playHomeMusic();
    static void playMusic(Mix_Music* music, int loops);
    static void playGameMusic();
    static void playGameOverMusic();
    static void stopMusic();

    // Sound Effects
    static void playChickenDrop();
    static void playCatch();
    static void playEggBreak();
    static void playLifeLost();

    static void cleanup();

private:
    static Mix_Music* homeMusic;
    static Mix_Music* gameMusic;
    static Mix_Music* gameOverMusic;

    static Mix_Chunk* chickenDrop;
    static Mix_Chunk* catchSound;
    static Mix_Chunk* eggBreak;
    static Mix_Chunk* lifeLost;
};

#endif
