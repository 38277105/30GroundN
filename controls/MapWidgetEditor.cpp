
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include "MapWidgetEditor.h"
#include "qmessagebox.h"
#include <QFileDialog>
#include "Utils/zygroundglobalconfig.h"

struct MapType_Data
{
    char    name[256];
    int     typeID;
};

#define MAPTYPE_STRUCT(n)   { #n, core::MapType::n }

static MapType_Data g_arrMapType[] =
{
    //    MAPTYPE_STRUCT(GoogleMap),
    //    MAPTYPE_STRUCT(GoogleSatellite),
    //    MAPTYPE_STRUCT(GoogleLabels),
    //    MAPTYPE_STRUCT(GoogleTerrain),
    //    MAPTYPE_STRUCT(GoogleHybrid),

    //    MAPTYPE_STRUCT(GoogleMapChina),
        MAPTYPE_STRUCT(GoogleSatelliteChina),
    //    MAPTYPE_STRUCT(GoogleLabelsChina),
    //    MAPTYPE_STRUCT(GoogleTerrainChina),
    //    MAPTYPE_STRUCT(GoogleHybridChina),

        MAPTYPE_STRUCT(OpenStreetMap),
    //    MAPTYPE_STRUCT(OpenStreetOsm),
    //    MAPTYPE_STRUCT(OpenStreetMapSurfer),
    //    MAPTYPE_STRUCT(OpenStreetMapSurferTerrain),

    //    MAPTYPE_STRUCT(YahooMap),
    //    MAPTYPE_STRUCT(YahooSatellite),
    //    MAPTYPE_STRUCT(YahooLabels),
    //    MAPTYPE_STRUCT(YahooHybrid),

        MAPTYPE_STRUCT(BingMap),
        MAPTYPE_STRUCT(BingSatellite),
    //    MAPTYPE_STRUCT(BingHybrid),

    //    MAPTYPE_STRUCT(ArcGIS_Map),
    //    MAPTYPE_STRUCT(ArcGIS_Satellite),
    //    MAPTYPE_STRUCT(ArcGIS_ShadedRelief),
    //    MAPTYPE_STRUCT(ArcGIS_Terrain),
    //    MAPTYPE_STRUCT(ArcGIS_WorldTopo),

    //    MAPTYPE_STRUCT(ArcGIS_MapsLT_Map),
    //    MAPTYPE_STRUCT(ArcGIS_MapsLT_OrtoFoto),
    //    MAPTYPE_STRUCT(ArcGIS_MapsLT_Map_Labels),
    //    MAPTYPE_STRUCT(ArcGIS_MapsLT_Map_Hybrid),

    //    MAPTYPE_STRUCT(PergoTurkeyMap),
    //    MAPTYPE_STRUCT(SigPacSpainMap),

    //    MAPTYPE_STRUCT(GoogleMapKorea),
    //    MAPTYPE_STRUCT(GoogleSatelliteKorea),
    //    MAPTYPE_STRUCT(GoogleLabelsKorea),
    //    MAPTYPE_STRUCT(GoogleHybridKorea),

    //    MAPTYPE_STRUCT(YandexMapRu),
    //    MAPTYPE_STRUCT(Statkart_Topo2),

    {"NULL", -1}
};

MapWidgetEditor::MapWidgetEditor(QWidget *parent) :
    QWidget(parent)
{
    // create configure obj
    m_conf = new QSettings("./data/demo.ini", QSettings::IniFormat);
    m_uav = NULL;
    m_autoPlan = NULL;
    // setup layout
    setupLayout();

    // set window title
   // setWindowTitle("MapWidget demo");
}

