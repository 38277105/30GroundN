#include "databoxresolver.h"
#include <QDebug>
#include <QDateTime>
#include <QCoreApplication>
#include <stdexcept>


DataBoxResolver::DataBoxResolver(QObject *parent) : QThread(parent)
{
    _readtag=true;
    connect(this,SIGNAL(signal_WriteBytes(char*,size_t)),this,SLOT(WriteBytes(char*,size_t)));
    fp_uav=NULL;
}


void DataBoxResolver::openserial(QString comName)
{
      fp_uav=NULL;
     _readtag=true;//启用循环读取
     _serialport=new QSerialPort(this);
     _serialport->setPortName(comName);

     if(!_serialport->open(QIODevice::ReadWrite)){
         printf("open serial error \n");
     }
     _serialport->setBaudRate(_portNum);
     _serialport->setDataBits(QSerialPort::Data8);
     _serialport->setStopBits(QSerialPort::OneStop);
     _serialport->setParity(QSerialPort::NoParity);
     _serialport->setFlowControl(QSerialPort::NoFlowControl);

     _serialport->clearError();
     _serialport->clear();

     disconnect(_serialport, SIGNAL(readyRead()), this, SLOT(readBytes()));
     connect(_serialport, SIGNAL(readyRead()), this, SLOT(readBytes()));

     offset = 0;
}

void DataBoxResolver::readBytes()
{

    char lengthH, lengthL;
    int length = 0;
    int data_len=0;
    int offset = 0;

    if(_serialport && _serialport->isOpen())
    {
           const qint64 maxLength = 128;
           char data[maxLength];
           memset(data,0,128*sizeof(char));
           //dataMutex.lock();
           qint64 numBytes = _serialport->bytesAvailable();

           //qDebug()<<"--------------recv:-->"<<numBytes;
           if (numBytes >= 76)
           {

               // Read as much data in buffer as possible without overflow
               if(maxLength < numBytes) numBytes = maxLength;
               qint64 sz = _serialport->read(data+offset, 1);
               unsigned char cTmp = data[0];
               int startIndex=0;
               while(0xAA!=cTmp)
               {
                   if(startIndex==numBytes)
                       return;
                    sz = _serialport->read(data+offset, 1);
                    cTmp = data[0];
                    startIndex++;
               }
                ++offset;
               if( 0xAA == cTmp )
               {//frame head 0

                    sz = _serialport->read(data+offset, 1);
                    ++offset;

                    cTmp = data[1];
                    if( 0xAF == cTmp )
                    {
                        sz = _serialport->read(data+offset, 1);
                        ++offset;
                        lengthH = data[2];


                        sz = _serialport->read(data+offset, 1);
                        ++offset;
                        lengthL = data[3];


                        length = ((unsigned short)lengthL&0xff) | ((unsigned short)((lengthH&0xff)<<8));
                        data_len=length-4;
                        //qDebug()<<"data length:"<<data_len;
                        if(data_len==0)
                        {
                             sendDateTime();
                             return;
                        }


                         AIRSENSORSTR m_airSensor;
                         UAVDATASTR uavDataStr;


                         qint64 lenTmp = _serialport->read(data+offset, data_len);

                         parseAirDetectData2Str(data,length,m_airSensor,uavDataStr);
                         emit datacoming(m_airSensor, uavDataStr);
                        }
                    }
           }

          // _dataMutex.unlock();
       }

}

void DataBoxResolver::sendDateTime()
{
  SWITCHFLOAT2CHAR swTmp;
  int year=  QDateTime::currentDateTime().toString("yyyy").toInt();
  char buffer[28];
   memset(buffer,0,28*sizeof(char));

   swTmp.i=year;
   buffer[0]=0xAA;
   buffer[1]=0xAF;
   buffer[2]=0x00;
   buffer[3]=0x1c;

   buffer[4]=swTmp.c.c0;
   buffer[5]=swTmp.c.c1;
   buffer[6]=swTmp.c.c2;
   buffer[7]=swTmp.c.c3;

   int month=  QDateTime::currentDateTime().toString("MM").toInt();
   swTmp.i=month;

   buffer[8]=swTmp.c.c0;
   buffer[9]=swTmp.c.c1;
   buffer[10]=swTmp.c.c2;
   buffer[11]=swTmp.c.c3;


   int mon_day=  QDateTime::currentDateTime().toString("dd").toInt();
   swTmp.i=mon_day;

   buffer[12]=swTmp.c.c0;
   buffer[13]=swTmp.c.c1;
   buffer[14]=swTmp.c.c2;
   buffer[15]=swTmp.c.c3;

   int hh=  QDateTime::currentDateTime().toString("HH").toInt();
   swTmp.i=hh;

   buffer[16]=swTmp.c.c0;
   buffer[17]=swTmp.c.c1;
   buffer[18]=swTmp.c.c2;
   buffer[19]=swTmp.c.c3;

   int minute=  QDateTime::currentDateTime().toString("mm").toInt();
   swTmp.i=minute;

   buffer[20]=swTmp.c.c0;
   buffer[21]=swTmp.c.c1;
   buffer[22]=swTmp.c.c2;
   buffer[23]=swTmp.c.c3;

   int sec=  QDateTime::currentDateTime().toString("ss").toInt();
   swTmp.i=sec;

   buffer[24]=swTmp.c.c0;
   buffer[25]=swTmp.c.c1;
   buffer[26]=swTmp.c.c2;
   buffer[27]=swTmp.c.c3;

  //qint64 len= _serialport->write(buffer,sizeof(char)*28);
   emit signal_WriteBytes(buffer,sizeof(char)*28);
  //qDebug()<<"write len:"<<len;

}

