#pragma once

#include <memory>
#include <QObject>

class SSLController : public QObject
{
    Q_OBJECT
public:
    explicit SSLController(QObject *parent = nullptr);
    ~SSLController();

    void setConfig(const QString& config);
    const QString& config() const;

    enum State {Stopped, Starting, Started};
    State state() const;

public slots:
    void start();
    void stop();

signals:
    void stateChanged(State state);

private:
    class SSLControllerPrivate;
    SSLControllerPrivate *p;
};
