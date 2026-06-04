#ifndef CHICKEN_H
#define CHICKEN_H

class Chicken {
private:

    float x, y;
    // Animation
    bool isLayingEgg = false;
    float animProgress = 0.0f;   // 0 → 1 transition

public:
    Chicken(float x, float y);
    void draw();
    void drawLegs();
    void update();
    void startLayEgg();   // trigger animation

    float getX() { return x; }
    float getY() { return y; }
};

#endif
