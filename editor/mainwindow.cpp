#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdirectxwidget.h"
#include "mytreeitem.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),
            this, SLOT(updateText(int)));

    connect(ui->calcBt, SIGNAL(clicked()),
            this, SLOT( startSym() ) );

    connect(ui->toolBar, SIGNAL(resetPressed()),
            ui->scene, SLOT(cameraReset()));
    connect(ui->toolBar, SIGNAL(stateChanged(int)),
            ui->scene, SLOT(changeState(int)));

    connect(ui->toolBar, SIGNAL(createBoxClicked()),
            ui->scene, SLOT(createBox()));
    connect(ui->toolBar, SIGNAL(createSphereClicked()),
            ui->scene, SLOT(createSphere()));
    connect(ui->toolBar, SIGNAL(createDummyClicked()),
            ui->scene, SLOT(createDummy()));

    connect(ui->scene,SIGNAL(modelChanged(QList<QString>)),
            ui->listWidget, SLOT(updateMap(QList<QString>)));

    connect(ui->scene,SIGNAL(calcStart()),
            this, SLOT(startedSym()));
    connect(ui->scene,SIGNAL(calcUpdated(int)),
            this, SLOT(updatedSym(int)));
    connect(ui->scene,SIGNAL(calcStop()),
            this, SLOT(finishedSym()));

    connect(ui->listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this,SLOT(itemChanged(QListWidgetItem*,QListWidgetItem*)));

    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),
            ui->scene,SLOT(setSec(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::updateText(int value){
    ui->lineEdit->setText(QString::number(value));
}

void MainWindow::on_actNew_triggered()
{
    ui->scene->clear();
}
void MainWindow::on_actAbout_triggered()
{
    QMessageBox::about(this,QString("About"),QString("Epilepsy Editor. 2010.\r\n Creators: Semenychev A., Zuzin A.\r\n Gr. P-61 SibSUTIS"));
}

void MainWindow::on_actExit_triggered()
{
    close();
}

void MainWindow::startSym(){
    ui->scene->startSim(ui->calcSb->value(), ui->retSB->value(), ui->muSB->value());
}

void MainWindow::startedSym(){
    ui->calcBt->setEnabled(false);
    ui->calcSb->setEnabled(false);
    ui->horizontalSlider->setMaximum(ui->calcSb->value());
    ui->horizontalSlider->setValue(0);
    ui->scene->setSec(0);
}

void MainWindow::updatedSym(int val){
    ui->progressBar->setValue(val);
}

void MainWindow::finishedSym(){
    ui->calcBt->setEnabled(true);
    ui->calcSb->setEnabled(true);
    ui->scene->setCalc(true);
}

void MainWindow::updateObject(QModelIndex indx,QModelIndex id){
    Q_UNUSED(id)
    IObject* object = ui->scene->getObjectByName(ui->listWidget->selectedItems()[0]->text());

    QAbstractItemModel *model = ui->tableView->model();

    if(object->getType() == IObject::EP_BOX){
        object->setPosition(D3DXVECTOR3(model->data(model->index(0,0,QModelIndex())).toFloat(),
                                        model->data(model->index(1,0,QModelIndex())).toFloat(),
                                        model->data(model->index(2,0,QModelIndex())).toFloat()));
        object->setRotation(D3DXVECTOR3(model->data(model->index(3,0,QModelIndex())).toFloat(),
                                        model->data(model->index(4,0,QModelIndex())).toFloat(),
                                        model->data(model->index(5,0,QModelIndex())).toFloat()));
        object->setSize(D3DXVECTOR3(model->data(model->index(6,0,QModelIndex())).toFloat(),
                                        model->data(model->index(7,0,QModelIndex())).toFloat(),
                                        model->data(model->index(8,0,QModelIndex())).toFloat()));
        object->setDentisty(model->data(model->index(9,0,QModelIndex())).toFloat());
        object->setReturation(model->data(model->index(10,0,QModelIndex())).toFloat());

    }
    else if(object->getType() == IObject::EP_SPHERE){
        object->setPosition(D3DXVECTOR3(model->data(model->index(0,0,QModelIndex())).toFloat(),
                                        model->data(model->index(1,0,QModelIndex())).toFloat(),
                                        model->data(model->index(2,0,QModelIndex())).toFloat()));
        object->setRotation(D3DXVECTOR3(model->data(model->index(3,0,QModelIndex())).toFloat(),
                                        model->data(model->index(4,0,QModelIndex())).toFloat(),
                                        model->data(model->index(5,0,QModelIndex())).toFloat()));
        object->setSize(D3DXVECTOR3(model->data(model->index(6,0,QModelIndex())).toFloat(),
                                        0,
                                        0));
        object->setDentisty(model->data(model->index(7,0,QModelIndex())).toFloat());
        object->setReturation(model->data(model->index(8,0,QModelIndex())).toFloat());
    }
    ui->scene->setCalc(false);
}

void MainWindow::itemChanged(QListWidgetItem* cur, QListWidgetItem* prev){
    Q_UNUSED(prev)
    if( !cur ) {
        ui->tableView->setModel(new QStandardItemModel());
        return;
    }
    IObject* object = ui->scene->getObjectByName(cur->text());

    initTable(object);
}

void MainWindow::on_deleteBt_clicked(){
    if(ui->listWidget->selectedItems().count() > 0){
        QString name = ui->listWidget->selectedItems()[0]->text();
        ui->scene->deleteObjectByName(name);
        ui->listWidget->updateMap(ui->scene->getObjectsList());
    }
}

void MainWindow::initTable(IObject *object){
    D3DXVECTOR3 position = object->getPosition();
    D3DXVECTOR3 rotation = object->getRotation();
    D3DXVECTOR3 size = object->getSize();
    float dentisty = object->getDentisty();
    float returation = object->getReturation();
    QAbstractItemModel *oldmodel = ui->tableView->model();


    if(object->getType() == IObject::EP_BOX){
        QStandardItemModel *model= new QStandardItemModel(11, 1);

        ui->tableView->setModel(model);
        QStringList listVert;

        ui->tableView->setItemDelegateForRow(0, new MyItemPosMeter());
        listVert.push_back("X");
        ui->tableView->setItemDelegateForRow(1, new MyItemPosMeter());
        listVert.push_back("Y");
        ui->tableView->setItemDelegateForRow(2, new MyItemPosMeter());
        listVert.push_back("Z");

        ui->tableView->setItemDelegateForRow(3, new MyItemGrad());
        listVert.push_back("Speed X");
        ui->tableView->setItemDelegateForRow(4, new MyItemGrad());
        listVert.push_back("Speed Y");
        ui->tableView->setItemDelegateForRow(5, new MyItemGrad());
        listVert.push_back("Speed Z");

        ui->tableView->setItemDelegateForRow(6, new MyItemSizeMeter());
        listVert.push_back("Length");
        ui->tableView->setItemDelegateForRow(7, new MyItemSizeMeter());
        listVert.push_back("Width");
        ui->tableView->setItemDelegateForRow(8, new MyItemSizeMeter());
        listVert.push_back("Height");

        ui->tableView->setItemDelegateForRow(9, new MyItemKg());
        listVert.push_back("Dentisty");
        ui->tableView->setItemDelegateForRow(10, new MyItemRet());
        listVert.push_back("Returation");
        model->setVerticalHeaderLabels(QStringList(listVert));

        QStringList listHor;
        listHor.push_back("Value");
        model->setHorizontalHeaderLabels(QStringList(listHor));
        model->setData(model->index(0,0,QModelIndex()), position.x );
        model->setData(model->index(1,0,QModelIndex()), position.y );
        model->setData(model->index(2,0,QModelIndex()), position.z );

        model->setData(model->index(3,0,QModelIndex()), rotation.x );
        model->setData(model->index(4,0,QModelIndex()), rotation.y );
        model->setData(model->index(5,0,QModelIndex()), rotation.z );

        model->setData(model->index(6,0,QModelIndex()), size.x );
        model->setData(model->index(7,0,QModelIndex()), size.y );
        model->setData(model->index(8,0,QModelIndex()), size.z );

        model->setData(model->index(9,0,QModelIndex()), dentisty );
        model->setData(model->index(10,0,QModelIndex()), returation );

    }
    else if(object->getType() == IObject::EP_SPHERE){
        QStandardItemModel *model= new QStandardItemModel(8, 1);

        ui->tableView->setModel(model);
        QStringList listVert;

        ui->tableView->setItemDelegateForRow(0, new MyItemPosMeter());
        listVert.push_back("X");
        ui->tableView->setItemDelegateForRow(1, new MyItemPosMeter());
        listVert.push_back("Y");
        ui->tableView->setItemDelegateForRow(2, new MyItemPosMeter());
        listVert.push_back("Z");

        ui->tableView->setItemDelegateForRow(3, new MyItemGrad());
        listVert.push_back("Speed X");
        ui->tableView->setItemDelegateForRow(4, new MyItemGrad());
        listVert.push_back("Speed Y");
        ui->tableView->setItemDelegateForRow(5, new MyItemGrad());
        listVert.push_back("Speed Z");

        ui->tableView->setItemDelegateForRow(6, new MyItemSizeMeter());
        listVert.push_back("Radius");

        ui->tableView->setItemDelegateForRow(7, new MyItemKg());
        listVert.push_back("Dentisty");
        ui->tableView->setItemDelegateForRow(8, new MyItemRet());
        listVert.push_back("Returation");
        model->setVerticalHeaderLabels(QStringList(listVert));

        QStringList listHor;
        listHor.push_back("Value");
        model->setHorizontalHeaderLabels(QStringList(listHor));

        model->setData(model->index(0,0,QModelIndex()), position.x );
        model->setData(model->index(1,0,QModelIndex()), position.y );
        model->setData(model->index(2,0,QModelIndex()), position.z );

        model->setData(model->index(3,0,QModelIndex()), rotation.x );
        model->setData(model->index(4,0,QModelIndex()), rotation.y );
        model->setData(model->index(5,0,QModelIndex()), rotation.z );

        model->setData(model->index(6,0,QModelIndex()), size.x );
        model->setData(model->index(7,0,QModelIndex()), dentisty );
        model->setData(model->index(8,0,QModelIndex()), returation );

   }

    if(oldmodel) delete oldmodel;
    connect(ui->tableView->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,SLOT(updateObject(QModelIndex,QModelIndex)));
}
