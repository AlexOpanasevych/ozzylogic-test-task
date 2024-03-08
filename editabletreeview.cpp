#include "editabletreeview.h"
#include "treemodel.h"

#include <QDebug>

EditableTreeView::EditableTreeView(QWidget * parent): QTreeView(parent) {


}

void EditableTreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    auto mcc = current.data(TreeModel::MCC);
    auto mnc = current.data(TreeModel::MNC);
    if(mcc.isValid() && mnc.isValid())
        mcc_mnc(mcc.toInt(), mnc.toInt());
}

void EditableTreeView::mcc_mnc(int mcc, int mnc)
{
    qDebug() << mcc << mnc;
}
