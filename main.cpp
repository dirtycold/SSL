#include <QApplication>
#include "SSLWidget.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    SSLWidget w;
    w.show();
    return app.exec();
}
