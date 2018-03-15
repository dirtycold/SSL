#include "SSLController.h"

#include <QProcess>
#include <QDebug>

class SSLController::SSLControllerPrivate
{
    friend class SSLController;

    QString config;
    QProcess proc;
};

SSLController& SSLController::instance()
{
    static SSLController instance;
    return instance;
}

void SSLController::setConfig(const QString& config)
{
    p->config = config;
}

void SSLController::start()
{
    if(started())
        stop();

    p->proc.setArguments({"-c", p->config});
    p->proc.start();
    p->proc.waitForStarted();
}

void SSLController::stop()
{
    p->proc.terminate();
    p->proc.waitForFinished();
}

bool SSLController::started() const
{
    return (p->proc.state() != QProcess::NotRunning);
}

SSLController::SSLController(): p(new SSLControllerPrivate)
{
    p->proc.setProgram("ss-local");
    p->proc.setProcessChannelMode(QProcess::ForwardedChannels);
}
