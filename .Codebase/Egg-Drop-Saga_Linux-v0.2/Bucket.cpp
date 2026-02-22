#include "Bucket.h"
#include <GL/glut.h>

const float GAME_WIDTH  = 800.0f;
const float GAME_HEIGHT = 720.0f;

Bucket::Bucket() {}
Bucket::Bucket(float x, float y) {
    this->x = x;
    this->y = y;
    width = 100;
    height = 30;
    speed = 50;
}

void Bucket::moveLeft()
{
    x -= speed;

    if (x < 0)
        x = 0;
}

void Bucket::moveRight()
{
    x += speed;

    if (x + width > GAME_WIDTH)
        x = GAME_WIDTH - width;
}

void Bucket::draw() {
    glColor3f(0.6, 0.3, 0.1);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

bool Bucket::checkCollision(Egg& egg) {
    return (egg.x > x &&
            egg.x < x + width &&
            egg.y - egg.radius < y + height);
}
