
#ifndef PADDLE_H
#define PADDLE_H


#include <QRect>


class Paddle {

public:
    Paddle(const int CUSTOM_WINDOW_WIDTH, const int CUSTOM_WINDOW_HEIGHT, const float SCALE_WINDOW_WIDTH, const float SCALE_WINDOW_HEIGHT);
    ~Paddle();

    void initState();
    void setDirX(int x);
    void moveAxisX();

    QRect getRectangle() { return rectangle; };

private:
    static const int SIZE_WIDTH = 120;
    static const int SIZE_HEIGHT = 30;

    static const int Y_OFFSET = 100;

    static const int SPEED = 2;

    int xDir;

    int edgeRight;

    int xPos;
    int yPos;

    QRect rectangle;
};


#endif // PADDLE_H

