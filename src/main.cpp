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
        QCoreApplication::translate("gameVersionOverride", "Override Minecraft version"),
        QCoreApplication::translate("gameVersionOverride", "e.g. 1.8.9 or 1.19 (exactly as listed in the launcher)"));
    parser.addOption(versionOption);

    QCommandLineOption assetIndexOption("assetIndex",
        QCoreApplication::translate("assetIndexOverride", "Override the assetIndex version which on default is derived from the game version but some new versions may break it"),
        QCoreApplication::translate("assetIndexOverride", "e.g. for 1.8.9 it is 1.8 but for 1.7.10 it is 1.7.10 as well"));
    parser.addOption(assetIndexOption);

    QCommandLineOption xmsOption("xms",
        QCoreApplication::translate("xmsOverride", "Override initial memory"),
        QCoreApplication::translate("xmsOverride", "Specify the initial memory in megabytes"));
    parser.addOption(xmsOption);

    QCommandLineOption xmxOption("xmx",
        QCoreApplication::translate("xmxOverride", "override maximum memory"),
        QCoreApplication::translate("xmxOverride", "Specify the maximum memory in megabytes"));
    parser.addOption(xmxOption);

    parser.process(app);

    if (parser.isSet(noGuiOption)) {
        Config config = Config::load();
        parser.isSet(versionOption) ? config.gameVersion = parser.value(versionOption) : config.gameVersion;
        parser.isSet(xmsOption) ? config.initialMemory = parser.value(xmsOption).toInt() : config.initialMemory;
        parser.isSet(xmxOption) ? config.maximumMemory = parser.value(xmxOption).toInt() : config.maximumMemory;
        OfflineLauncher(config, parser.isSet(assetIndexOption), parser.value(assetIndexOption), nullptr).launch();
        return 1;
    }
    else {
        MainWindow mainWindow;
        mainWindow.show();
        QApplication::exec();
        return 1;
    }
}
