#include "SSLController.h"

#include <QProcess>
#include <QDebug>

class SSLController::SSLControllerPrivate
{
    friend class SSLController;
    QString config;
    QProcess proc;
};

SSLController::SSLController(QObject *parent)
    :QObject(parent), p(new SSLControllerPrivate)
{
    p->proc.setProgram("/usr/local/bin/ss-local");
    p->proc.setProcessChannelMode(QProcess::ForwardedChannels);

    connect(&p->proc, &QProcess::stateChanged, [this](QProcess::ProcessState procState){
        auto state = State (procState);
        emit stateChanged(state);
    });
}

SSLController::~SSLController()
{
    stop();
    p->proc.waitForFinished();
    delete p;
}

void SSLController::setConfig(const QString &config)
{
    p->config = config;
}

const QString &SSLController::config() const
{
    return p->config;
}

SSLController::State SSLController::state() const
{
    auto state = State (p->proc.state());
    return state;
}

void SSLController::start()
{
    if (state() == Started)
        stop();

    p->proc.setArguments({"-c", p->config});
    p->proc.start();
    // p->proc.waitForStarted();
}

void SSLController::stop()
{
    p->proc.terminate();
    // p->proc.waitForFinished();
}

