#include "operatordelegate.h"

#include <QPainter>
#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QMouseEvent>
#include "../editformdialog.h"
#include "../treemodel.h"

OperatorDelegate::OperatorDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void OperatorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto customOption = option;

    auto model = qobject_cast<const TreeModel*>(index.model());

    if(((option.state & QStyle::State_MouseOver) ||
         (option.state & QStyle::State_Selected)) &&
        model->levelOf(index) == TreeModel::Operator) {
        // QFontMetrics metrics(option.font)

        initStyleOption(&customOption, index);

        customOption.text = customOption.text + "    +";

        const QWidget *widget = option.widget;
        QStyle *style = widget ? widget->style() : QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &customOption, painter, widget);

    }
    QStyledItemDelegate::paint(painter, customOption, index);
}

QWidget *OperatorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    EditFormDialog *diag = new EditFormDialog(parent);
    setEditorData(diag, index);
    return diag;
}

bool OperatorDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress && index.data(TreeModel::IsOperator).toBool()) {
        const auto e = static_cast<const QMouseEvent *>(event);

        if(e->button() == Qt::LeftButton) {
            EditFormDialog *diag = new EditFormDialog();
            setEditorData(diag, index);

            connect(diag, &EditFormDialog::finished, this, [=](int result){
                if(result == QDialog::Accepted) {
                    setModelData(diag, model, index);
                }
            });

            diag->exec();


            return true;
        }
    }
    return false;
}

void OperatorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto formDialog = qobject_cast<EditFormDialog*>(editor);
    formDialog->setEdit(true);
    formDialog->setCountry(QImage("Countries/" + index.data(TreeModel::Roles::CountryCode).toString(), "PNG"));
    formDialog->setOperator(QImage(/*"Operators/" + */index.data(Qt::DecorationRole).value<QImage>()));
    formDialog->setName(index.data(TreeModel::Roles::Name).toString());
    formDialog->setMcc(index.data(TreeModel::Roles::MCC).toInt());
    formDialog->setMnc(index.data(TreeModel::Roles::MNC).toInt());

}

void OperatorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto diag = qobject_cast<EditFormDialog*> (editor);
    auto originModel = qobject_cast<TreeModel*>(model);

    originModel->updateRow(index, diag->mcc(), diag->mnc(), diag->name());

}
