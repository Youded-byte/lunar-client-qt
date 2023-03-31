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

    if (version == "1.19.3")
        return "2";

    if (version.startsWith("1.17"))
        return "1.17";

    if (version.startsWith("1.18"))
        return "1.18";

    if (version.startsWith("1.19"))
        return "1.19";

    return version;
}

QString Utils::getGameVersion(const QString& version) {
    if (version == "1.19.0")
        return "1.19";
    if (version == "1.8")
        return "1.8.9";
    if (version == "1.7")
        return "1.7.10";
    return version;
}

QString Utils::getVersionFile(const QString& version) {
    if (version.length() < 2)
        return QString("v1_8");

    if (version.length() == 3)
        return QString(QString("v") + version.at(0)) + QString("_") + version.at(2);

    if (version.length() == 6)
        return QString(QString("v") + version.at(0)) + QString("_") + version.at(2) + version.at(3) + QString("_") + version.at(5);

    return QString(QString("v") + version.at(0) + QString("_") + version.at(2) +version.at(3));
}


QStringList Utils::getClassPath(const QStringList& files, const QString& version, const QString& modLoader) {
    QStringList classPath = QStringList();

    for (const QString& filename : files) {
        if (filename.length() < 8)
            continue;

        if (!filename.endsWith(QString(".jar")))
            continue;

        if (filename.contains(QString("v1_")))
            if (!filename.contains(Utils::getVersionFile(version)))
                continue;

        if (filename.startsWith(QString("OptiFine_v")) || filename.startsWith(QString("Forge_v")) || filename.startsWith(QString("Sodium_v")) || filename.startsWith(QString("Iris_v")) || filename.startsWith(QString("Indium_v")) || filename.contains(QString("NEU"), Qt::CaseInsensitive) || filename.contains(QString("LunatriusCore"), Qt::CaseInsensitive) || filename.contains(QString("SBA"), Qt::CaseInsensitive) || filename.contains(QString("DSM"), Qt::CaseInsensitive) || filename.contains(QString("Schematica"), Qt::CaseInsensitive) || filename.contains(QString("Sodium"), Qt::CaseInsensitive) || filename.contains(QString("Indium"), Qt::CaseInsensitive) || filename.contains(QString("Iris"), Qt::CaseInsensitive) || filename.contains(QString("replaymod"), Qt::CaseInsensitive))
            continue;

        if (modLoader != "Fabric")
            if (filename.contains(QString("fabric"), Qt::CaseInsensitive) || filename.contains(QString("argon"), Qt::CaseInsensitive))
                continue;

        if (modLoader != "Forge")
            if (filename.contains(QString("forge"), Qt::CaseInsensitive) || filename.contains(QString("skyblock"), Qt::CaseInsensitive))
                continue;

        if (modLoader == "Fabric")
            if (filename.contains(QString("optifine"), Qt::CaseInsensitive))
                continue;

        classPath << filename;
    }

    return classPath;
}

QStringList Utils::getExternalFiles(const QStringList& files, const QString& version, const QString &modLoader) {
    QStringList externalFiles = QStringList();
    QString versionFile = Utils::getVersionFile(version);
    bool addOptifine = false;
    QString optifineFileName = QString();

    for (const QString& filename : files) {
        if (filename.length() < 8)
            continue;

        if (!filename.endsWith(QString(".jar")))
            continue;

        if (filename.contains(QString("v1_")))
            if (!filename.contains(versionFile))
                continue;

        if (modLoader == "Fabric" && (filename.startsWith(QString("Sodium_v")) || filename.startsWith(QString("Indium_v")) || filename.startsWith(QString("Iris_v"))))
            externalFiles << filename;

        if (modLoader == "Forge")
            if (filename.startsWith(QString("Forge_v")) || filename.contains(QString("NEU"), Qt::CaseInsensitive) || filename.contains(QString("LunatriusCore"), Qt::CaseInsensitive)|| filename.contains(QString("SBA"), Qt::CaseInsensitive) || filename.contains(QString("DSM"), Qt::CaseInsensitive) || filename.contains(QString("Schematica"), Qt::CaseInsensitive))
                externalFiles << filename;

        if (modLoader != "Fabric" && filename.startsWith(QString("OptiFine_v"))) {
            addOptifine = true;
            optifineFileName = filename;
        }
    }

    if (addOptifine)
        externalFiles << optifineFileName;

    return externalFiles;
}

QString Utils::getNativesFile(const QStringList& files, const QString& version) {
    for (const QString& filename : files) {
        if (filename.endsWith(Utils::getVersionFile(version) + ".zip")) {
            return filename.left(filename.length() - 4);
        }
    }

    return "";
}


QStringList Utils::getOrderedAvailableVersions() {
    QString lunarDir = FS::getLunarDirectory();

    QDir multiverDir(FS::combinePaths(lunarDir, "offline", "multiver"));

    QStringList list = multiverDir.entryList({"*v1_*.zip"}, QDir::Files, QDir::Name);

    QStringList versionList = QStringList();

    for (const QString& filename : list) {
        QString version = filename;
        version.remove(0, version.indexOf(QString("-v1_")));
        version.remove(0, 2);
        version.chop(4);
        version.replace("_", "");
        QString parsedVersion = QString();
        if (version.length() == 4)
            parsedVersion = QString(version.at(0) + QString(".") + version.at(1) + version.at(2) + QString(".") + version.at(3));
        else if (version.length() == 2)
            parsedVersion = QString(version.at(0) + QString(".") + version.at(1));
        else
            parsedVersion = QString(version.at(0) + QString(".") + version.at(1) + QString(".") + version.at(2));
        versionList.append(parsedVersion);
    }

    std::sort(versionList.begin(), versionList.end(), [](const QString& a, const QString& b) {
        return QVersionNumber::fromString(a) < QVersionNumber::fromString(b);
    });

    return versionList;
}
QStringList Utils::getAvailableModLoaders(const QString& version) {
    QString lunarDir = FS::getLunarDirectory();

    QDir multiverDir(FS::combinePaths(lunarDir, "offline", "multiver"));

    QStringList list = multiverDir.entryList({ "*" + Utils::getVersionFile(version) + ".jar" }, QDir::Files, QDir::Name);

    QStringList modloaderList = QStringList();

    for (const QString& filename : list) {
        if (filename.startsWith("Forge", Qt::CaseInsensitive)) {
            modloaderList.append("Forge");
        }
        else if (filename.startsWith("Fabric", Qt::CaseInsensitive)) {
            modloaderList.append("Fabric");
        }
        else if (filename.startsWith("Optifine", Qt::CaseInsensitive)) {
            modloaderList.append("Optifine");
        }
    }
    return modloaderList;
}