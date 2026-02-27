#include "Chicken.h"
#include <GL/glut.h>
#include <cmath>

Chicken::Chicken(float x, float y) {
    this->x = x;
    this->y = y;
}

void Chicken::draw()
{
    glPushMatrix();
    glTranslatef(x, y, 0);

    float bodyR = 35.0f;
    float headX = 20;
    float headY = 55;
    float headR = 14;

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

for(int k=0;k<3;k++)
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
    // WING (Correct Orientation)
    // =========================
    glColor3f(1,1,1);

    glBegin(GL_POLYGON);
    glVertex2f(10, 5);     // top front
    glVertex2f(0, 15);     // upper curve
    glVertex2f(-15, 5);    // back top
    glVertex2f(-20, -5);   // mid back
    glVertex2f(-10, -15);  // bottom feather
    glVertex2f(5, -10);    // front bottom
    glEnd();

    // outline
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(10, 5);
    glVertex2f(0, 15);
    glVertex2f(-15, 5);
    glVertex2f(-20, -5);
    glVertex2f(-10, -15);
    glVertex2f(5, -10);
    glEnd();


    // =========================
    // BEAK
    // =========================
    glColor3f(1,0.8f,0);
    glBegin(GL_TRIANGLES);
    glVertex2f(headX+12, headY);
    glVertex2f(headX+25, headY+5);
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
    glColor3f(0,0,0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(headX+3, headY+4);
    for(int i=0;i<=20;i++){
        float a = 2*3.1416f*i/20;
        glVertex2f(headX+3 + cos(a)*3,
                   headY+4 + sin(a)*3);
    }
    glEnd();


    // =========================
    // LEGS + 3 FINGERS
    // =========================
    glLineWidth(2.5f);
    glBegin(GL_LINES);

    // left leg
    glVertex2f(-10,-35);
    glVertex2f(-10,-55);

    // left toes
    glVertex2f(-10,-55); glVertex2f(-18,-60);
    glVertex2f(-10,-55); glVertex2f(-10,-63);
    glVertex2f(-10,-55); glVertex2f(-2,-60);

    // right leg
    glVertex2f(10,-35);
    glVertex2f(10,-55);

    // right toes
    glVertex2f(10,-55); glVertex2f(2,-60);
    glVertex2f(10,-55); glVertex2f(10,-63);
    glVertex2f(10,-55); glVertex2f(18,-60);

    glEnd();

    glPopMatrix();
}
