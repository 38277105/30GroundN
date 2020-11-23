#ifndef WPDELETEBTN_H
#define WPDELETEBTN_H
#include <QPushButton>

class WPDeleteBtn : public QPushButton
{
    Q_OBJECT

public:
    explicit WPDeleteBtn(QWidget *parent = 0);
    int setRow(const int& row);
    int addRow();
    int subtractRow();
signals:
    void signal_deleteClicked(int);
private slots:
    void btn_clicked();
private:
    int         m_row;
};

#endif // WPDELETEBTN_H
