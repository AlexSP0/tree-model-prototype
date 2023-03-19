#include <mergeproxymodel.h>

MergeProxyModel::MergeProxyModel(TreeModel *rootModel)
    : m_rootModel(rootModel)
{}

MergeProxyModel::~MergeProxyModel() {}

QVariant MergeProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
    return item->data(role);
}

Qt::ItemFlags MergeProxyModel::flags(const QModelIndex &index) const
{
    return Qt::NoItemFlags;
}

QModelIndex MergeProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    QAbstractItemModel *currentModel = const_cast<QAbstractItemModel *>(parent.model());
    TreeModel *currentTreeModel      = static_cast<TreeModel *>(
        const_cast<QAbstractItemModel *>(parent.model()));

    TreeItem *parentItem;

    if (!parent.isValid())
    {
        //parentItem = currentTreeModel->getRoot();
        parentItem = m_rootModel->getRoot();

        TreeItem *childItem = parentItem->child(row, column);

        if (childItem)
        {
            return createIndex(row, column, childItem);
        }
        return QModelIndex();
    }
    else
    {
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    }

    if (parentItem->type == TreeItem::ItemType::simple
        || parentItem->type == TreeItem::ItemType::connectionFrom)
    {
        if (!currentModel->hasIndex(row, column, parent))
        {
            return QModelIndex();
        }

        TreeItem *childItem = parentItem->child(row, column);

        if (childItem)
        {
            return currentTreeModel->createIndex(row, column, childItem);
        }
        return QModelIndex();
    }
    else //TreeItem::ItemType::connectionFrom
    {
        TreeItem *toItem = static_cast<TreeItem *>(
            parentItem->connection.connectionIndex.internalPointer());

        TreeModel *toModel = static_cast<TreeModel *>(
            const_cast<QAbstractItemModel *>(parentItem->connection.connectionIndex.model()));

        return toModel->createIndex(row, column, toItem);
    }
}

QModelIndex MergeProxyModel::parent(const QModelIndex &child) const
{
    QAbstractItemModel *currentModel = const_cast<QAbstractItemModel *>(child.model());
    TreeModel *currentTreeModel      = static_cast<TreeModel *>(
        const_cast<QAbstractItemModel *>(child.model()));

    if (!child.isValid())
    {
        //TO DO!
        return QModelIndex();
    }

    TreeItem *childItem  = static_cast<TreeItem *>(child.internalPointer());
    TreeItem *parentItem = childItem->parent();

    if (childItem->type == TreeItem::ItemType::simple
        || childItem->type == TreeItem::ItemType::connectionTo)
    {
        if (parentItem == currentTreeModel->getRoot())
        {
            return QModelIndex();
        }

        return currentTreeModel->createIndex(parentItem->row(), parentItem->column(), parentItem);
    }
    else //TreeItem::ItemType::connectionFrom
    {
        return childItem->connection.connectionIndex;
    }

    return QModelIndex();
}

int MergeProxyModel::rowCount(const QModelIndex &parent) const
{
    //QAbstractItemModel *currentModel = const_cast<QAbstractItemModel *>(parent.model());
    TreeModel *currentTreeModel = dynamic_cast<TreeModel *>(
        const_cast<QAbstractItemModel *>(parent.model()));

    TreeItem *item;

    if (!parent.isValid() && parent.model() == nullptr)
    {
        return m_rootModel->getRoot()->childRowsCount();
    }

    if (!parent.isValid())
    {
        return (currentTreeModel->getRoot())->childRowsCount();
    }

    item = static_cast<TreeItem *>(parent.internalPointer());

    auto i = item->childRowsCount();

    return i;
}

int MergeProxyModel::columnCount(const QModelIndex &parent) const
{
    TreeModel *currentTreeModel = static_cast<TreeModel *>(
        const_cast<QAbstractItemModel *>(parent.model()));

    TreeItem *item;

    if (!parent.isValid() && parent.model() == nullptr)
    {
        return m_rootModel->getRoot()->childRowsCount();
    }

    if (!parent.isValid())
    {
        return currentTreeModel->getRoot()->childRowsCount();
    }

    item = static_cast<TreeItem *>(parent.internalPointer());

    auto i = item->childRowsCount();

    return i;
}

//bool MergeProxyModel::hasChildren(const QModelIndex &parent) const
//{
//    //TO DO immplement
//}

//QModelIndex MergeProxyModel::createIndex(int row, int column, quintptr id) const
//{
//    //TO DO immplement
//}

void MergeProxyModel::attachModel(QModelIndex from, QModelIndex to)
{
    TreeItem *fromItem = static_cast<TreeItem *>(from.internalPointer());
    TreeItem *toItem   = static_cast<TreeItem *>(to.internalPointer());

    fromItem->type = TreeItem::ItemType::connectionTo;
    toItem->type   = TreeItem::ItemType::connectionFrom;

    fromItem->connection.connectionIndex = to;
    toItem->connection.connectionIndex   = from;
}

QModelIndex MergeProxyModel::getFirst()
{
    return m_rootModel->getFirst();
}
