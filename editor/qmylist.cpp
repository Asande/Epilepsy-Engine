#include "qmylist.h"

QMyList::QMyList(QWidget *parent) :
    QListWidget(parent)
{
}

void QMyList::updateMap(QList<QString> list){
    this->clear();
    this->addItems(list);
}
