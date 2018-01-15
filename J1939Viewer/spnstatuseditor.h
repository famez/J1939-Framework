#ifndef SPNSTATUSEDITOR_H
#define SPNSTATUSEDITOR_H

#include <QMap>
#include <Types.h>
#include "customeditor.h"

typedef QMap<u8, QString> StatusMap;



class QComboBox;
class QToolButton;

class SpnStatusEditor : public TreeViewCustomEditor
{
    Q_OBJECT
private:

    /*
     * To store localy the data
     */
    StatusMap mData;

    QComboBox* mCombobox;
    QToolButton* mAddButton;
    QToolButton* mDelButton;
    QToolButton* mEditButton;

private slots:
    void addButtonClicked();
    void delButtonClicked();
    void editButtonClicked();

protected:
    void regionToPaintChanged(const QRect& region) final;

public:
    SpnStatusEditor();
    virtual ~SpnStatusEditor();

    void setData(const QVariant& data);
    QVariant getData() const;

};


//To make typename known to QVariant
Q_DECLARE_METATYPE(StatusMap)

#endif // SPNSTATUSEDITOR_H
