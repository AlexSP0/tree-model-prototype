#ifndef TREEITEM_H
#define TREEITEM_H

#include "treemodel.h"

#include <memory>
#include <QModelIndex>
#include <QVariant>

struct ConnectionPoint
{
    QModelIndex connectionIndex;
};

class TreeItem
{
public:
    enum Type
    {
        simple,
        connectionDown,
        connectionUp
    };

public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

    Type type;

    ConnectionPoint connection;

private:
    QList<TreeItem *> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
};

#endif
