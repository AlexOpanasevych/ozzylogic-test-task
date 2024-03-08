#include "treemodel.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <QImage>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QFont>
#include <QSqlResult>


TreeModel::TreeModel(QObject *parent)
    : SqlTreeModel(parent)
{
    insertModel(new QSqlQueryModel(this), Country - 1);
    insertModel(new QSqlQueryModel(this), Operator - 1, 0);

    setColumnMapping( Country, QList<int>() << 0);
    setColumnMapping( Operator, QList<int>() << 0);

    build();
}

TreeModel::~TreeModel()
{

}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    int level = levelOf(index);
    int row = mappedRow(index);


    switch ( role) {
    case IsOperator:
        return level == Operator;
    case Qt::DecorationRole:
        if(level == Country) {
            return QImage("Countries/" + rawData(level, row, 1).toString(), "PNG");
        }
        if (level == Operator) {
            return QImage("Operators/" + rawData(level, row, 0).toString() + '_' + rawData(level, row, 1).toString(), "PNG");
        }
        return {};
    case Qt::DisplayRole: {
        if(level == Country) {
            QVariant value = rawData( level, row, 2, role );
            return value;
        }
        if (level == Operator) {
            return QString("%1 (%2, %3)").arg(rawData( level, row, 2 ).toString(), rawData( level, row, 1 ).toString(), rawData( level, row, 0 ).toString());
        }
    }
    case Qt::FontRole:
        if(level == Country) {
            QFont font;
            font.setBold(true);
            return font;
        }
        return {};
    case MNC:
        if(level == Operator) {
            return rawData(level, row, 1).toInt();
        }
        return {};
    case MCC:
        if(level == Operator) {
            return rawData(level, row, 0).toInt();
        }
        return {};
    case CountryCode:
        if(level == Operator) {
            return rawData(level, row, 3).toString();
        }
        return {};
    case Name:
        if(level == Country) {
            return rawData(level, row, 2).toString();
        }
        if(level == Operator) {
            return rawData(level, row, 2).toString();
        }
        return {};
    }

    return {};
}

void TreeModel::updateQueries()
{
    build();
}

void TreeModel::build()
{
    static QString countryQuery = "SELECT c.mcc, c.code, c.name, c.mnc_length"
                                  " FROM countries_new as c";
    static QString operatorsQuery = "SELECT o.mcc, o.mnc, o.name, c.code"
                                    " FROM operators as o"
                                    " LEFT JOIN countries_new as c ON o.mcc = c.mcc";
    modelAt(Country)->setQuery(countryQuery, QSqlDatabase::database(_connectionName));

    while (modelAt(Country)->canFetchMore())
        modelAt(Country)->fetchMore();

    modelAt(Operator)->setQuery(operatorsQuery, QSqlDatabase::database(_connectionName));

    while (modelAt(Operator)->canFetchMore())
        modelAt(Operator)->fetchMore();

    updateData();

}

void TreeModel::updateRow(const QModelIndex &index, int mcc, int mnc, const QString &name)
{
    static QString updateQuery = "UPDATE %1 SET name = ? WHERE mcc = ? AND mnc = ?";

    int level = levelOf(index);

    if(level == Operator) {
        QSqlQuery query(QSqlDatabase::database(_connectionName));
        query.prepare(updateQuery.arg(_tableName2));
        query.bindValue(0, name);
        query.bindValue(1, mcc);
        query.bindValue(2, mnc);
        if(query.exec()) {
            qDebug() << "successfully updated row";
        }

        updateQueries();
    }
}

void TreeModel::insertRow(int mcc, int mnc, const QString &name)
{
    static QString insertQuery = QStringLiteral("INSERT INTO %1 (mcc, mnc, name) "
                                                "VALUES (?, ?, ?)");

    QSqlQuery query(QSqlDatabase::database(_connectionName));
    query.prepare(insertQuery.arg(_tableName2));
    query.bindValue(0, mcc);
    query.bindValue(1, mnc);
    query.bindValue(2, name);
    if(query.exec()) {
        qDebug() << "successfully inserted row";
    }

    updateQueries();
}

std::tuple<QString, QString> TreeModel::countryOperator(int mcc, int mnc)
{
    static QString searchQuery = QStringLiteral("SELECT o.mcc, o.mnc, o.name, c.code"
                                                " FROM operators as o"
                                                " LEFT JOIN countries_new as c ON o.mcc = c.mcc"
                                                " WHERE o.mcc = ? AND o.mnc = ?");

    QSqlQuery q(QSqlDatabase::database(_connectionName));
    q.prepare(searchQuery);
    q.bindValue(0, mcc);
    q.bindValue(1, mnc);
    q.exec();

    if(q.next()) {
        auto r = q.record();
        return {r.value("code").toString(), r.value("name").toString()};
    }

    return {};
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return {};
}


// bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
// {
    // static QString updateQuery = "UPDATE %1 WHERE % = ? AND";

    //     int level = levelOf(index);
    // int row = mappedRow(index);


    // switch(role) {
    // case Name:
    //     if(level == Operator) {
    //         // modelAt(level)->setData(modelAt(level)->index(row, 2), value);
    //     }
    //     break;
    // case MCC:
    //     if(level == Operator) {
    //         modelAt(level)->setData(modelAt(level)->index(row, 0), value);
    //     }
    //     break;
    // case MNC:
    //     if(level == Operator) {
    //         modelAt(level)->setData(modelAt(level)->index(row, 1), value);
    //     }
    //     break;
    // default:
    //     return false;
    // }
    // return false;
// }
