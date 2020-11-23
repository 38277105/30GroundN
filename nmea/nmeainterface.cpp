#include "nmeainterface.h"

nmeaInterface::nmeaInterface()
{
    __streamBuffer.clear();
}

//
//打开nmea传输
//
bool nmeaInterface::openNmea(char* port){
    __serialport.setPortName(port);

    __serialport.setBaudRate(115200);
    __serialport.setDataBits(QSerialPort::Data8);
    __serialport.setStopBits(QSerialPort::OneStop);
    __serialport.setParity(QSerialPort::NoParity);
    __serialport.setFlowControl(QSerialPort::NoFlowControl);
    __serialport.clearError();
    __serialport.clear();

    bool ret = false;
    if( !( ret =__serialport.open(QIODevice::ReadWrite) )){
        printf("open serial error : %s\n", __serialport.errorString());
        QString ss = __serialport.errorString();
        return false;
    }else{
        printf("the serial open ok : %s", port);
    }

   connect(&__serialport, SIGNAL(readyRead()), this, SLOT(onReadyRead()),Qt::DirectConnection);

   return ret;
}

//
//关闭nmea传输
//
bool nmeaInterface::closeNmea(){
    if( __serialport.isOpen() ){
        __serialport.close();
    }

    return !__serialport.isOpen();
}

//
//读信号
//
void nmeaInterface::onReadyRead(){
    char buffer[1024];
    memset( buffer,0,1024*sizeof(char) );

    qint64 numBytes = __serialport.bytesAvailable();

    int length = __serialport.read(buffer, numBytes);

    __readNmea(buffer,length);
}


//
//读nmea协议
//
int nmeaInterface::__readNmea(char* buffer, int length){
    __streamBuffer<<buffer;

    string nmeaPack;

    while(!__streamBuffer.eof()){
        getline(__streamBuffer,nmeaPack);
        __parseNmea(nmeaPack);
    }


    return nmeaPack.size();
}

//
//解析nmea
//
int  nmeaInterface::__parseNmea(string buffer){
    if( buffer.find("#HEADINGA") != std::string::npos ){ //#HEADINGA
        HEADINGA headinga;
        __paraseHeadinga(buffer, headinga);
        __sendHeading2Vehicle(headinga);
    }

    return 0;
}

//
//解析headinga包
//
bool nmeaInterface::__paraseHeadinga(string buffer, HEADINGA& headinga){
    int begin = 0;
    int end = 0;

    //header
    headinga.header = "#HEADINGA";

    //转到有效数据位
    end = buffer.find(';',begin);
    begin = end+1;

    //sol stat
    end = buffer.find(',',begin);
    headinga.solStat = buffer.substr(begin, end-begin);
    begin = end+1;

    //pos type
    end = buffer.find(',',begin);
    headinga.posType = buffer.substr(begin, end-begin);
    begin = end+1;

    //length
    end = buffer.find(',',begin);
    headinga.length = buffer.substr(begin, end-begin);
    begin = end+1;

    //heading
    end = buffer.find(',',begin);
    headinga.heading = buffer.substr(begin, end-begin);
    begin = end+1;

    //pitch
    end = buffer.find(',',begin);
    headinga.pitch = buffer.substr(begin, end-begin);
    begin = end+1;

    //reserved
    end = buffer.find(',',begin);
    headinga.reserved1 = buffer.substr(begin, end-begin);
    begin = end+1;

    //hdg
    end = buffer.find(',',begin);
    headinga.hdg = buffer.substr(begin, end-begin);
    begin = end+1;

    //ptch
    end = buffer.find(',',begin);
    headinga.ptch = buffer.substr(begin, end-begin);
    begin = end+1;

    //stn
    end = buffer.find(',',begin);
    headinga.stn = buffer.substr(begin, end-begin);
    begin = end+1;

    //#svs
    end = buffer.find(',',begin);
    headinga.svs = buffer.substr(begin, end-begin);
    begin = end+1;

    //#solnSvs
    end = buffer.find(',',begin);
    headinga.solnSvs = buffer.substr(begin, end-begin);
    begin = end+1;

    //#obs
    end = buffer.find(',',begin);
    headinga.obs = buffer.substr(begin, end-begin);
    begin = end+1;

    //#multi
    end = buffer.find(',',begin);
    headinga.multi = buffer.substr(begin, end-begin);
    begin = end+1;

    //reserved
    end = buffer.find(',',begin);
    headinga.reserved2 = buffer.substr(begin, end-begin);
    begin = end+1;

    //extSolSta
    end = buffer.find(',',begin);
    headinga.extSolStat = buffer.substr(begin, end-begin);
    begin = end+1;

    //reserved
    end = buffer.find(',',begin);
    headinga.reserved3 = buffer.substr(begin, end-begin);
    begin = end+1;

    //sigMask
    end = buffer.find('*',begin);
    headinga.sigMask = buffer.substr(begin, end-begin);
    begin = end+1;

    //crc
    headinga.length = buffer.substr(begin, buffer.length()-begin);
    begin = end;

    return true;
}

//
//发送方位角到飞机
//
bool nmeaInterface::__sendHeading2Vehicle(HEADINGA headinga){
    float yaw = atof(headinga.heading.c_str());
    emit paramSet(yaw, (char*)"A_GRD_YAW", 9);

    return true;
}
