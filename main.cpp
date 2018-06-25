#include <QApplication>
#include "SSLWidget.h"

int main(int argc, char* argv[])
{
    QApplication app (argc, argv);
    app.setQuitOnLastWindowClosed(false);
    SSLWidget widget;
    widget.show();
    return app.exec();
}
