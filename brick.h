
#ifndef BRICK_H
#define BRICK_H


#include <QRect>


class Brick {

public:
    Brick(int xPos, int yPos, const float SCALE_WINDOW_WIDTH, const float SCALE_WINDOW_HEIGHT);
    ~Brick();

    void initState(int x, int y);
    void setDestroyState(bool state);

    bool getDestroyState() { return isDestroyed; };
    QRect getRectangle()   { return rectangle;   };

private:
    static const int SIZE_WIDTH = 90;
    static const int SIZE_HEIGHT = 30;

    bool isDestroyed;

    QRect rectangle;
};


#endif // BRICK_H

