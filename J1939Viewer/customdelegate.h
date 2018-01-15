#ifndef CUSTOMDELEGATE_H
#define CUSTOMDELEGATE_H

#include <QStyledItemDelegate>


class FrameModel;

class CustomDelegate : public QStyledItemDelegate
{
private:

    FrameModel* mViewModel;

public:
    CustomDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;


    void setModel(FrameModel* model) { mViewModel = model; }
    void releaseModel() { mViewModel = NULL; }


    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void destroyEditor(QWidget *editor, const QModelIndex &index) const override;



};

#endif // CUSTOMDELEGATE_H
