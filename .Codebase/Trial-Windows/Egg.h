#ifndef EGG_H
#define EGG_H

class Egg {
public:
    float x, y;
    float radius;
    float speed;

    Egg(float x, float y, float speed);

    void update();
    void draw();
};

#endif
