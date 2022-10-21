
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QPainter>

#include "breakout.h"


Breakout::Breakout(QWidget *parent)
    : QWidget(parent)
{
    gameState = start;

    // Window settings
    this->resize(CUSTOM_WINDOW_WIDTH, CUSTOM_WINDOW_HEIGHT);
    this->setStyleSheet("background-color:black;");
    this->setWindowTitle("Arkanoid");
    this->show();
    qDebug() << "BREAKOUT OPENED";

    // Create game objects
    ball = new Ball(CUSTOM_WINDOW_WIDTH, CUSTOM_WINDOW_HEIGHT, SCALE_WINDOW_WIDTH, SCALE_WINDOW_HEIGHT);
    paddle = new Paddle(CUSTOM_WINDOW_WIDTH, CUSTOM_WINDOW_HEIGHT, SCALE_WINDOW_WIDTH, SCALE_WINDOW_HEIGHT);

    int count = 0;
    for (int y = 0; y < BRICK_ROWS; ++y)
    {
        for (int x = 0; x < BRICK_COLUMNS; ++x)
        {
            bricks[count] = new Brick(x * X_OFFSET + X_OFFSET_INITIAL, y * Y_OFFSET + Y_OFFSET_INITIAL, SCALE_WINDOW_WIDTH, SCALE_WINDOW_HEIGHT);
            count++;

            if (count == BRICKS_COUNT)
            {
                qDebug() << "Bricks created";
            }
        }
    }

    // Pre-initialize some variables to calculate collisions and cleaner code
    QRect ballRect   = ball   -> getRectangle();
    QRect paddleRect = paddle -> getRectangle();

    // Divide paddle width into 3 sides (left, center, right)
    paddleWidth = paddleRect.width();
    paddleWidthSide = paddleWidth / 3;  // 120 / 3  = 40

    // Find ball xPos & yPos centers
    ballWidth   = ballRect.width();
    ballHeight  = ballRect.height();

    ballWidthCenter  = ballWidth  / 2;  // 30 / 2 = 15
    ballHeightCenter = ballHeight / 2;  // 30 / 2 = 15
}


Breakout::~Breakout()
{
    delete ball;
    delete paddle;

    int count = 0;
    for (int i = 0; i < BRICKS_COUNT; ++i)
    {
        delete bricks[i];
        count++;

        if (count == BRICKS_COUNT)
        {
            qDebug() << "Bricks deleted";
        }
    }

    qDebug() << "BREAKOUT CLOSED";
}


void Breakout::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Space:  { playGame()  ; break; }
        case Qt::Key_P:      { pauseGame() ; break; }
        case Qt::Key_Escape: { qApp->exit(); break; }

        case Qt::Key_Left:   { isMoveLeft =  true; isMoveRight = false;  paddle->setDirX(-1);  break;}
        case Qt::Key_Right:  { isMoveRight = true; isMoveLeft =  false;  paddle->setDirX( 1);  break;}

        default: { QWidget::keyPressEvent(e); }
    }
}


void Breakout::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key())
    {
        case Qt::Key_Left:  { isMoveLeft = false; break; }
        case Qt::Key_Right: { isMoveRight = false; break; }
    }
}


void Breakout::playGame()
{
    if (gameState != play)
    {
        // Reset initial states if it's not the first run
        if (gameState != start)
        {
            ball   -> initState();
            paddle -> initState();

            for (int i = 0; i < BRICKS_COUNT; ++i)
            {
                bricks[i]->setDestroyState(false);
            }
        }
        // -----------------------------------------------------

        gameState = play;
        timerId = startTimer(TICK_DELAY);
    }
}


void Breakout::pauseGame()
{
    if (gameState != pause && gameState == play)
    {
        gameState = pause;
        killTimer(timerId);
        repaint();
    }
    else
    {
        gameState = play;
        timerId = startTimer(TICK_DELAY);
    }
}


void Breakout::loseGame()
{
    gameState = lose;
    killTimer(timerId);
}


void Breakout::winGame()
{
    gameState = win;
    killTimer(timerId);
}


void Breakout::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);

    moveObjects();
    checkCollision();
    repaint();
}


void Breakout::moveObjects()
{
    ball -> moveAxisXY();

    if (isMoveLeft == true || isMoveRight == true)
    {
        paddle -> moveAxisX();
    }
}


