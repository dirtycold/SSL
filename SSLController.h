#pragma once

#include <memory>
#include <QString>

class SSLController
{
public:
    static SSLController& instance();

    void setConfig(const QString& config);
    void start();
    void stop();
    bool started() const;

private:
    SSLController();
    ~SSLController() = default;

    SSLController(const SSLController&) = delete;
    SSLController& operator=(const SSLController&) = delete;

    class SSLControllerPrivate;
    std::unique_ptr<SSLControllerPrivate> p;
};
