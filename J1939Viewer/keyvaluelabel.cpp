#include "keyvaluelabel.h"

KeyValueLabel::KeyValueLabel(const QString& key, QWidget *parent) : QWidget(parent)
{

    QSizePolicy labelSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    mHorizontalLayout = new QHBoxLayout(this);
    mHorizontalLayout->setSpacing(1);
    mHorizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    mHorizontalLayout->setContentsMargins(0, 0, 0, 0);

    mKeyLabel = new QLabel(key + ":", this);

    mKeyLabel->setSizePolicy(labelSizePolicy);
    mHorizontalLayout->addWidget(mKeyLabel);
    mValueLabel = new QLabel(this);
    mValueLabel->setSizePolicy(labelSizePolicy);
    mHorizontalLayout->addWidget(mValueLabel);
    mHorizontalLayout->setStretch(0, 1);

}


void KeyValueLabel::setValue(const QString& value) {

    mValueLabel->setText(value);

}
