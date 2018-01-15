
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QDebug>

#include <J1939Frame.h>
#include <GenericFrame.h>
#include <J1939DataBase.h>
#include <J1939Factory.h>

#include "framemodel.h"
#include "frameitem.h"
#include "genericframeitem.h"
#include "spnitem.h"

#include "framedialog.h"
#include "spndialog.h"


//Actions for context menu
#define CONTEXT_MENU_ADD_ACTION         "Add"
#define CONTEXT_MENU_DELETE_ACTION      "Delete"


using namespace J1939;

FrameModel::FrameModel(QWidget *parent)
    : QAbstractItemModel(parent), mCurrentItem(NULL),
      mEditable(false)
{
    QList<QVariant> rootData;
    rootData << "Name" << "PGN" << "Source" << "Dst" << "Priority";
    mRootItem = new TreeItem(rootData);
}

FrameModel::~FrameModel()
{
    delete mRootItem;
}

QVariant FrameModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        if(mCurrentItem) return mCurrentItem->getHeaderData(section);

    return QVariant();
}


QModelIndex FrameModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        qDebug() << "[FrameModel::index] Item added:" << childItem;
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex FrameModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == mRootItem)
        return QModelIndex();

    qDebug() << "[FrameModel::parent] Item added:" << parentItem;
    return createIndex(parentItem->row(), 0, parentItem);
}

int FrameModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = mRootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int FrameModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        TreeItem* item = static_cast<TreeItem*>(parent.internalPointer());

        if(item->childCount() > 0) {
            TreeItem* child = item->child(0);
            return child->columnCount();
        }
    }


    //To determine length of header
    int maxColumns = 0;

    getMaxColumns(mRootItem, maxColumns);

    return maxColumns;
}

QVariant FrameModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

//    qDebug() << "[FrameModel::data] Item retrieved:" << item;
    return item->data(index.column());
}


Qt::ItemFlags FrameModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;


    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());


    return ((mEditable && item->editable(index.column())) ? Qt::ItemIsEditable : Qt::NoItemFlags) | QAbstractItemModel::flags(index);
}

void FrameModel::addFrame(const J1939::J1939Frame& frame) {


    FrameItem* item = NULL;
    if(frame.isGenericFrame()) {
        GenericFrame& genFrame = (GenericFrame&)frame;
        item = new GenericFrameItem(mRootItem, &genFrame);
    } else {
        item = new FrameItem(mRootItem, &frame);
    }

    if(item) {
        beginInsertRows(QModelIndex(), mRootItem->childCount(), mRootItem->childCount());
        mRootItem->appendChild(item);
        endInsertRows();
    }


}

void FrameModel::addFrames(const std::vector<J1939::J1939Frame*>& frames) {
    for(auto iter = frames.begin(); iter != frames.end(); ++iter) {
        addFrame(*(*iter));
    }
}



void FrameModel::setCurrentItem(const QModelIndex & index) {
    if (!index.isValid()) {
        mCurrentItem = NULL;
        mCurrentIndex = QModelIndex();
    } else {
        mCurrentItem = static_cast<TreeItem*>(index.internalPointer());
        mCurrentIndex = index;
        emit headerDataChanged(Qt::Horizontal, 0, mCurrentItem->columnCount() - 1);
    }

}

bool FrameModel::setData(const QModelIndex &index, const QVariant &value,
                        int role) {

    if (!index.isValid())
        return false;



    if (role != Qt::EditRole)
        return false;

    //Delegate to the items
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    bool retVal = item->setData(index.column(), value);

    emit dataChanged(index, index);

    return retVal;

}

void FrameModel::itemContextMenu(QPoint pos) {

    QMenu myMenu;

    QWidget* tree = (QWidget*)(QObject::parent());

    QPoint globalPos = tree->mapToGlobal(pos);

    if(!mCurrentItem || mCurrentItem == mRootItem) {     //Add J1939 frame option

        QAction* addAction = myMenu.addAction(CONTEXT_MENU_ADD_ACTION);

        QAction* selectedAction = myMenu.exec(globalPos);

        if(selectedAction == addAction) {

            FrameDialog dialog;
            QDialog::DialogCode code = (QDialog::DialogCode)(dialog.exec());


            if(code == QDialog::Accepted) {

                u32 pgn = (u32)(dialog.getPGN());

                GenericFrame frame(pgn);
                addFrame(frame);
            }
        }
        return;
    }


    if(mCurrentItem->getItemType() == FRAME_TREE_ITEM || mCurrentItem->getItemType() == GENERICFRAME_TREE_ITEM) {
        FrameItem* frameItem = (FrameItem*) mCurrentItem;

        QAction* deleteAction = myMenu.addAction(CONTEXT_MENU_DELETE_ACTION);

        QAction* addAction = NULL;


        if(mCurrentItem->getItemType() == GENERICFRAME_TREE_ITEM) {
            addAction = myMenu.addAction(CONTEXT_MENU_ADD_ACTION);
        }

        QAction* selectedAction = myMenu.exec(globalPos);

        if(!selectedAction) return;

        if(selectedAction == addAction) {


            SpnDialog dialog;
            QDialog::DialogCode code = (QDialog::DialogCode)(dialog.exec());


            if(code == QDialog::Accepted) {

                int number = dialog.getSPNNumber();

                QString type = dialog.getSPNType();

                GenericFrameItem* genFrameItem = (GenericFrameItem*)(frameItem);

                //Check if there is already a SPN with the same number
                if(genFrameItem->hasSPN((u32)(number))) {

                    QMessageBox message;

                    message.setText("SPN already included in PGN");

                    message.exec();

                    return;
                }
                beginInsertRows(mCurrentIndex, mCurrentItem->childCount(), mCurrentItem->childCount());
                if(type == "Numeric") {
                    genFrameItem->addSPN(SPN::SPN_NUMERIC, number);

                } else if(type == "Status") {
                    genFrameItem->addSPN(SPN::SPN_STATUS, number);
                }
                endInsertRows();

            }
        } else if(selectedAction == deleteAction) {

            //Warning!! The view will refresh the current item at this point!!!!

            TreeItem* currentItem = mCurrentItem;


            beginRemoveRows(QModelIndex(), mCurrentIndex.row(), mCurrentIndex.row());

            mRootItem->deleteChild(currentItem);

            endRemoveRows();

        }

    } else if(mCurrentItem->getItemType() == SPN_TREE_ITEM) {

        QAction* deleteAction = myMenu.addAction(CONTEXT_MENU_DELETE_ACTION);
        QAction* selectedAction = myMenu.exec(globalPos);

        SPNItem* spn = (SPNItem*)(mCurrentItem);

        if(deleteAction == selectedAction) {
            TreeItem* parent = mCurrentItem->parentItem();

            //Warning!! The view will refresh the current item at this point!!!!
            beginRemoveRows(mCurrentIndex.parent(), mCurrentIndex.row(), mCurrentIndex.row());

            GenericFrameItem* genFrameItem = (GenericFrameItem*)(parent);
            genFrameItem->deleteSPN(spn);

            endRemoveRows();
        }

    }

}