int MapWidgetEditor::setupLayout(void)
{
    // create OPMapWidget
    m_mapWidget = new MapWidget(this);

    double  lat, lng;
    int     zoom;
    lat  = m_conf->value("lastPos_lat", 34.257287).toDouble();
    lng  = m_conf->value("lastPos_lng", 108.888931).toDouble();
    zoom = m_conf->value("lastZoom", 11).toInt();

    internals::PointLatLng p(lat, lng);
    m_mapWidget->SetZoom(zoom);
    m_mapWidget->setConf(m_conf);
    m_isNeedUpdating = false;
    //m_isInitHome = false;
    m_mapWidget->MapWidget_getHomeData(m_homeData);
    m_mapType = m_mapWidget->GetMapType();
    connect(m_mapWidget, SIGNAL(transferWayPointDatas(QList<MapPointData>)), this, SLOT(receiveWPDatas(QList<MapPointData>)));
    connect(m_mapWidget, SIGNAL(signal_transferOneWayPointData(const MapPointData&)), this, SLOT(receiveOneWPData_slots(const MapPointData&)));
    connect(m_mapWidget, SIGNAL(OnCurrentPositionChanged(internals::PointLatLng)), this, SLOT(update_currentPos(internals::PointLatLng)));
    connect(m_mapWidget, SIGNAL(signal_map_WPMoved(const int&, const MapPointData&)), this, SIGNAL(signal_mapEditor_UpdateMovableWP(const int&, const MapPointData&)));

    //    connect(m_mapWidget, SIGNAL(zoomChanged(int)),
//            this, SLOT(mapWidget_zoomChanged(int)));
//    connect(m_mapWidget, SIGNAL(mousePress(QMouseEvent*)),
//            this, SLOT(mapWidget_mousePressEvent(QMouseEvent*)));

//    m_btnFatchMap = new QPushButton(wLeftPanel);
//    m_btnFatchMap->setText("Cache map");
//    m_btnFatchMap->setGeometry(10, 10, 90, 30);
//    connect(m_btnFatchMap, SIGNAL(clicked()), this, SLOT(actFatchMap()));

//    m_cbMoveUAV = new QCheckBox("Move UAV", wLeftPanel);
//    m_cbMoveUAV->setGeometry(10, 60, 90, 30);
//    connect(m_cbMoveUAV, SIGNAL(clicked(bool)), this, SLOT(moveUAVStateChanged(bool)));

    m_cbAirLine = new QCheckBox(QString::fromLocal8Bit("航路编辑"), this);
    m_cbAirLine->setCheckState(Qt::Unchecked);
    connect(m_cbAirLine, SIGNAL(clicked(bool)), this, SLOT(moveAirLineStateChanged(bool)));

    m_btn_upload = new QPushButton(QString::fromLocal8Bit("写入航路"),this);
    connect(m_btn_upload, SIGNAL(clicked()), this, SLOT(upload_slots()));

    m_btn_download = new QPushButton(QString::fromLocal8Bit("读取航路"),this);
    connect(m_btn_download, SIGNAL(clicked()), this, SLOT(download_slots()));

    m_btn_savelocal = new QPushButton(QString::fromLocal8Bit("保存航路"),this);
    connect(m_btn_savelocal, SIGNAL(clicked()), this, SLOT(save_click()));

    m_btn_openlocal = new QPushButton(QString::fromLocal8Bit("打开航路"),this);
    connect(m_btn_openlocal, SIGNAL(clicked()), this, SLOT(open_load_click()));

    m_cursorLabel = new QLabel(QString::fromLocal8Bit("光标位置"),this);

    m_lbl_timinglng = new QLabel(QString::fromLocal8Bit("经度:"),this);
    m_ledt_timinglng = new QLineEdit(this);

    m_lbl_timinglat = new QLabel(QString::fromLocal8Bit("纬度:"), this);
    m_ledt_timinglat = new QLineEdit(this);

    m_mapTypeLabel = new QLabel(QString::fromLocal8Bit("地图模式"));
    m_cboMapType = new QComboBox(this);
    setupMapType_list();
    setMapType();
    connect(m_cboMapType, SIGNAL(currentIndexChanged(int)), this, SLOT(changedMapType(int)));

    m_homeDataLabel = new QLabel(QString::fromLocal8Bit("HOME位置"),this);

    m_lbl_lng = new QLabel(QString::fromLocal8Bit("经度:"), this);
    m_ledt_lng = new QLineEdit(QString::number(m_homeData.lng, 'f', 7),this);
    connect(m_ledt_lng, SIGNAL(editingFinished()), this, SLOT(homeData_editingFinished()));

    m_lbl_lat = new QLabel(QString::fromLocal8Bit("纬度:"),this);

    m_ledt_lat = new QLineEdit(QString::number(m_homeData.lat, 'f', 7),this);
    connect(m_ledt_lat, SIGNAL(editingFinished()), this, SLOT(homeData_editingFinished()));

    m_lbl_alt = new QLabel(QString::fromLocal8Bit("高度:"),this);

    m_ledt_alt = new QLineEdit(QString::number(m_homeData.alt, 'f', 2),this);
    connect(m_ledt_alt, SIGNAL(editingFinished()), this, SLOT(homeData_editingFinished()));

    m_offset_setting = new QLabel(QString::fromLocal8Bit("地图偏移设置"),this);


    m_lbl_offset_Lat=new QLabel(QString::fromLocal8Bit("经度:"),this);
    m_tbx_offset_Lat=new QLineEdit(QString("0.0"),this);
    connect(m_tbx_offset_Lat, SIGNAL(editingFinished()), this, SLOT(OffsetChanged()));
    m_lbl_offset_Lon=new QLabel(QString::fromLocal8Bit("维度:"),this);
    m_tbx_offset_Lon=new QLineEdit(QString("0.0"),this);
    connect(m_tbx_offset_Lon, SIGNAL(editingFinished()), this, SLOT(OffsetChanged()));


    m_view_setting  = new QLabel(QString::fromLocal8Bit("常用设置"),this);
    m_SetUavasCenter= new QCheckBox(QString::fromLocal8Bit("视图以无人机为中心"), this);
    m_SetUavasCenter->setCheckState(Qt::Unchecked);
    connect(m_SetUavasCenter, SIGNAL(clicked(bool)), this, SLOT(SetViewCenterModeChanged(bool)));

    m_btn_downloadMap = new QPushButton(QString::fromLocal8Bit("地图下载"),this);
    m_btn_downloadMap->setToolTip(QString::fromLocal8Bit("按住键盘Shift或Alt,同时在地图上按住鼠标左键选中需要下载地图的区域,点击右边[缓存离线地图]按钮.下载地图文件为/data/OPMaps.qmdb"));
    connect(m_btn_downloadMap, SIGNAL(clicked()), this, SLOT(CacheOffMap()));

    m_isFinished = false;
    m_btn_autoPlan = new QPushButton(QString::fromLocal8Bit("自动规划"),this);
    connect(m_btn_autoPlan, SIGNAL(clicked()), this, SLOT(mapEdit_AutoPlan_slots()));

    m_mainLayout = new QGridLayout;
    m_mainLayout->addWidget(m_cboMapType,0, 5, 1, 6);
    m_mainLayout->addWidget(m_cbAirLine, 1, 5, 1, 6);
    m_mainLayout->addWidget(m_SetUavasCenter, 2, 5, 1, 6);
    m_mainLayout->addWidget(m_btn_upload,3, 5, 1, 3);
    m_mainLayout->addWidget(m_btn_download,3, 8, 1, 3);
    m_mainLayout->addWidget(m_btn_savelocal,4, 5, 1, 3);
    m_mainLayout->addWidget(m_btn_openlocal,4, 8, 1, 3);
    m_mainLayout->addWidget(m_btn_downloadMap,5, 5, 1, 3);
    m_mainLayout->addWidget(m_btn_autoPlan,5, 8, 1, 3);

    m_mainLayout->addWidget(m_homeDataLabel, 6, 5, 1, 6);
    m_mainLayout->addWidget(m_lbl_lng,7, 5, 1, 1);
    m_mainLayout->addWidget(m_ledt_lng,7, 6, 1, 5);
    m_mainLayout->addWidget(m_lbl_lat,8, 5, 1, 1);
    m_mainLayout->addWidget(m_ledt_lat,8, 6, 1, 5);
    m_mainLayout->addWidget(m_lbl_alt,9, 5, 1, 1);
    m_mainLayout->addWidget(m_ledt_alt,9, 6, 1, 5);

    m_mainLayout->addWidget(m_offset_setting,10,5,1,6);
    m_mainLayout->addWidget(m_lbl_offset_Lat,11,5,1,1);
    m_mainLayout->addWidget(m_tbx_offset_Lat,11,6,1,5);
    m_mainLayout->addWidget(m_lbl_offset_Lon,12,5,1,1);
    m_mainLayout->addWidget(m_tbx_offset_Lon,12,6,1,5);

    m_mainLayout->addWidget(m_cursorLabel,13, 5, 1, 6);
    m_mainLayout->addWidget(m_lbl_timinglng,14, 5, 1, 1);
    m_mainLayout->addWidget(m_ledt_timinglng,14, 6, 1, 5);
    m_mainLayout->addWidget(m_lbl_timinglat,15, 5, 1, 1);
    m_mainLayout->addWidget(m_ledt_timinglat,15, 6, 1, 5);

//   m_mainLayout->addWidget(m_view_setting,17,6,1,5);
//   m_mainLayout->addWidget(m_btn_downloadMap,19,5,1,5);
//   m_mainLayout->addWidget(m_btn_autoPlan, 20,5,1,5);
    this->setLayout(m_mainLayout);

    this->startTimer(30);

    return 0;
}

void MapWidgetEditor::ShowUAV(bool value)
{
  m_mapWidget->SetShowUAV(value);
  m_mapWidget->SetUavPic(":/uavs/images/airplane_new.png");
  m_mapWidget->AddUAV(0x0002);
}

