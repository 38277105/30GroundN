#include "buttondelegate.h"
#include <QStyle>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDialog>
#include <QDesktopWidget>
#include <QMessageBox>

ButtonDelegate::ButtonDelegate(QObject *parent) : QItemDelegate(parent)
{

}

void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const{
    QStyleOptionButton* buttons = m_btns.value(index);
    if (!buttons) {
        QStyleOptionButton* t_button = new QStyleOptionButton();
        t_button->text =QString::fromLocal8Bit("设置");
        t_button->state |= QStyle::State_Enabled;

        (const_cast<ButtonDelegate *>(this))->m_btns.insert(index, t_button);
    }
    painter->save();
    buttons = m_btns.value(index);
    buttons->rect = option.rect.adjusted(4, 4, -4 , -4); //

    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, option.palette.highlight());
    }
    painter->restore();
    QApplication::style()->drawControl(QStyle::CE_PushButton, buttons, painter);
}

bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {

        QMouseEvent* e =(QMouseEvent*)event;

        if (m_btns.contains(index)) {
            QStyleOptionButton* btns = m_btns.value(index);
            if (btns->rect.contains(e->x(), e->y())) {
                btns->state |= QStyle::State_Sunken;
            }

        }
    }
    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e =(QMouseEvent*)event;

        if (m_btns.contains(index)) {
            QStyleOptionButton* btns = m_btns.value(index);
            if (btns->rect.contains(e->x(), e->y())) {
                btns->state &= (~QStyle::State_Sunken);
               emit processMsg(index);
            }
        }
    }
    return true;
}
