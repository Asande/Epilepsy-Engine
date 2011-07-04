#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include "Graphics.h"
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    void changeEvent(QEvent *e);
    void initTable(IObject *object);

private:

    Ui::MainWindow *ui;

public slots:
    void updateText(int val);
    void startSym();
    void startedSym();
    void updatedSym(int);
    void finishedSym();

    void updateObject(QModelIndex,QModelIndex);

    void itemChanged(QListWidgetItem*,QListWidgetItem*);

    void on_deleteBt_clicked();

    void on_actNew_triggered();
    void on_actExit_triggered();
    void on_actAbout_triggered();

};

#endif // MAINWINDOW_H
