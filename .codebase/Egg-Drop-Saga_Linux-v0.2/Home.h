#ifndef HOME_H
#define HOME_H
const float GAME_WIDTH  = 1280.0f;
const float GAME_HEIGHT = 720.0f;

class Home {
public:
    Home();

    void update();
    void draw();
    void handleKeyboard(unsigned char key, int x, int y);
    void handleMouse(int button, int state, int x, int y);

private:
    void drawTitle();
    void drawPlayButton();
    void drawInstructions();

    float buttonScale;
    float buttonPulseTime;

    float buttonX, buttonY;
    float buttonWidth, buttonHeight;
};

#endif
