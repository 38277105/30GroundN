#include "curvecontroller.h"
#include <qwt_plot.h>
#include <qwt_time_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_legend.h>
#include <qwt_point_data.h>
#include <QHBoxLayout>
#include <QDir>
#include <QFile>
#include <QMessageBox>
//#include <QPushButton>


CurveController::CurveController(QObject *parent) : QObject(parent)
{
    m_plot=NULL;
    m_curve=NULL;
    m_data=NULL;
    m_container=NULL;

    initTime=QDateTime::currentDateTime().toMSecsSinceEpoch();
    maxCacheCount=200;
    initialControl();
    ChangeArgs(height);
}
CurveController::~CurveController()
{
    if(NULL!=m_plot)
    {
        delete m_plot;
        m_plot=NULL;
    }

}

void CurveController::initialControl()
{
    m_container=new QWidget();
    QHBoxLayout* lyout=new QHBoxLayout();
    m_container->setLayout(lyout);

   //左边
    m_plot=new qwt_time_plot(QwtText(QString::fromLocal8Bit("高度(height)-数据分析")));
    lyout->addWidget(m_plot);
    m_plot->resize(640,400);
    //设置坐标轴的名称
    m_plot->setAxisTitle(QwtPlot::xBottom, QString::fromLocal8Bit("x轴"));
    m_plot->setAxisTitle(QwtPlot::yLeft, QString::fromLocal8Bit("y轴"));
    //设置坐标轴的范围
    //plot.setAxisScale(QwtPlot::xBottom, 0.0, 2.0 * M_PI);
    //plot.setAxisScale(QwtPlot::yLeft, -1.0, 1.0);
    //设置右边标注
    m_plot->insertLegend(new QwtLegend(), QwtPlot::LeftLegend);

    //使用滚轮放大/缩小
    (void) new QwtPlotMagnifier( m_plot->canvas() );

    //使用鼠标左键平移
    (void) new QwtPlotPanner( m_plot->canvas() );

    //计算曲线数据
    QVector<double> xs;
    QVector<double> ys;
    for (double x = 0; x < 2.0 * M_PI; x+=(M_PI / 10.0))
    {
        xs.append(x);
        ys.append(qSin(x));
    }
    //构造曲线数据
    m_data = new QwtPointArrayData(xs, ys);
    m_curve=new QwtPlotCurve();
    //m_curve->setTitle(QwtText(QString::fromLocal8Bit("高度(height)-数据分析")));
    m_curve->setData(m_data);//设置数据
    m_curve->setStyle(QwtPlotCurve::Lines);//直线形式
    //m_curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//是曲线更光滑
    m_curve->setPen(QPen(Qt::black,2));//设置画笔
    m_curve->attach(m_plot);//把曲线附加到plot上

    //右边
    QWidget *rightPanel = new QWidget();
    rightPanel->setMinimumWidth(120);

    QPushButton*  m_btn_height = new QPushButton(rightPanel);
    m_btn_height->setText(QString::fromLocal8Bit("高度"));
    m_btn_height->setGeometry(10, 25, 110, 33);
    connect(m_btn_height,SIGNAL(clicked()),this,SLOT(btn_all_click()));

    QPushButton* m_btn_lat = new QPushButton(rightPanel);
    m_btn_lat->setText(QString::fromLocal8Bit("纬度"));
    m_btn_lat->setGeometry(10, 85, 110, 33);
    connect(m_btn_lat,SIGNAL(clicked()),this,SLOT(btn_all_click()));


    QPushButton* m_btn_lng = new QPushButton(rightPanel);
    m_btn_lng->setText(QString::fromLocal8Bit("经度"));
    m_btn_lng->setGeometry(10, 145, 110, 33);
    connect(m_btn_lng,SIGNAL(clicked()),this,SLOT(btn_all_click()));


    QPushButton* m_btn_roll = new QPushButton(rightPanel);
    m_btn_roll->setText(QString::fromLocal8Bit("滚转"));
    m_btn_roll->setGeometry(10, 205, 110, 33);
    connect(m_btn_roll,SIGNAL(clicked()),this,SLOT(btn_all_click()));

    QPushButton* m_btn_yaw = new QPushButton(rightPanel);
    m_btn_yaw->setText(QString::fromLocal8Bit("偏航"));
    m_btn_yaw->setGeometry(10, 265, 110, 33);
    connect(m_btn_yaw,SIGNAL(clicked()),this,SLOT(btn_all_click()));

    QPushButton* m_btn_pitch = new QPushButton(rightPanel);
    m_btn_pitch->setText(QString::fromLocal8Bit("俯仰"));
    m_btn_pitch->setGeometry(10, 325, 110, 33);
    connect(m_btn_pitch,SIGNAL(clicked()),this,SLOT(btn_all_click()));


    QPushButton* m_btn_battery_per = new QPushButton(rightPanel);
    m_btn_battery_per->setText(QString::fromLocal8Bit("电量"));
    m_btn_battery_per->setGeometry(10, 385, 110, 33);
    connect(m_btn_battery_per,SIGNAL(clicked()),this,SLOT(btn_all_click()));

    QPushButton* m_btn_battery_volt = new QPushButton(rightPanel);
    m_btn_battery_volt->setText(QString::fromLocal8Bit("电压"));
    m_btn_battery_volt->setGeometry(10, 445, 110, 33);
    connect(m_btn_battery_volt,SIGNAL(clicked()),this,SLOT(btn_all_click()));

    QPushButton* m_btn_throttle = new QPushButton(rightPanel);
    m_btn_throttle->setText(QString::fromLocal8Bit("油门"));
    m_btn_throttle->setGeometry(10, 505, 110, 33);
    connect(m_btn_throttle,SIGNAL(clicked()),this,SLOT(btn_all_click()));

    m_btn_downloadLog = new QPushButton(rightPanel);
    m_btn_downloadLog->setText(QString::fromLocal8Bit("日志下载"));
    m_btn_downloadLog->setGeometry(10, 565, 110, 33);

    m_cbx_logList = new QComboBox(rightPanel);
    m_cbx_logList->setGeometry(10, 625, 110, 33);
    //m_cbx_logList->addItem(QString::fromLocal8Bit("无日志"));
    connect(m_cbx_logList, SIGNAL(activated(int)), this, SLOT(cbx_loglist(int)));
    lyout->addWidget(rightPanel);
}

