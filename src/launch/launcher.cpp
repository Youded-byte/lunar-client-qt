//
// Created by nils on 11/27/21.
//

#include "launcher.h"

Launcher::Launcher(const Config& config, QObject *parent) : config(config), QObject(parent) {
}

QString Launcher::getAgentFlags(const QString &path, const QString &options) {
    return QStringLiteral("-javaagent:%1=%2")
        .arg(path,  options);
}

QString Launcher::getLevelHeadOptions(const bool& uselevelheadprefix, const QString& levelheadprefix, const int levelheadstringcolor, const bool& uselevelheadnicklevel, const QString& levelheadnicklevel, const int levelheadlevelcolor) {
    if (!uselevelheadprefix && uselevelheadnicklevel)
        return QStringLiteral("Level: ") + QStringLiteral("@") + QString::number(levelheadstringcolor) + QStringLiteral("@") + QString::number(levelheadlevelcolor) + QStringLiteral("@") + levelheadnicklevel;
    else if (!uselevelheadnicklevel && uselevelheadprefix)
        return levelheadprefix + QStringLiteral("@") + QString::number(levelheadstringcolor) + QStringLiteral("@") + QString::number(levelheadlevelcolor) + QStringLiteral("@") + QStringLiteral("-1");
    else return levelheadprefix + QStringLiteral("@") + QString::number(levelheadstringcolor) + QStringLiteral("@") + QString::number(levelheadlevelcolor) + QStringLiteral("@") + levelheadnicklevel;
};
