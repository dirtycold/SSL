#include "SSLWidget.h"
#include <QTreeWidget>
#include <QLayout>
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

#include <QDebug>

SSLWidget::SSLWidget(QWidget* parent)
    : QWidget(parent)
{
    static auto& controller = SSLController::instance();

    auto list = new QTreeWidget;
    list->setAnimated(true);
    list->setHeaderLabel("Profiles");

    auto reloadButton = new QToolButton;
    reloadButton->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)));

    typedef std::map<QString, QString> ConfigMap;
    static ConfigMap configList;

    auto reload = [list]()
    {
        list->clear();
        configList.clear();

        auto findConfig = [](const QString& folder)
        {
            ConfigMap map;
            QDir dir(folder);
            const auto infoList = dir.entryInfoList({"*.json"}, QDir::Filter::Files, QDir::SortFlag::Name);

            for(const auto& info : infoList)
            {
                map.insert({info.baseName(), info.absoluteFilePath()});
            }

            return map;
        };

        auto systemItem = new QTreeWidgetItem({"System"});
        list->addTopLevelItem(systemItem);
        static const QString systemPath ("/etc/shadowsocks");
        auto systemConfigList = findConfig(systemPath);
        for (const auto& key : systemConfigList)
        {
            systemItem->addChild(new QTreeWidgetItem({key.first}));
        }
        configList.insert(systemConfigList.cbegin(), systemConfigList.cend());
        systemItem->setFlags(Qt::ItemIsEnabled);

        auto userItem = new QTreeWidgetItem({"User"});
        list->addTopLevelItem(userItem);
        auto userFolder = QDir::home();
        userFolder.cd(".shadowsocks");
        static const QString userPath(userFolder.absolutePath());

        auto userConfigList = findConfig(userPath);
        for (const auto& key : userConfigList)
        {
            userItem->addChild(new QTreeWidgetItem({key.first}));
        }
        configList.insert(userConfigList.cbegin(), userConfigList.cend());
        userItem->setFlags(Qt::ItemIsEnabled);

        list->expandAll();
    };
    connect(reloadButton, &QToolButton::clicked, reload);

    auto startButton = new QPushButton;
    startButton->setText("Start");
    startButton->setCheckable(true);

    auto start = [list, reloadButton, startButton](bool state)
    {
        auto item = list->currentItem();
        if(item == nullptr)
            return;

        auto key = item->text(0);
        if(configList.find(key) == configList.cend())
            return;

        auto value = configList.at(key);

        if(state)
        {
            controller.setConfig(value);
            controller.start();
            item->setIcon(0, QIcon(QApplication::style()->standardIcon(QStyle::SP_DialogYesButton)));
            list->setEnabled(false);
            reloadButton->setEnabled(false);
            if(controller.started())
                startButton->setText("Stop");
            else
                startButton->setChecked(false);
        }
        else
        {
            controller.stop();
            item->setIcon(0, QIcon());
            list->setEnabled(true);
            reloadButton->setEnabled(true);
            startButton->setText("Start");
        }
    };
    connect(startButton, &QPushButton::clicked, start);

    auto buttonLayout  = new QHBoxLayout;
    buttonLayout->addWidget(reloadButton);
    buttonLayout->addWidget(startButton);

    auto layout = new QVBoxLayout;
    layout->addWidget(list);
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
        break;
    }
    default:
        QWidget::changeEvent(event);
        break;
    }
}
