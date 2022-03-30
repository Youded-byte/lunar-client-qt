//
// Created by nils on 11/4/21.
//

#include "mainwindow.h"

#include <QGridLayout>
#include <QListWidgetItem>
#include <QComboBox>
#include <QStatusBar>
#include <QScrollArea>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QMessageBox>
#include <QFileSystemModel>
#include <QIODevice>
#include <QLabel>

#include "pages/configurationpage.h"
#include "pages/generalpage.h"
#include "pages/minecraftpage.h"
#include "launch/launcher.h"
#include "buildconfig.h"
#include "widgets/widgetutils.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), config(Config::load()), offlineLauncher(config){
    setWindowTitle(QStringLiteral("Lunar Client Qt v") + BuildConfig::VERSION);
    static QString icon = QStringLiteral("icon.ico");
    if (QFile::exists(icon))
        setWindowIcon(QIcon(icon));
    else {
        QString lcloc =
#if defined(Q_OS_WIN)
            QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + QStringLiteral("/Programs/lunarclient/Lunar Client.exe");
#elif defined(Q_OS_DARWIN)
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/lunarclient/Lunar Client"; //Need location
#else
            QDir::homePath() + "/lunarclient/Lunar Client"; // Need location
#endif
        QFileInfo fin(lcloc);
        QFileSystemModel* model = new QFileSystemModel;
        QIcon ic = model->fileIcon(model->index(fin.filePath()));
        QPixmap pixmap = ic.pixmap(ic.actualSize(QSize(1028, 1028)));
        setWindowIcon(pixmap);
        QFile file(icon);
        file.open(QIODevice::WriteOnly);
        pixmap.save(&file, "ICO");
    }
    

    QWidget* centralWidget = new QWidget();

    QGridLayout* mainLayout = new QGridLayout();

    pageList = new QListWidget();
    pageStack = new QStackedWidget();

    pageStack->setContentsMargins(30, 10, 30, 10);

    pageList->setIconSize(QSize(32, 32));

    pages = {
        new GeneralPage(config),
        new MinecraftPage(config),
        new AgentsPage(config),
        new HelpersPage(config)
    };

    foreach(ConfigurationPage* page, pages){
        new QListWidgetItem(page->icon(), page->title(), pageList);
        pageStack->addWidget(page);
    }

    connect(pageList, &QListWidget::currentRowChanged, pageStack, &QStackedWidget::setCurrentIndex);

    pageList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pageList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    pageList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    QFont font;
    font.setPointSize(11);
    pageList->setFont(font);

    versionSelect = new QComboBox();
    versionSelect->addItems({"1.7", "1.8", "1.12", "1.16", "1.17", "1.18.1", "1.18.2"});

    launchButton = new QPushButton();
    launchButton->setMinimumHeight(80);
    connect(launchButton, &QPushButton::clicked, this, &MainWindow::launch);

    connect(&offlineLauncher, &OfflineLauncher::error, this, &MainWindow::errorCallback);

    resetLaunchButtons();

    mainLayout->addWidget(pageList);
    mainLayout->addWidget(versionSelect, 1, 0);
    mainLayout->addWidget(launchButton, 4, 0);


    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::StyledPanel);
    frame->setFrameShadow(QFrame::Sunken);

    QLabel *title = new QLabel;
    QFont titleFont;
    titleFont.setPointSize(13);
    title->setFont(titleFont);
    QLabel *description = new QLabel;

    QVBoxLayout *frameLayout = new QVBoxLayout(frame);

#ifdef INCLUDE_UPDATER
    QHBoxLayout *titleBtnLayout = new QHBoxLayout;
    QPushButton *checkForUpdates = new QPushButton("Check for updates");
    titleBtnLayout->addWidget(title, 1);
    titleBtnLayout->addWidget(checkForUpdates);

    frameLayout->addLayout(titleBtnLayout);

    connect(checkForUpdates, &QPushButton::clicked, [this]{updaterChecker.checkForUpdates(true);});
#else
    frameLayout->addWidget(title);
#endif
    frameLayout->addWidget(description);


    connect(pageList, &QListWidget::currentRowChanged, [this, title, description](int current) {
        title->setText(pages[current]->title());
        description->setText(pages[current]->description());
    });

    pageList->setCurrentRow(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(frame);
    layout->addWidget(pageStack, 1);
    mainLayout->addLayout(layout, 0, 1, -1, 1);

    setCentralWidget(WidgetUtils::layoutToWidget(mainLayout));
    resize(800, 600);

    load();

#ifdef INCLUDE_UPDATER
    connect(&updaterChecker, &UpdateChecker::updateAvailable, this, &MainWindow::updateAvailable);
    connect(&updaterChecker, &UpdateChecker::noUpdatesAvailable, this, &MainWindow::noUpdatesAvailable);

    updaterChecker.checkForUpdates(false);
#endif
}

void MainWindow::resetLaunchButtons() {
    launchButton->setEnabled(true);
    launchButton->setText(QStringLiteral("Launch"));
}


void MainWindow::launch(){
    apply();
    offlineLauncher.launch();
    if(config.closeOnLaunch)
        close();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    apply();
    config.save();
    event->accept();
}

void MainWindow::apply() {
            foreach(ConfigurationPage *page, pages) {
            page->apply();
        }
    config.gameVersion = versionSelect->currentText();
}

void MainWindow::load() {
            foreach(ConfigurationPage *page, pages) {
            page->load();
        }
    versionSelect->setCurrentText(config.gameVersion);
}


void MainWindow::errorCallback(const QString &message) {
    QMessageBox messageBox;
    messageBox.setText(message);
    messageBox.exec();
}

#ifdef INCLUDE_UPDATER

void MainWindow::updateAvailable(const QString &url) {
    QMessageBox messageBox;
    messageBox.setWindowTitle("Update available!");
    messageBox.setText(QString(
            "A new update is available!<br>"
            "To update lcqt follow the link below:<br>"
            "<a href='%1'>%1</a>"
    ).arg(url));
    messageBox.exec();
}

void MainWindow::noUpdatesAvailable() {
    QMessageBox messageBox;
    messageBox.setWindowTitle("No updates available!");
    messageBox.setText("No updates available!");
    messageBox.exec();
}

#endif
