#include "showcurrentwaypoints.h"
#include <QtAlgorithms>
ShowCurrentWayPoints::ShowCurrentWayPoints(QWidget *parent) :
    QTableWidget(parent)
{
   // setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint );
    this->setColumnCount(12);
    QStringList nameList;
    nameList << "指令" << "参数1" << "参数2" << "参数3" << "参数4" << "经度" << "纬度" << "高度" << "删除" << "上移" << "下移" << "距离(m)";
    this->setHorizontalHeaderLabels(nameList);
    //this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->resizeColumnsToContents();
    setEditTriggers(QAbstractItemView::DoubleClicked);
    m_rowIndex = 0;
    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(showTable_cellDoubleKK_slots(int,int)));
    connect(this, SIGNAL(cellClicked(int, int)), this, SLOT(showTable_cellKK_slots(int,int)));
}

ShowCurrentWayPoints::~ShowCurrentWayPoints()
{

}

int ShowCurrentWayPoints::showTable_init(const QList<MapPointData>& dataList){
    if(dataList.isEmpty())
        return false;
    for(QList<MapPointData>::const_iterator iter = dataList.begin(); iter != dataList.end(); iter++){
       showTable_inertOneDataItem(*iter);
    }
    return true;
}

int ShowCurrentWayPoints::showTable_inertOneDataItem(const MapPointData& OneWPData){
    ShowListItemData newShowItem;
    newShowItem.dParam1 = 0;
    newShowItem.dParam2 = 0;
    newShowItem.dParam3 = 0;
    newShowItem.dParam4 = 0;
    newShowItem.cCmdCbo = new wpcmdCbo();

    newShowItem.cDelBtn = new WPDeleteBtn();
    newShowItem.cDelBtn->setRow(m_rowIndex);
    connect(newShowItem.cDelBtn, SIGNAL(signal_deleteClicked(int)), this, SLOT(showTable_deleteTheRow_slots(int)));

    newShowItem.cUpBtn = new wpmvbtn(MV_UP);
    newShowItem.cUpBtn->setRow(m_rowIndex);
    connect(newShowItem.cUpBtn, SIGNAL(signal_up(int)), this, SLOT(showTable_upTheRow_slots(int)));

    newShowItem.cDownBtn = new wpmvbtn(MV_DOWN);
    newShowItem.cDownBtn->setRow(m_rowIndex);
    connect(newShowItem.cDownBtn, SIGNAL(signal_down(int)), this, SLOT(showTable_downTheRow_slots(int)));

    newShowItem.mpd = OneWPData;

    this->insertRow(m_rowIndex);
    this->setCellWidget(m_rowIndex, 0, newShowItem.cCmdCbo);
    this->setItem(m_rowIndex, 1, new QTableWidgetItem(QString::number(newShowItem.dParam1, 'f', 7)));
    this->setItem(m_rowIndex, 2, new QTableWidgetItem(QString::number(newShowItem.dParam2, 'f', 7)));
    this->setItem(m_rowIndex, 3, new QTableWidgetItem(QString::number(newShowItem.dParam3, 'f', 7)));
    this->setItem(m_rowIndex, 4, new QTableWidgetItem(QString::number(newShowItem.dParam4, 'f', 7)));
    this->setItem(m_rowIndex, 5,  new QTableWidgetItem(QString::number(newShowItem.mpd.lng, 'f', 7)));
    this->setItem(m_rowIndex, 6,  new QTableWidgetItem(QString::number(newShowItem.mpd.lat, 'f', 7)));
    this->setItem(m_rowIndex, 7,  new QTableWidgetItem(QString::number(newShowItem.mpd.alt, 'f', 2)));
    this->setCellWidget(m_rowIndex, 8, newShowItem.cDelBtn);
    this->setCellWidget(m_rowIndex, 9, newShowItem.cUpBtn);
    this->setCellWidget(m_rowIndex, 10, newShowItem.cDownBtn);
    if(m_rowIndex == 0){
        this->setItem(m_rowIndex, 11, new QTableWidgetItem(QString::number(showTable_distanceOfTwoWP(\
        m_homeLng, m_homeLat,newShowItem.mpd.lng, newShowItem.mpd.lat))));
    }else{
        this->setItem(m_rowIndex, 11, new QTableWidgetItem(QString::number(showTable_distanceOfTwoWP(\
            m_showListItems[m_rowIndex-1].mpd.lng, m_showListItems[m_rowIndex-1].mpd.lat,
            newShowItem.mpd.lng, newShowItem.mpd.lat))));
    }
    this->setCurrentCell(m_rowIndex, 0);
    this->resizeColumnsToContents();
    m_showListItems.push_back(newShowItem);
    m_rowIndex++;
    return true;
}

