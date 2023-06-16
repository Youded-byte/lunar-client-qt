#ifndef LUNAR_CLIENT_QT_MODSPAGE_H
#define LUNAR_CLIENT_QT_MODSPAGE_H

#include <QListWidget>
#include <QTableView>
#include <QPushButton>
#include <QCheckBox>

#include "configurationpage.h"
#include "gui/mods/modsmodel.h"
#include "gui/mods/modsview.h"

class ModsPage : public ConfigurationPage {
    Q_OBJECT
public:
    explicit ModsPage(Config& config, QWidget* parent = nullptr);

    QString title() override;

    QString description() override;

    QIcon icon() override;

    void apply() override;
    void load() override;
private slots:
    void onSelect(const QItemSelection& selected, const QItemSelection& deselected);

private:
    ModsModel* model;
    ModsView* mods;
    QCheckBox* useWeave;

    QPushButton* add;
    QPushButton* remove;
    QPushButton* moveUp;
    QPushButton* moveDown;
};


#endif //LUNAR_CLIENT_QT_MODSPAGE_H
