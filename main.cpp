#include "mainwindow.h"
#include <QApplication>
#include<QtMultimedia/QMediaPlayer>
#include<QUrl>
int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