int MapWidgetEditor::mapWidgetEditor_deleteOneWP(int index){
    m_mapWidget->Map_deleteOneWP(index);
    return true;
}

int MapWidgetEditor::mapWidgetEditor_updateOneWP(const int& row, const MapPointData& oneWP){
    m_mapWidget->MapWidget_updateOneWP(row, oneWP);
    return true;
}

int MapWidgetEditor::mapWidgetEditor_upOneWP(int row){
    m_mapWidget->mapWidget_upOneWP(row);
    return true;
}
int MapWidgetEditor::mapWidgetEditor_downOneWP(int row){
    m_mapWidget->mapWidget_downOneWP(row);
    return true;
}

void MapWidgetEditor::AddTableView(ShowCurrentWayPoints* editor, QSplitter* splitr){

    m_table_editor=editor;
    m_table_editor->showTable_SetHome(m_homeData.lng, m_homeData.lat, m_homeData.alt);
    splitr->addWidget(m_mapWidget);
    splitr->addWidget(editor);
    m_mainLayout->addWidget(splitr, 0, 0, 25, 4);

}

void MapWidgetEditor::upload_slots(void)
{
    QList<ProtocolDataType> mylist;
    m_table_editor->showTable_getDatas(mylist);

   emit signal_sendWP(mylist);
}

 void MapWidgetEditor::save_click()
 {
    QString fileName = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存文件"),"/waypoints",QString::fromLocal8Bit("文本文件 (*.txt)"));

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text)){
         QMessageBox::warning(this,"sdf","can't open",QMessageBox::Yes);
    }

    QList<ProtocolDataType> mylist;
    m_table_editor->showTable_getDatas(mylist);

    QTextStream in(&file);
    for(QList<ProtocolDataType>::iterator iter = mylist.begin(); iter != mylist.end(); iter++){
            in << QString::number((*iter).x, 'f', 7) << " " << QString::number((*iter).y, 'f', 7) << " " << QString::number((*iter).z, 'f', 7) << " " << (*iter).command  << "\n";
    }
    file.close();
    QMessageBox::warning(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("保存航路完成"),QMessageBox::Yes);
 }

void MapWidgetEditor::download_slots()
{
    QMessageBox::warning(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("下载航路请求已发送"),QMessageBox::Yes);
    emit signal_getWP();
}

void MapWidgetEditor::open_load_click()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开航路文件"),"/waypoints",QString::fromLocal8Bit("文本文件 (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         return;
    QList<ProtocolDataType> tmpDataList;
    QTextStream in(&file);
    QString line = in.readLine();
     while (!line.isNull()) {
         generateLocalWayPointList(tmpDataList, line);
         line = in.readLine();
     }
     m_table_editor->showTable_updateShowList_slots(tmpDataList);
     QMessageBox::warning(this,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("打开本地完成"),QMessageBox::Yes);
}
void MapWidgetEditor::moveUAVStateChanged(bool state)
{

    if( m_cbMoveUAV->checkState() == Qt::Checked ) {
        m_uav = m_mapWidget->AddUAV(0);
    } else {
        m_mapWidget->DeleteUAV(0);
        m_uav = NULL;
    }
}

void MapWidgetEditor::mapWidget_zoomChanged(int newZoom)
{
    printf(">>> zoom to: %d\n", newZoom);
}

void MapWidgetEditor::mapWidget_mousePressEvent(QMouseEvent *event)
{
    try
    {
        if( m_cbMoveUAV->checkState() == Qt::Checked ) {
            internals::PointLatLng p;

            p = m_mapWidget->currentMousePosition();
            printf("p  = %f %f\n", p.Lat(), p.Lng());

            if( m_uav != NULL ) m_uav->SetUAVPos(p, 460);
        }
    }
    catch(QString exception)
    {
        QMessageBox::about(this,"Error",exception);
    }
}

void MapWidgetEditor::moveAirLineStateChanged(bool state){
    m_mapWidget->setAirLineState(state);
}

void MapWidgetEditor::SetViewCenterModeChanged(bool state)
{
    ZYGroundGlobalConfig::setUavAsCenter=state;
}

void MapWidgetEditor::receiveWPDatas(QList<MapPointData> WPList){
    m_table_editor->showTable_init(WPList);
}

void MapWidgetEditor::receiveOneWPData_slots(const MapPointData& OneWPD){
    m_table_editor->showTable_inertOneDataItem(OneWPD);
}

void MapWidgetEditor::initSetHome(internals::PointLatLng pos,  double height){
    m_ledt_lng->setText(QString::number(pos.Lng(), 'f', 7));
    m_ledt_lat->setText(QString::number(pos.Lat(), 'f', 7));
    m_ledt_alt->setText(QString::number(height, 'f', 2));
    //m_isInitHome = true;
    emit m_ledt_lng->editingFinished();
    emit m_ledt_lat->editingFinished();
    emit m_ledt_alt->editingFinished();
}

void MapWidgetEditor::homeData_edited(QString text){
    switch (m_editType) {
    case 0:
        m_homeData.lng = text.toDouble();
        disconnect(m_ledt_lng, SIGNAL(textEdited(QString)), this, SLOT(homeData_edited(QString)));
        break;
    case 1:
        m_homeData.lat = text.toDouble();
        disconnect(m_ledt_lat, SIGNAL(textEdited(QString)), this, SLOT(homeData_edited(QString)));
        break;
    case 2:
        m_homeData.alt = text.toDouble();
        disconnect(m_ledt_alt, SIGNAL(textEdited(QString)), this, SLOT(homeData_edited(QString)));
        break;
    default:
        break;
    }
    if(m_isNeedUpdating){
        m_mapWidget->MapWidget_setHomeData(m_homeData);
        m_table_editor->showTable_SetHome(m_homeData.lng, m_homeData.lat, m_homeData.alt);
        m_isNeedUpdating = false;
    }
//    if(!m_isInitHome){
//        internals::PointLatLng newHomeLatLng;
//        newHomeLatLng.SetLng(m_homeData.lng);
//        newHomeLatLng.SetLat(m_homeData.lat);
//        emit signal_UpdateVehicleHome(newHomeLatLng, m_homeData.alt);
//    }
}

void MapWidgetEditor::homeData_editingFinished(){
    if(m_ledt_lng->text() != QString::number(m_homeData.lng, 'f', 7)){
        m_editType = 0;
        m_isNeedUpdating = true;
        connect(m_ledt_lng, SIGNAL(textEdited(QString)), this, SLOT(homeData_edited(QString)));
        emit m_ledt_lng->textEdited(m_ledt_lng->text());
    }
    if(m_ledt_lat->text() != QString::number(m_homeData.lat, 'f', 7)){
        m_editType = 1;
        m_isNeedUpdating = true;
        connect(m_ledt_lat, SIGNAL(textEdited(QString)), this, SLOT(homeData_edited(QString)));
        emit m_ledt_lat->textEdited(m_ledt_lat->text());
    }
    if(m_ledt_alt->text() != QString::number(m_homeData.alt, 'f', 2)){
        m_editType = 2;
        m_isNeedUpdating = true;
        connect(m_ledt_alt, SIGNAL(textEdited(QString)), this, SLOT(homeData_edited(QString)));
        emit m_ledt_alt->textEdited(m_ledt_alt->text());
    }
}

