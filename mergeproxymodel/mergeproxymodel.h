#ifndef MERGEPROXYMODEL_H
#define MERGEPROXYMODEL_H

#include "../tree-model-lib/treemodel.h"
#include <qabstractitemmodel.h>

struct ConnectionPoint
{
    QAbstractItemModel *sourceModel = nullptr;
    QModelIndex connectionIndex;
    QAbstractItemModel *connectedModel = nullptr;
};

class MergeProxyModel : public QAbstractItemModel
{
public:
    Q_OBJECT

public:
    MergeProxyModel(QAbstractItemModel *rootModel);
    ~MergeProxyModel();

    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;

    QModelIndex parent(const QModelIndex &child) const override;

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;

private:
    QAbstractItemModel *m_rootModel;
    QAbstractItemModel *m_currentModel;

    QList<QAbstractItemModel *> m_sourcesModels;
    QList<ConnectionPoint> m_connectionPoints;

    QAbstractItemModel *isAnyConnectionByIndex(int row,
                                               int column,
                                               QAbstractItemModel *sModel) const;
};

#endif