QWidget* CurveController::getPlot()
{
  return m_container;
}

void CurveController::ChangeArgs(ArgType argType)
{
    m_currentType=argType;
  switch(argType)
  {
    case height:
  {
        QString title_height=  QString::fromLocal8Bit("高度(height)-数据分析");
        QString x_height= QString::fromLocal8Bit("时间");
        QString y_height= QString::fromLocal8Bit("高度");
        ShowDisplay(title_height,x_height,y_height);
        }
      break;
    case latitude:
      {
      QString title_latitude=  QString::fromLocal8Bit("纬度(latitude)-数据分析");
      QString x_latitude= QString::fromLocal8Bit("时间");
      QString y_latitude= QString::fromLocal8Bit("纬度");
      ShowDisplay(title_latitude,x_latitude,y_latitude);}
    break;
    case longitude:
      {
      QString title_longitude=  QString::fromLocal8Bit("经度(longitude)-数据分析");
      QString x_longitude= QString::fromLocal8Bit("时间");
      QString y_longitude= QString::fromLocal8Bit("经度");
      ShowDisplay(title_longitude,x_longitude,y_longitude);
      }
    break;
    case roll:
      {
      QString title_roll=  QString::fromLocal8Bit("滚转(roll)-数据分析");
      QString x_roll= QString::fromLocal8Bit("时间");
      QString y_roll= QString::fromLocal8Bit("滚转");
      ShowDisplay(title_roll,x_roll,y_roll);
      }
    break;
    case yaw:
      {QString title_yaw=  QString::fromLocal8Bit("偏航(yaw)-数据分析");
      QString x_yaw= QString::fromLocal8Bit("时间");
      QString y_yaw= QString::fromLocal8Bit("偏航");
      ShowDisplay(title_yaw,x_yaw,y_yaw);
      }
    break;
    case pitch:
      {QString title_pitch=  QString::fromLocal8Bit("俯仰(pitch)-数据分析");
      QString x_pitch= QString::fromLocal8Bit("时间");
      QString y_pitch= QString::fromLocal8Bit("俯仰");
      ShowDisplay(title_pitch,x_pitch,y_pitch);
      }
    break;
    case batt_per:
      {QString title_battery_percent=  QString::fromLocal8Bit("电量(battery_percent)-数据分析");
      QString x_battery_percent= QString::fromLocal8Bit("时间");
      QString y_battery_percent= QString::fromLocal8Bit("电量");
      ShowDisplay(title_battery_percent,x_battery_percent,y_battery_percent);
      }
    break;
    case batt_volt:
      {
      QString title_battery_Volt=  QString::fromLocal8Bit("电压(voltage)-数据分析");
      QString x_battery_Volt= QString::fromLocal8Bit("时间");
      QString y_battery_Volt= QString::fromLocal8Bit("电压");
      ShowDisplay(title_battery_Volt,x_battery_Volt,y_battery_Volt);
    }
      break;
    case throttle:
      {
      QString title_throttle=  QString::fromLocal8Bit("油门(latitude)-数据分析");
      QString x_throttle= QString::fromLocal8Bit("时间");
      QString y_throttle= QString::fromLocal8Bit("油门");
      ShowDisplay(title_throttle,x_throttle,y_throttle);
      }
    break;

    default:
      break;
  }
}

