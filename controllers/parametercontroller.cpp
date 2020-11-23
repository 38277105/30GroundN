#include "parametercontroller.h"
#include "QMessageBox"
#include "controls/myhelper.h"
#include "frmmaincontroller.h"
#include "../Utils/zygroundglobalconfig.h"

//#include "controls/analisys/linechartmainwindow.h"

static ParameterController* g_parameterController=NULL;

ParameterController::ParameterController(Vehicle* t_vehicle):QObject(t_vehicle)
{
    m_progressBar=NULL;
    __vehicle=t_vehicle;
     connect(&__vehicle->mavLinkMessageInterface,SIGNAL(onParamValue(mavlink_param_value_t)),this,SLOT(onParamValue(mavlink_param_value_t)),Qt::AutoConnection);

    param_index=0;
    m_IsSet=false;
    m_IsGetFilgthMode=false;

    m_timer=NULL;
    m_timer=new QTimer();
    //m_timer->setInterval(2000);
    m_timer->setInterval(500);
    m_timer->setSingleShot(false);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(request_flightMode()));
    m_timer->start();


    m_param_timeout_timer=NULL;
    m_param_timeout_timer=new QTimer();


    m_param_timeout_timer->setInterval(5000);
    m_param_timeout_timer->setSingleShot(true);
    connect(m_param_timeout_timer,SIGNAL(timeout()),this,SLOT(onParamTimeOut()));

    g_parameterController=this;

    IsExternalUsed=false;

}

ParameterController::~ParameterController()
{
    if(m_timer!=NULL)
    {
        delete m_timer;
        m_timer=NULL;
    }
    if(m_param_timeout_timer!=NULL)
    {
        delete m_param_timeout_timer;
        m_param_timeout_timer=NULL;
    }
    if(m_progressBar!=NULL)
    {
        delete m_progressBar;
    }
}

 ParameterController* ParameterController::Instance()
 {
     //    if(g_parameterController==NULL)
     //        g_parameterController=new ParameterController(NULL);
    return g_parameterController;
 }
//
//获取参数列表
//
void ParameterController::request_parameterList()
{
    IsExternalUsed=false;
    m_timer->stop();
    m_IsSet=false;
    m_params.clear();

    param_index=0;

    if(NULL != __vehicle)
    {
        FrmMainController::Instance()->__requestDataStream(FrmMainController::Fast);
        QThread::msleep(300);
       // __vehicle->mavLinkMessageInterface.paramRequestRead(NULL,param_index);
        __vehicle->mavLinkMessageInterface.paramRequestList();
        //qDebug("---send paramRequestRead(NULL,%d)",param_index);

        if(m_progressBar!=NULL)
        {
            delete  m_progressBar;
            m_progressBar=NULL;
        }
        m_progressBar=new CRoundProgressBar(this->m_frmMain);
        m_progressBar->show();
        //等待进度条
    }
    else
    {
        //Log.Warning("没有发现可用的UAV对象");
    }
   // FrmMainController::Instance()->__requestDataStream(FrmMainController::Fast);
}


void ParameterController::request_flightMode()
{
    m_IsGetFilgthMode=true;
    m_IsSet=false;
    m_params.clear();
    if(NULL != __vehicle)
    {
        //qDebug("---send getFlightMode(basemode,customMode)!---");
        uint8_t basemode;
        uint32_t customMode;
        __vehicle->mavLinkMessageInterface.getFlightMode(basemode,customMode);

        emit OnModeDisplay((int)customMode);
        //qDebug("[mode = %d!|customMode=%d]",basemode,customMode);
    }

}
//
//设置参数
//
void ParameterController::set_parameter(char* param_id,float param_value,MAV_PARAM_TYPE para_type)
{
    IsExternalUsed=false;
    m_IsSet=true;
    if(NULL != __vehicle)
    {
        __vehicle->mavLinkMessageInterface.paramSet(param_value,param_id,para_type);

        //qDebug(QString::asprintf("------set-%s-=-%f-----",param_id,param_value).toLocal8Bit().data());
    }
    else
    {
        //Log.Warning("没有发现可用的UAV对象");
    }
}

void ParameterController::set_parameter_noanswer(char* param_id,float param_value,MAV_PARAM_TYPE para_type)
{
    IsExternalUsed=true;
    m_IsSet=true;
    if(NULL != __vehicle)
    {
        __vehicle->mavLinkMessageInterface.paramSet(param_value,param_id,para_type);

        //qDebug(QString::asprintf("------set %s = %f----",param_id,param_value).toLocal8Bit().data());
    }
    else
    {
        //Log.Warning("没有发现可用的UAV对象");
    }
}

