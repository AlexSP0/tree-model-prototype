#include <mergeproxymodel.h>

MergeProxyModel::MergeProxyModel(QAbstractItemModel *rootModel)
    : m_rootModel(rootModel)
    , m_currentModel(rootModel)
{}

QVariant MergeProxyModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

Qt::ItemFlags MergeProxyModel::flags(const QModelIndex &index) const
{
    return Qt::NoItemFlags;
}

QModelIndex MergeProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    QAbstractItemModel *model = nullptr;

    model = isAnyConnectionByIndex(parent.row(), parent.column(), m_currentModel);

    if (model)
    {
        const_cast<MergeProxyModel *>(this)->m_currentModel = model;
    }

    //!!!
    return m_currentModel->index(row, column, QModelIndex());
}

QModelIndex MergeProxyModel::parent(const QModelIndex &child) const {}

int MergeProxyModel::rowCount(const QModelIndex &parent) const
{
    QAbstractItemModel *model = nullptr;

    model = isAnyConnectionByIndex(parent.row(), parent.column(), m_currentModel);

    if (!model)
    {
        return m_currentModel->rowCount(parent);
    }
    else
    {
        return model->rowCount(parent);
    }
}

int MergeProxyModel::columnCount(const QModelIndex &parent) const
{
    QAbstractItemModel *model = nullptr;

    model = isAnyConnectionByIndex(parent.row(), parent.column(), m_currentModel);

    if (!model)
    {
        return m_rootModel->columnCount(parent);
    }
    else
    {
        return model->columnCount(parent);
    }
}

QModelIndex MergeProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    return QModelIndex();
}

QModelIndex MergeProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    return QModelIndex();
}

QAbstractItemModel *MergeProxyModel::isAnyConnectionByIndex(int row,
                                                            int column,
                                                            QAbstractItemModel *sModel) const
{
    for (const auto &connection : m_connectionPoints)
    {
        if (sModel == connection.sourceModel && connection.connectionIndex.row() == row
            && connection.connectionIndex.column() == column)
        {
            return connection.connectedModel;
        }
    }

    return nullptr;
}
