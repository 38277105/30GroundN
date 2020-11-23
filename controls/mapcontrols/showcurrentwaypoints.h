#ifndef SHOWCURRENTWAYPOINTS_H
#define SHOWCURRENTWAYPOINTS_H

#include <QWidget>
#include <QTableWidget>
#include "../MapWidget.h"
#include "wpdeletebtn.h"
#include "wpcmdcbo.h"
#include "wpmvbtn.h"
namespace Ui {
class ShowCurrentWayPoints;
}

//typedef  mavlink_mission_item_int_t  ProtocolDataType;
        typedef struct ProtocolDataType {
         float param1; /*< PARAM1, see MAV_CMD enum*/
         float param2; /*< PARAM2, see MAV_CMD enum*/
         float param3; /*< PARAM3, see MAV_CMD enum*/
         float param4; /*< PARAM4, see MAV_CMD enum*/
         double x; /*< PARAM5 / local: x position in meters * 1e4, global: latitude in degrees * 10^7*/
         double y; /*< PARAM6 / y position: local: x position in meters * 1e4, global: longitude in degrees *10^7*/
         float z; /*< PARAM7 / z position: global: altitude in meters (relative or absolute, depending on frame.*/
         uint16_t seq; /*< Waypoint ID (sequence number). Starts at zero. Increases monotonically for each waypoint, no gaps in the sequence (0,1,2,3,4).*/
         uint16_t command; /*< The scheduled action for the MISSION. see MAV_CMD in common.xml MAVLink specs*/
         uint8_t target_system; /*< System ID*/
         uint8_t target_component; /*< Component ID*/
         uint8_t frame; /*< The coordinate system of the MISSION. see MAV_FRAME in mavlink_types.h*/
         uint8_t current; /*< false:0, true:1*/
         uint8_t autocontinue; /*< autocontinue to next wp*/
        } ProtocolDataType;

typedef struct{
    MapPointData    mpd;
    double          dParam1;
    double          dParam2;
    double          dParam3;
    double          dParam4;
    wpcmdCbo*       cCmdCbo;
    WPDeleteBtn*    cDelBtn;
    wpmvbtn*        cUpBtn;
    wpmvbtn*        cDownBtn;
    int             ncmdType;
}ShowListItemData;

class ShowCurrentWayPoints : public QTableWidget
{
    Q_OBJECT
public:
    explicit ShowCurrentWayPoints(QWidget *parent = 0);
    ~ShowCurrentWayPoints();
    int  showTable_init(const QList<MapPointData>& dataList);
    int  showTable_inertOneDataItem(const MapPointData& OneWPData);
    void showTable_getDatas(QList<ProtocolDataType>& listdata);
    void showTable_SetHome(double lng, double lat, double height);
    void showTable_deleteTheRow(const int& row);
private:
    void showTable_delBtnRowAdd();
    void showTable_delBtnRowSubtract(int row);
    void showTable_moveTheDataList(bool flag, int row);
    void showTable_moveTheRow(bool flag, int row);
    void showTable_updateDistance(bool flag, int row);
    void showTable_insetTheRow(const int& row, ShowListItemData& data);
    void showTable_UpdateDistance(int row);
    int  showTable_distanceOfTwoWP(double sLng, double sLat, double dLng, double dLat);
signals:
    void signal_SCDeleteOneWP(int index);
    void signal_SCUpdateOneWP(const int&, const MapPointData& );
    void signal_SCUpMoveOneWp(int row);
    void signal_SCDownMoveOneWp(int row);
    void siganle_UpdateMapPointList(const QList<MapPointData>&);
public slots:
    void showTable_deleteTheRow_slots(int row);
    void showTable_upTheRow_slots(int row);
    void showTable_downTheRow_slots(int row);
    void showTable_cellDoubleKK_slots(int row, int column);
    void showTable_cellKK_slots(int, int);
    void showTable_cellValueChanged_slots(int row, int column);
    int  showTable_clearSL_slots();
    int  showTable_updateShowList_slots(const QList<ProtocolDataType>&);
    void showTable_updateMovableWP_slots(const int& row, const MapPointData& data);

private:
    QList<ShowListItemData>   m_showListItems;
    int                       m_rowIndex;
    double                    m_homeLng;
    double                    m_homeLat;
    double                    m_homeHeight;
};

#endif // SHOWCURRENTWAYPOINTS_H
