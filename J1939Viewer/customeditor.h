/*
 * The aim of tree view custom editor is to provide an interface to add custom editors to a treeview items,
 * in first place, this is used to add custom edition to status spn
 */

#ifndef CUSTOMEDITOR_H
#define CUSTOMEDITOR_H


#include <QWidget>

class TreeViewCustomEditor : public QWidget
{
    Q_OBJECT
private:


    /*
     * Current region to paint.
     */
    QRect mRegion;


protected:
    void paintEvent(QPaintEvent *event) override;

    /*
     * If the region to paint has changed, we notify to the extended clases. It is important not to call update() or repaint() methods as
     * this method is called from paintEvent() method.
     */
    virtual void regionToPaintChanged(const QRect& region) = 0;
public:
    TreeViewCustomEditor();
    virtual void setData(const QVariant& data) = 0;
    virtual QVariant getData() const = 0;
    void commitData() { emit commit(this); }

signals:
    void commit(QWidget*);

};

#endif // CUSTOMEDITOR_H
