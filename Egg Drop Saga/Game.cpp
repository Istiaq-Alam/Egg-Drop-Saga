#include "Game.h"
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <math.h>

void Game::drawText(float x, float y, const char* text)
{
    // Save current matrices
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

    // Restore matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Game::init() {
    score = 0;
    lives = 3;
    state = HOME;

    bucket = Bucket(GAME_WIDTH / 2 - 60, GAME_HEIGHT * 0.03f);
    currentEgg = nullptr;
    lastTime = glutGet(GLUT_ELAPSED_TIME);

    isPaused = false;
    pauseTimer = 0;

    chickens.clear();
    float wireY = GAME_HEIGHT * 0.7f;

    float spacing = GAME_WIDTH / 9.0f;

    for (int i = 0; i < 5; i++)
    {
        float xPos = spacing * (i + 1);
        chickens.push_back(Chicken(xPos, wireY));
    }
    srand(time(0));
}

void Game::spawnEgg() {
    int index = rand() % chickens.size();
    float x = chickens[index].getX();
    float y = chickens[index].getY() - 40;

    float speed = 1.9f + (score * 0.1f);
    if (speed > 5.0f) speed = 5.0f;

    currentEgg = new Egg(x, y, speed);
}

void Game::checkCollisions() {
    if (currentEgg == nullptr) return;

    // Check collision with bucket
    if (bucket.checkCollision(*currentEgg)) {
        score++;
        delete currentEgg;
        currentEgg = nullptr;
    }
    // Check collision with ground
    else if (currentEgg->y <= GAME_HEIGHT * 0.07f) {
        lives--;
        delete currentEgg;
        currentEgg = nullptr;
        if (lives <= 0) state = GAME_OVER;
    }
}


void Game::update() {
    background.update();
    if (isPaused) {
        return; // stop game logic while paused
    }

    if (state != PLAYING) return;

    if (currentEgg == nullptr)
        spawnEgg();

    if (currentEgg != nullptr)
        currentEgg->update();

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

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    background.draw(GAME_WIDTH, GAME_HEIGHT);

    if (state == HOME) {
        drawText(580, 450, "Egg Drop Saga");
        drawText(580, 400, "Press S to Start");
        drawText(500, 350, "Use A/D or Arrows to Move Bucket");
        drawText(500, 300, "Catch Eggs Before They Hit Ground");
        drawText(580, 250, "Press Q to Quit");
    }
    else if (state == PLAYING) {
        // Draw wire
        float wireY = GAME_HEIGHT * 0.68f;

        glColor3f(0.2, 0.2, 0.2);
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2f(GAME_WIDTH * 0.01f, wireY);
        glVertex2f(GAME_WIDTH * 0.61f, wireY);
        glEnd();

        // Draw chickens
        for (auto& c : chickens)
            c.draw();

        // Draw bucket
        bucket.draw();

        // Draw current egg
        if (currentEgg != nullptr)
            currentEgg->draw();

        // Draw Score
        std::stringstream ss;
        ss << "Score: " << score;
        drawText(30, GAME_HEIGHT - 40, ss.str().c_str());

        // Old numeric display
        // ss.str("");
        // ss << "Lives: " << lives;
        // drawText(700, 570, ss.str().c_str());
        float startX = 720;
        float startY = 615;
        float heartSize = 11.0f; // Adjust for size

        for (int i = 0; i < lives; i++) {
            drawHeart(startX + i * 23, startY, heartSize);
        }

        if (isPaused)
        drawText(580, 400, "PAUSED");

        else if (pauseTimer > 0) {
        drawText(580, 400, "RESUMED");
        pauseTimer--;
        }

    }
    else if (state == GAME_OVER) {
        drawText(580, 450, "GAME OVER");
        std::stringstream ss;
        ss << "Final Score: " << score;
        drawText(580, 350, ss.str().c_str());
        drawText(570, 300, "Press R to Play Again");
        drawText(580, 250, "Press Q to Quit");
    }

    glutSwapBuffers();
}

void Game::handleInput(unsigned char key) {
    if (state == HOME) {
        if (key == 's') state = PLAYING;
        if (key == 'q') exit(0);
    }
    else if (state == GAME_OVER) {
        if (key == 'r') init();
        if (key == 'q') exit(0);
    }
    else if (state == PLAYING) {
    if (key == 32) { // Spacebar ASCII
        isPaused = !isPaused;  // toggle pause
        if (!isPaused) {
            pauseTimer = 120; // show "Resumed" for 2 seconds
            }
        }

    else if (!isPaused) {
        if (key == 'a') bucket.moveLeft();
        if (key == 'd') bucket.moveRight();
            }

    }
}

void Game::handleSpecialInput(int key) {

    if(state != PLAYING)
        return;

    if (key == 32) { // Spacebar ASCII
        isPaused = !isPaused;  // toggle pause
        if (!isPaused) {
            pauseTimer = 120; // show "Resumed" for 2 seconds
            }
        }
    else if (!isPaused) {
        if(key == GLUT_KEY_LEFT)
            bucket.moveLeft();

        if(key == GLUT_KEY_RIGHT)
            bucket.moveRight();
    }
}
