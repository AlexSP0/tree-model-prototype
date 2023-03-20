#include <mergeproxymodel.h>

#include <QDebug>

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

    qWarning() << "PROXY model DATA: index"
               << "row:" << index.row() << "column" << index.column() << "role " << role
               << " item: " << item->data(Qt::DisplayRole);

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

    qWarning() << "PROXY model INDEX: "
               << "row: " << row << " column:" << column << "model: " << parent.model();

    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if (!parent.isValid())
    {
        //parentItem = currentTreeModel->getRoot();
        parentItem = m_rootModel->getRoot();

        TreeItem *childItem = parentItem->child(row);

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

    if (parentItem->type == TreeItem::Type::simple
        || parentItem->type == TreeItem::Type::connectionUp)
    {
        if (!currentModel->hasIndex(row, column, parent))
        {
            return QModelIndex();
        }

        TreeItem *childItem = parentItem->child(row);

        qWarning() << "PROXY model INDEX: " << childItem->data(Qt::DisplayRole);

        if (childItem)
        {
            return currentTreeModel->createIndex(row, column, childItem);
        }
        return QModelIndex();
    }
    else //TreeItem::ItemType::connectionDown
    {
        TreeItem *toItem = static_cast<TreeItem *>(
            parentItem->connection.connectionIndex.internalPointer());

        TreeModel *toModel = static_cast<TreeModel *>(
            const_cast<QAbstractItemModel *>(parentItem->connection.connectionIndex.model()));

        TreeItem *parentItemDownModel = toItem->parentItem();

        TreeItem *toItemDownModel = parentItemDownModel->child(row);

        qWarning() << "PROXY model INDEX: " << toItemDownModel->data(Qt::DisplayRole);

        auto itemIndex = toModel->createIndex(row, column, toItemDownModel);

        return itemIndex;
    }

    return QModelIndex();
}

QModelIndex MergeProxyModel::parent(const QModelIndex &child) const
{
    QAbstractItemModel *currentModel = const_cast<QAbstractItemModel *>(child.model());
    TreeModel *currentTreeModel      = static_cast<TreeModel *>(
        const_cast<QAbstractItemModel *>(child.model()));

    if (!child.isValid())
    {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem *>(child.internalPointer());

    TreeItem *parentItem;

    if (childItem->type == TreeItem::Type::simple
        || childItem->type == TreeItem::Type::connectionDown)
    {
        parentItem = childItem->parentItem();

        if (parentItem == currentTreeModel->getRoot() || parentItem == nullptr)
        {
            return QModelIndex();
        }

        return currentTreeModel->createIndex(parentItem->row(), 0, parentItem);
    }
    else
    {
        QModelIndex toItemIndex = childItem->connection.connectionIndex;

        TreeModel *toItemModel = static_cast<TreeModel *>(
            const_cast<QAbstractItemModel *>(toItemIndex.model()));

        TreeItem *toItem = static_cast<TreeItem *>(toItemIndex.internalPointer());

        qWarning() << "PROXY model PARENT: index"
                   << "row:" << child.row() << "column" << child.column()
                   << "model: " << child.model();
        qWarning() << "PROXY model parent: " << toItem->data(Qt::DisplayRole)
                   << "model: " << toItemIndex.model();

        return toItemModel->createIndex(toItemIndex.row(), toItemIndex.column(), toItem);
    }

    return QModelIndex();
}

int MergeProxyModel::rowCount(const QModelIndex &parent) const
{
    QAbstractItemModel *currentModel = const_cast<QAbstractItemModel *>(parent.model());
    TreeModel *currentTreeModel      = static_cast<TreeModel *>(
        const_cast<QAbstractItemModel *>(parent.model()));

    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
    {
        parentItem = m_rootModel->getRoot();
    }
    else
    {
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    }

    if (parentItem->type == TreeItem::Type::connectionDown)
    {
        QModelIndex toItemIndex = parentItem->connection.connectionIndex;
        TreeItem *toItem        = static_cast<TreeItem *>(toItemIndex.internalPointer());

        auto outerParent = toItem->parentItem();

        auto g = outerParent->childCount();

        return g;
    }
    else
    {
        auto i = parentItem->childCount();

        return i;
    }
}

int MergeProxyModel::columnCount(const QModelIndex &parent) const
{
    QAbstractItemModel *currentModel = const_cast<QAbstractItemModel *>(parent.model());
    TreeModel *currentTreeModel      = static_cast<TreeModel *>(
        const_cast<QAbstractItemModel *>(parent.model()));

    if (parent.isValid())
    {
        TreeItem *item = static_cast<TreeItem *>(parent.internalPointer());

        if (item->type == TreeItem::Type::connectionDown)
        {
            QModelIndex toItemIndex = item->connection.connectionIndex;
            TreeItem *toItem        = static_cast<TreeItem *>(toItemIndex.internalPointer());

            auto outerParent = toItem->parentItem();

            auto g = outerParent->columnCount();

            return g;
        }
        else
        {
            auto i = item->columnCount();

            return i;
        }
    }

    auto i = m_rootModel->getRoot()->columnCount();

    return i;
}

bool MergeProxyModel::hasChildren(const QModelIndex &parent) const
{
    TreeModel *toItemModel = static_cast<TreeModel *>(
        const_cast<QAbstractItemModel *>(parent.model()));

    TreeItem *toItem = static_cast<TreeItem *>(parent.internalPointer());

    bool flag = QAbstractItemModel::hasChildren(parent);

    return flag;
}

//QModelIndex MergeProxyModel::createIndex(int row, int column, quintptr id) const
//{
//    //TO DO immplement
//}

void MergeProxyModel::attachModel(QModelIndex from, QModelIndex to)
{
    TreeItem *fromItem = static_cast<TreeItem *>(from.internalPointer());
    TreeItem *toItem   = static_cast<TreeItem *>(to.internalPointer());

    fromItem->type = TreeItem::Type::connectionDown;
    toItem->type   = TreeItem::Type::connectionUp;

    fromItem->connection.connectionIndex = to;
    toItem->connection.connectionIndex   = from;

    TreeItem *toItemParent = toItem->parentItem();
    int toNumberChildren   = toItemParent->childCount();

    //attach all children
    for (int i = 0; i < toNumberChildren; ++i)
    {
        TreeItem *child                   = toItemParent->child(i);
        child->connection.connectionIndex = from;
        child->type                       = TreeItem::Type::connectionUp;
    }
}

QModelIndex MergeProxyModel::getFirst()
{
    return m_rootModel->getFirst();
}