void MapWidgetEditor::update_currentPos(internals::PointLatLng cpt){
//    m_mapWidget->SetCurrentPosition(cpt);
    m_ledt_timinglng->setText(QString::number(cpt.Lng(), 'f', 7));
    m_ledt_timinglat->setText(QString::number(cpt.Lat(), 'f', 7));
}

void  MapWidgetEditor::showWP(const QList<MapPointData>& dataList){
    m_mapWidget->loadLineWay(dataList, true, true);
}

void MapWidgetEditor::UpdateUAVState(internals::PointLatLng pos,double direction,int height){
    UAVItem* currentUAV=  m_mapWidget->GetUAV(0x0002);
    currentUAV->SetUAVHeading(direction);

    /*********偏移逻辑**************/
//    pos.SetLat(pos.lat+ZYGroundGlobalConfig::offset_lat);
//    pos.SetLng(pos.lng+ZYGroundGlobalConfig::offset_lon);
     /*********偏移逻辑**************/

    currentUAV->SetUAVPos(pos,height);
    if(ZYGroundGlobalConfig::setUavAsCenter==true)
    {
       m_mapWidget->SetCurrentPosition(pos);
    }
}

void MapWidgetEditor::changedMapType(int index){
    core::MapType::Types newType = (core::MapType::Types)g_arrMapType[index].typeID;
    if(m_mapType != newType){
        m_mapType = newType;
        m_mapWidget->SetMapType(m_mapType);
        m_mapWidget->ReloadMap();
    }
}

void MapWidgetEditor::generateLocalWayPointList(QList<ProtocolDataType>& dataList, const QString& strLineData){
    QString tmp = strLineData;
    QStringList spList = tmp.split(" ");
    ProtocolDataType tmpData;
    switch (spList.size()) {
    case 1:
        tmpData.x = spList[0].toDouble();
        break;
    case 2:
        tmpData.x = spList[0].toDouble();
        tmpData.y = spList[1].toDouble();
        break;
    case 3:
        tmpData.x = spList[0].toDouble();
        tmpData.y = spList[1].toDouble();
        tmpData.x = spList[2].toDouble();
        break;
    case 4:
        tmpData.x = spList[0].toDouble();
        tmpData.y = spList[1].toDouble();
        tmpData.z = spList[2].toDouble();
        tmpData.command = spList[3].toInt();
        break;
    default:
        return;
        break;
    }
    dataList.push_back(tmpData);
}

void MapWidgetEditor::setupMapType_list(){
    InitMapName();
    int     i=0;
    while(1) {
        if( g_arrMapType[i].typeID >= 0 )
            //m_cboMapType->addItem(g_arrMapType[i].name);
            m_cboMapType->addItem(m_mapName.value(i));
        else
            break;
        i ++;
    }
}

void MapWidgetEditor::setMapType(){
    int i = 0;
    while(1) {
        if( g_arrMapType[i].typeID == m_mapType) {
            m_cboMapType->setCurrentIndex(i);
            return;
        }
        if( g_arrMapType[i].typeID == -1 ) break;
        i ++;
    }

    // set default map
    m_cboMapType->setCurrentIndex(0);
}

void MapWidgetEditor::InitMapName(){
    m_mapName.insert(0, QString::fromLocal8Bit("谷歌卫星地图"));
    m_mapName.insert(1, QString::fromLocal8Bit("开放式街区地图"));
    m_mapName.insert(2, QString::fromLocal8Bit("Bing地图"));
    m_mapName.insert(3, QString::fromLocal8Bit("Bing卫星地图"));
}
double ZYGroundGlobalConfig::offset_lat=0;
double ZYGroundGlobalConfig::offset_lon=0;

void MapWidgetEditor::OffsetChanged()
{
    double xx=m_tbx_offset_Lat->text().toDouble();
    double yy=m_tbx_offset_Lon->text().toDouble();
    //this->m_mapWidget->Offset(xx,yy);
   ZYGroundGlobalConfig::offset_lon=xx;
   ZYGroundGlobalConfig::offset_lat=yy;
  this->m_mapWidget->update();
}

void MapWidgetEditor::CacheOffMap(){
    internals::RectLatLng  rect;

    rect = m_mapWidget->SelectedArea();
    if( rect.IsEmpty() ) {
        QMessageBox msgbox(QMessageBox::Warning,
                           "Warning", "Please select an area first!");
        msgbox.exec();

        return;
    }

    m_mapWidget->RipMap();
//    m_cacheMapManager = new CacheMapManager();
//    m_cacheMapManager->show();
}

void MapWidgetEditor::mapEdit_AutoPlan_slots(){
    if(!m_autoPlan){
        m_autoPlan = new AutoPlan();
        connect(m_autoPlan, SIGNAL(autoPlan_commitBoundaryData_signal(const QList<MapPointData>&)), this, SLOT(mapEdit_generateBoundary_slots(const QList<MapPointData>&)));
        connect(m_autoPlan, SIGNAL(autoPlan_updateInfoNums_siganl(const APInfo&)), this, SLOT(mapEdit_generateWayPoints_slots(const APInfo&)));
        connect(m_autoPlan, SIGNAL(autoPlan_finish_signal()), this, SLOT(mapEdit_generateTableDatas_slots()));
        connect(m_autoPlan, SIGNAL(autoPlan_clearBoundary_signal(const bool&)), this, SLOT(mapEdit_clearAutoPlanPoint_slots(const bool&)));
        connect(m_autoPlan, SIGNAL(autoPlan_isEnableClicke_signal(const bool&)), this, SLOT(mapEdit_isEnableClick_slots(const bool&)));
        connect(m_mapWidget, SIGNAL(mapWidget_addAutoPlanPoint_signal(const MapPointData&)), m_autoPlan, SLOT(autoPlan_mouseAddOnePoint_slots(const MapPointData&)));
        connect(m_autoPlan, SIGNAL(autoPlan_eraseOnePoint(const int&)), this, SLOT(mapEdit_ereseOnePoint_slots(const int&)));
        connect(m_autoPlan, SIGNAL(autoPlan_enableButtons()), this, SLOT(mapEdit_AutoPlan_enableButtons_slots()));
    }
    m_autoPlan->show();
    disableOtherBtn();
}

void MapWidgetEditor::mapEdit_AutoPlan_enableButtons_slots(){
    enableOtherBtn();
}

