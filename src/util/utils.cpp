//
// Created by nils on 6/7/22.
//

#include "utils.h"

#include <QStringList>
#include <QDir>
#include <QVersionNumber>

#include "fs.h"

QString Utils::getAgentFlags(const QString &name) {
    return "-javaagent:" + FS::combinePaths(FS::getAgentsDirectory(), name);
}

QString Utils::getAgentFlags(const QString &name, const QString &option) {
    return getAgentFlags(name) + "=" + option;
}

QString Utils::getAssetsIndex(const QString &version) {
    if(version == "1.7")
        return "1.7.10";

    if(version.startsWith("1.18"))
        return "1.18";

    return version;
}

QString Utils::getGameVersion(const QString& version) {
    if (version == "1.19")
        return "1.19.2";
    if (version == "1.18")
        return "1.18.2";
    return version;
}

QString Utils::getVersionFile(const QString& version) {
    if (version.length() < 2)
        return QString("v1_8");

    if (version.length() == 3)
        return QString(QString("v") + version.at(0)) + QString("_") + QString(version.at(2));

    return QString(QString("v") + version.at(0)) + QString("_") + QString(version.at(2)) + QString(version.at(3));
}


QStringList Utils::getClassPath(const QStringList& files, const QString& version) {
    QStringList classPath = QStringList();
    QString versionFile = Utils::getVersionFile(version);
    bool isFabric = Utils::isFabricVersion(version);

    for (const QString& filename : files) {
        if (filename.length() < 8)
            continue;

        if (!filename.endsWith(QString(".jar")))
            continue;

        if (filename.contains(QString("v1_")))
            if (!filename.contains(versionFile))
                continue;

        if (filename.startsWith(QString("Sodium_v")) || filename.startsWith(QString("Indium_v")) || filename.startsWith(QString("Iris_v")) || filename.startsWith(QString("OptiFine_v")))
            continue;

        if (isFabric)
            if (filename.startsWith(QString("optifine")))
                continue;

        if (!isFabric)
            if (filename.startsWith(QString("sodium")) || filename.startsWith(QString("fabric")))
                continue;   

        classPath << filename;
    }

    return classPath;
}

QStringList Utils::getExternalFiles(const QStringList& files, const QString& version) {
    QStringList externalFiles = QStringList();
    QString versionFile = Utils::getVersionFile(version);
    bool isFabric = Utils::isFabricVersion(version);

    for (const QString& filename : files) {
        if (filename.length() < 8)
            continue;

        if (!filename.endsWith(QString(".jar")))
            continue;

        if (filename.contains(QString("v1_")))
            if (!filename.contains(versionFile))
                continue;

        if (isFabric && (filename.startsWith(QString("Sodium_v")) || filename.startsWith(QString("Indium_v")) || filename.startsWith(QString("Iris_v"))))
            externalFiles << filename;

        if (!isFabric && filename.startsWith(QString("OptiFine_v")))
            externalFiles << filename;
    }

    return externalFiles;
}


bool Utils::isFabricVersion(const QString& version) {
    if (version == "1.7" || version == "1.8" || version == "1.12")
        return false;
    return true;
}


QStringList Utils::getOrderedAvailableVersions() {
    QString lunarDir = FS::getLunarDirectory();

    QDir multiverDir(FS::combinePaths(lunarDir, "offline", "multiver"));

    QStringList list = multiverDir.entryList({"v1_*"}, QDir::Files, QDir::Name);

    QStringList versionList = QStringList();

    for (const QString& filename : list) {
        QString version = filename;
        version.remove(version.indexOf(QString("-")), 256);
        version.remove(0, 1);
        version.replace("_", ".");
        versionList.append(version);
    }

    std::sort(versionList.begin(), versionList.end(), [](const QString& a, const QString& b){
        return QVersionNumber::fromString(a) < QVersionNumber::fromString(b);
    });

    return versionList;
}