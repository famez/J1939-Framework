
#include <QtAlgorithms>
#include <QVariant>
#include <QDebug>

#include "treeitem.h"


TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;

    qDebug() << "[TreeItem::TreeItem] Item created: " << this;
}

TreeItem::TreeItem(TreeItem *parent)
{
    m_parentItem = parent;
    qDebug() << "[TreeItem::TreeItem] 2. Item created: " << this;
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
    qDebug() << "[TreeItem::TreeItem] Item destroyed: " << this;
}


void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    return m_childItems.value(row);
}


const TreeItem *TreeItem::constChild(int row) const
{
    return m_childItems.value(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const
{
    return 1;
}

QVariant TreeItem::data(int column) const
{
    return m_itemData.value(column);
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

QVariant TreeItem::getHeaderData(int section) const {
    if(section == 0)
        return "Select a frame";
    return "";
}

void TreeItem::setColumnEditable(int column, bool editable) {

    if(editable) {
        mEditableColumns.insert(column);
    } else{

        auto iter = mEditableColumns.find(column);

        if(iter != mEditableColumns.end()) {
            mEditableColumns.erase(iter);
        }
    }
}


void TreeItem::setColumnPersistentEdition(int column, bool editable) {

    if(editable) {
        mPersistentEditableColumns.insert(column);
    } else{

        auto iter = mPersistentEditableColumns.find(column);

        if(iter != mPersistentEditableColumns.end()) {
            mPersistentEditableColumns.erase(iter);
        }
    }
}

void TreeItem::setDelegateType(int column, EDelegateType type) {

    auto iter = mDelagateTypes.find(column);

    if(type == DEL_TYPE_NONE) {
        if(iter != mDelagateTypes.end()) {
            mDelagateTypes.erase(iter);
        }

    } else {
            mDelagateTypes[column] = type;
    }

}

EDelegateType TreeItem::getDelegateType(int column) const {

    auto iter = mDelagateTypes.find(column);

    if(iter == mDelagateTypes.end()) {
        return DEL_TYPE_NONE;
    }

    return iter.value();

}

void TreeItem::deleteChild(TreeItem *child) {

    for(auto iter = m_childItems.begin(); iter != m_childItems.end(); ++iter) {
        if(child == *iter) {
            delete child;
            m_childItems.erase(iter);
            break;
        }
    }
}


void TreeItem::deleteChildren() {


    for(auto iter = m_childItems.begin(); iter != m_childItems.end(); ++iter) {

        delete *iter;

    }

    m_childItems.clear();

}

