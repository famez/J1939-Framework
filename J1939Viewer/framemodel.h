#ifndef FRAMEMODEL_H
#define FRAMEMODEL_H

#include <QAbstractItemModel>
#include <QPoint>
#include <Types.h>

#include "treeitem.h"
#include "customeditor.h"

namespace J1939 {
class J1939Frame;
}

class FrameModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FrameModel(QWidget *parent = 0);
    ~FrameModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation,
                       int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                     const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    void setCurrentItem(const QModelIndex & index);


    void addFrame(const J1939::J1939Frame&);
    void addFrames(const std::vector<J1939::J1939Frame *> &);

    /*
     * Updates the data related to a frame
     */
    void updateFrame(const J1939::J1939Frame&);

    /*
     * Returns true if there is already a frame with the same soure and pgn
     */

    bool hasFrame(u32 pgn, u8 src);


    /*
     * Clears the whole model unpopulating it
     */
    void clear();

    /*
     *  Called when it is necessary to save the current model to persistent
     * in [path] File where to store the data
     */
    void onSave(const QString& path);


    /*
     * Called when a J1939 Database is opened to be read
     * in [path] File where to store the data
     */
    void onOpen(const QString& path);

    /*
     * If parameter is equals to true, edition is allowed for those items whose columns are marked as editable.
     * Otherwise, if parameter is false, the model is not editable at all.
     */
    void enableEdition(bool editable) { mEditable = editable; }

    /*
     * Checks if the given index and/or its children require persistent edition and returns the affected items in a list
     *
     */

    QList<QModelIndex> getIndexesWithPersistentEdition(const QModelIndex& parent, int first, int last) const;


    /*
     * Returns the type of delegate for the model index
     */
    EDelegateType getDelegateType(const QModelIndex&index) const;


    /*
     * Returns the custom editor for the given index.
     */
    TreeViewCustomEditor* getCustomEditor(const QModelIndex& index) const;

public slots:

    void itemContextMenu(QPoint);


private:
   TreeItem *mRootItem;
   TreeItem *mCurrentItem;
   QModelIndex mCurrentIndex;
   bool mEditable;

   void getMaxColumns(const TreeItem *item, int &maxValue) const;



signals:
    void actionAddFrame(J1939::J1939Frame*);


};

#endif // FRAMEMODEL_H
