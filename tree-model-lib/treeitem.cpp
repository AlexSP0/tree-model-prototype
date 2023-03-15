#include "treeitem.h"

TreeItem::TreeItem(TreeItem *parent)
    : m_parent(parent)
{}

TreeItem::~TreeItem()
{
    for (auto currentColumn : m_childsItems)
    {
        qDeleteAll(currentColumn);
    }
}

bool TreeItem::appendChildToNextRow(TreeItem *item)
{
    m_childsItems.append(QVector{item});

    return true;
}

bool TreeItem::appendItemExitingRow(TreeItem *item, int row)
{
    if (row >= childRowsCount())
    {
        return false;
    }
    auto &currentRow = m_childsItems[row];
    currentRow.append(item);

    return true;
}

TreeItem *TreeItem::child(int row, int column)
{
    if (row < 0 || row >= m_childsItems.size())
    {
        return nullptr;
    }

    if (column < 0 || column >= m_childsItems.at(row).size())
    {
        return nullptr;
    }

    return (m_childsItems.at(row)).at(column);
}

int TreeItem::childRowsCount() const
{
    return m_childsItems.count();
}

int TreeItem::columnsCountInRow(int row) const
{
    if (row > 0 && row < m_childsItems.size())
    {
        return (m_childsItems.at(row)).count();
    }
    return 0;
}

QVariant TreeItem::data(int role) const
{
    return m_itemData[role];
}

void TreeItem::setData(QVariant data, int role)
{
    m_itemData[role] = data;
}

int TreeItem::row() const
{
    int row    = 0;
    int column = -1;

    for (auto &currentRow : m_parent->m_childsItems)
    {
        column = currentRow.indexOf(const_cast<TreeItem *>(this));
        if (column != -1)
        {
            return row;
        }
        row++;
    }
    return row;
}

int TreeItem::column() const
{
    int row    = 0;
    int column = -1;

    for (auto &currentRow : m_parent->m_childsItems)
    {
        column = currentRow.indexOf(const_cast<TreeItem *>(this));
        if (column != -1)
        {
            return column;
        }
        row++;
    }
    return column;
}

TreeItem *TreeItem::parent() const
{
    return m_parent;
}
