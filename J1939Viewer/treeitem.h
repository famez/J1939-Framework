#ifndef TREEITEM_H
#define TREEITEM_H

#include <QSet>
#include <QVariant>
#include <QAction>

#include "common.h"
#include "customeditor.h"


#define DEFAULT_TREE_ITEM       "default_tree_item"

class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, TreeItem *parentItem = 0);
    explicit TreeItem(TreeItem *parentItem = 0);
    virtual ~TreeItem();


    TreeItem *child(int row);
    const TreeItem *constChild(int row) const;
    int childCount() const;
    virtual int columnCount() const;
    virtual QVariant data(int column) const;
    virtual bool setData(int, const QVariant&) { return false; }

    void deleteChild(TreeItem *);

    void deleteChildren();

    int row() const;
    TreeItem *parentItem();
    void appendChild(TreeItem *child);
    bool editable(int column) const { return (mEditableColumns.find(column) != mEditableColumns.end()); }
    bool persistentEdition(int column) const { return (mPersistentEditableColumns.find(column) != mPersistentEditableColumns.end()); }
    EDelegateType getDelegateType(int column) const;

    virtual QVariant getHeaderData(int section) const;

    virtual QString getItemType() const { return DEFAULT_TREE_ITEM; }


    /*
     * If the tree item needs to display the data in a different way other than the usual one, reimplement this function by allocating the object and returning the pointer to the instance.
     * It is important to remark that the object must be allocated first because the class in charge to display the editor and commit its data will become the owner of the object.
     */
    virtual TreeViewCustomEditor* getCustomEditor(int column) const { Q_UNUSED(column); return NULL; }


private:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
    QSet<int> mEditableColumns;
    QSet<int> mPersistentEditableColumns;
    QMap<int, EDelegateType> mDelagateTypes;


protected:
    void setColumnEditable(int column, bool editable);
    void setColumnPersistentEdition(int column, bool persistent);
    void setDelegateType(int column, EDelegateType type);

};

#endif // TREEITEM_H
