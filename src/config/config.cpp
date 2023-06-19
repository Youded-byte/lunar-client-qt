//
// Created by nils on 12/1/21.
//

#include "config.h"

#include <QStandardPaths>
#include <QJsonObject>
#include <QProcess>
#include <QJsonArray>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QJsonDocument>

const QString Config::configFilePath = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/lunar-client-qt/settings.json";

void Config::save() {
    QJsonObject saveObj;

    saveObj["version"] = gameVersion;
    saveObj["modLoader"] = modLoader;

    saveObj["keepMemorySame"] = keepMemorySame;
    saveObj["initialMemory"] = initialMemory;
    saveObj["maxMemory"] = maximumMemory;

    saveObj["useCustomJre"] = useCustomJre;
    saveObj["customJrePath"] = customJrePath;

    saveObj["closeOnLaunch"] = closeOnLaunch;

    saveObj["jvmArgs"] = jvmArgs;

    saveObj["useCustomMinecraftDir"] = useCustomMinecraftDir;
    saveObj["customMinecraftDir"] = customMinecraftDir;

    saveObj["joinServerOnLaunch"] = joinServerOnLaunch;
    saveObj["serverIp"] = serverIp;

    saveObj["useWeave"] = useWeave;

    saveObj["windowWidth"] = windowWidth;
    saveObj["windowHeight"] = windowHeight;

    QJsonArray arr;
    for(const Agent& agent : agents){
        QJsonObject agentObj;
        agentObj["path"] = agent.path;
        agentObj["option"] = agent.option;
        agentObj["enabled"] = agent.enabled;

        arr.append(agentObj);
    }

    saveObj["agents"] = arr;

    QJsonArray arr2;
    foreach(const QString& str, helpers) {
        arr2.append(str);
    }

    saveObj["helpers"] = arr2;
    saveJsonToConfig(saveObj);
}

Config Config::load() {
    QJsonObject jsonObj = loadJsonFromConfig();

    QJsonArray arr = jsonObj["agents"].toArray();

    QList<Agent> agents;

    for(auto val : arr){
        if(val.isObject()){
            QJsonObject obj = val.toObject();

            QString path = obj["path"].toString();
            QString option = obj["option"].toString({});
            bool enabled = obj["enabled"].toBool(true);

            if(QFile::exists(path)){
                agents.append({path, option, enabled});
            }
        }else{
            QString path = val.toString();
            agents.append({path, {}});
        }
    }

    arr.empty();
    arr = jsonObj["helpers"].toArray();

    QStringList helpers;

    foreach(const QJsonValue& val, arr) {
        QString path = val.toString();
        if (QFile::exists(path)) {
            helpers.append(path);
        }
    }

    QDir modsDir(FS::getWeaveModsDirectory());
    if (!modsDir.exists())
        modsDir.mkpath(modsDir.path());

    QFileInfoList list = modsDir.entryInfoList({ "*.jar", "*.jar.disabled" }, QDir::Files, QDir::Name);

    QList<Mod> mods = QList<Mod>();
    for (const QFileInfo& file : list) {
        mods.append(Mod(file.fileName().remove(".jar.disabled").remove(".jar"), !file.completeSuffix().endsWith("jar.disabled")));
    }

    return {
        jsonObj["version"].toString("1.8"),
        jsonObj["modLoader"].toString("Optifine"),
        jsonObj["keepMemorySame"].toBool(true),
        jsonObj["initialMemory"].toInt(3072),
        jsonObj["maxMemory"].toInt(3072),
        jsonObj["useCustomJre"].toBool(false),
        jsonObj["customJrePath"].toString(),
        jsonObj["jvmArgs"].toString(),
        jsonObj["closeOnLaunch"].toBool(false),
        jsonObj["useCustomMinecraftDir"].toBool(false),
        jsonObj["customMinecraftDir"].toString(),
        jsonObj["joinServerOnLaunch"].toBool(false),
        jsonObj["serverIp"].toString(),
        jsonObj["windowWidth"].toInt(640),
        jsonObj["windowHeight"].toInt(480),
        jsonObj["useWeave"].toBool(false),
        agents,
        helpers,
        mods
    };
}

void Config::saveJsonToConfig(const QJsonObject &jsonObject) {
    QString path = QFileInfo(configFilePath).absolutePath();
    QDir dir;
    if(!dir.exists(path)){
        dir.mkdir(path);
    }

    QFile configFile(configFilePath);

    configFile.open(QIODevice::WriteOnly);

    configFile.write(QJsonDocument(jsonObject).toJson());

    configFile.close();
}

QJsonObject Config::loadJsonFromConfig() {
    QFile configFile(configFilePath);
    configFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QJsonObject jsonObj = QJsonDocument::fromJson(configFile.readAll()).object();

    configFile.close();

    return jsonObj;
}
