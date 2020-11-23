#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QObject>
#include <QItemDelegate>
#include <QPushButton>

class ButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ButtonDelegate(QObject *parent = 0);


    void  paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

    bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    signals:
    void processMsg(const QModelIndex& index);
    public slots:


    private:

    typedef QMap<QModelIndex, QStyleOptionButton* >  collButtons;
    collButtons m_btns;
};

#endif // BUTTONDELEGATE_H
