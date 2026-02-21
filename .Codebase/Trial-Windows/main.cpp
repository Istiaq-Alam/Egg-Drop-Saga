/*
 * Egg-Catcher Game
 *
 * Written by Istiak Alam - Tanveer Ratul [2026]
 *
 */

#include <GL/glut.h>
#include "Game.h"

Game game;
const float GAME_WIDTH = 800.0f;
const float GAME_HEIGHT = 600.0f;

void reshape(int width, int height)
{
    float windowRatio = (float)width / (float)height;
    float gameRatio = GAME_WIDTH / GAME_HEIGHT;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (windowRatio > gameRatio)
    {
        float newWidth = GAME_HEIGHT * windowRatio;
        gluOrtho2D(0, newWidth, 0, GAME_HEIGHT);
    }
    else
    {
        float newHeight = GAME_WIDTH / windowRatio;
        gluOrtho2D(0, GAME_WIDTH, 0, newHeight);
    }

    glMatrixMode(GL_MODELVIEW);
}

void display() {
    game.render();
}

void timer(int value) {
    game.update();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    game.handleInput(key);

    if (key == 'f')
        glutFullScreen();

    if (key == 27) //ESC to small window
        glutReshapeWindow(800, 600);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));

    glutCreateWindow("Egg Drop Saga - Test Game Sequential Skeleton");
    glutFullScreen();

    glClearColor(0.5, 0.8, 1.0, 1.0); // Sky blue
    gluOrtho2D(0, 800, 0, 600);

    game.init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
