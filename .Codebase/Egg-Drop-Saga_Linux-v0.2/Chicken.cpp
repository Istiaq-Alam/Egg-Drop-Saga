#include "Chicken.h"
#include <GL/glut.h>
#include <cmath>

Chicken::Chicken(float x, float y) {
    this->x = x;
    this->y = y;
}

void Chicken::draw() {
    // Body (circle)
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 20; i++) {
        float angle = 2 * 3.1416f * i / 20;
        glVertex2f(x + cos(angle) * 15,
                   y + sin(angle) * 15);
    }
    glEnd();

    // Beak (triangle)
    glColor3f(1.0, 0.5, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 15, y);
    glVertex2f(x + 25, y + 5);
    glVertex2f(x + 15, y + 10);
    glEnd();
}
