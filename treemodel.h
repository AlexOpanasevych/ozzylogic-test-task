#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "sqltreemodel.h"

namespace {
const QString _connectionName = QStringLiteral("test_ozzylogik_connection");
const QString _databaseName = QStringLiteral("system.db");
const QString _tableName1 = QStringLiteral("countries_new");
const QString _tableName2 = QStringLiteral("operators");
}

class TreeModel : public SqlTreeModel
{
    Q_OBJECT
public:
    enum Levels {
        Country,
        Operator
    };
    enum Roles {MCC = Qt::UserRole + 1, MNC, IsOperator, CountryCode, Name};
    explicit TreeModel(QObject *parent = nullptr);
    ~TreeModel();
    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;

    // SqlTreeModel interface
protected:
    void updateQueries();

private:
    void build();

    // QAbstractItemModel interface
public:
    // bool setData(const QModelIndex &index, const QVariant &value, int role);
    void updateRow(const QModelIndex & index, int mcc, int mnc, const QString & name);
    void insertRow(int mcc, int mnc, const QString & name);

    std::tuple<QString, QString> countryOperator(int mcc, int mnc);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

};

#endif // TREEMODEL_H
