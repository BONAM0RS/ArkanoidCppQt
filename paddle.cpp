
#include <QDebug>
#include "paddle.h"


Paddle::Paddle(const int CUSTOM_WINDOW_WIDTH, const int CUSTOM_WINDOW_HEIGHT, const float SCALE_WINDOW_WIDTH, const float SCALE_WINDOW_HEIGHT)
{
    xDir = 0;

    int wSize = SCALE_WINDOW_WIDTH * SIZE_WIDTH;
    int hSize = SCALE_WINDOW_HEIGHT * SIZE_HEIGHT;

    xPos = (CUSTOM_WINDOW_WIDTH - wSize) / 2;
    yPos = CUSTOM_WINDOW_HEIGHT - SCALE_WINDOW_HEIGHT * Y_OFFSET;

    edgeRight = CUSTOM_WINDOW_WIDTH;

    rectangle.setRect(xPos, yPos, wSize, hSize);

    qDebug() << "Paddle created";
}


Paddle::~Paddle()
{
    qDebug() << "Paddle deleted";
}


void Paddle::initState()
{
    rectangle.moveTo(xPos, yPos);

    qDebug() << "Paddle replaced at initial position";
}


void Paddle::setDirX(int x)
{
    xDir = x;
}


void Paddle::moveAxisX()
{
    if ( (rectangle.left() >= 0 && xDir == -1) || (rectangle.right() <= edgeRight && xDir == 1) )
    {
        rectangle.translate(xDir * SPEED, 0);
    }
}

