#include "widget_pro_1.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    widget_pro_1 w;
    w.setWindowTitle("task_manager");
    w.resize(500, 400);
    return a.exec();
}
