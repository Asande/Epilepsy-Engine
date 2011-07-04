#ifndef QDIRECTXWIDGET_H
#define QDIRECTXWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QMessageBox>

#include "render/Graphics.h"
#include "render/iobject.h"
#include "render/epbox.h"
#include "render/epsphere.h"

#define CAMROTATE 1
#define CAMDIST 2

class QDirectXWidget : public QWidget, public Graphics
{
Q_OBJECT
public:

    explicit QDirectXWidget(QWidget *parent = 0);
    ~QDirectXWidget();

    IObject* getObjectByName(QString name);
    void deleteObjectByName(QString name);

    QList<QString> getObjectsList();

    void setCalc(bool);
    void clear();

protected:

    QTimer *xTimer;
    int mouselastx, mouselasty;
    bool isPressed;
    DWORD state;



    void createDevice();

    void resizeEvent(QResizeEvent *pEvent);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void modelChanged(QList<QString>);
    void calcStart();
    void calcUpdated(int);
    void calcStop();

public slots:
    void render();
    void cameraReset();
    void changeState(int);

    void setSec(int);

    void createBox();
    void createSphere();
    void createDummy();


    void startSim(int,float,float);

};

#endif // QDIRECTXWIDGET_H