void ShowCurrentWayPoints::showTable_getDatas(QList<ProtocolDataType>& listdata)
{
     QList<ShowListItemData>::ConstIterator cur_data= m_showListItems.begin();
     for(;cur_data!=m_showListItems.end();cur_data++)
     {
        ProtocolDataType t_data;
        t_data.x = (*cur_data).mpd.lat;
        t_data.y = (*cur_data).mpd.lng;
        t_data.z = (*cur_data).mpd.alt;
        t_data.command = (*cur_data).cCmdCbo->getCurrentIndex();
        listdata.append(t_data);
     }
}

void ShowCurrentWayPoints::showTable_SetHome(double lng, double lat, double height){
    m_homeLng = lng;
    m_homeLat = lat;
    m_homeHeight = height;
    if(m_showListItems.size() > 0){
        showTable_UpdateDistance(0);
    }
}

void ShowCurrentWayPoints::showTable_deleteTheRow(const int& row){
    this->removeRow(row);
    m_showListItems.removeAt(row);
    showTable_delBtnRowSubtract(row);
    m_rowIndex--;
}

void ShowCurrentWayPoints::showTable_delBtnRowAdd(){
    for(QList<ShowListItemData>::iterator iter = m_showListItems.begin(); iter != m_showListItems.end(); iter++){
        (*iter).cDelBtn->addRow();
        (*iter).cUpBtn->addRow();
        (*iter).cDownBtn->addRow();
    }
}

void ShowCurrentWayPoints::showTable_delBtnRowSubtract(int row){
    int st = row;
    for(QList<ShowListItemData>::iterator iter = m_showListItems.begin(); iter != m_showListItems.end(); iter++, st--){
        if(st <= 0){
            (*iter).cDelBtn->subtractRow();
            (*iter).cUpBtn->subtractRow();
            (*iter).cDownBtn->subtractRow();
        }
    }
}

void ShowCurrentWayPoints::showTable_moveTheDataList(bool flag, int row){
    if(0 == flag){//up
        ShowListItemData tmpData = m_showListItems.takeAt(row);
        m_showListItems.insert(row-1, tmpData);
        //m_showListItems[row].cCmdCbo->setRow(row);
        m_showListItems[row].cDelBtn->setRow(row);
        m_showListItems[row].cUpBtn->setRow(row);
        m_showListItems[row].cDownBtn->setRow(row);

    }else if (1 == flag){//down
        ShowListItemData tmpData = m_showListItems.takeAt(row);
        m_showListItems.insert(row+1, tmpData);
        //m_showListItems[row].cCmdCbo->setRow(row);
        m_showListItems[row].cDelBtn->setRow(row);
        m_showListItems[row].cUpBtn->setRow(row);
        m_showListItems[row].cDownBtn->setRow(row);;
    }
}

void ShowCurrentWayPoints::showTable_moveTheRow(bool flag, int row){
    if(0 == flag){//up
        m_showListItems[row].ncmdType = m_showListItems[row].cCmdCbo->getCurrentIndex();
        this->removeRow(row);
        showTable_insetTheRow(row-1, m_showListItems[row]);
    }else if (1 == flag){//down
        m_showListItems[row].ncmdType = m_showListItems[row].cCmdCbo->getCurrentIndex();
        this->removeRow(row);
        showTable_insetTheRow(row+1, m_showListItems[row]);
    }
}

