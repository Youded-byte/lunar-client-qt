#include <QApplication>
#include <QStandardPaths>
#include <QCommandLineParser>

#include "gui/mainwindow.h"
#include "launch/offlinelauncher.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("lunar-client-qt"));

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();  

    QCommandLineOption noGuiOption("nogui",
        QCoreApplication::translate("main", "Launch Lunar Client Qt without GUI. Enables the use of overriding options"));
    parser.addOption(noGuiOption);

    QCommandLineOption versionOption("gameVersion",
        QCoreApplication::translate("gameOverride", "Override Minecraft version"),
        QCoreApplication::translate("gameOverride", "e.g. 1.8.9 or 1.19 (exactly as listed in the launcher)"));
    parser.addOption(versionOption);

    QCommandLineOption assetIndexOption("assetIndex",
        QCoreApplication::translate("gameOverride", "Override the assetIndex version which on default is derived from the game version but some new versions may break it"),
        QCoreApplication::translate("gameOverride", "e.g. for 1.8.9 it is 1.8 but for 1.7.10 it is 1.7.10 as well"));
    parser.addOption(assetIndexOption);

    QCommandLineOption xmsOption("xms",
        QCoreApplication::translate("gameOverride", "Override initial memory"),
        QCoreApplication::translate("gameOverride", "Specify the initial memory in megabytes"));
    parser.addOption(xmsOption);

    QCommandLineOption xmxOption("xmx",
        QCoreApplication::translate("gameOverride", "Specify the maximum memory in megabytes"),
        QCoreApplication::translate("gameOverride", "Specify the maximum memory in megabytes"));
    parser.addOption(xmxOption);

    parser.process(app);

    if (parser.isSet(noGuiOption)) {
        Config config = Config::load();
        parser.isSet(versionOption) ? config.gameVersion = parser.value(versionOption) : 0;
        parser.isSet(xmsOption) ? config.initialMemory = parser.value(xmsOption).toInt() : 0;
        parser.isSet(xmxOption) ? config.maximumMemory = parser.value(xmxOption).toInt() : 0;

        if (parser.isSet(assetIndexOption) == true)
            OfflineLauncher(config, parser.isSet(assetIndexOption), parser.value(assetIndexOption), nullptr).launch();
        else
            OfflineLauncher(config, false, NULL, nullptr).launch();
        return 1;
    }
    else {
        MainWindow mainWindow;
        mainWindow.show();
        QApplication::exec();
        return 1;
    }
}
