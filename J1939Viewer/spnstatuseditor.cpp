#include <QDebug>
#include <QComboBox>
#include <QToolButton>
#include <QMessageBox>


#include "spnstatuseditor.h"
#include "spnstatusdialog.h"

SpnStatusEditor::SpnStatusEditor() {

    mCombobox = new QComboBox(this);
    mAddButton = new QToolButton(this);
    mDelButton = new QToolButton(this);
    mEditButton = new QToolButton(this);

    {
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVG/plus.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mAddButton->setIcon(icon);
    }

    {
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVG/minus.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mDelButton->setIcon(icon);
    }

    {
        QIcon icon;
        icon.addFile(QStringLiteral(":/SVG/edit.svg"), QSize(), QIcon::Normal, QIcon::Off);
        mEditButton->setIcon(icon);
    }

    connect(mAddButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    connect(mDelButton, SIGNAL(clicked(bool)), this, SLOT(delButtonClicked()));
    connect(mEditButton, SIGNAL(clicked(bool)), this, SLOT(editButtonClicked()));

}


void SpnStatusEditor::setData(const QVariant& data) {

    if(!data.canConvert<StatusMap>()) {
        qDebug() << "[SpnStatusEditor::setData] The returned argument is not a StatusMap";
        return;
    }

    //Cast data to StatusMap
    mData = qvariant_cast<StatusMap>(data);

    //Clear combobox to refill it
    mCombobox->clear();

    //Fill the combobox with the values from QVariantMap
    for(auto iter = mData.begin(); iter != mData.end(); ++iter) {

        QString statusNumber = QString::number(iter.key());
        mCombobox->addItem("(" + statusNumber + ") " + iter.value(), QVariant(iter.key()));
    }

}


QVariant SpnStatusEditor::getData() const {

    return QVariant::fromValue(mData);

}

SpnStatusEditor::~SpnStatusEditor() {

}


void SpnStatusEditor::regionToPaintChanged(const QRect& region) {

    //Adjust geometry to region to paint
    QRect comboboxGeometry = mCombobox->geometry();
    comboboxGeometry.setHeight(region.height());
    mCombobox->setGeometry(comboboxGeometry);
    //mCombobox->setSizeAdjustPolicy();

    //Adjust geometry to region to paint
    QRect buttonGeo;
    buttonGeo.setLeft(comboboxGeometry.right() + 1);
    buttonGeo.setWidth(region.height());
    buttonGeo.setTop(0);
    buttonGeo.setHeight(region.height());
    mAddButton->setGeometry(buttonGeo);

    //Adjust geometry to region to paint
    buttonGeo.setLeft(buttonGeo.right() + 1);
    buttonGeo.setWidth(region.height());
    mDelButton->setGeometry(buttonGeo);

    //Adjust geometry to region to paint
    buttonGeo.setLeft(buttonGeo.right() + 1);
    buttonGeo.setWidth(region.height());
    mEditButton->setGeometry(buttonGeo);

}


void SpnStatusEditor::addButtonClicked() {

    SpnStatusDialog statusDialog;

    QDialog::DialogCode code = (QDialog::DialogCode)(statusDialog.exec());

    if(code == QDialog::Accepted) {

        u8 number = statusDialog.getNumber();
        QString desc = statusDialog.getDescription();

        if(mData.find(number) == mData.end()) {
            mData[number] = desc;
            commitData();
        } else {
            QMessageBox box;
            box.setText("There is already a status with this number");
            box.exec();
        }
    }
}


void SpnStatusEditor::delButtonClicked() {

    QVariant data = mCombobox->currentData();

    //In case there is at least one item, we will be able to do the conversion
    //Otherwise, the conversion will fail
    if(data.canConvert<u8>()) {
        u8 statNumber = qvariant_cast<u8>(data);
        auto iter = mData.find(statNumber);

        if(iter != mData.end()) {
            mData.erase(iter);
            commitData();
        }
    }
}


void SpnStatusEditor::editButtonClicked() {


    QVariant data = mCombobox->currentData();

    //In case there is at least one item, we will be able to do the conversion
    //Otherwise, the conversion will fail
    if(!data.canConvert<u8>()) {
        return;
    }


    u8 statNumber = qvariant_cast<u8>(data);
    auto iter = mData.find(statNumber);

    if(iter == mData.end()) {
        return;
    }


    SpnStatusDialog statusDialog;

    statusDialog.setNumber(statNumber);
    statusDialog.setDescription(iter.value());

    QDialog::DialogCode code = (QDialog::DialogCode)(statusDialog.exec());

    if(code == QDialog::Accepted) {

//        u8 number = statusDialog.getNumber();
        QString desc = statusDialog.getDescription();
        iter.value() = desc;
        commitData();
    }

}