void ShowCurrentWayPoints::showTable_updateDistance(bool flag, int row){
    if(0 == flag){//up
        if(row==1){
            this->item(row-1, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
                m_homeLng, m_homeLat, m_showListItems[row-1].mpd.lng, \
                m_showListItems[row-1].mpd.lat)));
        }else{
            this->item(row-1, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
                m_showListItems[row-2].mpd.lng, m_showListItems[row-2].mpd.lat,\
                m_showListItems[row-1].mpd.lng, m_showListItems[row-1].mpd.lat)));
        }
        this->item(row, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
            m_showListItems[row-1].mpd.lng, m_showListItems[row-1].mpd.lat,\
            m_showListItems[row].mpd.lng, m_showListItems[row].mpd.lat)));
        if(row+1<m_rowIndex){
            this->item(row+1, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
                m_showListItems[row].mpd.lng, m_showListItems[row].mpd.lat,\
                m_showListItems[row+1].mpd.lng, m_showListItems[row+1].mpd.lat)));
        }
    }else if (1 == flag){//down
        if(row == 0){
            this->item(row, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
                m_homeLng, m_homeLat, m_showListItems[row].mpd.lng, \
                m_showListItems[row].mpd.lat)));
        }else{
            this->item(row, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
                m_showListItems[row-1].mpd.lng, m_showListItems[row-1].mpd.lat,\
                m_showListItems[row].mpd.lng, m_showListItems[row].mpd.lat)));
        }
        this->item(row+1, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
            m_showListItems[row].mpd.lng, m_showListItems[row].mpd.lat,\
            m_showListItems[row+1].mpd.lng, m_showListItems[row+1].mpd.lat)));
        if(row+2 < m_rowIndex){
            this->item(row+2, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
                m_showListItems[row+1].mpd.lng, m_showListItems[row+1].mpd.lat,\
                m_showListItems[row+2].mpd.lng, m_showListItems[row+2].mpd.lat)));
        }
    }
    this->resizeColumnsToContents();
}

void ShowCurrentWayPoints::showTable_insetTheRow(const int& row, ShowListItemData& data){
    data.cCmdCbo = new wpcmdCbo();
    data.cCmdCbo->SetNowIndex(data.ncmdType);

    data.cDelBtn  = new WPDeleteBtn();
    data.cDelBtn->setRow(row);
    connect(data.cDelBtn, SIGNAL(signal_deleteClicked(int)), this, SLOT(showTable_deleteTheRow_slots(int)));

    data.cUpBtn = new wpmvbtn(MV_UP);
    data.cUpBtn->setRow(row);
    connect(data.cUpBtn, SIGNAL(signal_up(int)), this, SLOT(showTable_upTheRow_slots(int)));

    data.cDownBtn = new wpmvbtn(MV_DOWN);
    data.cDownBtn->setRow(row);
    connect(data.cDownBtn, SIGNAL(signal_down(int)), this, SLOT(showTable_downTheRow_slots(int)));

    this->insertRow(row);
    this->setCellWidget(row, 0, data.cCmdCbo);
    this->setItem(row, 1,  new QTableWidgetItem(QString::number(data.dParam1, 'f', 7)));
    this->setItem(row, 2,  new QTableWidgetItem(QString::number(data.dParam2, 'f', 7)));
    this->setItem(row, 3,  new QTableWidgetItem(QString::number(data.dParam3, 'f', 7)));
    this->setItem(row, 4,  new QTableWidgetItem(QString::number(data.dParam4, 'f', 7)));
    this->setItem(row, 5,  new QTableWidgetItem(QString::number(data.mpd.lng, 'f', 7)));
    this->setItem(row, 6,  new QTableWidgetItem(QString::number(data.mpd.lat, 'f', 7)));
    this->setItem(row, 7,  new QTableWidgetItem(QString::number(data.mpd.alt, 'f', 2)));
    this->setCellWidget(row, 8,  data.cDelBtn);
    this->setCellWidget(row, 9,  data.cUpBtn);
    this->setCellWidget(row, 10, data.cDownBtn);
    this->setItem(row, 11, new QTableWidgetItem(QString::number(0)));
    this->resizeColumnsToContents();
}

void ShowCurrentWayPoints::showTable_UpdateDistance(int row)
{
    if(row == 0){
        this->item(row, 11)->setText(QString::number(showTable_distanceOfTwoWP(m_homeLng, \
            m_homeLat,m_showListItems[row].mpd.lng, m_showListItems[row].mpd.lat)));
    }
    if(row>0){
        this->item(row, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
            m_showListItems[row-1].mpd.lng, m_showListItems[row-1].mpd.lat,\
            m_showListItems[row].mpd.lng, m_showListItems[row].mpd.lat)));
    }
    if(row+1 < m_rowIndex){
        this->item(row+1, 11)->setText(QString::number(showTable_distanceOfTwoWP(\
            m_showListItems[row].mpd.lng, m_showListItems[row].mpd.lat,\
            m_showListItems[row+1].mpd.lng, m_showListItems[row+1].mpd.lat)));
    }
    this->resizeColumnsToContents();
}

void ShowCurrentWayPoints::showTable_deleteTheRow_slots(int row){
    this->removeRow(row);
    m_showListItems.removeAt(row);
    showTable_delBtnRowSubtract(row);
    m_rowIndex--;
    emit signal_SCDeleteOneWP(row);
}

