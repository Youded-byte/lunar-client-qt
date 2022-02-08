#ifndef LUNAR_CLIENT_QT_HELPERSPAGE_H
#define LUNAR_CLIENT_QT_HELPERSPAGE_H

#include <QListWidget>
#include <QPushButton>

#include "configurationpage.h"

class HelpersPage : public ConfigurationPage {
Q_OBJECT
public:
    explicit HelpersPage(Config& config, QWidget* parent = nullptr);

    QString title() override;

    QString description() override;

    QIcon icon() override;

    void apply() override;
    void load() override;
private slots:
    void onSelect(const QItemSelection& selected, const QItemSelection& deselected);
    void addHelper(const QString& path, bool select = false);

    QStringList getHelpers();
private:
    QListWidget* helpers;

    QPushButton* add;
    QPushButton* remove;
    QPushButton* moveUp;
    QPushButton* moveDown;
};


#endif //LUNAR_CLIENT_QT_HELPERSPAGE_H
