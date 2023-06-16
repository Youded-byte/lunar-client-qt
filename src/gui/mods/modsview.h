#ifndef LUNAR_CLIENT_QT_MODSVIEW_H
#define LUNAR_CLIENT_QT_MODSVIEW_H

#include <QTableView>

class ModsView : public QTableView {
public:
    explicit ModsView(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};


#endif //LUNAR_CLIENT_QT_MODSVIEW_H
