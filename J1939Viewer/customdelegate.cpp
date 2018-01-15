#include <QDebug>
#include <QComboBox>

#include "customdelegate.h"
#include "treeitem.h"
#include "statusspnitem.h"
#include "framemodel.h"
#include "customeditor.h"


CustomDelegate::CustomDelegate() : mViewModel(NULL) {

}


void CustomDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const {

    QStyledItemDelegate::paint(painter, option, index);
}

QSize CustomDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const {
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget* CustomDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const {

    if (!index.isValid() || !mViewModel)
        return QStyledItemDelegate::createEditor(parent, option, index);

    //Be careful when calling getCustomEditor method, the caller is in charge of releasing the pointer
    TreeViewCustomEditor* editor = mViewModel->getCustomEditor(index);

    if(editor) {
        //If we don't set the parent, the widget will be shown as an independent window
        editor->setParent(parent);
        editor->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
        connect(editor, SIGNAL(commit(QWidget*)), this, SIGNAL(commitData(QWidget*)));      //Actually lets commit data when calling commitData() function over TreeViewCustomEditor class
        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void CustomDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

    if (!index.isValid() || !mViewModel)
        return;

     TreeViewCustomEditor* customEditor = dynamic_cast<TreeViewCustomEditor*>(editor);

    if(customEditor) {

        //The widget is a custom editor
        customEditor->setData(index.data());

        return;
    }

    QStyledItemDelegate::setEditorData(editor, index);

}


void CustomDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const {

    if (!index.isValid() || !mViewModel)
        return;

    TreeViewCustomEditor* customEditor = dynamic_cast<TreeViewCustomEditor*>(editor);

    if(customEditor) {
        mViewModel->setData(index, customEditor->getData());
        return;
    }

    QStyledItemDelegate::setModelData(editor, model, index);

}


void CustomDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const {

    disconnect(editor);

    QStyledItemDelegate::destroyEditor(editor, index);
}


