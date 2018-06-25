#pragma once

#include <QWidget>

class SSLWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SSLWidget(QWidget *parent = nullptr);
    ~SSLWidget();

private:
    class SSLWidgetPrivate;
    SSLWidgetPrivate *p;

protected:
    void changeEvent(QEvent *event);
};
