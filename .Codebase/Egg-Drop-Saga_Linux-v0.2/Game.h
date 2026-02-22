#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Bucket.h"
#include "Egg.h"
#include "Chicken.h"
#include "Background.h"

#ifndef GAME_DIMENSIONS_H
#define GAME_DIMENSIONS_H

const float GAME_WIDTH  = 1280.0f;
const float GAME_HEIGHT = 720.0f;

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
