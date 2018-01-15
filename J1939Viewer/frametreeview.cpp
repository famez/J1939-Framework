#include <QMouseEvent>

#include "frametreeview.h"
#include "framemodel.h"


#include "treeitem.h"
#include "statusspnitem.h"


FrameTreeView::FrameTreeView(QWidget *parent) : QTreeView(parent) {

    /*
     * With this, it is possible to display comboboxes in the treeview such as for example to show the different values for the status of a SPN.
     */
    setItemDelegate(&mDelegate);

}


FrameTreeView::~FrameTreeView() {

    setItemDelegate(NULL);

}

//Override to change the header dynamically
void FrameTreeView::currentChanged(const QModelIndex & current, const QModelIndex & previous) {

    FrameModel* frameModel = (FrameModel*)(model());

    if(frameModel) {
        frameModel->setCurrentItem(current);
    }


    QTreeView::currentChanged(current, previous);
}


void FrameTreeView::mousePressEvent(QMouseEvent * event) {


    //To unset current index when context menu is pressed outside a valid item

    if(event->button() == Qt::RightButton) {
        setCurrentIndex(QModelIndex());
    }


    QTreeView::mousePressEvent(event);

}


void FrameTreeView::rowsInserted(const QModelIndex &parent,int first, int last) {

    FrameModel* frameModel = (FrameModel*)(model());
    QList<QModelIndex> indexes = frameModel->getIndexesWithPersistentEdition(parent, first, last);

    for(auto iter = indexes.begin(); iter != indexes.end(); ++iter) {
        openPersistentEditor(*iter);
    }

    QTreeView::rowsInserted(parent, first, last);
}

void FrameTreeView::setModel(QAbstractItemModel *model) {

    QTreeView::setModel(model);

    FrameModel* frameModel = (FrameModel*)(model);

    mDelegate.setModel(frameModel);


}
