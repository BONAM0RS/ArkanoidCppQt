
#include <QDebug>
#include "brick.h"


Brick::Brick(int xPos, int yPos, const float SCALE_WINDOW_WIDTH, const float SCALE_WINDOW_HEIGHT)
{
    isDestroyed = false;

    int wSize = SCALE_WINDOW_WIDTH * SIZE_WIDTH;
    int hSize = SCALE_WINDOW_HEIGHT * SIZE_HEIGHT;

    rectangle.setRect(xPos, yPos, wSize, hSize);

    // qDebug() << "Brick created";
}


Brick::~Brick()
{
    // qDebug() << "Brick deleted";
}


void Brick::initState(int x, int y)
{
    rectangle.translate(x, y);
}


void Brick::setDestroyState(bool state)
{
    isDestroyed = state;
}