void ShowCurrentWayPoints::showTable_upTheRow_slots(int row){
    if(0 == row)
        return;
    showTable_moveTheRow(0, row);
    showTable_moveTheDataList(0, row);
    showTable_updateDistance(0, row);
    emit signal_SCUpMoveOneWp(row);
}

void ShowCurrentWayPoints::showTable_downTheRow_slots(int row){
    if(m_rowIndex == row+1)
        return;
    showTable_moveTheRow(1, row);
    showTable_moveTheDataList(1, row);
    showTable_updateDistance(1, row);
    emit signal_SCDownMoveOneWp(row);
}

void ShowCurrentWayPoints::showTable_cellDoubleKK_slots(int row, int column){
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(showTable_cellValueChanged_slots(int,int)));
}

void ShowCurrentWayPoints::showTable_cellKK_slots(int, int){

}

void ShowCurrentWayPoints::showTable_cellValueChanged_slots(int row, int column){

    switch (column) {
    case 5:
        m_showListItems[row].mpd.lng = this->item(row, column)->text().toDouble();
        break;
    case 6:
        m_showListItems[row].mpd.lat = this->item(row, column)->text().toDouble();
        break;
    case 7:
        m_showListItems[row].mpd.alt = this->item(row, column)->text().toDouble();
        break;
    default:
        break;
    }
    showTable_UpdateDistance(row);
    emit signal_SCUpdateOneWP(row, m_showListItems[row].mpd);
    disconnect(this, SIGNAL(cellChanged(int,int)), this, SLOT(showTable_cellValueChanged_slots(int,int)));
}

int  ShowCurrentWayPoints::showTable_clearSL_slots(){
    int nRow = this->rowCount();
    for(int n = 0; n < nRow; n++){
        this->removeRow(0);
    }
    m_showListItems.clear();
    m_rowIndex = 0;
    return true;
}

int ShowCurrentWayPoints::showTable_updateShowList_slots(const QList<ProtocolDataType> & data)
{
    showTable_clearSL_slots();
    QList<MapPointData> tmpMapPoints;
    for(QList<ProtocolDataType>::const_iterator iter = data.begin(); iter != data.end(); iter++, m_rowIndex++){
        ShowListItemData tmpData;
        tmpData.mpd.lat = (*iter).x;
        tmpData.mpd.lng = (*iter).y;
        tmpData.mpd.alt = (*iter).z;
        tmpData.dParam1 = 0;
        tmpData.dParam2 = 0;
        tmpData.dParam3 = 0;
        tmpData.dParam4 = 0;
        tmpData.ncmdType = (*iter).command;
        showTable_insetTheRow(m_rowIndex, tmpData);
        m_showListItems.push_back(tmpData);
        tmpMapPoints.push_back(tmpData.mpd);
    }
    emit siganle_UpdateMapPointList(tmpMapPoints);
    return true;
}

int  ShowCurrentWayPoints::showTable_distanceOfTwoWP(double sLng, double sLat, double dLng, double dLat){
    internals::PointLatLng p1, p2;
    p1.SetLng(sLng);
    p1.SetLat(sLat);
    p2.SetLng(dLng);
    p2.SetLat(dLat);
    double EarthRadiusKm = 6378.137;
    double dLat1InRad = p1.Lat() * (M_PI / 180);
    double dLong1InRad = p1.Lng() * (M_PI / 180);
    double dLat2InRad = p2.Lat() * (M_PI / 180);
    double dLong2InRad = p2.Lng() * (M_PI / 180);
    double dLongitude = dLong2InRad - dLong1InRad;
    double dLatitude = dLat2InRad - dLat1InRad;
    double a = pow(sin(dLatitude / 2), 2) + cos(dLat1InRad) * cos(dLat2InRad) * pow(sin(dLongitude / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double dDistance = EarthRadiusKm * c;
    return dDistance*1000;
}

void ShowCurrentWayPoints::showTable_updateMovableWP_slots(const int& row, const MapPointData& data){
    m_showListItems[row].mpd = data;
    this->item(row, 5)->setText(QString::number(m_showListItems[row].mpd.lng, 'f', 7));
    this->item(row, 6)->setText(QString::number(m_showListItems[row].mpd.lat, 'f', 7));
    this->item(row, 7)->setText(QString::number(m_showListItems[row].mpd.alt, 'f', 2));
    showTable_UpdateDistance(row);
}





