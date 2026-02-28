#include "Chicken.h"
#include <GL/glut.h>
#include <cmath>

Chicken::Chicken(float x, float y) {
    this->x = x;
    this->y = y;
}

void Chicken::startLayEgg()
{
    isLayingEgg = true;
}

void Chicken::update()
{
    if(isLayingEgg)
    {
        animProgress += 0.05f;   // speed of animation

        if(animProgress >= 1.0f)
        {
            animProgress = 1.0f;
            isLayingEgg = false;
        }
    }
}

void Chicken::drawLegs()
{
    // =========================
    // LEGS + 3 FINGERS
    // =========================
    glLineWidth(2.5f);
    glBegin(GL_LINES);

    // left leg
    glVertex2f(-10,-32);
    glVertex2f(-10,-45);

    // left toes
    glVertex2f(-10,-45); glVertex2f(-18,-55);
    glVertex2f(-10,-45); glVertex2f(-10,-58);
    glVertex2f(-10,-45); glVertex2f(-2,-55);

    // right leg
    glVertex2f(10,-35);
    glVertex2f(10,-45);

    // right toes
    glVertex2f(10,-45); glVertex2f(2,-55);
    glVertex2f(10,-45); glVertex2f(10,-58);
    glVertex2f(10,-45); glVertex2f(18,-55);

    glEnd();
}

void Chicken::draw()
{
    float bodyR = 35.0f;
    float headX = 20;
    float headY = 55;
    float headR = 14;
    float bodyLift   = animProgress * 35.0f;     // body moves up
    float wingAngle  = animProgress * 70.0f;     // wing rotate upward
    float headTilt   = animProgress * 25.0f;     // head tilt up
    float beakOpen   = animProgress * 8.0f;      // beak opening
    float eyeClose   = animProgress;             // 0=open 1=closed
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Draw LEGS FIRST (so they stay fixed)
    drawLegs();

    // Now move body up
    glTranslatef(0, bodyLift, 0);


    // =========================
    // BODY
    // =========================
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0,0);
    for(int i=0;i<=60;i++){
        float a = 2*3.1416f*i/60;
        glVertex2f(cos(a)*bodyR, sin(a)*bodyR);
    }
    glEnd();

    glColor3f(0,0,0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<=60;i++){
        float a = 2*3.1416f*i/60;
        glVertex2f(cos(a)*bodyR, sin(a)*bodyR);
    }
    glEnd();


    // =========================
// TAIL (Connected & Natural)
// =========================

float tailPos[3][2] = {
    {-35, 20},  // top
    {-42, 11}  // bottom left
    //{-35, 18}   // bottom right
};

for(int k=0;k<2;k++)
{
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(tailPos[k][0], tailPos[k][1]);
    for(int i=0;i<=40;i++){
        float a = 2*3.1416f*i/40;
        glVertex2f(tailPos[k][0] + cos(a)*12,
                   tailPos[k][1] + sin(a)*12);
    }
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<=40;i++){
        float a = 2*3.1416f*i/40;
        glVertex2f(tailPos[k][0] + cos(a)*12,
                   tailPos[k][1] + sin(a)*12);
    }
    glEnd();
}


    // =========================
    // NECK
    // =========================
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(20,30);
    for(int i=0;i<=40;i++){
        float a = 2*3.1416f*i/40;
        glVertex2f(20 + cos(a)*12,
                   30 + sin(a)*20);
    }
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<=40;i++){
        float a = 2*3.1416f*i/40;
        glVertex2f(20 + cos(a)*12,
                   30 + sin(a)*20);
    }
    glEnd();


    // =========================
    // HEAD
    // =========================
    glPushMatrix();
    glTranslatef(headX, headY, 0);
    glRotatef(headTilt, 0, 0, 1);
    glTranslatef(-headX, -headY, 0);
    glColor3f(1,1,1);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(headX,headY);
    for(int i=0;i<=40;i++){
        float a = 2*3.1416f*i/40;
        glVertex2f(headX + cos(a)*headR,
                   headY + sin(a)*headR);
    }
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    for(int i=0;i<=40;i++){
        float a = 2*3.1416f*i/40;
        glVertex2f(headX + cos(a)*headR,
                   headY + sin(a)*headR);
    }
    glEnd();


    // =========================
// WING (Tilted Feather Style)
// =========================

glPushMatrix();

// Move to wing anchor position on body
glTranslatef(3, 3, 0);

// Rotate upward (adjust angle if needed)
glRotatef(25.0f + wingAngle, 0, 0, 1);   // 25 degrees tilt

glColor3f(1,1,1);
glBegin(GL_POLYGON);

// Upper arc
for(int i = 160; i <= 360; i++){
    float a = i * 3.1416f / 180.0f;
    glVertex2f(cos(a) * 28,
               sin(a) * 18);
}

// Lower arc
for(int i = 0; i <= 160; i++){
    float a = i * 3.1416f / 180.0f;
    glVertex2f(cos(a) * 25,
               sin(a) * 12);
}

glEnd();


// Outline
glColor3f(0,0,0);
glLineWidth(3);
glBegin(GL_LINE_LOOP);

for(int i = 160; i <= 360; i++){
    float a = i * 3.1416f / 180.0f;
    glVertex2f(cos(a) * 28,
               sin(a) * 18);
}
for(int i = 0; i <= 160; i++){
    float a = i * 3.1416f / 180.0f;
    glVertex2f(cos(a) * 25,
               sin(a) * 12);
}

glEnd();

// Inner feather line
glBegin(GL_LINE_STRIP);
for(int i = 180; i <= 350; i++){
    float a = i * 3.1416f / 180.0f;
    glVertex2f(cos(a) * 20,
               sin(a) * 10);
}
glEnd();

glPopMatrix();


    // =========================
    // BEAK
    // =========================
    glColor3f(1,0.8f,0);
    glBegin(GL_TRIANGLES);

    // upper beak
    glVertex2f(headX+12, headY);
    glVertex2f(headX+25, headY+5 + beakOpen);
    glVertex2f(headX+12, headY+10);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(headX+12, headY);
    glVertex2f(headX+25, headY+5);
    glVertex2f(headX+12, headY+10);
    glEnd();


    // =========================
    // COMB (Vertical Ellipses Big→Medium→Small)
    // =========================
    float combSize[3][2] = {
        {6,6},
        {5,8},
        {6,8}   // biggest
    };

    for(int i=0;i<3;i++){
        float cx = headX - 8 + i*6;
        float cy = headY + 14 + i*2;

        glColor3f(0.9f,0.1f,0.1f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx,cy);
        for(int j=0;j<=30;j++){
            float a = 2*3.1416f*j/30;
            glVertex2f(cx + cos(a)*combSize[i][0],
                       cy + sin(a)*combSize[i][1]);
        }
        glEnd();

        glColor3f(0,0,0);
        glBegin(GL_LINE_LOOP);
        for(int j=0;j<=30;j++){
            float a = 2*3.1416f*j/30;
            glVertex2f(cx + cos(a)*combSize[i][0],
                       cy + sin(a)*combSize[i][1]);
        }
        glEnd();
    }


    // =========================
    // EYE
    // =========================
    if(eyeClose < 0.8f)
{
    // normal eye
    glColor3f(0,0,0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(headX+3, headY+4);
    for(int i=0;i<=20;i++){
        float a = 2*3.1416f*i/20;
        glVertex2f(headX+3 + cos(a)*3,
                   headY+4 + sin(a)*3);
    }
    glEnd();

}
else
{
    // closed eye line
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(headX, headY+4);
    glVertex2f(headX+8, headY+4);
    glEnd();
}




    glPopMatrix();
}
