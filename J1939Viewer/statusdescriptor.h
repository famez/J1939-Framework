/*
 * Widget to show the description of a status item with its associated color.
 */


#ifndef STATUSDESCRIPTOR_H
#define STATUSDESCRIPTOR_H


#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QWidget>

QT_BEGIN_NAMESPACE


class QHBoxLayout;

class StatusDescriptor : public QWidget
{
    Q_OBJECT

public:
    explicit StatusDescriptor(const QString& description, QColor color, QWidget *parent = 0);
    ~StatusDescriptor();


private:
    QWidget *mHorizontalLayoutWidget;
    QHBoxLayout *mHorizontalLayout;
    QFrame *mStatusColorFrame;
    QLabel *mLabel;

};

#endif // STATUSDESCRIPTOR_H
