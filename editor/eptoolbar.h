#ifndef EPTOOLBAR_H
#define EPTOOLBAR_H
#include <QToolBar>
#include <QToolButton>

class EpToolBar : public QToolBar{
Q_OBJECT
public:
    EpToolBar(QWidget *parent = 0);
    ~EpToolBar();
private:
    QToolButton camrotate, camdist, camreset; // Camera control
    QToolButton crbox, crsphere, crbuster; // Object creation

    void initToolBar();

public slots:

    void on_camreset_clicked();
    void on_camrotate_toggled();
    void on_camdist_toggled();

    void on_crbox_clicked();
    void on_crsphere_clicked();
    void on_crbuster_clicked();

signals:

    void resetPressed();
    void stateChanged(int state);

    void createBoxClicked();
    void createSphereClicked();
    void createDummyClicked();

};

#endif // EPTOOLBAR_H
