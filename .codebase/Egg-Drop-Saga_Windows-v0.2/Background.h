#ifndef BACKGROUND_H
#define BACKGROUND_H

class Background {
private:
    float cloudOffset;
    float sunAngle;

public:
    Background();
    void update();
    void draw(int screenWidth, int screenHeight);
};

#endif

