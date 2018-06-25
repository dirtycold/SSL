#include <QApplication>
#include "SSLWidget.h"

int main(int argc, char* argv[])
{
    QApplication app (argc, argv);
    app.setQuitOnLastWindowClosed(false);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    SSLWidget widget;
    widget.show();
    return app.exec();
}
