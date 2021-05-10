#include <iostream>

#include "Python.h"

#include <QApplication>
#include <QTransform>

#include "mainwindow.h"

int main(int argv, char *args[])
{
    Q_INIT_RESOURCE(diagnostic); // 删掉也可以
#ifdef Q_OS_ANDROID
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argv, args);

    // We found Python interpreter here already contains the search path. So we don't set the module search path here.
    Py_Initialize();

    MainWindow mainWindow;
    mainWindow.setGeometry(100, 100, 800, 500);
    mainWindow.show();

    int exec_state = app.exec();

    if(Py_FinalizeEx() < 0){
        return 120;
    }
    return exec_state;
}