void CurveController::ShowDisplay(QString tittle,QString XTittle,QString YTittle)
{
    m_plot->setTitle(QwtText(tittle));//
    m_plot->setAxisTitle(QwtPlot::xBottom, XTittle);//QString::fromLocal8Bit("time")
    m_plot->setAxisTitle(QwtPlot::yLeft, YTittle);//QString::fromLocal8Bit("heigt")

}

/********************appendargs()*************************/

void CurveController::addlat(float_t lat)
{
  collect_lat_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
  collect_lat.push_back(lat);
  if(collect_lat_time.count()>maxCacheCount)
  {
      collect_lat_time.pop_front();
      collect_lat.pop_front();
  }
   if(m_currentType==latitude)
   {
       m_data=new QwtPointArrayData(collect_lat_time,collect_lat);//collect_alt_time,collect_alt
       m_curve->setData(m_data);//设置数据
        m_plot->replot();
   }
}
void CurveController::addlng(float_t lng)
{
    collect_lng_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
    collect_lng.push_back(lng);
    if(collect_lng_time.count()>maxCacheCount)
    {
        collect_lng_time.pop_front();
        collect_lng.pop_front();
    }

    if(m_currentType==longitude)
    {
        m_data=new QwtPointArrayData(collect_lng_time,collect_lng);//collect_alt_time,collect_alt
        m_curve->setData(m_data);//设置数据
        m_plot->replot();
    }
}
void CurveController::addalt(float_t alt)
{
    collect_alt_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
    collect_alt.push_back(alt);
    if(collect_alt_time.count()>maxCacheCount)
    {
        collect_alt_time.pop_front();
        collect_alt.pop_front();
    }
    if(m_currentType==height)
    {

       // m_data=NULL;
        m_data=new QwtPointArrayData(collect_alt_time,collect_alt);//collect_alt_time,collect_alt
        m_curve->setData(m_data);//设置数据
        m_plot->replot();

    }
}
void CurveController::addroll(float_t roll)
{
    collect_roll_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
    collect_roll.push_back(roll);
    if(collect_roll_time.count()>maxCacheCount)
    {
        collect_roll_time.pop_front();
        collect_roll.pop_front();
    }
    if(m_currentType==roll)
    {

        m_data=new QwtPointArrayData(collect_roll_time,collect_roll);//collect_alt_time,collect_alt
        m_curve->setData(m_data);//设置数据
         m_plot->replot();
    }
}
void CurveController::addyaw(float_t yaw)
{
    collect_yaw_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
    collect_yaw.push_back(yaw);
    if(collect_yaw_time.count()>maxCacheCount)
    {
        collect_yaw_time.pop_front();
        collect_yaw.pop_front();
    }
    if(m_currentType==roll)
    {

        m_data=new QwtPointArrayData(collect_yaw_time,collect_yaw);//collect_alt_time,collect_alt
        m_curve->setData(m_data);//设置数据
         m_plot->replot();
    }
}
void CurveController::addpitch(float_t pitch)
{
    collect_pitch_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
    collect_pitch.push_back(pitch);
    if(collect_pitch_time.count()>maxCacheCount)
    {
        collect_pitch_time.pop_front();
        collect_pitch.pop_front();
    }
    if(m_currentType==pitch)
    {

        m_data=new QwtPointArrayData(collect_pitch_time,collect_pitch);//collect_alt_time,collect_alt
        m_curve->setData(m_data);//设置数据
         m_plot->replot();
    }
}
void CurveController::addthrottle(float_t throttle)
{
    collect_throttle_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
    collect_throttle.push_back(throttle);
    if(collect_throttle_time.count()>maxCacheCount)
    {
        collect_throttle_time.pop_front();
        collect_throttle.pop_front();
    }
    if(m_currentType==throttle)
    {
        m_data=new QwtPointArrayData(collect_throttle_time,collect_throttle);//collect_alt_time,collect_alt
        m_curve->setData(m_data);//设置数据
         m_plot->replot();
    }
}
void CurveController::addbatterypercent(float_t battery_percent)
{
    collect_battery_percent_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
    collect_battery_percent.push_back(battery_percent);
    if(collect_battery_percent_time.count()>maxCacheCount)
    {
        collect_battery_percent_time.pop_front();
        collect_battery_percent.pop_front();
    }
    if(m_currentType==batt_per)
    {
        m_data=new QwtPointArrayData(collect_battery_percent_time,collect_battery_percent);//collect_alt_time,collect_alt
        m_curve->setData(m_data);//设置数据
         m_plot->replot();
    }
}
void CurveController::addbatteryvolt(float_t battery_Volt)
{
    collect_battery_Volt_time.push_back(QDateTime::currentDateTime().toMSecsSinceEpoch()-initTime);
    collect_battery_Volt.push_back(battery_Volt);
    if(collect_battery_Volt_time.count()>maxCacheCount)
    {
        collect_battery_Volt_time.pop_front();
        collect_battery_Volt.pop_front();
    }
    if(m_currentType==batt_volt)
    {
        m_data=new QwtPointArrayData(collect_battery_Volt_time,collect_battery_Volt);//collect_alt_time,collect_alt
        m_curve->setData(m_data);//设置数据
         m_plot->replot();
    }
}

