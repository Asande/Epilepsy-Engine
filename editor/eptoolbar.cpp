#include "eptoolbar.h"

EpToolBar::EpToolBar(QWidget *parent) : QToolBar(parent)
{
    initToolBar();
    connect(&camreset, SIGNAL(clicked()), this, SLOT(on_camreset_clicked()));
    connect(&camrotate, SIGNAL(toggled(bool)), this, SLOT(on_camrotate_toggled()));
    connect(&camdist, SIGNAL(toggled(bool)), this, SLOT(on_camdist_toggled()));

    connect(&crbox, SIGNAL(clicked()), this, SLOT(on_crbox_clicked()));
    connect(&crsphere, SIGNAL(clicked()), this, SLOT(on_crsphere_clicked()));
    connect(&crbuster, SIGNAL(clicked()), this, SLOT(on_crbuster_clicked()));
}

EpToolBar::~EpToolBar(){

}

void EpToolBar::initToolBar(){
    camrotate.setCheckable(true);
    camrotate.setChecked(true);
    camrotate.setAutoExclusive(true);
    camrotate.setText("CamRotate");

    camdist.setCheckable(true);
    camdist.setAutoExclusive(true);
    camdist.setText("CamDistance");

    camreset.setText("CamReset");

    crbox.setText("Box");
    crsphere.setText("Sphere");
    crbuster.setText("Sceleton");

    this->addWidget(&camrotate);
    this->addWidget(&camdist);
    this->addWidget(&camreset);
    this->addSeparator();
    this->addWidget(&crbox);
    this->addWidget(&crsphere);
    //this->addWidget(&crbuster);
}

void EpToolBar::on_camreset_clicked(){ resetPressed(); }
void EpToolBar::on_camrotate_toggled(){ stateChanged(1); }
void EpToolBar::on_camdist_toggled(){ stateChanged(2); }

void EpToolBar::on_crbox_clicked(){ createBoxClicked(); }
void EpToolBar::on_crsphere_clicked(){ createSphereClicked(); }
void EpToolBar::on_crbuster_clicked(){ createDummyClicked(); }
