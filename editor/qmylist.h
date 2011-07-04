#ifndef QMYLIST_H
#define QMYLIST_H

#include <QListWidget>
#include "iobject.h"

class QMyList : public QListWidget
{
Q_OBJECT
public:
    explicit QMyList(QWidget *parent = 0);
signals:

public slots:
    void updateMap(QList<QString>);

};

#endif // QMYLIST_H
