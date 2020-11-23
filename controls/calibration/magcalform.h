#ifndef MAGCALFORM_H
#define MAGCALFORM_H

#include <QWidget>
#include "libeigen/magdef.h"

namespace Ui {
class MagCalForm;
}

class QCPGraph;
class MagCalForm : public QWidget
{
    Q_OBJECT

public:
    explicit MagCalForm(QWidget *parent = nullptr);
    ~MagCalForm();

private slots:
    void on_pbCut_clicked();
    void on_cbxMag_currentTextChanged(const QString &arg1);
    void on_pbSaveMag_clicked();
    void RecvMagData(int magId,const float* x,const float* y,const float* z,int sz);
private:
    void MagTestData();
    void ShowSrcMagData();
private:
    Ui::MagCalForm *ui;
    SendTest    cc;
    QString     m_sBasePath;
    int         m_nCurMagId;
    MagDataItem m_magDataSrc[MAX_MAG];
    MagDataItem m_magDataCut[MAX_MAG];
};
#endif // MAGCALFORM_H
