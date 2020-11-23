#ifndef TESTFORM_H
#define TESTFORM_H

#include <QWidget>

namespace Ui {
class TestForm;
}

class TestForm : public QWidget
{
    Q_OBJECT

public:
    explicit TestForm(QWidget *parent = 0);
    ~TestForm();

private slots:
    void on_pbSend_clicked();
    void on_showCmdInfo(int cmd,QString str);
    void on_pbClear_clicked();

private:
    Ui::TestForm *ui;
};

#endif // TESTFORM_H
