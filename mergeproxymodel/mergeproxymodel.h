#ifndef MERGEPROXYMODEL_H
#define MERGEPROXYMODEL_H

#include "../tree-model-lib/treemodel.h"
#include <qabstractitemmodel.h>

class MergeProxyModel : public QAbstractItemModel
{
public:
    Q_OBJECT

public:
    MergeProxyModel(TreeModel *rootModel);
    ~MergeProxyModel();

    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    //bool hasChildren(const QModelIndex &parent) const override; //TO DO

    //QModelIndex createIndex(int row, int column, quintptr id) const override; //TO DO

    void attachModel(QModelIndex from, QModelIndex to);

    QModelIndex getFirst();

private:
    TreeModel *m_rootModel;
};

#endif