void Breakout::checkCollision()
{
    // Predefine rectangles for cleaner code
    QRect ballRect   = ball   -> getRectangle();
    QRect paddleRect = paddle -> getRectangle();

    // Lose condition
    if (ballRect.bottom() >= BOTTOM_EDGE)
    {
        loseGame();
    }

    // Win condition
    int count = 0;
    for (int i = 0; i < BRICKS_COUNT; ++i)
    {
        if (bricks[i]->getDestroyState() == true)
        {
            count++;
        }

        if (count == BRICKS_COUNT)
        {
            winGame();
        }
    }

    // Ball & Paddle collision
    if (ballRect.intersects(paddleRect))
    {
        // Find paddle left and right borders
        int paddlePosLeft = paddleRect.left();  //  xPos top left
        int paddleEdgeLeft  = paddlePosLeft + paddleWidthSide;
        int paddleEdgeRight = paddlePosLeft + paddleWidth - paddleWidthSide;  //  120 - 120 / 3 = 120 - 40 = 80

        // Find ball center
        int ballPosLeft   = ballRect.left();    //  xPos top left
        int ballPosCenter = ballPosLeft + ballWidthCenter;

        // Ball intersects Paddle left side
        if (ballPosCenter < paddleEdgeLeft)
        {
            ball->setDirX(-1);
        }

        // Ball intersects Paddle center side
        if (ballPosCenter > paddleEdgeLeft && ballPosCenter < paddleEdgeRight)
        {
            ball->setDirX(0);
        }

        // Ball intersects Paddle right side
        if (ballPosCenter > paddleEdgeRight)
        {
            ball->setDirX(1);
        }

        // Always bounce up
        ball->setDirY(-1);
    }

    // Ball & Brick collision
    for (int i = 0; i < BRICKS_COUNT; ++i)
    {
        // Predefine rectangles for cleaner code
        QRect brickRect = bricks[i] -> getRectangle();

        if (ballRect.intersects(brickRect))
        {
            if (bricks[i]->getDestroyState() == false)
            {
                int ballPosLeft = ballRect.left();   // xPos top left
                int ballPosTop  = ballRect.top();    // yPos top left

                //                top
                //     top left    0    top right
                //             0       0
                //         left         right
                //             0       0
                //  bottom left    0    bottom right
                //              bottom

                QPoint pointLeft   (ballPosLeft,             ballPosTop + ballHeightCenter);
                QPoint pointRight  (ballPosLeft + ballWidth, ballPosTop + ballHeightCenter);

                QPoint pointTop    (ballPosLeft + ballWidthCenter, ballPosTop);
                QPoint pointBottom (ballPosLeft + ballWidthCenter, ballPosTop + ballHeight);

                QPoint pointTopLeft     (ballRect.topLeft());
                QPoint pointTopRight    (ballRect.topRight());

                QPoint pointBottomLeft  (ballRect.bottomLeft());
                QPoint pointBottomRight (ballRect.bottomRight());

                // Ball's left points are inside the brick
                if (brickRect.contains(pointLeft) || brickRect.contains(pointTopLeft))
                {
                    ball->setDirX(1);  // so the ball bounces right
                }
                // Ball's right points are inside the brick
                else if (brickRect.contains(pointRight) || brickRect.contains(pointTopRight))
                {
                    ball->setDirX(-1); // so the ball bounces left
                }

                // Ball's top point is inside the brick
                if (brickRect.contains(pointTop))
                {
                    ball->setDirY(1);  // so the ball bounces down
                }
                // Ball's bottom points are inside the brick
                else if (brickRect.contains(pointBottom) || brickRect.contains(pointBottomLeft) || brickRect.contains(pointBottomRight))
                {
                    ball->setDirY(-1); // so the ball bounces up
                }

                bricks[i]->setDestroyState(true);
            }
        }
    }
}


void Breakout::paintEvent(QPaintEvent *e) {

    Q_UNUSED(e);
    QPainter painter(this);

    switch (gameState)
    {
        case start: { showMessage(&painter, "ARE YOU READY?", "Press 'Space' button to start")     ; drawObjects(&painter); break; }
        case pause: { showMessage(&painter, "GAME PAUSED"   , "Press 'P' button to continue" )     ; drawObjects(&painter); break; }

        case lose:  { showMessage(&painter, "GAME LOST :("  , "Press 'Space' button to try again") ; break; }
        case win:   { showMessage(&painter, "VICTORY! :)"   , "Press 'Space' button to replay"   ) ; break; }

        default:    { drawObjects(&painter); }
    }
}


void Breakout::showMessage(QPainter *painter, QString mainMsg, QString subMsg)
{
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    painter->setFont(font);

    painter->setPen(Qt::white);

    int mainTextWidth = fm.QFontMetrics::horizontalAdvance(mainMsg);
    int subTextWidth = fm.QFontMetrics::horizontalAdvance(subMsg);

    painter->drawText((CUSTOM_WINDOW_WIDTH / 2) - mainTextWidth / 2, CUSTOM_WINDOW_HEIGHT / 2, mainMsg);
    painter->drawText((CUSTOM_WINDOW_WIDTH / 2) - subTextWidth / 2, CUSTOM_WINDOW_HEIGHT - 25, subMsg);
}


void Breakout::drawObjects(QPainter *painter)
{
    painter->setBrush(Qt::blue);
    painter->drawEllipse( ball->getRectangle() );

    painter->setBrush(Qt::white);
    painter->drawRect( paddle->getRectangle() );

    for (int i = 0; i < BRICKS_COUNT; ++i)
    {
        if (bricks[i]->getDestroyState() == false)
        {
            painter->setBrush(Qt::red);
            painter->drawRect( bricks[i]->getRectangle() );
        }
    }
}

