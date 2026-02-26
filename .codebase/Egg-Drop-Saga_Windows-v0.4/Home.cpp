#include "Home.h"
#include "Background.h"
#include <GL/glut.h>
#include <cmath>
#include <cstring>

Home::Home()
{
    buttonWidth = 250;
    buttonHeight = 80;

    buttonX = 400 - buttonWidth / 2;  // assuming 800 width
    buttonY = 250;

    buttonScale = 1.0f;
    buttonPulseTime = 0.0f;
}

void Home::update()
{
    buttonPulseTime += 0.05f;
    buttonScale = 1.0f + 0.05f * sin(buttonPulseTime);
}

void Home::draw()
{
    // Draw background
    //Background background; -----> discomment it for pause the background animation
    //background.draw(GAME_WIDTH, GAME_HEIGHT);

    drawTitle();
    drawPlayButton();
    drawInstructions();
}

void Home::drawTitle()
{
    const char* title1 = "EGG DROP";
    const char* title2 = "SAGA";

    float centerX = 520;
    float y1 = 390;
    float y2 = 365;

    // ---- 1. Shadow Layer ----
    glColor3f(0.2f, 0.05f, 0.0f);
    glRasterPos2f(centerX - 170 + 6, y1 - 6);
    for (int i = 0; i < strlen(title1); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title1[i]);

    glRasterPos2f(centerX - 145 + 6, y2 - 6);
    for (int i = 0; i < strlen(title2); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title2[i]);

    // ---- 2. Outline Layer (fake stroke) ----
    glColor3f(0.4f, 0.1f, 0.0f);

    for (int dx = -2; dx <= 2; dx++)
    for (int dy = -2; dy <= 2; dy++)
    {
        if (dx == 0 && dy == 0) continue;

        glRasterPos2f(centerX - 170 + dx, y1 + dy);
        for (int i = 0; i < strlen(title1); i++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title1[i]);

        glRasterPos2f(centerX - 145 + dx, y2 + dy);
        for (int i = 0; i < strlen(title2); i++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title2[i]);
    }

    // ---- 3. Main Fill (Golden) ----
    glColor3f(1.0f, 0.8f, 0.0f);
    glRasterPos2f(centerX - 170, y1);
    for (int i = 0; i < strlen(title1); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title1[i]);

    glColor3f(1.0f, 0.9f, 0.3f);
    glRasterPos2f(centerX - 145, y2);
    for (int i = 0; i < strlen(title2); i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, title2[i]);
}

void Home::drawPlayButton()
{
    glPushMatrix();

    glTranslatef(buttonX + buttonWidth/2, buttonY + buttonHeight/2, 0);
    glScalef(buttonScale, buttonScale, 1);
    glTranslatef(-buttonWidth/2, -buttonHeight/2, 0);

    // Button shadow
    glColor3f(0.2f, 0.1f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(5, -5);
        glVertex2f(buttonWidth + 5, -5);
        glVertex2f(buttonWidth + 5, buttonHeight - 5);
        glVertex2f(5, buttonHeight - 5);
    glEnd();

    // Button body (orange gradient feel)
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.6f, 0.0f);
        glVertex2f(0, 0);

        glColor3f(1.0f, 0.8f, 0.1f);
        glVertex2f(buttonWidth, 0);

        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex2f(buttonWidth, buttonHeight);

        glColor3f(1.0f, 0.7f, 0.0f);
        glVertex2f(0, buttonHeight);
    glEnd();

    // PLAY text
    glColor3f(1,1,1);
    glRasterPos2f(buttonWidth/2 - 50, buttonHeight/2 - 5);
    const char* text = "Press S to Start";
    for (int i = 0; i < strlen(text); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

    glPopMatrix();
}

void Home::drawInstructions()
{
    glColor3f(0,0,0);

    const char* line1 = "Catch Eggs Before They Hit Ground.";
    const char* line2 = "Move: A/D or Arrow Keys";
    const char* line3 = "Press SPACE to Pause & Press Q to Quit";

    glRasterPos2f(300, 180);
    for (int i = 0; i < strlen(line1); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, line1[i]);

    glRasterPos2f(330, 150);
    for (int i = 0; i < strlen(line2); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, line2[i]);

    glRasterPos2f(280, 120);
    for (int i = 0; i < strlen(line3); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, line3[i]);
}


