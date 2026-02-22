#include "Background.h"
#include <GL/glut.h>
#include <cmath>

Background::Background() {
    cloudOffset = 0.0f;
    sunAngle = 0.0f;
}

void Background::update() {
    cloudOffset += 1.0f;

    // Reset cloud smoothly based on world width (1280)
    if (cloudOffset > 1280)
        cloudOffset = -300;

    sunAngle += 0.5f;
}

void Background::draw(int screenWidth, int screenHeight) {

    // ---------- Sky Gradient ----------
    glBegin(GL_QUADS);
        glColor3f(0.4f, 0.7f, 1.0f);
        glVertex2f(0, screenHeight);

        glColor3f(0.8f, 0.9f, 1.0f);
        glVertex2f(0, 0);
        glVertex2f(screenWidth, 0);

        glColor3f(0.4f, 0.7f, 1.0f);
        glVertex2f(screenWidth, screenHeight);
    glEnd();


    // ---------- Sun (Responsive Position) ----------
    float sunX = screenWidth * 0.15f;
    float sunY = screenHeight * 0.75f;
    float sunRadius = screenHeight * 0.06f;

    glPushMatrix();
    glTranslatef(sunX, sunY, 0);
    glRotatef(sunAngle, 0, 0, 1);

    glColor3f(1.0f, 0.8f, 0.0f);

    for (int i = 0; i < 360; i += 20) {
        float rad1 = i * M_PI / 180.0f;
        float rad2 = (i + 10) * M_PI / 180.0f;

        glBegin(GL_TRIANGLES);
            glVertex2f(0, 0);
            glVertex2f(sunRadius * cos(rad1),
                       sunRadius * sin(rad1));
            glVertex2f(sunRadius * cos(rad2),
                       sunRadius * sin(rad2));
        glEnd();
    }

    glPopMatrix();


    // ---------- Moving Cloud (Responsive) ----------
    glColor3f(1.0f, 1.0f, 1.0f);

    float cloudY = screenHeight * 0.65f;
    float cloudRadiusX = screenWidth * 0.04f;
    float cloudRadiusY = screenHeight * 0.03f;

    float x = cloudOffset;

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex2f(x + screenWidth * 0.2f + cloudRadiusX * cos(theta),
                   cloudY + cloudRadiusY * sin(theta));
    }
    glEnd();
}