/************************appendargs()*************************/

 void CurveController::addstate(VehicleState& state)
 {
   addlat(state.latitude);
   addlng(state.lontitude);
   addbatterypercent(state.batterypercent);
   addbatteryvolt(state.batteryVolt);
   addalt(state.height);
   addroll(state.roll);
   addpitch(state.pitch);
   addyaw(state.yaw);
   addthrottle(state.throttle);
 }

 void CurveController::btn_all_click()
 {
  QPushButton* btn_cur=(QPushButton*)sender();
  QString text=btn_cur->text();
  if(text==QString::fromLocal8Bit("高度"))
  {
      ChangeArgs(height);
  }
  else  if(text==QString::fromLocal8Bit("纬度"))
  {
      ChangeArgs(latitude);
  }
  else  if(text==QString::fromLocal8Bit("经度"))
  {
      ChangeArgs(longitude);
  }
  else  if(text==QString::fromLocal8Bit("滚转"))
  {
      ChangeArgs(roll);
  }
  else  if(text==QString::fromLocal8Bit("偏航"))
  {
      ChangeArgs(yaw);
  }
  else  if(text==QString::fromLocal8Bit("俯仰"))
  {
      ChangeArgs(pitch);
  }
  else  if(text==QString::fromLocal8Bit("油门"))
  {
      ChangeArgs(throttle);
  }
  else  if(text==QString::fromLocal8Bit("电量"))
  {
      ChangeArgs(batt_per);
  }
  else  if(text==QString::fromLocal8Bit("电压"))
  {
      ChangeArgs(batt_volt);
  }

 }

void CurveController::rcv_logEntry(mavlink_log_entry_t logEntry){
    bool isNew = true;
    mavlink_log_entry_t var;
    foreach (var, logList){
        if(var.id == logEntry.id && var.num_logs== logEntry.num_logs && var.last_log_num==logEntry.last_log_num &&\
                var.time_utc==logEntry.time_utc && var.size==logEntry.size){
            isNew = false;
        }
    }
    if(isNew){
        logList.push_back(logEntry);
        qDebug() << logEntry.id << "/" << logEntry.num_logs << "/" << logEntry.last_log_num << "/" << logEntry.time_utc << "/" << logEntry.size;
        updateCbxLogList();
    }
}

void CurveController::rcv_logData(mavlink_log_data_t logData){
    if(logData.count == 0){
        return;
    }
    QByteArray tmp((char*)logData.data, logData.count);
    qDebug() << logData.id << "/" << logData.ofs << "/" << logData.count << "/" << tmp;
    m_currentLogFileName->write((char*)logData.data, logData.count);
    if(logData.ofs + logData.count < m_currentLogSize){
        uint8_t count;
        uint32_t ofs = logData.ofs+logData.count;
        if(m_currentLogSize - ofs >= m_logDataCount){
            count = m_logDataCount;
        }else {
            count = m_currentLogSize - ofs;
        }
        emit requestLogData(logData.id, ofs, count);
    }
    if(logData.ofs + logData.count > m_currentLogSize|logData.ofs + logData.count == m_currentLogSize){
        m_currentLogFileName->close();
        parseLogFile(QString::number(logData.id));
    }
}


void CurveController::updateCbxLogList(){
    disconnect(m_cbx_logList, SIGNAL(activated(int)), this, SLOT(cbx_loglist(int)));
    for(int i = m_cbx_logList->count(); i < logList.length(); i++){
        m_cbx_logList->addItem(QString::number(logList[i].id));
    }
    connect(m_cbx_logList, SIGNAL(activated(int)), this, SLOT(cbx_loglist(int)));
}

