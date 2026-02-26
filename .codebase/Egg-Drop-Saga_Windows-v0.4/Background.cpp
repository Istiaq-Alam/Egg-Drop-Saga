#include "Background.h"
#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

const float GAME_WIDTH  = 800.0f;
const float GAME_HEIGHT = 720.0f;

Background::Background()
{
    cloudOffset = 0.0f;
    cloudOffset1 = 0.0f;
    cloudOffset2 = 0.0f;
    cloudOffset3 = 0.0f;
    sunAngle = 0.0f;
    timeOfDay = 0.0f;
    skySpeed = 0.0005f; // faster/slower cycle
    windTime = 0.0f;
    std::srand(std::time(0));

    for (int i = 0; i < 150; i++)
    {
        Star s;
        s.x = rand() % (int)GAME_WIDTH;
        s.y = rand() % (int)(GAME_HEIGHT * 0.9f);
        s.size = 1 + rand() % 3;

        stars.push_back(s);
    }
}

void Background::update()
{
    cloudOffset1 += 0.2f;  // far clouds slow
    cloudOffset2 += 0.5f;  // mid clouds medium
    cloudOffset3 += 1.0f;  // front clouds fast

    if (cloudOffset1 > GAME_WIDTH) cloudOffset1 = -GAME_WIDTH;
    if (cloudOffset2 > GAME_WIDTH) cloudOffset2 = -GAME_WIDTH;
    if (cloudOffset3 > GAME_WIDTH) cloudOffset3 = -GAME_WIDTH;

    sunAngle += 0.2f;
    windTime += 0.02f;
    timeOfDay += skySpeed;

    if (timeOfDay > 1.0f)
        timeOfDay = 0.0f;
}

void drawTree(float x, float y, float scale)
{
    // Trunk
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 20*scale, y);
    glVertex2f(x + 20*scale, y + 100*scale);
    glVertex2f(x, y + 100*scale);
    glEnd();

    // Leaves (3 circles)
    glColor3f(0.2f, 0.7f, 0.2f);

    for(int i=0; i<3; i++)
    {
        float offset = i*30*scale;
        glBegin(GL_POLYGON);
        for(int j=0; j<360; j++)
        {
            float theta = j*M_PI/180.0f;
            glVertex2f(x + 10*scale + offset + 40*scale*cos(theta),
                       y + 120*scale + 40*scale*sin(theta));
        }
        glEnd();
    }
}

void drawCloud(float x, float y, float scale)
{
    // Main white body
    glColor3f(0.92f, 0.95f, 0.98f);

    for(int i=0; i<5; i++)
    {
        float cx = x + (i * 30.0f * scale);
        float cy = y + ( (i%2==0)? 10.0f*scale : 0 );

        glBegin(GL_POLYGON);
        for(int j=0; j<=360; j+=10)
        {
            float theta = j * M_PI / 180.0f;
            glVertex2f(cx + 35*scale*cos(theta),
                       cy + 25*scale*sin(theta));
        }
        glEnd();
    }

    // Bottom soft shadow
    glColor3f(0.75f, 0.85f, 0.92f);

    glBegin(GL_POLYGON);
    for(int j=0; j<=360; j+=10)
    {
        float theta = j * M_PI / 180.0f;
        glVertex2f(x + 60*scale + 60*scale*cos(theta),
                   y - 10*scale + 20*scale*sin(theta));
    }
    glEnd();
}

void Background::draw(int screenWidth, int screenHeight)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float nightFactor = 0.0f;

    if (timeOfDay > 0.5f)
        nightFactor = (timeOfDay - 0.5f) * 2.0f;
    // ---------- Animated Sky Gradient ----------

    float rTop, gTop, bTop;
    float rBottom, gBottom, bBottom;

// DAY COLORS
    float dayTopR = 0.2f, dayTopG = 0.5f, dayTopB = 0.9f;
    float dayBotR = 0.6f, dayBotG = 0.85f, dayBotB = 1.0f;

// SUNSET COLORS
    float sunTopR = 0.9f, sunTopG = 0.4f, sunTopB = 0.3f;
    float sunBotR = 1.0f, sunBotG = 0.7f, sunBotB = 0.4f;

// NIGHT COLORS
    float nightTopR = 0.05f, nightTopG = 0.05f, nightTopB = 0.2f;
    float nightBotR = 0.2f, nightBotG = 0.2f, nightBotB = 0.4f;

