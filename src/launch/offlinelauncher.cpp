//
// Created by nils on 11/4/21.
//

#include "offlinelauncher.h"

#include <QCoreApplication>
#include <QProcess>
#include <QDir>
#include <QDirIterator>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QElapsedTimer>
#include <QIODevice>

#include "util/fs.h"
#include "util/utils.h"

OfflineLauncher::OfflineLauncher(const Config& config, const bool useCustomAssetIndex, const QString& customAssetIndex, QObject *parent) : Launcher(config, useCustomAssetIndex, customAssetIndex, parent) {
}


bool OfflineLauncher::launch() {
    if (config.gameVersion.isEmpty()) {
        emit error("No version selected!\nDo you have lunar installed?");
        return false;
    }

    QProcess process;
    process.setProgram(config.useCustomJre ? config.customJrePath : findJavaExecutable());

    process.setStandardInputFile(QProcess::nullDevice());
    process.setStandardOutputFile(QProcess::nullDevice());
    process.setStandardErrorFile(QProcess::nullDevice());

    QString workingDir = FS::combinePaths(
        FS::getLunarDirectory(),
        "offline",
        "multiver"
    );

    process.setWorkingDirectory(workingDir);

    QStringList workingDirFiles = QDir(workingDir).entryList(QDir::Files, QDir::Time | QDir::Reversed);

    QFileInfoList libsList = QDir(FS::getLibsDirectory()).entryInfoList(QDir::Files);

    QStringList classPath = Utils::getClassPath(workingDirFiles, config.gameVersion, config.modLoader);

    QStringList ichorClassPath = classPath;

    QString nativesFile = Utils::getNativesFile(workingDirFiles, config.gameVersion);

    for(const QFileInfo& info : libsList) {
        classPath << info.absoluteFilePath();
    }

    QStringList args{
         "--add-modules", "jdk.naming.dns",
         "--add-exports", "jdk.naming.dns/com.sun.jndi.dns=java.naming",
         "-Djna.boot.library.path=" + nativesFile,
         "-Dlog4j2.formatMsgNoLookups=true",
         "--add-opens", "java.base/java.io=ALL-UNNAMED",
         QString("-Xms%1m").arg(config.initialMemory),
         QString("-Xmx%1m").arg(config.maximumMemory),
         "-Djava.library.path=" + nativesFile,
         "-cp", classPath.join(QDir::listSeparator())
    };

    args << Utils::getAgentFlags("NativesPrepare", nativesFile);

    for(const Agent& agent : config.agents)
        if(agent.enabled)
            args << "-javaagent:" + agent.path + '=' + agent.option;

    if(config.useWeave)
        args << Utils::getAgentFlags("WeaveLoader");

    args << QProcess::splitCommand(config.jvmArgs);

    args << QStringList{
            "com.moonsworth.lunar.genesis.Genesis",
            "--version", Utils::getGameVersion(config.gameVersion),
            "--accessToken", "0",
            "--assetIndex", useCustomAssetIndex ? customAssetIndex : Utils::getAssetsIndex(config.gameVersion),
            "--userProperties", "{}",
            "--gameDir", config.useCustomMinecraftDir ? config.customMinecraftDir : FS::getMinecraftDirectory(),
            "--launcherVersion", "2.15.1",
            "--width", QString::number(config.windowWidth),
            "--height", QString::number(config.windowHeight),
            "--workingDirectory", ".",
            "--classpathDir", ".",
            "--ichorClassPath", ichorClassPath.join(QString(",")),
            "--ichorExternalFiles", Utils::getExternalFiles(workingDirFiles, config.gameVersion, config.modLoader).join(QString(","))
    };

    if(config.joinServerOnLaunch)
        args << "--server" << config.serverIp;

    process.setArguments(args);

    //Removes the windir environment variable, preventing lunar from reading your hosts file and executing tasklist on windows

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("windir");
    env.remove("JAVA_OPTS");
    env.remove("_JAVA_OPTS");
    env.remove("JAVA_OPTIONS");
    env.remove("_JAVA_OPTIONS");
    env.remove("JAVA_TOOL_OPTIONS");
    env.remove("_JAVA_TOOL_OPTIONS");
    env.remove("JDK_JAVA_OPTIONS");
    env.remove("_JDK_JAVA_OPTIONS");

    process.setProcessEnvironment(env);
    process.setStandardOutputFile(FS::combinePaths(QCoreApplication::applicationDirPath(), "latest.log"), QIODevice::Truncate);
    process.setStandardErrorFile(FS::combinePaths(QCoreApplication::applicationDirPath(), "error.log"), QIODevice::Truncate);

    if(!process.startDetached()){
        emit error("Failed to start process: " + process.errorString());
        return false;
    }

    if (!config.helpers.isEmpty())
    {
        foreach(const QString & path, config.helpers)
            HelperLaunch(path);
    }

    return true;
}

QString OfflineLauncher::findJavaExecutable() {
    QDir jreDir = QDir(FS::combinePaths(FS::getLunarDirectory(), "jre"));

    QFileInfoList jreSubDirs = jreDir.entryInfoList(QDir::Dirs, QDir::Time | QDir::Reversed);

    for (QFileInfo jreSubDir : jreSubDirs) {

        if (jreSubDir.fileName().length() != 40)
            continue;

        QFileInfoList jreSubDirSubDirs = QDir(jreSubDir.absoluteFilePath()).entryInfoList(QDir::Dirs, QDir::Time | QDir::Reversed);

        for (const QFileInfo& jreSubDirSubDir : jreSubDirSubDirs) {
            QString potentialExecutable = FS::combinePaths(
                jreSubDirSubDir.absoluteFilePath(),
                "bin",
#ifdef Q_OS_WIN
                "javaw.exe"
#else
                "java"
#endif

            );

            if (QFileInfo(potentialExecutable).isExecutable())
                return potentialExecutable;
        }
    }

    return {};
}

void OfflineLauncher::HelperLaunch(const QString& helper) {
    QProcess process;
#ifdef Q_OS_WIN
    process.setCreateProcessArgumentsModifier(([] (QProcess::CreateProcessArguments* args)
    {
        args->flags |= 0x00000010; //CREATE_NEW_CONSOLE
    }));
#endif
    process.setProgram(helper);
    process.startDetached(); 
}