int map_projection_init_timestamped(struct map_projection_reference_s *ref, double lat_0, double lon_0)
{

    ref->lat_rad = lat_0 * M_DEG_TO_RAD;
    ref->lon_rad = lon_0 * M_DEG_TO_RAD;
    ref->sin_lat = sin(ref->lat_rad);
    ref->cos_lat = cos(ref->lat_rad);

    return 0;
}

 int map_projection_project(const struct map_projection_reference_s *ref, double lat, double lon, double *x,
                    double *y)
{
    double lat_rad = lat * M_DEG_TO_RAD;
    double lon_rad = lon * M_DEG_TO_RAD;

    double sin_lat = sin(lat_rad);
    double cos_lat = cos(lat_rad);
    double cos_d_lon = cos(lon_rad - ref->lon_rad);

    double c = acos(ref->sin_lat * sin_lat + ref->cos_lat * cos_lat * cos_d_lon);
    double k = (fabs(c) < DBL_EPSILON) ? 1.0 : (c / sin(c));

    *x = k * (ref->cos_lat * sin_lat - ref->sin_lat * cos_lat * cos_d_lon) * CONSTANTS_RADIUS_OF_EARTH;
    *y = k * cos_lat * sin(lon_rad - ref->lon_rad) * CONSTANTS_RADIUS_OF_EARTH;

    return 0;
}

 int map_projection_reproject(const struct map_projection_reference_s *ref, double x, double y, double *lat,
                      double *lon)
{
    double x_rad = x / CONSTANTS_RADIUS_OF_EARTH;
    double y_rad = y / CONSTANTS_RADIUS_OF_EARTH;
    double c = sqrtf(x_rad * x_rad + y_rad * y_rad);
    double sin_c = sin(c);
    double cos_c = cos(c);

    double lat_rad;
    double lon_rad;

    if (fabs(c) > DBL_EPSILON) {
        lat_rad = asin(cos_c * ref->sin_lat + (x_rad * sin_c * ref->cos_lat) / c);
        lon_rad = (ref->lon_rad + atan2(y_rad * sin_c, c * ref->cos_lat * cos_c - x_rad * ref->sin_lat * sin_c));

    } else {
        lat_rad = ref->lat_rad;
        lon_rad = ref->lon_rad;
    }

    *lat = lat_rad * 180.0 / M_PI;
    *lon = lon_rad * 180.0 / M_PI;

    return 0;
}

 void MapWidgetEditor::mapEdit_generateBoundary_slots(const QList<MapPointData>& datas){
    m_boundaryDatas.clear();
    m_boundaryDatas = datas;
    m_isReAngle = false;
    m_isFinished = false;
    m_mapWidget->map_generateBoundary(datas, false);
 }

 void MapWidgetEditor::mapEdit_generateWayPoints_slots(const APInfo& info){
     if(m_boundaryDatas.empty()){
         return;
     }
     m_autoPlanWayPointsList.clear();
     m_table_editor->showTable_clearSL_slots();
     GenerateRealPlanList(m_boundaryDatas, info, m_autoPlanWayPointsList);
     if(m_autoPlanWayPointsList.size() > 99){
         m_autoPlanWayPointsList.clear();
         QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("航路点数目过多"));
         return;
     }
     m_mapWidget->loadLineWay(m_autoPlanWayPointsList, false, true);
 }

 void MapWidgetEditor::mapEdit_generateTableDatas_slots(){
    m_isFinished = true;
    mapEdit_clearBoundary();
    m_mapWidget->map_updateBoundaryDataToTable(m_autoPlanWayPointsList);
 }

 void MapWidgetEditor::mapEdit_clearAutoPlanPoint_slots(const bool& flag){
    if(flag&&m_isFinished){
        mapEdit_clearBoundaryTable();
        m_isFinished = false;
    }else{
        mapEdit_clearBoundary();
        mapEdit_clearBoundaryPoints();
    }
}

void MapWidgetEditor::mapEdit_isEnableClick_slots(const bool& bState){
    m_mapWidget->mapWidget_setAutoPlanIsEnableClick(bState);
}

void MapWidgetEditor::mapEdit_ereseOnePoint_slots(const int& index){
    m_mapWidget->map_eraseOneBoundaryPoint(index);
}

void MapWidgetEditor::mapEdit_clearBoundary(){
    m_boundaryDatas.clear();
    m_mapWidget->map_clearBoundary();
}

void MapWidgetEditor::mapEdit_clearBoundaryPoints(){
    for(int i = 0; i < m_autoPlanWayPointsList.size(); i++){
        mapWidgetEditor_deleteOneWP(0);
    }
    m_autoPlanWayPointsList.clear();
}

void MapWidgetEditor::mapEdit_clearBoundaryTable(){
    for(int i = 0; i < m_autoPlanWayPointsList.size(); i++){
        m_table_editor->showTable_deleteTheRow(0);
    }
    mapEdit_clearBoundary();
    mapEdit_clearBoundaryPoints();
}

void initOneWPD(MapPointData& dst){
    dst.lng =0;
    dst.lat = 0;
    dst.alt = 15;
}

void MapWidgetEditor::GenerateRealPlanList(const QList<MapPointData>& datas, const APInfo& info, QList<MapPointData>& dataList){
    struct map_projection_reference_s* originLatLng = new map_projection_reference_s;
    double angle = info.strAngle.toDouble()*M_DEG_TO_RAD;

    QList<COPointData> COlist, PlanCOlist;
    COPointData homeCoordinate;
    InitOriginOfCoor(datas, originLatLng);
    LngLatToCoordinate(originLatLng, angle, m_homeData.lng, m_homeData.lat, homeCoordinate.x, homeCoordinate.y);
    for(QList<MapPointData>::const_iterator iter = datas.begin(); iter != datas.end(); iter++){
        COPointData tmp;
        LngLatToCoordinate(originLatLng, angle, (*iter).lng, (*iter).lat, tmp.x, tmp.y);
        COlist.push_back(tmp);
    }
    if(!reenterAngle(COlist, m_reAngleIndex, m_isReAngle)) {
        return;
    }
    generateCOPlanList(COlist, homeCoordinate, info.strVD.toDouble(), info.strHD.toDouble(), PlanCOlist);
    //generateCOPlanList(COlist, info.strVD.toDouble(), info.strHD.toDouble(), PlanCOlist);
    for(QList<COPointData>::iterator iter = PlanCOlist.begin(); iter != PlanCOlist.end(); iter++){
        MapPointData tmp;
        CoordinateToLngLat(originLatLng, angle, (*iter).x, (*iter).y, tmp.lng, tmp.lat);
        dataList.push_back(tmp);
    }
}

