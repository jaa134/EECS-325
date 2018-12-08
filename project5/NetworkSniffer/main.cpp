/*
 * Jacob Alspaw
 * jaa134@case.edu
 * 12/07/2018
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
