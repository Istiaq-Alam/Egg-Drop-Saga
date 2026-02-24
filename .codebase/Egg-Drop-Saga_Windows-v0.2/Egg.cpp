#include "Egg.h"
#include <GL/glut.h>
#include <cmath>

#define GRAVITY 0.03f

Egg::Egg(float x, float y, float speed)
{
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->radius = 10;
    this->isBroken = false;
    this->breakTimer = 0.0f;
    squashTimer = 0.0f;
}

void Egg::update(float groundY)
{
    if (!isBroken && !isSquashing)
    {
        y -= speed;

        if (y - radius <= groundY)
        {
            y = groundY + radius;
            isSquashing = true;   // trigger squash first
        }
    }
    else if (isSquashing)
    {
        squashTimer += 0.016f;

        if (squashTimer > 0.12f)   // squash duration
        {
            isSquashing = false;
            isBroken = true;
        }
    }
    else if (isBroken)
    {
        breakTimer += 0.016f;
    }
}

void drawShadow(float x, float groundY, float eggY, float radius)
{
    // ---- CONFIG ----
    float maxFallHeight = 600.0f;  // adjust to your scene height
    float minScale = 1.0f;         // near ground
    float maxScale = 2.5f;         // when very high
    float minAlpha = 0.25f;        // light shadow (high)
    float maxAlpha = 0.65f;        // dark shadow (near ground)

    // ---- NORMALIZE HEIGHT ----
    float height = eggY - groundY;
    float t = height / maxFallHeight;

    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    // ---- SCALE LOGIC ----
    float scale = minScale + (maxScale - minScale) * t;

    // ---- ALPHA LOGIC (reverse of scale) ----
    float alpha = maxAlpha - (maxAlpha - minAlpha) * t;

    float shadowWidth  = radius * scale;
    float shadowHeight = radius * 0.45f * scale;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, alpha);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, groundY);

    for (int i = 0; i <= 40; i++)
    {
        float angle = 2 * 3.1416f * i / 40;
        glVertex2f(
            x + cos(angle) * shadowWidth,
            groundY + sin(angle) * shadowHeight
        );
    }
    glEnd();

    glDisable(GL_BLEND);
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
        glVertex2f(x + cos(angle) * radius * 2.5f, groundY + 5 + sin(angle) * radius);
    }
    glEnd();

    // Yolk base with gradient
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.8f, 0.0f);
    glVertex2f(x, groundY + 5);

    for (int i = 0; i <= 40; i++)
    {
        float angle = 3.1416f * i / 40;
        float nx = cos(angle);
        float ny = sin(angle);

        // gradient: top lighter, bottom darker
        float shade = 0.8f + 0.2f * (ny + 1.0f) / 2.0f;
        glColor3f(shade, shade * 0.8f, 0.0f);

        glVertex2f(x + nx * radius, groundY + 5 + ny * radius);
    }
    glEnd();

    // Yolk shiny highlight
    glBegin(GL_TRIANGLE_FAN);
    float highlightRadius = radius * 0.3f;
    float highlightX = x - radius * 0.2f;
    float highlightY = groundY + 5 + radius * 0.3f;
    glColor3f(1.0f, 1.0f, 1.0f); // bright white
    glVertex2f(highlightX, highlightY);
    for (int i = 0; i <= 20; i++)
    {
        float angle = 2 * 3.1416f * i / 20;
        glVertex2f(
        highlightX + cos(angle) * highlightRadius * 0.7f, // squash X
        highlightY + sin(angle) * highlightRadius * 1.0f  // full Y
    );
    }
    glEnd();
    }

void Egg::draw(float groundY)
{
    if (!isBroken)
{
    float scaleX = 1.0f;
    float scaleY = 1.0f;

    if (isSquashing)
    {
        float t = squashTimer / 0.12f;

        // Ease curve
        float squashAmount = sin(t * 3.1416f);

        scaleY = 1.0f - 0.5f * squashAmount;   // shrink vertically
        scaleX = 1.0f + 0.4f * squashAmount;   // stretch horizontally
    }

    drawShadow(x, groundY, y, radius);

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scaleX, scaleY, 1.0f);
    glTranslatef(-x, -y, 0);

    // ===== Egg Body (Brown Gradient) =====
    glBegin(GL_TRIANGLE_FAN);

    // Center slightly lighter
    glColor3f(1.0f, 0.6f, 0.00f);
    glVertex2f(x, y);

    for (int i = 0; i <= 60; i++)
    {
        float angle = 2 * 3.1416f * i / 60;
        float nx = cos(angle);
        float ny = sin(angle);

        float stretch = (ny < 0) ? 1.12f : 1.0f;

        // Right side darker shading
        float sideShade = 0.85f - 0.15f * nx;

        float r = 0.90f * sideShade;
        float g = 0.68f * sideShade;
        float b = 0.45f * sideShade;

        glColor3f(r, g, b);

        glVertex2f(
            x + nx * radius * 0.8f * stretch,
            y + ny * radius * 1.2f
        );
    }
    glEnd();


    // ===== Elliptical Highlight =====
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float highlightX = x - radius * 0.25f;
    float highlightY = y + radius * 0.55f;

    glBegin(GL_TRIANGLE_FAN);

    // soft center white
    glColor4f(1.0f, 1.0f, 1.0f, 0.55f);
    glVertex2f(highlightX, highlightY);

    for (int i = 0; i <= 40; i++)
    {
        float angle = 2 * 3.1416f * i / 40;

        // elliptical stretch
        float hx = cos(angle) * radius * 0.45f;
        float hy = sin(angle) * radius * 0.65f;

        // soft fade out
        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);

        glVertex2f(highlightX + hx, highlightY + hy);
    }
    glEnd();

    glDisable(GL_BLEND);


    // ===== Outline =====
    glColor3f(0.4f, 0.4f, 0.4f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= 60; i++)
    {
        float angle = 2 * 3.1416f * i / 60;
        float nx = cos(angle);
        float ny = sin(angle);
        float stretch = (ny < 0) ? 1.12f : 1.0f;

        glVertex2f(
            x + nx * radius * 0.8f * stretch,
            y + ny * radius * 1.2f
        );
    }
    glEnd();

    glPopMatrix();

    glPopMatrix();
}
else
{
    drawShadow(x, groundY, 0, radius);
    drawBrokenEgg(x, groundY, radius);
}
}