void MapWidgetEditor::InitOriginOfCoor(const QList<MapPointData>& datas, struct map_projection_reference_s* ref){
    double tmpLng,tmpLat;
    tmpLng = 180;
    tmpLat = 90;
    for(QList<MapPointData>::const_iterator iter = datas.begin(); iter != datas.end(); iter++){
        if((*iter).lng < tmpLng){
            tmpLng = (*iter).lng;
        }
        if((*iter).lat < tmpLat){
            tmpLat = (*iter).lat;
        }
    }
    map_projection_init_timestamped(ref, tmpLat, tmpLng);
}

int MapWidgetEditor::LngLatToCoordinate(struct map_projection_reference_s* ref, const double& angle, \
                                        const double& lng, const double& lat, double& x, double& y){
    double tmpx, tmpy;
    map_projection_project(ref, lat, lng, &x, &y);
    tmpx = x*qCos(angle)+y*qCos(angle+M_PI/2);
    tmpy = x*qCos(M_PI/2-angle)+y*qCos(angle);
    x = tmpx;
    y = tmpy;
    return true;
}

int MapWidgetEditor::generateCOPlanList(const QList<COPointData>& srcList, const double& disx, const double& disy, QList<COPointData>& dstList){
    double sty, endy;
    getYSection(srcList, sty, endy);
    //dstList = srcList;
    double tmpy = sty;
    while(tmpy < endy){
//        for(QList<COPointData>::const_iterator iter = srcList.begin(); iter != srcList.end(); iter++){
//            if(tmpy >= (*iter).y&&(!isNew(dstList, *iter))){
//                dstList.push_back(*iter);
//            }
//        }
        generatePointsOfLineY(srcList, tmpy, disx, dstList);
        if(dstList.size() >=100){
            QMessageBox::warning(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("航路点超过100个"), QMessageBox::Yes);
            dstList.clear();
            break;
        }
        tmpy += disy;
    }
    return true;
}

COPointData MapWidgetEditor::getMaxXPoint(QList<COPointData>& srcList){
    double tmpx = srcList[0].x;
    int index = 0;
    for(int i = 1; i < srcList.size(); i++){
        if(tmpx < srcList[i].x){
            tmpx = srcList[i].x;
            index = i;
        }
    }
    COPointData tmpPoint = srcList[index];
    srcList.removeAt(index);
    return tmpPoint;
}

int MapWidgetEditor::generateCOPlanList(const QList<COPointData>& srcList, const COPointData& homeCO, \
                                        const double& disx, const double& disy,  QList<COPointData>& dstList){
    //generate line list
    double sty, endy;
    QList<COLineData>   sumLines;
    getYSection(srcList, sty, endy);
    double tmpy = sty;
    while(tmpy < endy){
        QList<COPointData> tmpList;
        for(int i=0; i< srcList.size(); i++){
            int is, id;
            if(i == srcList.size()-1){
                is = i;
                id = 0;
            }else{
                is = i;
                id = i+1;
            }
            double st, end;
            if(srcList[is].y > srcList[id].y){
                st = srcList[id].y;
                end = srcList[is].y;
            }else{
                st = srcList[is].y;
                end = srcList[id].y;
            }
            COPointData tmpP;
            if(LineIntersection(srcList[is], srcList[id], tmpy, tmpP)){
                if(!isNew(tmpList, tmpP)){
                    tmpList.push_back(tmpP);
                }
            }
        }
        if(tmpList.size() == 1){
            COLineData tmpLine;
            tmpLine.nNum = 1;
            tmpLine.pointUp = tmpList[0];
            tmpLine.isStartUp = true;
            sumLines.push_back(tmpLine);
        }else if(tmpList.size() == 2){
            COLineData tmpLine;
            tmpLine.nNum = 2;
            tmpLine.pointUp = getMaxXPoint(tmpList);
            tmpLine.pointDown = getMaxXPoint(tmpList);
            sumLines.push_back(tmpLine);
        }else if(tmpList.size() == 3){
            COLineData tmpLine;
            tmpLine.nNum = 2;
            tmpLine.pointUp = getMaxXPoint(tmpList);
            getMaxXPoint(tmpList);
            tmpLine.pointDown   = getMaxXPoint(tmpList);
            sumLines.push_back(tmpLine);
        }else if(tmpList.size() == 4){
            COLineData tmpLine, tmpLine1;
            tmpLine.nNum = 2;
            tmpLine.pointUp = getMaxXPoint(tmpList);
            tmpLine.pointDown   = getMaxXPoint(tmpList);
            sumLines.push_back(tmpLine);

            tmpLine1.nNum = 2;
            tmpLine1.pointUp = getMaxXPoint(tmpList);
            tmpLine1.pointDown   = getMaxXPoint(tmpList);
            sumLines.push_back(tmpLine1);
        }
        tmpy += disy;
    }
    //check list for min distance to home to start
    COPointData stPoint = homeCO;
    bool isFirst = true;
    while(!sumLines.empty()){
        //get minDistance point
        int index = 0;
        sumLines[index].isStartUp = true;
        double minDistance;
        if(isFirst){
            minDistance = DistanceOfTwoPoints(srcList, stPoint, sumLines[index].pointUp);
            isFirst = false;
        }else{
            minDistance = DistanceOfTwoPoints(srcList, stPoint, sumLines[index].pointUp, sumLines[index].isCrossUp);
        }
        for(int i = 0; i < sumLines.size(); i++){
            if(i!=0){
                double tmpDistance = DistanceOfTwoPoints(srcList, stPoint, sumLines[i].pointUp, sumLines[i].isCrossUp);
                if(minDistance > tmpDistance){
                    index = i;
                    sumLines[i].isStartUp = true;
                    minDistance = tmpDistance;
                }
            }
            if(sumLines[i].nNum == 2){
                double tmpDistance = DistanceOfTwoPoints(srcList, stPoint, sumLines[i].pointDown, sumLines[i].isCrossDown);
                if(minDistance > tmpDistance){
                    index = i;
                    sumLines[i].isStartUp = false;
                    minDistance = tmpDistance;
                }
            }
        }
        //generate waypoint
        {
            if(sumLines[index].isStartUp == true){
                if(sumLines[index].nNum == 1){
                    if(sumLines[index].isCrossUp){
                        dstList.push_back(srcList[m_reAngleIndex]);
                    }
                    if(!isNew(dstList, sumLines[index].pointUp)){
                        dstList.push_back(sumLines[index].pointUp);
                    }
                }else{
                    if(sumLines[index].isCrossUp){
                        dstList.push_back(srcList[m_reAngleIndex]);
                    }
                    if(!isNew(dstList, sumLines[index].pointUp)){
                        dstList.push_back(sumLines[index].pointUp);
                    }
                    double tmpx = sumLines[index].pointUp.x;
                    while(tmpx > sumLines[index].pointDown.x){
                        COPointData tmp;
                        tmp.x = tmpx;
                        tmp.y = sumLines[index].pointDown.y;
                        if(!isNew(dstList, tmp)){
                            dstList.push_back(tmp);
                        }
                        tmpx -= disx;
                    }
                    if(!isNew(dstList, sumLines[index].pointDown)){
                        dstList.push_back(sumLines[index].pointDown);
                    }
                }
            }else if(sumLines[index].isStartUp == false){
                if(sumLines[index].isCrossDown){
                    dstList.push_back(srcList[m_reAngleIndex]);
                }
                if(!isNew(dstList, sumLines[index].pointDown)){
                    dstList.push_back(sumLines[index].pointDown);
                }
                double tmpx = sumLines[index].pointDown.x;
                while(tmpx < sumLines[index].pointUp.x){
                    COPointData tmp;
                    tmp.x = tmpx;
                    tmp.y = sumLines[index].pointDown.y;
                    if(!isNew(dstList, tmp)){
                        dstList.push_back(tmp);
                    }
                    tmpx += disx;
                }
                if(!isNew(dstList, sumLines[index].pointUp)){
                    dstList.push_back(sumLines[index].pointUp);
                }
            }
        }
        stPoint = dstList.last();
        sumLines.removeAt(index);
    }//while(!sumLines.empty())
    return true;
}

