#ifndef WPCMDCBO_H
#define WPCMDCBO_H
#include <QComboBox>
class wpcmdCbo : public QComboBox
{
    Q_OBJECT
public:
    wpcmdCbo(QWidget *parent = 0);
    void initItems(const int& cItem);
    int getCurrentIndex();
    int SetNowIndex(int index);
private:
    void initCommonMap();
signals:
    void signal_cboChanged(int row, int value);
private slots:
    void CboValueChanged(QString text);
private:
    int                       m_row;
    QMap<int, QString>        m_cmdMap;
    QStringList               m_cmdList;
    int                       m_state;
};

#endif // WPCMDCBO_H
