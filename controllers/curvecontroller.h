#ifndef CURVECONTROLLER_H
#define CURVECONTROLLER_H

#include <QObject>
#include <QMap>
#include <QDateTime>
#include <QVector>
#include <QPushButton>
#include <QComboBox>
#include <QFile>
#include "vehicles/vehiclestate.h"
#include "showlogtable.h"
class QwtPlot;
class QwtPlotCurve;
class QwtPointArrayData;

typedef enum ArgTypeData
{
    longitude,
    latitude,
    height,
    roll,
    yaw,
    pitch,
    batt_per,
    batt_volt,
    throttle
}ArgType;

class CurveController : public QObject
{
    Q_OBJECT
public:
    explicit CurveController(QObject *parent = 0);
    ~CurveController();

    void initialControl();
    QWidget* getPlot();
    void ShowDisplay(QString tittle,QString XTittle,QString YTittle);

public:
    QPushButton*  m_btn_downloadLog;
    QComboBox*    m_cbx_logList;
signals:
    void requestLogData(uint16_t id, uint32_t ofs, uint32_t count);
public slots:
   void ChangeArgs(ArgType argName);
   void addlat(float_t lat);
   void addlng(float_t lng);
   void addalt(float_t alt);
   void addroll(float_t roll);
   void addyaw(float_t yaw);
   void addpitch(float_t pitch);
   void addthrottle(float_t throttle);
   void addbatterypercent(float_t battery_percent);
   void addbatteryvolt(float_t battery_Volt);
   void addstate(VehicleState& state);

   void btn_all_click();
   void rcv_logEntry(mavlink_log_entry_t);
   void rcv_logData(mavlink_log_data_t);
   void cbx_loglist(int index);

private:
   void updateCbxLogList();
   void startDownloadLog(mavlink_log_entry_t);
   void parseLogFile(const QString& filename);
   bool fillLogdata(LogData& dst, const QByteArray& src);
   uint16_t two8toOne16(const uint8_t& highB, const uint8_t& lowB);
   uint32_t four8toOne32(const uint8_t& highB, const uint8_t& highB1, const uint8_t& lowB, const uint8_t& lowB1);
   void cpyArray(uint8_t* dst, uint8_t* src,int size);
private:

   QVector<double> collect_lat_time;
   QVector<double> collect_lng_time;
   QVector<double> collect_alt_time;
   QVector<double> collect_roll_time;
   QVector<double> collect_yaw_time;
   QVector<double> collect_pitch_time;
   QVector<double> collect_throttle_time;
   QVector<double> collect_battery_percent_time;
   QVector<double> collect_battery_Volt_time;

   QVector<double> collect_lat;
   QVector<double> collect_lng;
   QVector<double> collect_alt;
   QVector<double> collect_roll;
   QVector<double> collect_yaw;
   QVector<double> collect_pitch;
   QVector<double> collect_throttle;
   QVector<double> collect_battery_percent;
   QVector<double> collect_battery_Volt;

   //Control
   QwtPlot* m_plot;
   QwtPlotCurve* m_curve;
   QwtPointArrayData * m_data;
   QWidget* m_container;
   ArgType m_currentType;
   double initTime;
   int maxCacheCount;
   QList<mavlink_log_entry_t> logList;
   uint32_t m_currentLogSize;
   uint32_t m_logDataCount;
   QFile*    m_currentLogFileName;
   QList<LogData> m_currentLogDataList;
   showlogtable m_showlogtable;
};



#endif // CURVECONTROLLER_H
