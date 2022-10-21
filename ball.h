
#ifndef BALL_H
#define BALL_H


#include <QRect>


class Ball {

public:
    Ball(const int CUSTOM_WINDOW_WIDTH, const int CUSTOM_WINDOW_HEIGHT, const float SCALE_WINDOW_WIDTH, const float SCALE_WINDOW_HEIGHT);
    ~Ball();

    void initState();
    void setDirX(int x);
    void setDirY(int y);
    void moveAxisXY();

    QRect getRectangle() { return rectangle; };

private:
    static const int SIZE_WIDTH = 30;
    static const int SIZE_HEIGHT = 30;

    static const int Y_OFFSET = 140;

    static const int SPEED = 1;

    int xDir;
    int yDir;

    int edgeRight;
    int edgeBottom;

    int xPos;
    int yPos;

    QRect rectangle;
};


#endif // BALL_H