//
//  发送命令让飞控将参数从内容保存到磁盘或者闪存中。
//
void ParameterController::send_save_cmd()
{
    FrmMainController::Instance()->__vehicle->mavLinkMessageInterface.doCommand(MAV_CMD_PREFLIGHT_STORAGE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

//
//参数列表信号
//
void ParameterController::onParamValue(mavlink_param_value_t paramValue)
{
    QString strParamID;
    qint32 param_value_temp;
    strParamID.sprintf("%s",paramValue.param_id);
    char srcArray[4];
    memcpy(srcArray, &paramValue.param_value, sizeof(int));
    param_value_temp =*((int*)srcArray);
    if(strParamID == "ESC_CALI_EN" && param_value_temp == 1)
    {
        g_bSetESC = true;
    }

    //qDebug()<<"Param:"<<paramValue.param_id<<" Value:"<<paramValue.param_value;
    if(IsExternalUsed)
    {
        return;
    }
     ParamMaxCount=paramValue.param_count;
     //qDebug()<<"Argument Reading Progress: "<<param_index<<"/"<<ParamMaxCount;
     if(m_IsSet)
     {
         myHelper::ShowMessageBoxInfo("设置完成!");
         m_IsSet=false;
         return;
     }
     else
     {
       double value_progress=((double)m_params.count())/((double)ParamMaxCount-1);
      if(m_progressBar && CRoundProgressBar::m_IsClosed!=true)
      {
        m_progressBar->setScanValue(QString::number((int)(value_progress*100)));
      }
     }

      ParamData t_data;
      char id[24];
      memset(id,0,24*sizeof(char));
      memcpy(id,paramValue.param_id,16*sizeof(char));
      t_data.key=id;
      if(paramValue.param_type == MAV_PARAM_TYPE_REAL32){
          t_data.value=t_data.value.sprintf("%f",paramValue.param_value);
      }else if(paramValue.param_type == MAV_PARAM_TYPE_INT32){
          char srcArray[4];
          memcpy(srcArray, &paramValue.param_value, sizeof(int));
          int value =*((int*)srcArray);
          qDebug()<<"Param:"<<paramValue.param_id<<" Value:"<<value;
          t_data.value=t_data.value.sprintf("%d",value);
      }
        emit OnParameterChanged(1,1,paramValue.param_id,t_data.value);


      t_data.type=paramValue.param_type;
      t_data.count=paramValue.param_count;
      m_params[t_data.key] = t_data; //向map里插入键-值

      param_index++;
      if(param_index<ParamMaxCount)
      {
         // __vehicle->mavLinkMessageInterface.paramRequestRead(NULL,param_index);
          m_param_timeout_timer->start();
      }
      else {
        //停止申请
        emit OnParamRecv(&m_params.values());

        delete  m_progressBar;
        m_progressBar=NULL;
        CRoundProgressBar::m_IsClosed=true;

        m_timer->start();
        m_param_timeout_timer->stop();
        ParamMaxCount=0;
        param_index=0;

        FrmMainController::Instance()->__requestDataStream(FrmMainController::Fast);


//         mainWin=new LineChartMainWindow();
//         mainWin->show();


      }
}

void ParameterController::onParamTimeOut()
{
    if(IsExternalUsed)
    {
        return;
    }
    // __vehicle->mavLinkMessageInterface.paramRequestRead(NULL,param_index);
    // request_parameterList();
    // m_param_timeout_timer->start();

    emit OnParamRecv(&m_params.values());

    delete  m_progressBar;
    m_progressBar=NULL;
    CRoundProgressBar::m_IsClosed=true;

    m_timer->start();
    m_param_timeout_timer->stop();
    ParamMaxCount=0;
    param_index=0;

    //FrmMainController::Instance()->__requestDataStream(FrmMainController::Fast);
}


void ParameterController::set_callback(ParameterTableView* view)
{
  connect(this,SIGNAL(OnParamRecv(QList<ParamData>*)),view,SLOT(sync_args_msgcomes(QList<ParamData>*)));
}
//
void ParameterController::SetControl(frmMain* m_control)
{
    this->m_frmMain=m_control;
    connect(this,SIGNAL(OnModeDisplay(int)),m_frmMain,SLOT(UpdateFlightMode(int)));
}
