#include "Egg.h"
#include <GL/glut.h>
#include <cmath>

#define GRAVITY 0.3f

Egg::Egg(float x, float y, float speed)
{
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->radius = 12;
    this->isBroken = false;
}

void Egg::update(float groundY)
{
    if (isBroken) return;

    // Apply gravity
    speed += GRAVITY;
    y -= speed;

    // Ground collision
    if (y - radius <= groundY)
    {
        y = groundY + radius;
        isBroken = true;
    }
}

void drawShadow(float x, float groundY, float height, float radius)
{
    float scale = 1.0f - (height / 600.0f);
    if (scale < 0.3f) scale = 0.3f;

    float shadowWidth = radius * 2 * scale;
    float shadowHeight = radius * 0.5f * scale;

    glColor4f(0, 0, 0, 0.3f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, groundY);

    for (int i = 0; i <= 40; i++)
    {
        float angle = 2 * 3.1416f * i / 40;
        glVertex2f(x + cos(angle) * shadowWidth,
                   groundY + sin(angle) * shadowHeight);
    }
    glEnd();
}

void drawBrokenEgg(float x, float groundY, float radius)
{
    // Egg white splash
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, groundY + 5);
    for (int i = 0; i <= 40; i++)
    {
        float angle = 2 * 3.1416f * i / 40;
        glVertex2f(x + cos(angle) * radius * 2,
                   groundY + 5 + sin(angle) * radius);
    }
    glEnd();

    // Yolk
    glColor3f(1.0f, 0.8f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, groundY + 5);
    for (int i = 0; i <= 40; i++)
    {
        float angle = 2 * 3.1416f * i / 40;
        glVertex2f(x + cos(angle) * radius,
                   groundY + 5 + sin(angle) * radius);
    }
    glEnd();
}

void Egg::draw(float groundY)
{
    if (!isBroken)
    {
        // Draw shadow
        drawShadow(x, groundY, y - groundY, radius);

        // Draw oval egg (better than circle)
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1, 1, 1);
        glVertex2f(x, y);

        for (int i = 0; i <= 40; i++)
        {
            float angle = 2 * 3.1416f * i / 40;
            float stretch = (sin(angle) < 0) ? 1.15f : 1.0f;

            glVertex2f(
                x + cos(angle) * radius * 0.8f * stretch,
                y + sin(angle) * radius * 1.2f
            );
        }
        glEnd();
    }
    else
    {
        drawShadow(x, groundY, 0, radius);
        drawBrokenEgg(x, groundY, radius);
    }
}
