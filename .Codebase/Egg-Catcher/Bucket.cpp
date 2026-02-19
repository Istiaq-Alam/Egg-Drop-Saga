#include "Bucket.h"
#include <GL/glut.h>

Bucket::Bucket() {}
Bucket::Bucket(float x, float y) {
    this->x = x;
    this->y = y;
    width = 100;
    height = 30;
    speed = 50;
}

void Bucket::moveLeft() {
    if (x > 0) x -= speed;
}

void Bucket::moveRight() {
    if (x + width < 800) x += speed;
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
