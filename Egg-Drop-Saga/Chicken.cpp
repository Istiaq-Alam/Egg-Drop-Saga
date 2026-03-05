#include "Chicken.h"
#include <GL/glut.h>
#include <cmath>

Chicken::Chicken(float x, float y) {
    this->x = x;
    this->y = y;
}

void Chicken::draw() {

    // =============================
    // BODY (Oval)
    // =============================
    glColor3f(1.0f, 0.95f, 0.3f);

    float bodyRX = 28.0f;
    float bodyRY = 22.0f;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 40; i++) {
        float angle = 2.0f * 3.1416f * i / 40;
        glVertex2f(x + cos(angle) * bodyRX,
                   y + sin(angle) * bodyRY);
    }
    glEnd();


    // =============================
    // HEAD
    // =============================
    float headX = x + 22;
    float headY = y + 18;
    float headR = 14;

    glColor3f(1.0f, 0.95f, 0.3f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(headX, headY);
    for (int i = 0; i <= 30; i++) {
        float angle = 2.0f * 3.1416f * i / 30;
        glVertex2f(headX + cos(angle) * headR,
                   headY + sin(angle) * headR);
    }
    glEnd();


    // =============================
    // WING
    // =============================
    glColor3f(0.95f, 0.85f, 0.2f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x - 5, y + 5);
    for (int i = 0; i <= 30; i++) {
        float angle = 2.0f * 3.1416f * i / 30;
        glVertex2f(x - 5 + cos(angle) * 12,
                   y + 5 + sin(angle) * 8);
    }
    glEnd();


    // =============================
    // BEAK
    // =============================
    glColor3f(1.0f, 0.5f, 0.0f);

    glBegin(GL_TRIANGLES);
    glVertex2f(headX + 12, headY);
    glVertex2f(headX + 25, headY + 4);
    glVertex2f(headX + 12, headY + 8);
    glEnd();


    // =============================
    // EYE
    // =============================
    glColor3f(0.0f, 0.0f, 0.0f);

    float eyeX = headX + 4;
    float eyeY = headY + 5;
    float eyeR = 3;

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(eyeX, eyeY);
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * 3.1416f * i / 20;
        glVertex2f(eyeX + cos(angle) * eyeR,
                   eyeY + sin(angle) * eyeR);
    }
    glEnd();


    // =============================
    // COMB (3 bumps)
    // =============================
    glColor3f(0.9f, 0.1f, 0.1f);

    for (int i = 0; i < 3; i++) {
        float combX = headX - 5 + (i * 6);
        float combY = headY + 12;

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(combX, combY);
        for (int j = 0; j <= 20; j++) {
            float angle = 2.0f * 3.1416f * j / 20;
            glVertex2f(combX + cos(angle) * 4,
                       combY + sin(angle) * 4);
        }
        glEnd();
    }


    // =============================
    // TAIL
    // =============================
    glColor3f(1.0f, 0.9f, 0.2f);

    glBegin(GL_TRIANGLES);
    glVertex2f(x - 28, y + 10);
    glVertex2f(x - 40, y + 20);
    glVertex2f(x - 20, y + 18);

    glVertex2f(x - 30, y);
    glVertex2f(x - 45, y + 5);
    glVertex2f(x - 25, y + 12);
    glEnd();


    // =============================
    // LEGS
    // =============================
    glColor3f(1.0f, 0.6f, 0.0f);

    glLineWidth(3);

    glBegin(GL_LINES);
    glVertex2f(x - 5, y - 20);
    glVertex2f(x - 5, y - 35);

    glVertex2f(x + 8, y - 20);
    glVertex2f(x + 8, y - 35);
    glEnd();
}
