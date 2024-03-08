#ifndef SQLTREEMODEL_H
#define SQLTREEMODEL_H

#include <QAbstractItemModel>

class SqlTreeModelPrivate;

class QSqlQueryModel;

class SqlTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    explicit SqlTreeModel( QObject* parent = NULL );

    ~SqlTreeModel();

public:

    void appendModel(QSqlQueryModel* model , int foreignKeyColumn = 1);

    void insertModel(QSqlQueryModel* model, int parentLevel = -1 , int foreignKeyColumn = 1);

    QSqlQueryModel* modelAt( int level ) const;

    void setColumnMapping( int level, const QList<int>& columnMapping );

    void updateData();

    int levelOf( const QModelIndex& index ) const;

    int mappedRow( const QModelIndex& index ) const;

    int mappedColumn( const QModelIndex& index ) const;

    int rowId( const QModelIndex& index ) const;

    int rowParentId( const QModelIndex& index ) const;

    QVariant rawData( int level, int row, int column, int role = Qt::DisplayRole ) const;

    QModelIndex findIndex( int level, int id, int column ) const;

public: // overrides
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    int rowCount( const QModelIndex& parent = QModelIndex() ) const;

    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& index ) const;

    QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;

    bool setHeaderData( int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole );
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

protected:

    virtual void updateQueries();

private:
    SqlTreeModelPrivate* d;
};

#endif
