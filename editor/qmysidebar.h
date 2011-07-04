#ifndef QMYSIDEBAR_H
#define QMYSIDEBAR_H

#include <QFrame>
#include "iobject.h"

class QMySidebar : public QFrame
{
Q_OBJECT
public:
    explicit QMySidebar(QWidget *parent = 0);

signals:

public slots:
    void updateList(QMap<QString,IObject*>);

};

#endif // QMYSIDEBAR_H
