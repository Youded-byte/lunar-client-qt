//
// Created by nils on 11/27/21.
//

#include "launcher.h"

Launcher::Launcher(const Config& config, QObject *parent) : config(config), QObject(parent) {
}

QString Launcher::getLevelHeadOptions(const bool& uselevelheadprefix, const QString& levelheadprefix, const bool& uselevelheadnicklevel, const QString& levelheadnicklevel) {
    if (!uselevelheadprefix && uselevelheadnicklevel)
        return QStringLiteral("@") + levelheadnicklevel;
    else if (!uselevelheadnicklevel && uselevelheadprefix)
        return levelheadprefix;
    else return levelheadprefix + "@" + levelheadnicklevel;
};
