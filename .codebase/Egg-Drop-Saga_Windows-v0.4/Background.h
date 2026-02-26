#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <vector>



class Background {
private:
    float cloudOffset;
    float cloudOffset1;
    float cloudOffset2;
    float cloudOffset3;
    float sunAngle;
    float timeOfDay;
    float skySpeed;
    float windTime;
    struct Star {
    float x;
    float y;
    float size;
};

std::vector<Star> stars;

public:
    Background();
    void update();
    void draw(int screenWidth, int screenHeight);
};

#endif
