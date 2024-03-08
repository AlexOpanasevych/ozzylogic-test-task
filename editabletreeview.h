#ifndef EDITABLETREEVIEW_H
#define EDITABLETREEVIEW_H

#include <QTreeView>

class EditableTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit EditableTreeView(QWidget * parent = nullptr);

    // QAbstractItemView interface
protected slots:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
private:
    void mcc_mnc(int mcc, int mnc);
};

#endif // EDITABLETREEVIEW_H
