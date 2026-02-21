#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Bucket.h"
#include "Egg.h"
#include "Chicken.h"

enum GameState { HOME, PLAYING, GAME_OVER };

class Game {
private:
    int score;
    int lives;
    GameState state;

    Bucket bucket;
    Egg* currentEgg;
    std::vector<Chicken> chickens;

public:
    void init();
    void update();
    void render();
    void handleInput(unsigned char key);

private:
    void spawnEgg();
    void checkCollisions();
    void drawText(float x, float y, const char* text);
};

#endif
