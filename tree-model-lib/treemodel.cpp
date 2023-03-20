#include "treemodel.h"

#include <QDebug>
#include <QStringList>

TreeModel::TreeModel(const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new TreeItem({tr("Title"), tr("Summary")});
    setupModelData(data.split('\n'), m_rootItem);
}

TreeModel::~TreeModel()
{
    delete m_rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        auto i = static_cast<TreeItem *>(parent.internalPointer())->columnCount();
        return i;
    }

    auto g = m_rootItem->columnCount();
    return g;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

    qWarning() << "Tree model DATA: index"
               << "row:" << index.row() << "column" << index.column() << "role " << role
               << " item: " << item->data(Qt::DisplayRole);

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootItem->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    qWarning() << "Tree model INDEX: "
               << "row: " << row << " column:" << column << "model: " << parent.model();

    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row);

    qWarning() << "Tree model INDEX: " << childItem->data(Qt::DisplayRole);

    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem  = static_cast<TreeItem *>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    qWarning() << "Tree model PARENT: index"
               << "row:" << index.row() << "column" << index.column() << "model: " << index.model()
               << "Tree model PARENT: " << parentItem->data(Qt::DisplayRole);

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TreeItem *>(parent.internalPointer());

    auto i = parentItem->childCount();

    return i;
}

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent) {}

void TreeModel::populateModel(QString name)
{
    m_rootItem->appendChild(new TreeItem({QVariant(name + "1")}, m_rootItem));
    m_rootItem->appendChild(new TreeItem({QVariant(name + "2")}, m_rootItem));
    m_rootItem->appendChild(new TreeItem({QVariant(name + "3")}, m_rootItem));

    TreeItem *item2 = m_rootItem->child(1);
    item2->appendChild(new TreeItem({QVariant("2" + name + "1")}, item2));
    item2->appendChild(new TreeItem({QVariant("2" + name + "2")}, item2));
}

QModelIndex TreeModel::getFirst() const
{
    return QModelIndex();
}

TreeItem *TreeModel::getRoot() const
{
    return m_rootItem;
}

QModelIndex TreeModel::createIndex(int arow, int acolumn, TreeItem *aid) const
{
    return QAbstractItemModel::createIndex(arow, acolumn, aid);
}
