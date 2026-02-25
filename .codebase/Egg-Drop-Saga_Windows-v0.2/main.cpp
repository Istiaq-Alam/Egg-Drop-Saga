/*
 * Egg-Catcher Game
 *
 * Written by Istiak Alam [2026]
 *
 */
#include <windows.h>
#include <GL/glut.h>
#include "Game.h"
#define IDI_ICON1 101
#include "AudioManager.h"

Game game;

void display() {
    game.render();
}

void reshape(int width, int height)
{
    float windowAspect = (float)width / height;
    float gameAspect = GAME_WIDTH / GAME_HEIGHT;

    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = width;
    int viewportHeight = height;

    if (windowAspect > gameAspect)
    {
        viewportWidth = height * gameAspect;
        viewportX = (width - viewportWidth) / 2;
    }
    else if (windowAspect < gameAspect)
    {
        viewportHeight = width / gameAspect;
        viewportY = (height - viewportHeight) / 2;
    }

    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GAME_WIDTH, 0, GAME_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
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
        glutReshapeWindow(1080, 780);
}

void specialKeys(int key, int x, int y) {
    game.handleSpecialInput(key);
}

int main(int argc, char** argv) {
//    AudioManager::init();
//    AudioManager::loadSounds();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutInitWindowSize(1080, 780);
    glutInitWindowPosition(300, 0);

    glutReshapeFunc(reshape);
    glutCreateWindow("Egg Drop Saga - Test Game Sequential Skeleton");

    //Icon setup
    HWND hwnd = GetActiveWindow();
    HICON hIcon = (HICON)LoadImage(
        GetModuleHandle(NULL),
        MAKEINTRESOURCE(IDI_ICON1),
        IMAGE_ICON,
        32, 32,
        LR_DEFAULTCOLOR
    );
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glClearColor(0.5, 0.8, 1.0, 1.0); // Sky blue
    gluOrtho2D(0, 800, 0, 640);

    game.init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, timer, 0);

    AudioManager::init();
    AudioManager::load();
    AudioManager::playHomeMusic();

    glutMainLoop();
    AudioManager::cleanup();
    return 0;
}
