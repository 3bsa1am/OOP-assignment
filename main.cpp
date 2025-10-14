#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QPalette>
#include <QLinearGradient>
#include <QBrush>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPalette palette;
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0.0, QColor(0, 70, 180));
    gradient.setColorAt(1.0, QColor(150, 200, 255));
    palette.setBrush(QPalette::Window, QBrush(gradient));
    app.setPalette(palette);

    MainWindow w;
    w.show();

    return app.exec();
}
