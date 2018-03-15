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

#include "SSLController.h"

#include <QDebug>

SSLWidget::SSLWidget(QWidget* parent)
    : QWidget(parent)
{
    static auto& controller = SSLController::instance();

    auto list = new QTreeWidget;
    list->setHeaderLabel("Profiles");

    auto reloadButton = new QToolButton;
    reloadButton->setIcon(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)));

    typedef std::map<QString, QString> ConfigMap;
    static ConfigMap configList;

    auto reload = [list]()
    {
        list->clear();
        configList.clear();

        auto systemItem = new QTreeWidgetItem({"System"});
        list->addTopLevelItem(systemItem);

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

        static const QString systemPath ("/etc/shadowsocks");
        auto systemConfigList = findConfig(systemPath);
        for (const auto& key : systemConfigList)
        {
            systemItem->addChild(new QTreeWidgetItem({key.first}));
        }
        configList.insert(systemConfigList.cbegin(), systemConfigList.cend());

        systemItem->setExpanded(true);

        auto userItem = new QTreeWidgetItem({"User"});
        list->addTopLevelItem(userItem);

    };
    connect(reloadButton, &QToolButton::clicked, reload);

    auto startButton = new QPushButton;
    startButton->setText("Start");
    startButton->setCheckable(true);

    auto start = [list, startButton](bool state)
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
            if(controller.started())
                startButton->setText("Stop");
            else
                startButton->setChecked(false);
        }
        else
        {
            controller.stop();
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
}
