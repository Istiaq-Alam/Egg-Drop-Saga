#include "Bucket.h"
#include <GL/glut.h>
#include <math.h>

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

void Bucket::draw()
{
    // =========================
    // Ground Shadow (soft ellipse under basket)
    // =========================
    float shadowWidth = width * 0.8f;    // slightly smaller than basket
    float shadowHeight = height * 0.25f;  // flat ellipse
    float shadowX = x + width / 2.0f;
    float shadowY = y - shadowHeight * 0.5f; // slightly below bucket
    // Bouncy Shadow //
    //float bounce = sin(glutGet(GLUT_ELAPSED_TIME) * 0.005f) * 0.05f;
    //shadowWidth *= (1.0f + bounce);
    //shadowHeight *= (1.0f - bounce);

    //glColor4f(0.0f, 0.0f, 0.0f, 0.6f); // semi-transparent black
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


        //glVertex2f(shadowX, shadowY); // center
        for (int i = 0; i <= 360; i++)
        {   glBegin(GL_TRIANGLE_FAN);
            glColor4f(0.0f, 0.0f, 0.0f, 0.6f); // semi-transparent black
            float angle = i * 3.1415926f / 180.0f;
            float dx = shadowWidth * cos(angle) / 2.0f;
            float dy = shadowHeight * sin(angle) / 2.0f;
            glVertex2f(shadowX + dx, shadowY + dy);
        }

    glEnd();

    glDisable(GL_BLEND);



    float centerX = x + width / 2.0f;
    float centerY = y + height; // top of the basket body

    float radiusX = width / 2.0f;
    float radiusY = height;

    // =========================
    // Basket Body (Semi Ellipse DOWNWARD) with gradient shading
    // =========================
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(centerX, centerY); // center top point

        for (int i = 0; i <= 180; i++)
        {
            float angle = i * 3.1415926f / 180.0f;
            float dx = radiusX * cos(angle);
            float dy = radiusY * sin(angle);

            // Gradient: lighter in middle, darker on edges
            float shade = 0.6f + 0.4f * sin(angle);
            glColor3f(0.76f * shade, 0.60f * shade, 0.42f * shade);

            glVertex2f(centerX + dx, centerY - dy);
        }
    glEnd();

    // =========================
    // Top Rim (Curved and 3D effect)
    // =========================
    float rimHeight = 10.0f;

    glBegin(GL_QUADS);
        // lighter top
        glColor3f(0.70f, 0.50f, 0.30f);
        glVertex2f(x, centerY);
        glVertex2f(x + width, centerY);

        // darker bottom
        glColor3f(0.55f, 0.35f, 0.18f);
        glVertex2f(x + width, centerY + rimHeight);
        glVertex2f(x, centerY + rimHeight);
    glEnd();

    // =========================
    // Vertical Weave Lines (curved along basket)
    // =========================
    //glColor3f(0.45f, 0.28f, 0.14f);
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = -radiusX + 5; i <= radiusX - 5; i += 8)
    {
        float curveHeight = radiusY * sqrt(1.0f - (i*i)/(radiusX*radiusX));

        glBegin(GL_LINES);
            glVertex2f(centerX + i, centerY);
            glVertex2f(centerX + i, centerY - curveHeight);
        glEnd();
    }

    // =========================
    // Smooth Outline (thicker at rim)
    // =========================
    glColor3f(0.2f, 0.1f, 0.05f);
    glLineWidth(3);

    glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= 180; i++)
        {
            float angle = i * 3.1415926f / 180.0f;
            float dx = radiusX * cos(angle);
            float dy = radiusY * sin(angle);
            glVertex2f(centerX + dx, centerY - dy);
        }
    glEnd();

    // =========================
    // Rim Outline
    // =========================
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, centerY);
        glVertex2f(x + width, centerY);
        glVertex2f(x + width, centerY + rimHeight);
        glVertex2f(x, centerY + rimHeight);
    glEnd();
}


bool Bucket::checkCollision(Egg& egg) {
    return (egg.x > x &&
            egg.x < x + width &&
            egg.y - egg.radius < y + height);
}