void CurveController::startDownloadLog(mavlink_log_entry_t logEntryData){

    //创建目录
    QDir dir;
    if(!dir.exists(LOG_DIR)){
        dir.mkdir(LOG_DIR);
    }
    QString fileName = QString::number(logEntryData.id);
    m_currentLogFileName = new QFile(LOG_DIR+fileName);
    if(!m_currentLogFileName->open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QMessageBox::warning(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("打开日志文件失败"),QMessageBox::Yes);
        return;
    }
    //开始下载
    m_currentLogSize = logEntryData.size;
    m_logDataCount = 90;
    emit requestLogData(logEntryData.id, 0, m_logDataCount);
}

void CurveController::parseLogFile(const QString& filename){
    QFile file(LOG_DIR+filename);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(NULL,QString::fromUtf8("提示"),QString::fromUtf8("打开日志文件失败"),QMessageBox::Yes);
        return;
    }
    m_currentLogDataList.clear();
    uint8_t buf[1024] = {0};
    qint64 lastLen = 0;
    bool isReset = true;
    while(!file.atEnd()){
        if(isReset){
            memset(buf, 0, sizeof(buf));
            lastLen = 0;
        }
        qint64 nowLen = file.readLine((char*)buf+lastLen, sizeof(buf)-lastLen);
        if(buf[lastLen+nowLen-1] != '\n'){
            continue;
        }
        if(buf[lastLen+nowLen-2] != '\r'){
            isReset = false;
            lastLen = nowLen;
            buf[lastLen] = '\n';
            continue;
         }
        if (lastLen+nowLen != -1) {
            QByteArray data((char*)buf, lastLen+nowLen);
            qDebug() << data;
            LogData tmp;
            if(!fillLogdata(tmp, data)){
                continue;
            }
            m_currentLogDataList.push_back(tmp);
        }
        {
            isReset = true;
            lastLen = 0;
        }
    }
    if(!m_currentLogDataList.empty()){
        m_showlogtable.setTableData(m_currentLogDataList);
        m_showlogtable.show();
    }
}

bool CurveController::fillLogdata(LogData& dst, const QByteArray& src){
    QByteArray tmpSrc = src;
    char* ptSrc = tmpSrc.data();
    int sIndex =-1;
    for(int i = 0; i < tmpSrc.length()-1; i++){
        if(ptSrc[i] == char(0x95) && ptSrc[i+1] == char(0xA3)){
            sIndex = i;
            break;
        }
    }
    if(sIndex == -1){
        qDebug() << "no find";
        return false;
    }
    dst.magic = two8toOne16(ptSrc[sIndex+1], ptSrc[sIndex]);
    dst.index = two8toOne16(ptSrc[sIndex+3], ptSrc[sIndex+2]);
    dst.size = two8toOne16(ptSrc[sIndex+5], ptSrc[sIndex+4]);
    dst.type = ptSrc[sIndex+6];
    dst.count = ptSrc[sIndex+7];
    dst.time_stamp = four8toOne32(ptSrc[sIndex+11], ptSrc[sIndex+10], ptSrc[sIndex+9], ptSrc[sIndex+8]);
    cpyArray(dst.msg, (uint8_t*)ptSrc+sIndex+12, dst.count);
    qDebug()<< dst.magic << "/" << dst.index << "/" << dst.size << "/" <<\
               dst.type << "/" << dst.count << "/" << dst.time_stamp << "/" \
            << QString::fromLocal8Bit((char*)dst.msg, dst.count);
    return true;
 }

uint16_t CurveController::two8toOne16(const uint8_t& highB, const uint8_t& lowB){
    uint16_t tmp = 0;
    return (uint16_t)(((tmp|highB)<<8)|lowB);
}

uint32_t CurveController::four8toOne32(const uint8_t& highB, const uint8_t& highB1, const uint8_t& lowB, const uint8_t& lowB1){
    uint32_t tmp = 0;
    tmp = (((tmp|highB)<<24|highB1)<<16|lowB)<<8|lowB1;
    return tmp;
}

void CurveController::cpyArray(uint8_t* dst, uint8_t* src, int size){
    for(int i=0; i< size; i++){
        dst[i]=src[i];
    }
}

void CurveController::cbx_loglist(int index){
    if(QString::number(logList[index].id) == m_cbx_logList->itemText(index)){
        qDebug() << index;
        startDownloadLog(logList[index]);
    }
}
