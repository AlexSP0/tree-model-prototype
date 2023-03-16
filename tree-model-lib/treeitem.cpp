#include "treeitem.h"

TreeItem::TreeItem(TreeItem *parent)
    : m_parent(parent)
    , type(ItemType::simple)
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
    item->m_parent = this;

    m_childsItems.append(QVector{item});

    return true;
}

bool TreeItem::appendItemExitingRow(TreeItem *item, int row)
{
    if (row >= childRowsCount())
    {
        return false;
    }

    item->m_parent = this;

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
    auto i = m_childsItems.count();
    return i;
}

int TreeItem::columnsCountInRow(int row) const
{
    if (row >= 0 && row < m_childsItems.size())
    {
        auto i = (m_childsItems.at(row)).count();
        return i;
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
