#include "SSLWidget.h"

#include <QLayout>
#include <QListWidget>
#include <QPushButton>
#include <QToolButton>
#include <QStyle>
#include <QApplication>
#include <QMap>
#include <QDir>
#include <QFileInfo>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>

#include "SSLController.h"
#include "SSLPathHelper.h"

#include <QDebug>

class SSLWidget::SSLWidgetPrivate
{
    friend class SSLWidget;
    SSLController controller;
};

SSLWidget::SSLWidget(QWidget *parent)
    :QWidget(parent), p(new SSLWidgetPrivate)
{
    auto listWidget = new QListWidget;
    auto reloadButton = new QToolButton;
    reloadButton->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)));

    typedef QMap<QString, QString> ConfigMap;
    static ConfigMap configMap;

    auto reload = [listWidget]()
    {
        listWidget->clear();
        configMap.clear();

        auto findConfig = [](const QString& folder, const QString& comment = QString())
        {
            ConfigMap map;
            QDir dir(folder);
            const auto infoList = dir.entryInfoList({"*.json"}, QDir::Filter::Files, QDir::SortFlag::Name);
            for (const auto& info : infoList)
            {
                auto name = info.baseName();
                if (!comment.isEmpty())
                    name += " (" + comment + ")";
                map.insert(name, info.absoluteFilePath());
            }
            configMap.unite(map);
            return map.keys();
        };

        listWidget->addItems(findConfig(SSLPathHelper::systemPath(), "System"));
        listWidget->addItems(findConfig(SSLPathHelper::userPath()));
    };
    connect(reloadButton, &QToolButton::clicked, reload);

    auto startButton = new QPushButton;
    startButton->setText("Start");
    startButton->setCheckable(true);

    auto start = [this, listWidget](bool state)
    {
        if(state)
        {
            auto key = listWidget->currentItem()->text();
            auto value = configMap.value(key);
            p->controller.setConfig(value);
            p->controller.start();
        }
        else
        {
            p->controller.stop();
        }

    };
    connect(startButton, &QPushButton::clicked, start);

    auto stateChanged = [listWidget, reloadButton, startButton](SSLController::State state)
    {
        switch (state)
        {
        case SSLController::State::Starting:
        case SSLController::State::Started:
            {
                listWidget->setEnabled(false);
                reloadButton->setEnabled(false);
                startButton->setText("Stop");
                startButton->setChecked(true);
                break;
            }
        case SSLController::State::Stopped:
        default:
            {
                listWidget->setEnabled(true);
                reloadButton->setEnabled(true);
                startButton->setChecked(false);
                startButton->setText("Start");
                break;
            }
        }
    };
    connect(&p->controller, &SSLController::stateChanged, stateChanged);

    auto buttonLayout  = new QHBoxLayout;
    buttonLayout->setMargin(3);
    buttonLayout->setSpacing(3);
    buttonLayout->addWidget(reloadButton);
    buttonLayout->addWidget(startButton);

    auto layout = new QVBoxLayout;
    layout->setMargin(3);
    layout->setSpacing(3);
    layout->addWidget(listWidget);
    layout->addLayout(buttonLayout);
    setLayout(layout);

    reload();

    auto showAction = new QAction("Show window", this);
    connect(showAction, &QAction::triggered, [this]()
    {
        showNormal();
        setFocus();
    });

    auto quitAction = new QAction("Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    auto menu = new QMenu(this);
    menu->addAction(showAction);
    menu->addAction(quitAction);

    auto tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation)));
    tray->setContextMenu(menu);
    tray->show();
}

SSLWidget::~SSLWidget()
{
    delete p;
}

void SSLWidget::changeEvent(QEvent* event)
{
    switch(event->type()) {
    case QEvent::WindowStateChange:
    {
        if(isMinimized())
        {
            hide();
            event->ignore();
        }
        else
        {
            show();
            event->accept();
        }
        break;
    }
    default:
        QWidget::changeEvent(event);
        break;
    }
}
