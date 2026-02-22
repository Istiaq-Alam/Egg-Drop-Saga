#include "Egg.h"
#include <GL/glut.h>
#include <cmath>

Egg::Egg(float x, float y, float speed) {
    this->x = x;
    this->y = y;
    this->speed = speed;
    radius = 10;
}

void Egg::update() {
    y -= speed;
}

void Egg::draw() {
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 20; i++) {
        float angle = 2 * 3.1416f * i / 20;
        glVertex2f(x + cos(angle) * radius,
                   y + sin(angle) * radius);
    }
    glEnd();
}
