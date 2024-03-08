#include "mainwindow.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardItem>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QPushButton>
#include "editabletreeview.h"
#include "editformdialog.h"
#include "delegates/operatordelegate.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{


    Q_ASSERT(qApp);
    // QDir dir(qApp->applicationDirPath());
    // dir.cdUp();
    // dir.cd("ozzylogik_test_task");
    QSqlDatabase db = QSqlDatabase::database(_connectionName);
    if(!db.isValid()) {
        db = QSqlDatabase::addDatabase("QSQLITE", _connectionName);
        // qDebug() << dir.absolutePath() + "/" + _databaseName;
        db.setDatabaseName(_databaseName);
        if(!db.open()) {
            qDebug() << "Database not opened, error" << db.lastError();
        }
        else {
            qDebug() << "Database opened";
        }
    }

    view = new EditableTreeView(this);

    model = new TreeModel(this);
    view->setModel(model);

    this->setCentralWidget(view);

    view->setLayout(new QHBoxLayout(this));

    QPushButton * button = new QPushButton(this);
    connect(button, &QPushButton::clicked, this, &MainWindow::onAddRowButtonClicked);
    button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
    button->setText("+");
    view->layout()->addWidget(button);
    view->layout()->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    view->layout()->setContentsMargins(QMargins(0, 0, 25, 15));

    // view->setItemDelegateForColumn(0, new CountryDelegate(this));
    view->setItemDelegateForColumn(0, new OperatorDelegate(this));
    view->viewport()->setAttribute( Qt::WA_Hover );

    resize(640, 480);

}

MainWindow::~MainWindow() {}

void MainWindow::onAddRowButtonClicked()
{
    EditFormDialog * diag = new EditFormDialog(this);

    auto search = [=](){
        diag->setCountry({});
        diag->setOperator({});
        auto [countryCode, name] = model->countryOperator(diag->mcc(), diag->mnc());
        if(!countryCode.isEmpty()) {
            diag->setCountry(QImage("Countries/" + countryCode, "PNG"));
            diag->setOperator(QImage(QString("Operators/%1_%2").arg(diag->mcc()).arg(diag->mnc()), "PNG"));
        }
    };

    connect(diag, &EditFormDialog::mccChanged, this, search);
    connect(diag, &EditFormDialog::mncChanged, this, search);
    connect(diag, &EditFormDialog::accepted, this, [=](){
        if(model) {
            model->insertRow(diag->mcc(), diag->mnc(), diag->name());
        }
    });
    diag->exec();
}

// int MyModel::rowCount(const QModelIndex &parent) const
// {
//     return values.size();
// }

// QVariant MyModel::data(const QModelIndex &index, int role) const
// {
//     if(!checkIndex(index))
//         return {};

//     int row = index.row();
//     switch(role) {
//     case Value:
//         return values[row];
//     default:
//         return {};
//     }
// }

// QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role) const
// {
//     // switch(orientation) {
//     // case Qt::Vertical:
//     //     switch(section) {
//     //     case 0:
//     //         return "shit";
//     //     default:
//     //         return "?";
//     //     }
//     // }
//     return "?";
// }