int MapWidgetEditor::CoordinateToLngLat(struct map_projection_reference_s* ref, const double& angle, const double& x, const double& y, double& lng, double& lat){
    double tmpx, tmpy, dx, dy;
    tmpx = x*qCos(angle)+y*qCos(M_PI/2-angle);
    tmpy = x*qCos(angle+M_PI/2)+y*qCos(angle);
    dx = tmpx;
    dy = tmpy;
//    dx = x;
//    dy = y;
    map_projection_reproject(ref, dx, dy, &lat, &lng);
    return true;
}

bool MapWidgetEditor::isNew(const QList<COPointData>& srcList, const COPointData& item){
    if(srcList.size() >=100)
        return false;
    for(QList<COPointData>::const_iterator iter = srcList.begin(); iter != srcList.end(); iter++){
        if(QString::number(item.x, 'g', 7).toDouble() == QString::number((*iter).x, 'g', 7).toDouble()&&\
            QString::number(item.y, 'g', 7).toDouble() == QString::number((*iter).y, 'g', 7).toDouble()){
            return true;
        }
//        if( item.x == (*iter).x && item.y == (*iter).y){
//            return true;
//        }
    }
    return false;
}

bool MapWidgetEditor::LineIntersection(const COPointData& stPoint, const COPointData& endPoint, const double& y, COPointData& point){
    if(stPoint.x < endPoint.x){
        double tmpx = ((endPoint.x - stPoint.x)/(endPoint.y - stPoint.y))*(y-stPoint.y) + stPoint.x;
        if(stPoint.x <= tmpx && tmpx <= endPoint.x){
            point.y = y;
            point.x = tmpx;
            return true;
        }
    }else{
        double tmpx = ((endPoint.x - stPoint.x)/(endPoint.y - stPoint.y))*(y-stPoint.y) + stPoint.x;
        if(endPoint.x <= tmpx && tmpx <= stPoint.x){
            point.y = y;
            point.x = tmpx;
            return true;
        }
    }
    return false;
}

void MinDistancOfTwoCOPoints(const QList<COPointData>& srcList, const COPointData& srcPoint, bool& minIndex){
    double tmpDistance = 0;
    tmpDistance = (srcList[0].x-srcPoint.x)*(srcList[0].x-srcPoint.x) + (srcList[0].y-srcPoint.y)*(srcList[0].y-srcPoint.y);
    if(tmpDistance <= (srcList[1].x-srcPoint.x)*(srcList[1].x-srcPoint.x) + (srcList[1].y-srcPoint.y)*(srcList[1].y-srcPoint.y)){
        minIndex = 0;
    }else{
        minIndex = 1;
    }
}

int MapWidgetEditor::generatePointsOfLineY(const QList<COPointData>& srcList, const double& y, const double& dis, QList<COPointData>& dstList){
    QList<COPointData> tmpList;
    for(int i=0; i< srcList.size(); i++){
        int is, id;
        if(i == srcList.size()-1){
            is = i;
            id = 0;
        }else{
            is = i;
            id = i+1;
        }
        double st, end;
        if(srcList[is].y > srcList[id].y){
            st = srcList[id].y;
            end = srcList[is].y;
        }else{
            st = srcList[is].y;
            end = srcList[id].y;
        }
        COPointData tmpP;
        if(LineIntersection(srcList[is], srcList[id], y, tmpP)){
            if(!isNew(tmpList, tmpP)){
                tmpList.push_back(tmpP);
            }
        }
    }
    if(tmpList.size() == 1){
        if(!isNew(dstList, tmpList[0])){
            dstList.push_back(tmpList[0]);
        }
    }else if(tmpList.size() == 2){
        bool minFlag = false;
        MinDistancOfTwoCOPoints(tmpList, dstList[dstList.size()-1], minFlag);
        if(!minFlag){
            if(!isNew(dstList, tmpList[0])){
                dstList.push_back(tmpList[0]);
            }
            double tmpx = tmpList[0].x;
            if(tmpList[0].x < tmpList[1].x){
                while(tmpx < tmpList[1].x){
                    COPointData tmp;
                    tmp.x = tmpx;
                    tmp.y = y;
                    if(!isNew(dstList, tmp)){
                        dstList.push_back(tmp);
                    }
                    tmpx += dis;
                }

            }else{
                while(tmpx > tmpList[1].x){
                    COPointData tmp;
                    tmp.x = tmpx;
                    tmp.y = y;
                    if(!isNew(dstList, tmp)){
                        dstList.push_back(tmp);
                    }
                    tmpx -= dis;
                }
            }
            if(!isNew(dstList, tmpList[1])){
                dstList.push_back(tmpList[1]);
            }
        }else{
            if(!isNew(dstList, tmpList[1])){
                dstList.push_back(tmpList[1]);
            }
            double tmpx = tmpList[1].x;
            if(tmpList[0].x < tmpList[1].x){
                while(tmpx > tmpList[0].x){
                    COPointData tmp;
                    tmp.x = tmpx;
                    tmp.y = y;
                    if(!isNew(dstList, tmp)){
                        dstList.push_back(tmp);
                    }
                    tmpx -= dis;
                }

            }else{
                while(tmpx < tmpList[0].x){
                    COPointData tmp;
                    tmp.x = tmpx;
                    tmp.y = y;
                    if(!isNew(dstList, tmp)){
                        dstList.push_back(tmp);
                    }
                    tmpx += dis;
                }
            }
            if(!isNew(dstList, tmpList[0])){
                dstList.push_back(tmpList[0]);
            }
        }
    }
    return true;
}

