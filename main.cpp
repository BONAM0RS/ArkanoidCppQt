
#include <QApplication>
#include "breakout.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Breakout window;

    return app.exec();
}

