#ifndef CHICKEN_H
#define CHICKEN_H

class Chicken {
private:
    float x, y;

public:
    Chicken(float x, float y);
    void draw();

    float getX() { return x; }
    float getY() { return y; }
};

#endif
