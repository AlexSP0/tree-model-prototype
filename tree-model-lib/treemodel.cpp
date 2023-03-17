#include "treemodel.h"

TreeModel::TreeModel()
    : m_rootItem(new TreeItem(nullptr))
{}

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
    int rrr = parent.row();
    int ccc = parent.column();

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

    auto i = item->childRowsCount();

    return i;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    TreeItem *item;
    if (!parent.isValid())
    {
        return m_rootItem->columnsCountInRow(parent.row());
    }

    item = static_cast<TreeItem *>(parent.internalPointer());

    auto i = item->columnsCountInRow(parent.row());
    return i;
}

void TreeModel::populateModel(QString name)
{
    TreeItem *item1 = new TreeItem();
    QString itemName1;
    itemName1.append(name).append("1");
    item1->setData(QVariant(itemName1), Qt::DisplayRole);

    QString itemName2;
    itemName2.append(name).append("2");
    TreeItem *item2 = new TreeItem();
    item2->setData(QVariant(itemName2), Qt::DisplayRole);

    QString itemName3;
    itemName3.append(name).append("3");
    TreeItem *item3 = new TreeItem();
    item3->setData(QVariant(itemName3), Qt::DisplayRole);

    QString itemName4;
    itemName4.append(name).append("4");
    TreeItem *item4 = new TreeItem();
    item4->setData(QVariant(itemName4), Qt::DisplayRole);

    QString itemName5;
    itemName5.append(name).append("5");
    TreeItem *item5 = new TreeItem();
    item5->setData(QVariant(itemName5), Qt::DisplayRole);

    m_rootItem->appendChildToNextRow(item1);
    m_rootItem->appendChildToNextRow(item2);
    item1->appendChildToNextRow(item3);
    item1->appendItemExitingRow(item4, 0);
    item1->appendItemExitingRow(item5, 0);
}

QModelIndex TreeModel::getFirst() const
{
    return createIndex(0, 0, m_rootItem->child(0, 0));
}

TreeItem *TreeModel::getRoot() const
{
    return m_rootItem;
}

QModelIndex TreeModel::createIndex(int arow, int acolumn, TreeItem *aid) const
{
    return QAbstractItemModel::createIndex(arow, acolumn, aid);
}
