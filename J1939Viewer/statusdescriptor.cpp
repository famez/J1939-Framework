#include <QHBoxLayout>

#include "statusdescriptor.h"

StatusDescriptor::StatusDescriptor(const QString &description, QColor color, QWidget *parent) :
    QWidget(parent)
{

    mHorizontalLayout = new QHBoxLayout(this);
    mHorizontalLayout->setSpacing(1);
    mHorizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    mHorizontalLayout->setContentsMargins(0, 0, 0, 0);
    mStatusColorFrame = new QFrame(this);
    QSizePolicy frameSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    mStatusColorFrame->setSizePolicy(frameSizePolicy);
    mStatusColorFrame->setMinimumSize(QSize(10, 10));
    mStatusColorFrame->setFrameShape(QFrame::Box);
    mStatusColorFrame->setFrameShadow(QFrame::Plain);
    mHorizontalLayout->addWidget(mStatusColorFrame);
    mLabel = new QLabel(description, this);
    QSizePolicy labelSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    mLabel->setSizePolicy(labelSizePolicy);
    mHorizontalLayout->addWidget(mLabel);
    mHorizontalLayout->setStretch(0, 1);

    QPalette palette;
    palette.setColor(QPalette::Foreground, color);
    color.setAlpha(80);
    palette.setColor(QPalette::Background, color);
    mStatusColorFrame->setPalette(palette);
    mStatusColorFrame->setAutoFillBackground(true);

}

StatusDescriptor::~StatusDescriptor() {

}

