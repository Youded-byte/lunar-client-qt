#ifndef LUNAR_CLIENT_QT_MODSMODEL_H
#define LUNAR_CLIENT_QT_MODSMODEL_H

#include "util/fs.h"
#include "config/config.h"

#include <QAbstractTableModel>
#include <QString>
#include <QStringList>
#include <QJsonObject>
#include <QFileInfo>

class ModsModel : public QAbstractTableModel {
Q_OBJECT
public:
    explicit ModsModel(QList<Mod> &mods, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool removeRows(int row, int count, const QModelIndex &parent) override;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent,
                  int destinationChild) override;

    Qt::DropActions supportedDropActions() const override;

    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column,
                         const QModelIndex &parent) const override;

    bool
    dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    void addMod(const QFileInfo& modFileInfo);

    bool containsMod(const QString& mod) const;
private:
    QList<Mod> &mods;

    enum Column {
        NAME = 0,
        NUM_COLS
    };
};


#endif //LUNAR_CLIENT_QT_MODSMODEL_H