void FrameModel::clear() {

    beginRemoveRows(QModelIndex(), 0, mRootItem->childCount());

    mRootItem->deleteChildren();

    endRemoveRows();

    mCurrentItem = NULL;
    mCurrentIndex = QModelIndex();

}


void FrameModel::getMaxColumns(const TreeItem* item, int& maxValue) const {

    if(item->columnCount() > maxValue) {
        maxValue = item->columnCount();
    }

    if(item->childCount() > 0) {
        for(int i = 0; i < item->childCount(); ++i) {
            const TreeItem* child = item->constChild(i);
            getMaxColumns(child, maxValue);
        }
    }

}

void FrameModel::onSave(const QString& path) {

    J1939DataBase db;

    for(int i = 0; i < mRootItem->childCount(); ++i) {
        TreeItem* child = mRootItem->child(i);

        if(child->getItemType() == GENERICFRAME_TREE_ITEM) {
            GenericFrameItem* frameItem = (GenericFrameItem*)child;
            db.addFrame(*(frameItem->getGenericFrame()));
        }

    }

    std::string stdPath = path.toUtf8().constData();

    db.writeJsonFile(stdPath);


}

void FrameModel::onOpen(const QString& path) {

    J1939DataBase database;

    database.parseJsonFile(path.toUtf8().constData());

    const std::vector<GenericFrame>& genericFrames = database.getParsedFrames();

    J1939Factory::getInstance().unregisterAllFrames();
    J1939Factory::getInstance().registerPredefinedFrames();

    std::vector<J1939Frame*> frames;

    for(auto iter = genericFrames.begin(); iter != genericFrames.end(); ++iter) {
        frames.push_back((J1939Frame*)(&(*iter)));
        J1939Factory::getInstance().registerFrame(*iter);
    }

    clear();

    addFrames(frames);

}


void FrameModel::updateFrame(const J1939::J1939Frame& /*frame*/) {

    /*u32 pgn = frame.getPGN();
    u8 src = frame.getSrcAddr();
    for(int i = 0; i < mRootItem->childCount(); ++i) {
        TreeItem* child = mRootItem->child(i);

        if(child->getItemType() == GENERICFRAME_TREE_ITEM) {
            GenericFrameItem* frameItem = (GenericFrameItem*)child;
            const J1939Frame* frameToUpdt = frameItem->getFrame();

            if(pgn == frameToUpdt->getPGN() && src == frameToUpdt->getSrcAddr()) {

                frameToUpdt->copy(frame);
                return;
            }
        }

    }*/
}


bool FrameModel::hasFrame(u32 pgn, u8 src) {

    for(int i = 0; i < mRootItem->childCount(); ++i) {
        TreeItem* child = mRootItem->child(i);

        FrameItem* item = dynamic_cast<FrameItem*>(child);
        const J1939Frame* frame = item->getFrame();

        if(pgn == frame->getPGN() && src == frame->getSrcAddr()) {
            return true;
        }

    }

    return false;

}

QList<QModelIndex> FrameModel::getIndexesWithPersistentEdition(const QModelIndex& parent, int first, int last) const {

    QList<QModelIndex> retVal;

    for(int row = first; row <= last; ++row) {
        QModelIndex child = index(row, 0, parent);
        TreeItem *item = static_cast<TreeItem*>(child.internalPointer());

        for(int column = 0; column < item->columnCount(); ++column) {

            if(item->persistentEdition(column)) {
                QModelIndex modelIndex = index(row, column, parent);
                retVal.append(modelIndex);
            }
        }

        retVal.append(getIndexesWithPersistentEdition(child, 0, item->childCount() - 1));
    }

    return retVal;

}


EDelegateType FrameModel::getDelegateType(const QModelIndex& index) const {

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->getDelegateType(index.column());

}


TreeViewCustomEditor* FrameModel::getCustomEditor(const QModelIndex& index) const {

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    return item->getCustomEditor(index.column());

}
