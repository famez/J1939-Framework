

#include <GenericFrame.h>
#include <SPN/SPNNumeric.h>
#include <SPN/SPNStatus.h>
#include <J1939DataBase.h>

#include <QMenu>
#include <QAction>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framemodel.h"



#include <QDebug>



using namespace J1939;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTreeView* tree = ui->treeView;

    FrameModel* model = new FrameModel(tree);

    //To change frames and several items from the database, enable edition.
    model->enableEdition(true);

    tree->setModel(model);


    QMenu* menu = new QMenu(tr("&File"), this);


    /* Add some choices to the menu. */
    menu->addAction(tr("&Open"),
                   this, SLOT(open()), tr("Ctrl+O"));
    mSaveAction = menu->addAction(tr("Save"),
                   this, SLOT(save()), tr("Ctrl+S"));

    mSaveAction->setEnabled(false);     //Not possible until database open

    menu->addAction(tr("&Inspect/Edit TRC file..."),
                   this, SLOT(openTRCInspector()), tr("Ctrl+T"));


    menuBar()->addMenu(menu);

    tree->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(tree, SIGNAL(customContextMenuRequested(QPoint)), model, SLOT(itemContextMenu(QPoint)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open() {

    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Database"), "", tr("Json Files (*.json)"));

    if(fileName == "") return;



    mCurrentPath = fileName;
    mSaveAction->setEnabled(true);

    FrameModel* model = (FrameModel*)(ui->treeView->model());

    model->onOpen(mCurrentPath);

}


void MainWindow::save() {

    if(!mCurrentPath.size()) return;

    FrameModel* model = (FrameModel*)(ui->treeView->model());

    model->onSave(mCurrentPath);


}


void MainWindow::openTRCInspector() {

    mInspector.show();

    mInspector.loadFile();

}
