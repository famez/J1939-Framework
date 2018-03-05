#ifndef KEYVALUELABEL_H
#define KEYVALUELABEL_H



#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QFrame>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QWidget>

class KeyValueLabel : public QWidget
{
public:
    KeyValueLabel(const QString& key, QWidget *parent);
    void setValue(const QString& value);

private:
    QWidget *mHorizontalLayoutWidget;
    QHBoxLayout *mHorizontalLayout;
    QLabel *mKeyLabel;
    QLabel *mValueLabel;
};

#endif // KEYVALUELABEL_H
