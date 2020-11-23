#ifndef FIRMWAREFORM_H
#define FIRMWAREFORM_H

#include <QWidget>
#include <QTimer>
namespace Ui {
class FirmWareForm;
}

class FirmwareUpgradeController;
class FirmWareForm : public QWidget
{
    Q_OBJECT

public:
    explicit FirmWareForm(QWidget *parent = nullptr);
    ~FirmWareForm();
private slots:
    void AppendTxt(QString str);
    void SetProgress(float val);
    void FindPort();
    void on_pbSelFile_clicked();

    void on_pbFlashing_clicked();

private:
    Ui::FirmWareForm *ui;
    FirmwareUpgradeController* m_pFirm;
    QTimer                     m_tmPort;
    QString                    m_fmFile;
};

#endif // FIRMWAREFORM_H
