#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "treemodel.h"

#include <QAbstractListModel>
#include <QMainWindow>
#include <QTreeView>

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onAddRowButtonClicked();
// widgets
private:
    TreeModel * model = nullptr;
    QTreeView * view = nullptr;
};
#endif // MAINWINDOW_H
