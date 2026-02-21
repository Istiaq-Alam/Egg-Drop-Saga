#ifndef BUCKET_H
#define BUCKET_H

#include "Egg.h"

class Bucket {
private:
    float x, y;
    float width, height;
    float speed;

public:
    Bucket();
    Bucket(float x, float y);

    void moveLeft();
    void moveRight();
    void draw();

    bool checkCollision(Egg& egg);

    float getX() { return x; }
    float getY() { return y; }
    float getWidth() { return width; }
    float getHeight() { return height; }
};

#endif
