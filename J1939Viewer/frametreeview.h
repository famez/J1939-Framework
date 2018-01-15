#ifndef FRAMETREEVIEW_H
#define FRAMETREEVIEW_H

#include <QTreeView>

#include "customdelegate.h"

class FrameTreeView : public QTreeView
{
    Q_OBJECT
private:
    CustomDelegate mDelegate;

protected slots:
    void rowsInserted(const QModelIndex &parent, int start, int last) final;

public:
    explicit FrameTreeView(QWidget *parent = Q_NULLPTR);
    virtual ~FrameTreeView();


    void currentChanged(const QModelIndex & current, const QModelIndex & previous);
    void mousePressEvent(QMouseEvent * event);

    void setModel(QAbstractItemModel *model) override;


};

#endif // FRAMETREEVIEW_H
