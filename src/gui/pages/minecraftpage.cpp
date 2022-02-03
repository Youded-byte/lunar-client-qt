//
// Created by nils on 12/2/21.
//

#include "minecraftpage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <gui/widgets/widgetutils.h>

MinecraftPage::MinecraftPage(Config &config, QWidget *parent) : ConfigurationPage(config, parent) {
    customMinecraftDir = new QCheckBox(QStringLiteral("Custom .minecraft Directory"));
    minecraftPathChooser = new FileChooser(QFileDialog::Directory);

    serverToJoin = new QLineEdit();
    serverToJoin->setDisabled(true);

    connect(serverToJoin, &QLineEdit::returnPressed, [this](){serverToJoin->clearFocus();});

    joinServerOnLaunch = new QCheckBox(QStringLiteral("Join Server On Launch"));

    connect(joinServerOnLaunch, &QCheckBox::toggled, serverToJoin, &QLineEdit::setEnabled);

    QVBoxLayout* serverContainer = new QVBoxLayout();
    serverContainer->setSpacing(6);
    serverContainer->addWidget(joinServerOnLaunch, 0, Qt::AlignHCenter);
    serverContainer->addWidget(serverToJoin);

    useAutoggMessage = new QCheckBox(QStringLiteral("AutoGG Message"));
    autoggMessage = new QLineEdit();
    autoggMessage->setPlaceholderText(QStringLiteral("gg"));

    useLevelHeadPrefix = new QCheckBox(QStringLiteral("LevelHead Prefix"));
    levelHeadPrefix = new QLineEdit();
    levelHeadPrefix->setPlaceholderText(QStringLiteral("Level: "));

    useLevelHeadNick = new QCheckBox(QStringLiteral("LevelHead Nick Level (No level is shown with -1)"));
    levelHeadNickLevel = new QLineEdit();
    levelHeadNickLevel->setPlaceholderText(QString::number(-1));

    


    useCosmetics = new QCheckBox(QStringLiteral("Enable Cosmetics"));
    unlockCosmetics = new QCheckBox(QStringLiteral("Unlock All Cosmetics"));

    connect(useCosmetics, &QCheckBox::toggled, unlockCosmetics, &QCheckBox::setEnabled);

    windowWidth = new QSpinBox();
    windowHeight = new QSpinBox();

    windowWidth->setMinimum(0);
    windowWidth->setMaximum(99999);

    windowHeight->setMinimum(0);
    windowHeight->setMaximum(99999);

    QHBoxLayout* windowResContainer = new QHBoxLayout();
    windowResContainer->setSpacing(30);
    windowResContainer->addWidget(new QLabel(QStringLiteral("Window Width")));
    windowResContainer->addWidget(windowWidth, 1);
    windowResContainer->addWidget(new QLabel(QStringLiteral("Window Height")));
    windowResContainer->addWidget(windowHeight, 1);


    QHBoxLayout* betterHurtCamContainer = new QHBoxLayout();
    QLabel* betterHurtCamLabel = new QLabel();
    betterHurtCamValue = new QSlider(Qt::Horizontal);
    betterHurtCamValue->setMinimum(0);
    betterHurtCamValue->setMaximum(36);
    betterHurtCamValue->setPageStep(1);
    useBetterHurtCam = new QCheckBox(QStringLiteral("Better Hurtcam"));
    connect(betterHurtCamValue, &QSlider::valueChanged, [betterHurtCamLabel](double val) {betterHurtCamLabel->setText("BetterHurtCam Value:  " + QString::number(val)); });
    connect(useBetterHurtCam, &QCheckBox::toggled, useBetterHurtCam, &QCheckBox::setEnabled);
    betterHurtCamContainer->addWidget(useBetterHurtCam);
    betterHurtCamContainer->addWidget(betterHurtCamLabel, 0, Qt::AlignHCenter);
    betterHurtCamContainer->setSpacing(18);
    betterHurtCamContainer->addWidget(betterHurtCamValue);
        
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(40);
    mainLayout->addLayout(WidgetUtils::createOptional(customMinecraftDir, minecraftPathChooser));
    mainLayout->addLayout(serverContainer);
    mainLayout->addLayout(windowResContainer);
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addLayout(WidgetUtils::createOptional(useAutoggMessage, autoggMessage));
    mainLayout->addLayout(hLayout);
    QHBoxLayout* h2Layout = new QHBoxLayout;
    h2Layout->addLayout(WidgetUtils::createOptional(useLevelHeadPrefix, levelHeadPrefix));
    h2Layout->addLayout(WidgetUtils::createOptional(useLevelHeadNick, levelHeadNickLevel));
    //mainLayout->addLayout(WidgetUtils::createOptional(useBetterHurtCam, betterHurtCamLabel));
    mainLayout->addLayout(h2Layout);
    mainLayout->addLayout(betterHurtCamContainer);
    mainLayout->addWidget(useCosmetics, 1, Qt::AlignHCenter);
    mainLayout->addWidget(unlockCosmetics, 0, Qt::AlignCenter);
    mainLayout->addStretch(1);

    setLayout(mainLayout);
}

QString MinecraftPage::title() {
    return QStringLiteral("Minecraft");
}

QIcon MinecraftPage::icon() {
    return QIcon(":/res/icons/minecraft.svg");
}

void MinecraftPage::apply() {
    config.useCustomMinecraftDir = customMinecraftDir->isChecked();
    config.customMinecraftDir = minecraftPathChooser->getPath();

    config.joinServerOnLaunch = joinServerOnLaunch->isChecked();
    config.serverIp = serverToJoin->text();

    config.useLevelHeadPrefix = useLevelHeadPrefix->isChecked();
    config.levelHeadPrefix = levelHeadPrefix->text();

    config.useLevelHeadNick = useLevelHeadNick->isChecked();
    config.levelHeadNickLevel = levelHeadNickLevel->text().toInt();

    config.useBetterHurtCam = useBetterHurtCam->isChecked();
    config.betterHurtCamValue = (double)betterHurtCamValue->value();

    config.useAutoggMessage = useAutoggMessage->isChecked();
    config.autoggMessage = autoggMessage->text();

    config.useCosmetics = useCosmetics->isChecked();
    config.unlockCosmetics = unlockCosmetics->isChecked();

    config.windowWidth = windowWidth->value();
    config.windowHeight = windowHeight->value();
}

void MinecraftPage::load() {
    customMinecraftDir->setChecked(config.useCustomMinecraftDir);
    minecraftPathChooser->setPath(config.customMinecraftDir);

    joinServerOnLaunch->setChecked(config.joinServerOnLaunch);
    serverToJoin->setText(config.serverIp);

    useLevelHeadPrefix->setChecked(config.useLevelHeadPrefix);
    levelHeadPrefix->setText(config.levelHeadPrefix);

    useLevelHeadNick->setChecked(config.useLevelHeadNick);
    levelHeadNickLevel->setText(QString::number(config.levelHeadNickLevel));

    useBetterHurtCam->setChecked(config.useBetterHurtCam);
    betterHurtCamValue->setValue(config.betterHurtCamValue);

    useAutoggMessage->setChecked(config.useAutoggMessage);
    autoggMessage->setText(config.autoggMessage);

    useCosmetics->setChecked(config.useCosmetics);
    unlockCosmetics->setChecked(config.unlockCosmetics);

    windowWidth->setValue(config.windowWidth);
    windowHeight->setValue(config.windowHeight);
}