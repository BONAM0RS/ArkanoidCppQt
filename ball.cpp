
#include <QDebug>
#include "ball.h"


Ball::Ball(const int CUSTOM_WINDOW_WIDTH, const int CUSTOM_WINDOW_HEIGHT, const float SCALE_WINDOW_WIDTH, const float SCALE_WINDOW_HEIGHT)
{
    xDir = 1;
    yDir = -1;

    int wSize = SCALE_WINDOW_WIDTH * SIZE_WIDTH;
    int hSize = SCALE_WINDOW_HEIGHT * SIZE_HEIGHT;

    xPos = (CUSTOM_WINDOW_WIDTH - wSize) / 2;
    yPos = CUSTOM_WINDOW_HEIGHT - SCALE_WINDOW_HEIGHT * Y_OFFSET;

    edgeRight = CUSTOM_WINDOW_WIDTH;
    edgeBottom = CUSTOM_WINDOW_HEIGHT;

    rectangle.setRect(xPos, yPos, wSize, hSize);

    qDebug() << "Ball created";
}


Ball::~Ball()
{
    qDebug() << "Ball deleted";
}


void Ball::initState()
{
    xDir = 1;
    yDir = -1;

    rectangle.moveTo(xPos, yPos);

    qDebug() << "Ball replaced at initial position";
}


void Ball::setDirX(int x)
{
    xDir = x;
}


void Ball::setDirY(int y)
{
    yDir = y;
}


void Ball::moveAxisXY()
{
    if (rectangle.top()   <= 0)         { yDir =  1; }  // rectangle.y()
    if (rectangle.left()  <= 0)         { xDir =  1; }  // rectangle.x()
    if (rectangle.right() >= edgeRight) { xDir = -1; }  // rectangle.x() + wSize

    // for test purposes
    // if (rectangle.bottom() >= edgeBottom - 100) { yDir = -1; }
    // qDebug() << "x, y =" << rectangle.x() << ',' << rectangle.y();

    rectangle.translate(xDir * SPEED, yDir * SPEED);
}