void DataBoxResolver::queryports(QList<QString>& devlist)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        //qDebug() << "Name : " << info.portName();
        //qDebug() << "Description : " << info.description();
        //qDebug() << "Manufacturer: " << info.manufacturer();
        //qDebug() << "Serial Number: " << info.serialNumber();
        //qDebug() << "System Location: " << info.systemLocation();
        devlist.append(info.portName());
    }
}

void DataBoxResolver::stop()
{
    _readtag=false;     //关闭用循环读取
    if (_serialport) {
        _serialport->close();
        delete _serialport;
        _serialport = NULL;
    }
    fclose(fp_uav);
    fp_uav=NULL;
}

int  DataBoxResolver::parseAirDetectData2Str(char* buffer, size_t length,AIRSENSORSTR& airSensorStr, UAVDATASTR& uavDataStr){
    int offset = 0;
    int len = 0;
    unsigned short loC = 0;
    unsigned short hiC = 0;

    SWITCHFLOAT2CHAR swTmp;

    //判断头 长度
    unsigned char headA = buffer[0];
    unsigned char headB = buffer[1];
    if( 0xAA != headA || 0xAF != headB || 0x4c != length){
        return 0;
    }
    offset += 2;

    //长度
    hiC = buffer[offset];
    ++offset;
    loC = buffer[offset];
    ++offset;
    len = ((unsigned short)loC&0xff) | ((unsigned short)((hiC&0xff)<<8));
    if( 0x4c != len )return 0;

    //year
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.year = swTmp.i;

    //mon
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.mon = swTmp.i;

    //day
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.day = swTmp.i;

    //hour
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.hour = swTmp.i;

    //min
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.min = swTmp.i;

    //sec
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.sec = swTmp.i;

    //经度
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.longitude = swTmp.f;

    //纬度
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.latitude = swTmp.f;

    //高度
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    uavDataStr.hight = swTmp.f;

    //O3
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.O3 = swTmp.f;

//    if(airSensorStr.O3 <0.0001){
//        int asdf = 0;
//    }

    //SO2
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.SO2 = swTmp.f;

    //CO
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.CO = swTmp.f;

    //NO2
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.NO2 = swTmp.f;

    //pm1.0
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.pm1_0 = swTmp.f;

    //pm2.5
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.pm2_5 = swTmp.f;

    //pm10
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.pm10 = swTmp.f;

    //温度
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.temperature = swTmp.f;

    //湿度
    swTmp.c.c0 = buffer[offset];
    ++offset;
    swTmp.c.c1 = buffer[offset];
    ++offset;
    swTmp.c.c2 = buffer[offset];
    ++offset;
    swTmp.c.c3 = buffer[offset];
    ++offset;
    airSensorStr.humidity = swTmp.f;

    return offset;
}

void DataBoxResolver::run()
{
  this->openserial(this->_portName);
  this->exec();
}

void DataBoxResolver::SaveFile(QList<AIRSENSORSTR>& airSensorStr, QList<UAVDATASTR>& uavDataStr)
{

    QDateTime dt;
    QTime time;
    QDate date;
    dt.setTime(time.currentTime());
    dt.setDate(date.currentDate());
    QString t_timestr = dt.toString("yyyy_MM_dd_hh");
    char uav_str[128]="";
    std::sprintf(uav_str,"./sensordata/Data_%s.csv",t_timestr.toStdString().c_str());

    if(fp_uav==NULL)
    {
        if((fp_uav=fopen(uav_str,"a+"))==NULL)
        {
           fp_uav=fopen(uav_str,"w+");
        }
    }
    std::fseek(fp_uav,0L,SEEK_END);
   int fileSize= std::ftell(fp_uav);
    if(fileSize<100)
    {
        std::fprintf(fp_uav,"DATE/TIME,longitude,lat,height,O3(μg/m3),SO2(μg/m3),CO(mg/m3),NO2(μg/m3),pm1,pm2_5,pm10,temperature,humidity\n");
    }
    for(int i=0;i<uavDataStr.count();i++)
    {
     std::fprintf(fp_uav,"%04d/%02d/%02d %d:%d:%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",uavDataStr[i].year,
                    uavDataStr[i].mon,
                    uavDataStr[i].day,
                    uavDataStr[i].hour,
                    uavDataStr[i].min,
                    uavDataStr[i].sec,
                    uavDataStr[i].longitude,
                    uavDataStr[i].latitude,
                    uavDataStr[i].hight,
                    airSensorStr[i].O3,
                    airSensorStr[i].SO2,
                    airSensorStr[i].CO,
                    airSensorStr[i].NO2,
                    airSensorStr[i].pm1_0,
                    airSensorStr[i].pm2_5,
                    airSensorStr[i].pm10,
                    airSensorStr[i].temperature,
                    airSensorStr[i].humidity);

    }
    fflush(fp_uav);
//    }
//    catch(int errorNum)
//    {
//        FILE* m_fp=NULL;
//        char fileName[128]="./exception.txt";
//        if((m_fp=fopen(fileName,"a+"))==NULL)
//        {
//           m_fp=fopen(fileName,"w+");
//        }
//        QTextStream txtOutput(m_fp);
//        txtOutput << "save sensor data error !" << endl;
//        fflush(m_fp);
//        fclose(m_fp);
//    }
}

void DataBoxResolver::setPortName(QString portName,int PortNum)
{
    _portName=portName;
    _portNum=PortNum;
}

void DataBoxResolver::WriteBytes(char* buffer, size_t length)
{
    if(_serialport && _serialport->isOpen())
    {
       _serialport->write(buffer,length);
    }
}