int MapWidgetEditor::getYSection(const QList<COPointData>& srcList, double& st, double& end){
    double tmps, tmpe;
    tmps = tmpe = srcList[0].y;
    for(QList<COPointData>::const_iterator iter = srcList.begin(); iter != srcList.end(); iter++){
        if((*iter).y > tmpe){
            tmpe = (*iter).y;
        }
        if((*iter).y < tmps){
            tmps = (*iter).y;
        }
    }
    st = tmps;
    end = tmpe;
    return true;
}

bool MapWidgetEditor::reenterAngle(const QList<COPointData>& srcList, int& reAngleIndex, bool& isReAngle){
    int st, mid, end;
    int lowIndex, upIndex;
    int lowNum, upNum;
    lowNum = upNum = 0;
    for(int i = 0; i < srcList.size(); i++){
        if(i == 0){
            mid = 0;
            st = srcList.size()-1;
            end = 1;
        }else if(i == srcList.size()-1){
            mid = srcList.size() -1;
            st = mid -1;
            end = 0;
        }else{
            mid = i;
            st = mid -1;
            end = mid+1;
        }
        double dFlag = (srcList[mid].x - srcList[st].x)*(srcList[end].y - srcList[st].y)\
                - (srcList[mid].y - srcList[st].y)*(srcList[end].x - srcList[st].x);
        if(dFlag < 0){
            lowIndex = mid;
            lowNum++;
        }else{
            upIndex = mid;
            upNum++;
        }
    }
    if(lowNum == 0 || upNum == 0){
        isReAngle = false;
        return true;
    }else if(lowNum == 1){
        reAngleIndex = lowIndex;
        isReAngle = true;
        return true;
    }else if(upNum == 1){
        reAngleIndex = upIndex;
        isReAngle = true;
        return true;
    }
    return false;
}

double determinant(double v1, double v2, double v3, double v4)  // 行列式
{
    return (v1*v4-v2*v3);
}

bool intersect3(COPointData st1, COPointData end1, COPointData st2, COPointData end2)
{
    if(st1 == st2 || st1 == end2 || end1 == st2 || end1 == end2){
        return true;
    }
    double delta = determinant(end1.x-st1.x, st2.x-end2.x, end1.y-st1.y, st2.y-end2.y);
    if ( delta<=(1e-6) && delta>=-(1e-6) )  // delta=0，表示两线段重合或平行
    {
        return false;
    }
    double namenda = determinant(st2.x-st1.x, st2.x-end2.x, st2.y-st1.y, st2.y-end2.y) / delta;
    if ( namenda-1>(1e-6) || namenda<-(1e-6) )
    {
        return false;
    }
    double miu = determinant(end1.x-st1.x, st2.x-st1.x, end1.y-st1.y, st2.y-st1.y) / delta;
    if ( miu-1>(1e-6) || miu<-(1e-6) )
    {
        return false;
    }
    return true;
}

double MapWidgetEditor::DistanceOfTwoPoints(const QList<COPointData>& srcList, COPointData& stPoint, COPointData& endPoint, bool& isCrossReAngle){
    isCrossReAngle = false;
    if(!m_isReAngle){
        return (stPoint.x - endPoint.x)*(stPoint.x - endPoint.x) + (stPoint.y - endPoint.y)*(stPoint.y - endPoint.y);
    }else{
        int st, mid, end;
        if(m_reAngleIndex == 0){
            mid = 0;
            st = srcList.size()-1;
            end = 1;
        }else if(m_reAngleIndex == srcList.size()-1){
            mid = srcList.size() -1;
            st = mid -1;
            end = 0;
        }else{
            mid = m_reAngleIndex;
            st = mid -1;
            end = mid+1;
        }
        if(intersect3(stPoint, endPoint, srcList[st], srcList[mid]) && intersect3(stPoint, endPoint, srcList[mid], srcList[end])){
            isCrossReAngle = true;
            return (stPoint.x - srcList[m_reAngleIndex].x)*(stPoint.x - srcList[m_reAngleIndex].x) + \
                    (stPoint.y - srcList[m_reAngleIndex].y)*(stPoint.y - srcList[m_reAngleIndex].y) + \
                    (srcList[m_reAngleIndex].x - endPoint.x)*(srcList[m_reAngleIndex].x - endPoint.x) + \
                    (srcList[m_reAngleIndex].y - endPoint.y)*(srcList[m_reAngleIndex].y - endPoint.y);
        }else{
            return (stPoint.x - endPoint.x)*(stPoint.x - endPoint.x) + (stPoint.y - endPoint.y)*(stPoint.y - endPoint.y);
        }
    }
}

double MapWidgetEditor::DistanceOfTwoPoints(const QList<COPointData>& srcList, COPointData& stPoint, COPointData& endPoint){
    if(!m_isReAngle){
        return (stPoint.x - endPoint.x)*(stPoint.x - endPoint.x) + (stPoint.y - endPoint.y)*(stPoint.y - endPoint.y);
    }else{
        int st, mid, end;
        if(m_reAngleIndex == 0){
            mid = 0;
            st = srcList.size()-1;
            end = 1;
        }else if(m_reAngleIndex == srcList.size()-1){
            mid = srcList.size() -1;
            st = mid -1;
            end = 0;
        }else{
            mid = m_reAngleIndex;
            st = mid -1;
            end = mid+1;
        }
        if(intersect3(stPoint, endPoint, srcList[st], srcList[mid]) && intersect3(stPoint, endPoint, srcList[mid], srcList[end])){
            return (stPoint.x - srcList[m_reAngleIndex].x)*(stPoint.x - srcList[m_reAngleIndex].x) + \
                    (stPoint.y - srcList[m_reAngleIndex].y)*(stPoint.y - srcList[m_reAngleIndex].y) + \
                    (srcList[m_reAngleIndex].x - endPoint.x)*(srcList[m_reAngleIndex].x - endPoint.x) + \
                    (srcList[m_reAngleIndex].y - endPoint.y)*(srcList[m_reAngleIndex].y - endPoint.y);
        }else{
            return (stPoint.x - endPoint.x)*(stPoint.x - endPoint.x) + (stPoint.y - endPoint.y)*(stPoint.y - endPoint.y);
        }
    }
}

int MapWidgetEditor::disableOtherBtn(){
    bool flag = true;
    m_cbAirLine->setDisabled(flag);
    m_btn_upload->setDisabled(flag);
    m_btn_download->setDisabled(flag);
    m_btn_savelocal->setDisabled(flag);
    m_btn_openlocal->setDisabled(flag);
    m_btn_downloadMap->setDisabled(flag);
    return true;
}
int MapWidgetEditor::enableOtherBtn(){
    bool flag = false;
    m_cbAirLine->setDisabled(flag);
    m_btn_upload->setDisabled(flag);
    m_btn_download->setDisabled(flag);
    m_btn_savelocal->setDisabled(flag);
    m_btn_openlocal->setDisabled(flag);
    m_btn_downloadMap->setDisabled(flag);
    return true;
}
