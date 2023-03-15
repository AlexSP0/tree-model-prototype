#include "treemodel.h"

TreeModel::TreeModel()
    : m_rootItem(new TreeItem(nullptr))
{
    populateModel();
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
    return item->data(role);
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    TreeItem *parentItem;

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row, column);

    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    TreeItem *child  = static_cast<TreeItem *>(index.internalPointer());
    TreeItem *parent = child->parent();

    if (parent == m_rootItem)
    {
        return QModelIndex();
    }

    return createIndex(parent->row(), parent->column(), parent);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *item;
    if (!parent.isValid())
    {
        return m_rootItem->childRowsCount();
    }

    item = static_cast<TreeItem *>(parent.internalPointer());
    return item->childRowsCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    TreeItem *item;
    if (!parent.isValid())
    {
        return m_rootItem->columnsCountInRow(parent.row());
    }

    item = static_cast<TreeItem *>(parent.internalPointer());
    return item->columnsCountInRow(parent.row());
}

void TreeModel::populateModel()
{
    TreeItem *item1 = new TreeItem();
    item1->setData(QVariant("item1"), Qt::DisplayRole);
    TreeItem *item2 = new TreeItem();
    item2->setData(QVariant("item2"), Qt::DisplayRole);
    TreeItem *item3 = new TreeItem();
    item3->setData(QVariant("item3"), Qt::DisplayRole);
    TreeItem *item4 = new TreeItem();
    item4->setData(QVariant("item4"), Qt::DisplayRole);
    m_rootItem->appendChildToNextRow(item1);
    m_rootItem->appendChildToNextRow(item2);
    m_rootItem->appendItemExitingRow(item3, 0);
    m_rootItem->appendItemExitingRow(item4, 1);
}
