#include "Background.h"
#include <GL/glut.h>
#include <cmath>

Background::Background() {
    cloudOffset = 0.0f;
    sunAngle = 0.0f;
}

void Background::update() {
    cloudOffset += 1.0f;

    if (cloudOffset > 800)
        cloudOffset = -200;

    sunAngle += 0.5f;
}

void Background::draw(int screenWidth, int screenHeight) {

    // ---------- Sky Gradient ----------
    glBegin(GL_QUADS);
        glColor3f(0.4f, 0.7f, 1.0f); // top sky
        glVertex2f(0, screenHeight);

        glColor3f(0.8f, 0.9f, 1.0f); // bottom sky
        glVertex2f(0, 0);
        glVertex2f(screenWidth, 0);

        glColor3f(0.4f, 0.7f, 1.0f);
        glVertex2f(screenWidth, screenHeight);
    glEnd();


    // ---------- Sun ----------
    glPushMatrix();
    glTranslatef(200, screenHeight - 50, 0);
    glRotatef(sunAngle, 0, 0, 1);

    glColor3f(1.0f, 0.8f, 0.0f);
    for (int i = 0; i < 360; i += 20) {
        glBegin(GL_TRIANGLES);
            glVertex2f(0, 0);
            glVertex2f(40 * cos(i * 3.14 / 180), 40 * sin(i * 3.14 / 180));
            glVertex2f(40 * cos((i+10) * 3.14 / 180), 40 * sin((i+10) * 3.14 / 180));
        glEnd();
    }
    glPopMatrix();


    // ---------- Moving Cloud ----------
    glColor3f(1.0f, 1.0f, 1.0f);
    float x = cloudOffset;

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14 / 180;
        glVertex2f(x + 200 + 40 * cos(theta),
                   screenHeight - 150 + 20 * sin(theta));
    }
    glEnd();
}
