//
// Created by nils on 2/3/22.
//

#include "utils.h"

#include <QDir>
#include <QFile> 
#include <QApplication>

bool Utils::getAgentExists(const QString &name){
    if (QFile::exists(getAgentsDirectory() + QDir::separator() + name))
        return true;
    else
        return false;
}

QString Utils::getLibsDirectory() {
    return QDir::cleanPath(QApplication::applicationDirPath() + QDir::separator() + "libs");
}

QString Utils::getAgentsDirectory() {
    return QDir::cleanPath(QApplication::applicationDirPath() + QDir::separator() + "agents");
}

QString Utils::getAgentFlags(const QString &name) {
    if (getAgentExists(name))
        return QStringLiteral("-javaagent:") + getAgentsDirectory() + QDir::separator() + name;
    else
        return QStringLiteral("");
}

QString Utils::getAgentFlags(const QString &name, const QString &option) {
    if (getAgentExists(name))
        return getAgentFlags(name) + "=" + option;
    else
        return QStringLiteral("");
}
