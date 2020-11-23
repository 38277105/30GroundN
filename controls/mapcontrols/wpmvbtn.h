#ifndef WPMVBTN_H
#define WPMVBTN_H
#include <QPushButton>

typedef enum {
    MV_UP,
    MV_DOWN
}WPMVBTN_TYPE;

class wpmvbtn : public QPushButton
{
    Q_OBJECT
public:
    wpmvbtn(WPMVBTN_TYPE m_type, QWidget *parent = 0);
    int setRow(const int& row);
    int addRow();
    int subtractRow();
signals:
    void signal_up(int row);
    void signal_down(int row);
private slots:
    void btn_clicked();
private:
    int             m_row;
    WPMVBTN_TYPE    m_type;
};

#endif // WPMVBTN_H
