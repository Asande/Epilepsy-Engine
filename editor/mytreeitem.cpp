#include "mytreeitem.h"

MyItemPosMeter::MyItemPosMeter(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *MyItemPosMeter::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
     editor->setMinimum(-1000);
     editor->setMaximum(1000);
     editor->setSingleStep(0.2);
     editor->setSuffix("m");

     return editor;
 }

void MyItemPosMeter::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     double value = index.model()->data(index, Qt::EditRole).toDouble();

     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->setValue(value);

 }

void MyItemPosMeter::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->interpretText();
     double value = spinBox->value();

     model->setData(index, value, Qt::EditRole);
 }

void MyItemPosMeter::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//-------------------------------------------------------------------
MyItemSizeMeter::MyItemSizeMeter(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *MyItemSizeMeter::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
     editor->setMinimum(0);
     editor->setMaximum(50);
     editor->setSingleStep(0.2);
     editor->setSuffix("m");

     return editor;
 }

void MyItemSizeMeter::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     double value = index.model()->data(index, Qt::EditRole).toDouble();

     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->setValue(value);

 }

void MyItemSizeMeter::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->interpretText();
     double value = spinBox->value();

     model->setData(index, value, Qt::EditRole);
 }

void MyItemSizeMeter::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

//---------------------------------------------------------------------

MyItemGrad::MyItemGrad(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *MyItemGrad::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
     editor->setMinimum(-100);
     editor->setMaximum(100);
     editor->setSingleStep(0.2);
     editor->setSuffix("sm/s");

     return editor;
 }

void MyItemGrad::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     double value = index.model()->data(index, Qt::EditRole).toDouble();

     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->setValue(value);

 }

void MyItemGrad::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->interpretText();
     double value = spinBox->value();

     model->setData(index, value, Qt::EditRole);
 }

void MyItemGrad::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

//--------------------------------------------------------------------

MyItemKg::MyItemKg(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *MyItemKg::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
     editor->setMinimum(0.2);
     editor->setMaximum(13);
     editor->setSingleStep(0.2);
     editor->setSuffix("kg");

     return editor;
 }

void MyItemKg::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     double value = index.model()->data(index, Qt::EditRole).toDouble();

     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->setValue(value);

 }

void MyItemKg::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->interpretText();
     double value = spinBox->value();

     model->setData(index, value, Qt::EditRole);
 }

void MyItemKg::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//--------------------------------------------------------------------

MyItemRet::MyItemRet(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *MyItemRet::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
     editor->setMinimum(0);
     editor->setMaximum(1);
     editor->setSingleStep(0.1);
     editor->setSuffix("kg/m3");

     return editor;
 }

void MyItemRet::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     double value = index.model()->data(index, Qt::EditRole).toDouble();

     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->setValue(value);

 }

void MyItemRet::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
     spinBox->interpretText();
     double value = spinBox->value();

     model->setData(index, value, Qt::EditRole);
 }

void MyItemRet::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
