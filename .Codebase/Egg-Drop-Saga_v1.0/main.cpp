/*
 * Egg-Catcher Game
 *
 * Written by Istiak Alam - Tanveer Ratul [2026]
 *
 */

#include <GL/glut.h>
#include "Game.h"

Game game;

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
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Egg Drop Saga - Test Game Sequential Skeleton");

    glClearColor(0.5, 0.8, 1.0, 1.0); // Sky blue
    gluOrtho2D(0, 800, 0, 600);

    game.init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}
