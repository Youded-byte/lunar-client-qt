//
// Created by nils on 6/7/22.
//

#ifndef LUNAR_CLIENT_QT_UTILS_H
#define LUNAR_CLIENT_QT_UTILS_H

#include <QString>

namespace Utils {
    bool getAgentExists(const QString &name);
    QString getLibsDirectory();
    QString getAgentsDirectory();


    QString getAgentFlags(const QString& name);
    QString getAgentFlags(const QString& name, const QString& option);
    QString getAssetsIndex(const QString& version);

    QStringList getOrderedAvailableVersions();
};


#endif //LUNAR_CLIENT_QT_UTILS_H
