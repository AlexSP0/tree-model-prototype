#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>

#include "treeitem.h"

class TreeItem;

class TreeModel : public QAbstractItemModel
{
public:
    Q_OBJECT
public:
    TreeModel();
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    void populateModel(QString name);

    QModelIndex getFirst() const;

    TreeItem *getRoot() const;

    QModelIndex createIndex(int arow, int acolumn, TreeItem *aid) const;

private:
    TreeItem *m_rootItem;
};

#endif // TREEMODEL_H