// Blend logic
    if (timeOfDay < 0.5f)
    {
        float t = timeOfDay * 2.0f;

        rTop = dayTopR * (1-t) + sunTopR * t;
        gTop = dayTopG * (1-t) + sunTopG * t;
        bTop = dayTopB * (1-t) + sunTopB * t;

        rBottom = dayBotR * (1-t) + sunBotR * t;
        gBottom = dayBotG * (1-t) + sunBotG * t;
        bBottom = dayBotB * (1-t) + sunBotB * t;
    }
    else
    {
        float t = (timeOfDay - 0.5f) * 2.0f;

        rTop = sunTopR * (1-t) + nightTopR * t;
        gTop = sunTopG * (1-t) + nightTopG * t;
        bTop = sunTopB * (1-t) + nightTopB * t;

        rBottom = sunBotR * (1-t) + nightBotR * t;
        gBottom = sunBotG * (1-t) + nightBotG * t;
        bBottom = sunBotB * (1-t) + nightBotB * t;
    }

    glBegin(GL_QUADS);
    glColor3f(rTop, gTop, bTop);
    glVertex2f(0, screenHeight);

    glColor3f(rBottom, gBottom, bBottom);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);

    glColor3f(rTop, gTop, bTop);
    glVertex2f(screenWidth, screenHeight);
    glEnd();

    if (timeOfDay < 0.5f)
    {
        // -------- DRAW SUN --------
        glColor3f(1.0f, 0.8f, 0.2f);
        float sunX = screenWidth * 0.15f;
        float sunY = screenHeight * 0.75f;
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 360; i += 10)
        {
            float theta = i * M_PI / 180.0f;
            glVertex2f(sunX + 40*cos(theta),
                       sunY + 40*sin(theta));
        }
        glEnd();
    }
    else
    {
        // -------- DRAW MOON --------
        glColor3f(0.9f, 0.9f, 1.0f);
        float sunX = screenWidth * 0.15f;
        float sunY = screenHeight * 0.75f;
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 360; i += 10)
        {
            float theta = i * M_PI / 180.0f;
            glVertex2f(sunX + 35*cos(theta),
                       sunY + 35*sin(theta));
        }
        glEnd();

        // Crescent cut
        glColor3f(0.05f, 0.05f, 0.2f);

        glBegin(GL_POLYGON);
        for (int i = 0; i <= 360; i += 10)
        {
            float theta = i * M_PI / 180.0f;
            glVertex2f(sunX + 25*cos(theta) + 10,
                       sunY + 30*sin(theta));
        }
        glEnd();
    }
    // -------- STARS --------
  if (timeOfDay > 0.5f)
{
    float nightFactor = (timeOfDay - 0.5f) * 2.0f;

    glColor4f(1.0f, 1.0f, 1.0f, nightFactor);

    for (auto &s : stars)
    {
        glBegin(GL_QUADS);
            glVertex2f(s.x, s.y);
            glVertex2f(s.x + s.size, s.y);
            glVertex2f(s.x + s.size, s.y + s.size);
            glVertex2f(s.x, s.y + s.size);
        glEnd();
    }
}
    // ---------- Far Mountains ----------
    glColor3f(0.70f, 0.75f, 0.85f);

    glBegin(GL_TRIANGLES);
    glVertex2f(screenWidth * 0.1f, screenHeight * 0.4f);
    glVertex2f(screenWidth * 0.25f, screenHeight * 0.65f);
    glVertex2f(screenWidth * 0.4f, screenHeight * 0.4f);

    glVertex2f(screenWidth * 0.35f, screenHeight * 0.4f);
    glVertex2f(screenWidth * 0.55f, screenHeight * 0.7f);
    glVertex2f(screenWidth * 0.75f, screenHeight * 0.4f);
    glEnd();

    // ---------- Mid Mountains ----------
    glColor3f(0.55f, 0.65f, 0.80f);

    glBegin(GL_TRIANGLES);
    glVertex2f(screenWidth * 0.0f, screenHeight * 0.4f);
    glVertex2f(screenWidth * 0.2f, screenHeight * 0.6f);
    glVertex2f(screenWidth * 0.4f, screenHeight * 0.4f);

    glVertex2f(screenWidth * 0.5f, screenHeight * 0.4f);
    glVertex2f(screenWidth * 0.7f, screenHeight * 0.6f);
    glVertex2f(screenWidth * 0.9f, screenHeight * 0.4f);
    glEnd();

    // ---------- Back Grass ----------
    glColor3f(0.55f, 0.85f, 0.35f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(screenWidth, 0);
    glVertex2f(screenWidth, screenHeight * 0.4f);
    glVertex2f(0, screenHeight * 0.4f);
    glEnd();

// ---------- Front Hill Curve ----------
    glColor3f(0.45f, 0.75f, 0.25f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 180; i++)
    {
        float theta = i * M_PI / 180.0f;
        float x = screenWidth/2 + (screenWidth/2) * cos(theta);
        float y = screenHeight * 0.35f + (screenHeight * 0.15f) * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();


    glPushMatrix();
    glTranslatef(-cloudOffset1, 0, 0);

    float floatOffset = sin(windTime) * 9.0f;

    drawCloud(100, screenHeight*0.85f + floatOffset, 0.5f);
    drawCloud(400, screenHeight*0.80f + floatOffset, 0.6f);
    drawCloud(700, screenHeight*0.88f + floatOffset, 0.4f);

    glPopMatrix();


    glPushMatrix();
    glTranslatef(-cloudOffset2, 0, 0);
    float float2 = sin(windTime + 1.5f) * 7.0f;
    drawCloud(50, screenHeight*0.70f + float2, 0.8f);
    drawCloud(350, screenHeight*0.75f + float2, 0.9f);
    drawCloud(750, screenHeight*0.72f + float2, 0.7f);

    glPopMatrix();



    /* ---------- Animated Cloud ----------
    glPushMatrix();
    glTranslatef(-cloudOffset3, 0, 0);
    float float1 = sin(windTime) * 5.0f;
    drawCloud(0, screenHeight*0.55f + float1, 1.2f);
    drawCloud(500, screenHeight*0.60f + float1, 1.1f);

    glPopMatrix();

        glPushMatrix();
        glTranslatef(cloudOffset, 0, 0);

        float baseY = screenHeight * 0.7f;
        glColor3f(1.0f, 1.0f, 1.0f);

        for(int c=0; c<3; c++)
        {
            float cx = screenWidth * 0.2f + c*40;
            glBegin(GL_POLYGON);
            for(int i=0; i<360; i++)
            {
                float theta = i*M_PI/180.0f;
                glVertex2f(cx + 30*cos(theta),
                           baseY + 20*sin(theta));
            }
            glEnd();
        }

        glPopMatrix();*/



    // ---------- Trees ----------
    drawTree(screenWidth * 0.8f, screenHeight * 0.25f, 1.5f); // Big right
    drawTree(screenWidth * 0.1f, screenHeight * 0.25f, 0.8f); // Small left
    drawTree(screenWidth * 0.5f, screenHeight * 0.35f, 0.4f); // Far small tree


}
