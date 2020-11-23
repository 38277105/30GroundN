#include "inputdelegate.h"
#include "qlineedit.h"


InputDelegate::InputDelegate(QObject *parent):QItemDelegate(parent)
{

}

QWidget *InputDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &option,const  QModelIndex &index) const
{
         QLineEdit *editor =  new  QLineEdit(parent);
         editor->installEventFilter(const_cast<InputDelegate*>(this));
         return  editor;

}

void  InputDelegate::setEditorData(QWidget *editor,  const  QModelIndex &index)  const
{
   QString text = index.model()->data(index, Qt::EditRole).toString();
   QLineEdit *lineEdit =  static_cast <QLineEdit*>(editor);
   lineEdit->setText(text);
}

void  InputDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
   QLineEdit *lineEdit =  static_cast <QLineEdit*>(editor);
   QString text = lineEdit->text();
   model->setData(index, text, Qt::EditRole);
}

void  InputDelegate::updateEditorGeometry(QWidget *editor,
    const  QStyleOptionViewItem &option,  const  QModelIndex &index) const
{
   editor->setGeometry(option.rect);
}

