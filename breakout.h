
#ifndef BREAKOUT_H
#define BREAKOUT_H


#include <QWidget>

#include "ball.h"
#include "brick.h"
#include "paddle.h"


class Breakout : public QWidget {

public:
    Breakout(QWidget *parent = nullptr);
    ~Breakout();

protected:
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);

    void playGame();
    void pauseGame();
    void loseGame();
    void winGame();

    void timerEvent(QTimerEvent *);

    void moveObjects();
    void checkCollision();

    void paintEvent(QPaintEvent *);

    void showMessage(QPainter *, QString mainMsg, QString subMsg);
    void drawObjects(QPainter *);    

private:
    static const int TICK_DELAY = 4; // ms

    static const int DEFAULT_WINDOW_WIDTH = 800;
    static const int DEFAULT_WINDOW_HEIGHT = 600;

    static const int CUSTOM_WINDOW_WIDTH = 800;
    static const int CUSTOM_WINDOW_HEIGHT = 600;

    static const int BOTTOM_EDGE = CUSTOM_WINDOW_HEIGHT;

    static constexpr float SCALE_WINDOW_WIDTH = static_cast<float>(CUSTOM_WINDOW_WIDTH) / static_cast<float>(DEFAULT_WINDOW_WIDTH);
    static constexpr float SCALE_WINDOW_HEIGHT = static_cast<float>(CUSTOM_WINDOW_HEIGHT) / static_cast<float>(DEFAULT_WINDOW_HEIGHT);

    static const int X_OFFSET_INITIAL = SCALE_WINDOW_WIDTH * 55;
    static const int Y_OFFSET_INITIAL = SCALE_WINDOW_HEIGHT * 30;

    static const int X_OFFSET = SCALE_WINDOW_WIDTH * 100;
    static const int Y_OFFSET = SCALE_WINDOW_HEIGHT * 40;

    static const int BRICK_ROWS = 4;
    static const int BRICK_COLUMNS = 7;
    static const int BRICKS_COUNT = BRICK_ROWS * BRICK_COLUMNS;

    Brick *bricks[BRICKS_COUNT];
    Paddle *paddle;
    Ball *ball;

    int paddleWidth;
    int paddleWidthSide;

    int ballWidth;
    int ballHeight;

    int ballWidthCenter;
    int ballHeightCenter;

    enum gameStates
    {
        start,
        play,
        pause,
        lose,
        win
    };

    gameStates gameState;

    bool isMoveLeft;
    bool isMoveRight;

    int timerId;
};


#endif // BREAKOUT_H

