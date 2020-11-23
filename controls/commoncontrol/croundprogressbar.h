#ifndef CROUNDPROGRESSBAR_H
#define CROUNDPROGRESSBAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QThreadPool>
#include <QThread>
#include <QDialog>


class CRoundProgressBar : public QDialog
{
    Q_OBJECT

public:
    explicit CRoundProgressBar(QWidget *parent = 0);
    ~CRoundProgressBar();

    QString m_strValue;
    static bool m_IsClosed;

public slots:

    void setScanValue(QString strValue);

    private:

    qreal m_value;

    qreal m_outerRadius;

    qreal m_innerRadius;

    qreal m_colorPieRadius;

    qreal m_coverCircleRadius;

    //qreal m_currentValue;

    //qreal m_longHand;

    //qreal m_okHand;

    //qreal m_shortHand;

    qreal m_space;

    //bool m_bReverse;

    //QTimer* updateTimer;

    QPointF m_center;

    QRectF m_pieRect;


    private:

    void drawOuterCircle(QPainter* painter);

    void drawInnerCircle(QPainter* painter);

    void drawColorPies(QPainter* painter, QString strValue);

    void drawCoverLines(QPainter* painter);

    void drawCoverCircle(QPainter* painter);

    void drawMarkAndText(QPainter* painter);

    void drawGraph(QPainter* painter);

    void drawTextRect(QPainter* painter);

    void resetVariables(QPainter* painter);

    /**********窗口可移动***********/
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    /**********窗口可移动***********/

    protected:

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void closeEvent();

};


class RunnableTask : public QRunnable
{


public:
    RunnableTask(CRoundProgressBar* progressBar)
    {
        m_ProgressBar = progressBar;
    }

    void run()
    {
        for (int i = 1; i <= 98; i++)
        {
            if(m_ProgressBar)
            {
                QMetaObject::invokeMethod(m_ProgressBar, "setScanValue", Qt::QueuedConnection, Q_ARG(QString, QString::asprintf("%d",i)));
                QThread::msleep(100);
            }
        }
    }

private:
    CRoundProgressBar* m_ProgressBar;
public :
//    void deleteui()
//    {
//        if(m_ProgressBar!=NULL)
//        {
//            delete m_ProgressBar;
//            m_ProgressBar=NULL;
//        }
//    }

};



#endif // CROUNDPROGRESSBAR_H
