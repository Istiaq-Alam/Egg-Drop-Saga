#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

class AudioManager {
public:
    static bool init();
    static void loadSounds();
    static void playCatch();
    static void playMiss();
    static void playGameOver();
    static void cleanup();

private:
//    static Mix_Chunk* catchSound;
//static Mix_Chunk* missSound;
//static Mix_Chunk* gameOverSound;
};

#endif
