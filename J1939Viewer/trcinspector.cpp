#include <QMenu>
#include <QFileDialog>
#include <QDebug>

#include <J1939Frame.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>
#include <GenericFrame.h>



#include "trcinspector.h"
#include "ui_trcinspector.h"
#include "trcloader.h"
#include "framemodel.h"
#include "numericspnitem.h"
#include "statusspnitem.h"
#include "spnchart.h"
#include "spnloader.h"
//#include "colouredrangesarea.h"
//#include "scrollzoomstatusrangearea.h"
#include "spnstatusedition.h"

using namespace J1939;

TRCInspector::TRCInspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TRCInspector),
    mLoader(NULL)
{
    ui->setupUi(this);

    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);

    QTreeView* tree = ui->treeView;

    FrameModel* model = new FrameModel(tree);

    tree->setModel(model);

}

TRCInspector::~TRCInspector() {
    delete ui;
}


void TRCInspector::loadFile() {

    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open TRC file"), "", tr("TRC Files (*.trc)"));

    if(fileName == "") {
        qDebug() << "No filename specified";
        return;
    }

    startBackgroundLoader(fileName);

}

void TRCInspector::loaderFinished() {
    mLoader->wait();

    disconnect(mLoader);

    delete mLoader;

    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);

    FrameModel* model = (FrameModel*)(ui->treeView->model());

    for(auto iter = mFrameList.begin(); iter != mFrameList.end(); ++iter) {
        J1939Frame* frame = iter->second;
        if(!model->hasFrame(frame->getPGN(), frame->getSrcAddr())) {
            model->addFrame(*frame);
        }
    }

    //To enable double click over SPNs and inspect them
    connect(ui->treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(itemClicked(QModelIndex)));

}

void TRCInspector::startBackgroundLoader(const QString& file) {

    mLoader = new TRCLoader(file, &mFrameList);

    connect(mLoader, SIGNAL(finished()), this, SLOT(loaderFinished()));
    connect(mLoader, SIGNAL(progressUpdated(int)), this, SLOT(updateProgress(int)));

    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    mLoader->start();
}

void TRCInspector::updateProgress(int progress) {
    ui->progressBar->setValue(progress);
}


void TRCInspector::itemClicked(QModelIndex index) {

    //Managed out of scope of tree model cause only we need to know if the item is a SPN and
    //the number of the SPN.

    TreeItem* item = (TreeItem*)(index.internalPointer());

    NumericSPNItem* numSpnItem = dynamic_cast<NumericSPNItem*>(item);

    if(numSpnItem) {

        //showNumericSPNViewer(numSpnItem);

    }

    StatusSPNItem* statSpnItem = dynamic_cast<StatusSPNItem*>(item);

    bool initialized = false;
    u8 lastStatus = 0;
    int count = 0;

    if(statSpnItem) {

        //Calculate the last timestamp
        u64 time = mFrameList.back().first;
//        StatusRangesArea* area = new StatusRangesArea(time);

        SPNStatus* spn = (SPNStatus*)(statSpnItem->getSPN());

        SpnStatusEdition* statEditor = new SpnStatusEdition(*spn, time);

//        ScrollZoomWidget* widget = new ScrollZoomWidget(statEditor);

        for(auto iter = mFrameList.begin(); iter != mFrameList.end(); ++iter) {

            if(iter->second->isGenericFrame()) {
                GenericFrame* frame = static_cast<GenericFrame*>(iter->second);
                u32 spnNumber = statSpnItem->getSPNNumber();

                if(frame->hasSPN(spnNumber)) {
                    SPNStatus* status = static_cast<SPNStatus*>(frame->getSPN(spnNumber));
                    u8 currentStatus = status->getValue();
                    if(!initialized || lastStatus != currentStatus) {
                        statEditor->addStatusRange(iter->first, currentStatus);
                        initialized = true;
                        lastStatus = currentStatus;
                        count++;
                    }

                }
            }
        }

        qDebug() << "[TRCInspector::itemClicked] Number of items: " << count;

//        widget->show();

        statEditor->show();

    }

}


void TRCInspector::showNumericSPNViewer(NumericSPNItem* spnItem) {

    u32 number = spnItem->getSPNNumber();

    //We know that spn is a numeric spn because of dynamic cast is successfull
    Q_ASSERT(spnItem->getSPN()->getType() == SPN::SPN_NUMERIC);

    const SPNNumeric* spn = static_cast<const SPNNumeric*>(spnItem->getSPN());

    //Calculate ranges of the chart

    SPNNumeric spnCopy(*spn);

    spnCopy.setValue(0);

    int bottom = spnCopy.getFormatedValue();

    spnCopy.setValue(spn->getMaxValue());

    int top = spnCopy.getFormatedValue();

    /*QRect ranges;

    ranges.setLeft(0);
    ranges.setRight(mFrameList.back().first);       //Last timestamp
    ranges.setBottom(bottom - 5);
    ranges.setTop(top + 5);

    SPNViewer* chart = new SPNViewer(number, QString::fromStdString(spn->getName()), QString::fromStdString(spn->getUnits()), ranges);

    chart->visualize();

    SPNLoader* loader = new SPNLoader(chart, &mFrameList);


    loader->start();*/

}
