#include "Game.h"
#include "Home.h"
#include "Chicken.h"
#include "AudioManager.h"
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <cmath>

#define GROUND_Y (GAME_HEIGHT * 0.03f)

void Game::drawText(float x, float y, const char* text)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, GAME_WIDTH, 0, GAME_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);

    while (*text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Game::init()
{
    score = 0;
    lives = 3;
    state = HOME;
    AudioManager::playHomeMusic();
    Mix_VolumeMusic(32);          // 0-128
    Mix_Volume(-1, 32);           // all channels


    if (currentEgg != nullptr) {
        delete currentEgg;
        currentEgg = nullptr;
    }

    bucket = Bucket(GAME_WIDTH / 2 - 60, GAME_HEIGHT * 0.03f);
    lastTime = glutGet(GLUT_ELAPSED_TIME);

    isPaused = false;
    pauseTimer = 0;

    chickens.clear();

    float wireY = GAME_HEIGHT * 0.74f;
    float spacing = GAME_WIDTH / 6.0f;

    for (int i = 0; i < 5; i++)
    {
        float xPos = spacing * (i + 1);
        chickens.push_back(Chicken(xPos, wireY));
    }


    srand(time(0));
}

void Game::spawnEgg()
{
    AudioManager::playChickenDrop();

    int index = rand() % chickens.size();

    // 🔥 Trigger animation for that chicken
    //chickens[index].startLayEgg();

    float x = chickens[index].getX();
    float y = chickens[index].getY() - 40;

    float speed = 1.9f + (score * 0.1f);
    if (speed > 5.0f) speed = 5.0f;

    currentEgg = new Egg(x, y, speed);
}

void Game::checkCollisions()
{
    if (currentEgg == nullptr) return;

    // Catch by bucket
    if (!currentEgg->isBroken && bucket.checkCollision(*currentEgg))
    {
        AudioManager::playCatch();
        score++;
        delete currentEgg;
        currentEgg = nullptr;
        return;
    }

    // After break animation duration
    if (currentEgg->isBroken && currentEgg->breakTimer > 0.6f)
    {
        lives--;
        delete currentEgg;
        currentEgg = nullptr;

        if (lives <= 0){
            state = GAME_OVER;
            AudioManager::playGameOverMusic();
        }
    }
}

void Game::update()
{
    background.update();

    if (state == HOME){
        home.update();
    }

    if (isPaused) return;

    if (state != PLAYING) return;
    // Update all chickens (animation)
    //for (auto& c : chickens)
    //c.update();
    if (currentEgg == nullptr)
        spawnEgg();

    if (currentEgg != nullptr)
        currentEgg->update(GROUND_Y);

    checkCollisions();
}

// Gradient Shaded Heart
void drawHeart(float centerX, float centerY, float size)
{
    glShadeModel(GL_SMOOTH);

    // ===== Fill with Gradient =====
    glBegin(GL_TRIANGLE_FAN);

    // Center (brighter red)
    glColor3f(1.0f, 0.2f, 0.3f);
    glVertex2f(centerX, centerY);

    for (float t = 0; t <= 2 * M_PI; t += 0.02f)
    {
        float x = 17 * pow(sin(t), 3);
        float y = 14 * cos(t)
                - 5 * cos(2 * t)
                - 2 * cos(3 * t)
                - cos(4 * t);

        float finalX = centerX + (x / 20.0f) * size;
        float finalY = centerY + (y / 17.0f) * size;

        // Bottom darker, top brighter
        if (y < 0)
            glColor3f(0.7f, 0.0f, 0.0f);   // dark red
        else
            glColor3f(1.0f, 0.1f, 0.2f);   // bright red

        glVertex2f(finalX, finalY);
    }

    glEnd();

    // ===== Black Outline =====
    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    for (float t = 0; t <= 2 * M_PI; t += 0.02f)
    {
        float x = 16 * pow(sin(t), 3);
        float y = 13 * cos(t)
                - 5 * cos(2 * t)
                - 2 * cos(3 * t)
                - cos(4 * t);

        glVertex2f(centerX + (x / 20.0f) * size,
                   centerY + (y / 17.0f) * size);
    }
    glEnd();
}


void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // 🔥 Always reset projection every frame
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluOrtho2D(0, GAME_WIDTH, 0, GAME_HEIGHT);

    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();

    // Draw background first
    background.draw(GAME_WIDTH, GAME_HEIGHT);

    if (state == HOME)
    {
        home.draw();
    }

    else if (state == PLAYING)
    {
        // ================= WIRE =================
        float wireY = GAME_HEIGHT * 0.65f;
        glColor3f(0.6f, 0.3f, 0.2f);
        glBegin(GL_QUADS);
        glVertex2f(GAME_WIDTH * 0.01f, wireY);
        glVertex2f(GAME_WIDTH * 0.99f, wireY);
        glVertex2f(GAME_WIDTH * 0.99f, wireY + 20);
        glVertex2f(GAME_WIDTH * 0.01f, wireY + 20);
        glEnd();

        // ================= CHICKENS =================
        for (auto& c : chickens)
            c.draw();

        // ================= GROUND =================
        float groundY = 0;

        glColor3f(0.698f, 0.745f, 0.710f);
        glBegin(GL_QUADS);
        glVertex2f(GAME_WIDTH - 20, groundY);
        glVertex2f(GAME_WIDTH, groundY + 27);
        glVertex2f(GAME_WIDTH, groundY + 40);
        glVertex2f(GAME_WIDTH - 20, groundY + 40);
        glEnd();

        glColor3f(0.5f, 0.2f, 0.20f);
        glBegin(GL_QUADS);
        glVertex2f(0, groundY + 10);
        glVertex2f(GAME_WIDTH - 20, groundY + 10);
        glVertex2f(GAME_WIDTH, groundY + 40);
        glVertex2f(20, groundY + 40);
        glEnd();

        glColor3f(0.5f, 0.5f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(0, groundY);
        glVertex2f(GAME_WIDTH - 20, groundY);
        glVertex2f(GAME_WIDTH - 20, groundY + 10);
        glVertex2f(0, groundY + 10);
        glEnd();

        // ================= EGG =================
        if (currentEgg != nullptr)
            currentEgg->draw(GROUND_Y);

        // ================= BUCKET =================
        bucket.draw();

        // ================= UI =================
        std::stringstream ss;
        ss << "Score: " << score;
        drawText(30, GAME_HEIGHT - 40, ss.str().c_str());

        float startX = 720;
        float startY = 615;
        float heartSize = 11.0f;

        for (int i = 0; i < lives; i++)
            drawHeart(startX + i * 23, startY, heartSize);

        if (isPaused)
            drawText(370, 400, "PAUSED");
        else if (pauseTimer > 0)
        {
            drawText(370, 400, "RESUMED");
            pauseTimer--;
        }
    }

    else if (state == GAME_OVER)
    {
        drawText(370, 450, "GAME OVER");

        std::stringstream ss;
        ss << "Final Score: " << score;
        drawText(370, 400, ss.str().c_str());

        drawText(340, 350, "Press R to Home Screen");
        drawText(365, 300, "Press Q to Quit");
    }

    glutSwapBuffers();
}

void Game::handleInput(unsigned char key)
{
    if (state == HOME)
    {
        if (key == 's'){
        state = PLAYING;
        AudioManager::playGameMusic();
        }
        if (key == 'q') exit(0);
    }
    else if (state == GAME_OVER)
    {
        if (key == 'r'){
        init();
        AudioManager::playHomeMusic();
        }
        if (key == 'q') exit(0);
    }
    else if (state == PLAYING)
    {
        AudioManager::playGameMusic();
        if (key == 32) // Space
        {
            isPaused = !isPaused;
            if (!isPaused)
                pauseTimer = 120;
        }
        else if (!isPaused)
        {
            if (key == 'a') bucket.moveLeft();
            if (key == 'd') bucket.moveRight();
        }
    }
}

void Game::handleSpecialInput(int key)
{
    if (state != PLAYING || isPaused)
        return;

    if (key == GLUT_KEY_LEFT)
        bucket.moveLeft();

    if (key == GLUT_KEY_RIGHT)
        bucket.moveRight();
}
