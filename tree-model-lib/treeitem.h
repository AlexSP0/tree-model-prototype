#ifndef TREEITEM_H
#define TREEITEM_H

#include "treemodel.h"

#include <memory>
#include <QVariant>
#include <QVector>

struct ConnectionPoint
{
    QModelIndex connectionIndex;
};

class TreeItem
{
public:
    enum ItemType
    {
        simple,
        connectionTo,
        connectionFrom
    };

public:
    ItemType type;

    ConnectionPoint connection;

public:
    explicit TreeItem(TreeItem *parent = nullptr);
    ~TreeItem();

    bool appendChildToNextRow(TreeItem *item);

    bool appendItemExitingRow(TreeItem *item, int row);

    TreeItem *child(int row, int column);

    int childRowsCount() const;

    int columnsCountInRow(int row) const;

    QVariant data(int role) const;

    void setData(QVariant data, int role);

    int row() const;

    int column() const;

    TreeItem *parent() const;

protected:
    QVector<QVector<TreeItem *>> m_childsItems;

    QMap<int, QVariant> m_itemData;

    TreeItem *m_parent;
};

#endif
