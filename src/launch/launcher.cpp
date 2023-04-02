//
// Created by nils on 11/27/21.
//

#include "launcher.h"

Launcher::Launcher(const Config& config, const bool useCustomAssetIndex, const QString& customAssetIndex, QObject *parent) : config(config), useCustomAssetIndex(useCustomAssetIndex), customAssetIndex(customAssetIndex), QObject(parent) {
}

QString Launcher::getLevelHeadOptions(const bool& uselevelheadprefix, const QString& levelheadprefix, const bool& uselevelheadnicklevel, const QString& levelheadnicklevel) {
    if (!uselevelheadprefix && uselevelheadnicklevel)
        return QStringLiteral("Level: @") + levelheadnicklevel;
    else if (!uselevelheadnicklevel && uselevelheadprefix)
        return levelheadprefix;
    else return levelheadprefix + "@" + levelheadnicklevel;
};
