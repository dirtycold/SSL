#pragma once

#include <QString>
#include <QDir>

static const QString folder = QStringLiteral("shadowsocks");
static const QString program = QStringLiteral("ss-local");

class SSLPathHelper
{
public:
    static QString systemPath()
    {
#if defined(Q_OS_WIN)
        auto systemPath = QString("");
#elif defined(Q_OS_MACOS)
        auto systemPath = QString("/usr/local/etc/") + folder;
#else
        auto systemPath = QString("/usr/local/") + folder;
#endif
        return systemPath;
    }

    static QString userPath()
    {
        auto userFolder = QDir::home();
        userFolder.cd("." + folder);
        return userFolder.absolutePath();
    }

    static QString executablePath()
    {
#if defined(Q_OS_WIN)
        auto execPath = QString("");
#elif defined(Q_OS_MACOS)
        auto execPath = QString("/usr/local/bin/") + program;
#else
        auto execPath = QString("/usr/bin/") + program;
#endif
        return execPath;
    }
};
