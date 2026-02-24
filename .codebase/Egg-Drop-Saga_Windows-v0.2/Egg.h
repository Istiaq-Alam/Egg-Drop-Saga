#ifndef EGG_H
#define EGG_H

class Egg {
public:
    float x, y;
    float speed;
    float radius;
    float breakTimer;
    bool isSquashing;
    float squashTimer;
    bool isBroken;

    Egg(float x, float y, float speed);

    void update(float groundY);
    void draw(float groundY);
};

#endif
