#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Bucket.h"
#include "Egg.h"
#include "Chicken.h"
#include "Background.h"
#include "Home.h"

#ifndef GAME_DIMENSIONS_H
#define GAME_DIMENSIONS_H


#endif



enum GameState { HOME, PLAYING, GAME_OVER };

class Game {
private:
    int score;
    int lives;
    int lastTime;
    bool isPaused;
    int pauseTimer; // counts frames or seconds for showing "Resumed"
    GameState state;
    Home home;
    Background background;
    Bucket bucket;
    Egg* currentEgg;
    std::vector<Chicken> chickens;

public:
    void init();
    void update();
    void render();
    void handleInput(unsigned char key);
    void handleSpecialInput(int key);

private:
    void spawnEgg();
    void checkCollisions();
    void drawText(float x, float y, const char* text);
};

#endif
