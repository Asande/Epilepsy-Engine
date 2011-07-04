#include "qdirectxwidget.h"

QDirectXWidget::QDirectXWidget(QWidget *parent) :
    QWidget(parent)
{
    xTimer = new QTimer();
    QWidget::connect(xTimer, SIGNAL(timeout()), this, SLOT(render()));
    xTimer->start(50);

    mouselastx = 0;
    mouselasty = 0;
    isPressed = false;
    state = 1;
}

void QDirectXWidget::setCalc(bool val){
    calculated = val;
}

QDirectXWidget::~QDirectXWidget(){
    delete xTimer;
}

void QDirectXWidget::render(){
    static float lastTime = (float)GetTickCount();
    float currentTime = (float)GetTickCount();
    float deltaTime = (currentTime - lastTime) * 0.001f;

    UpdateDX(deltaTime);
    RenderDX();

    lastTime = currentTime;
}

void QDirectXWidget::resizeEvent(QResizeEvent *pEvent){
    Q_UNUSED(pEvent);

    if(!device) createDevice();

    int width = this->rect().right();
    int height = this->rect().bottom();
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    CleanData();
    if(device)
        device->Reset(&d3dpp);

    InitDX();
    ResizeDX( width, height );
}

void QDirectXWidget::createDevice(){
    int width = this->rect().right();
    int height = this->rect().bottom();

    if( !CreateDX((HWND)this->winId(), width, height) )
        return;
    if( !InitDX() )
        return;
}

void QDirectXWidget::mousePressEvent(QMouseEvent *event){
    isPressed = true;
    mouselastx = event->x();
    mouselasty = event->y();
}
void QDirectXWidget::mouseReleaseEvent(QMouseEvent *event){

    Q_UNUSED(event)
    this->setCursor(Qt::ArrowCursor);
    isPressed = false;

}

IObject* QDirectXWidget::getObjectByName(QString name){
    return scene[name];
}

void QDirectXWidget::mouseMoveEvent(QMouseEvent *event){
    int deltax, deltay;
    if(isPressed){
        deltax = event->x() - mouselastx;
        deltay = event->y() - mouselasty;

        if(state == CAMROTATE){
            this->setCursor(Qt::ClosedHandCursor);
            mainCam->rotateCamera(deltax,deltay,100);
        }
        if(state == CAMDIST){
            this->setCursor(Qt::SizeVerCursor);
            if(deltay > 0)
                mainCam->changeDistance( 0.4f * deltay );
            else
                mainCam->changeDistance( -0.4f * (-deltay) );
        }
        mouselastx += deltax;
        mouselasty += deltay;
    }
}

void QDirectXWidget::cameraReset(){
    mainCam->reset();
}

void QDirectXWidget::changeState(int state){
    this->state = state;
}

QList<QString> QDirectXWidget::getObjectsList(){
    return scene.keys();
}

void QDirectXWidget::createBox(){
    int i = 0;
    while(1){
        QString name = "Box" + QString::number(i);
        if(scene.contains(name))
        {
            i++;
        }
        else
        {
            scene.insert(name, new EpBox(device));
            break;
        }
    }
    modelChanged(scene.keys());
    calculated = false;
}



void QDirectXWidget::createSphere(){
    int i = 0;
    while(1){
        QString name = "Sphere" + QString::number(i);
        if(scene.contains( name ) )
        {
            i++;
        }
        else
        {
            scene.insert(name, new EpSphere(device));
            break;
        }
    }
    modelChanged(scene.keys());
    calculated = false;
}

void QDirectXWidget::createDummy(){
    int i = 0;
    while(1){
        QString name = "Dummy" + QString::number(i);
        if(scene.contains( name ) )
        {
            i++;
        }
        else
        {
            //scene.insert(name, new IObject());
            break;
        }
    }
    modelChanged(scene.keys());
    calculated = false;
}

void QDirectXWidget::deleteObjectByName(QString name){
    scene.remove(name);
}

void QDirectXWidget::startSim(int sec, float planeCoef, float mu){

    controller = new EpController(scene.count(), 60.f, (float)sec, true, true, planeCoef, mu);

    QMap<QString,IObject*>::iterator it;
    for(it = scene.begin(); it != scene.end();++it ){
        int type = it.value()->getType();
        switch (type){
        case 0:
            controller->addQuadratePrimitive(
                     it.value()->getPosition(),
                     it.value()->getSize(),
                     it.value()->getRotation(),
                     it.value()->getReturation(),
                     it.value()->getDentisty());
            break;
        case 1:
            controller->addSpherePrimitive(
                     it.value()->getPosition(),
                     it.value()->getSize().x,
                     it.value()->getRotation(),
                     it.value()->getReturation(),
                     it.value()->getDentisty());
            break;
        default:
            return;
        }
    }

    calcStart();

    for(int i = 0 ; i < sec * 60 ; i++)
    {
        int progress = controller->colculateStep();
        calcUpdated( (progress * 5) /(3 * sec) );
    }

    int i = 0;
    for(it = scene.begin(); it != scene.end();++it ){
        it.value()->queue = controller->getObject(i++);
    }

    calcStop();

    delete controller;

}

void QDirectXWidget::setSec(int val){
   scenesecond = val;
}

void QDirectXWidget::clear(){
    calculated = false;
    scene.clear();
    modelChanged(scene.keys());
}
