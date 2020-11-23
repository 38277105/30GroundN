#ifndef QTESTWIN_MapWidgetEditor
#define QTESTWIN_MapWidgetEditor
#include <QSplitter>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include "opmapcontrol_ex/mapwidget/opmapwidget.h"
#include "MapWidget.h"
#include "controls/mapcontrols/showcurrentwaypoints.h"
#include "controls/mapcontrols/cachemapmanager.h"
#include "controls/mapcontrols/autoplan.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define M_DEG_TO_RAD 0.01745329251994
#define CONSTANTS_RADIUS_OF_EARTH			6371000
#define DBL_EPSILON 2.2204460492503131e-16
#define M_PI (3.14159265358979323846f)

struct map_projection_reference_s {
    double lat_rad;
    double lon_rad;
    double sin_lat;
    double cos_lat;
};

typedef struct _COPointData{
    double x;
    double y;
    struct _COPointData& operator = (const struct _COPointData& data){
        x = data.x;
        y = data.y;
        return *this;
    }
    bool operator == (const struct _COPointData &data){
        return qAbs(x-data.x) < (1e-5) && qAbs(y-data.y) < (1e-5);
    }
}COPointData;

typedef struct{
    int         nNum;
    bool        isStartUp;
    bool        isCrossUp;
    bool        isCrossDown;
    COPointData pointUp;
    COPointData pointDown;
}COLineData;

class MapWidgetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidgetEditor(QWidget *parent = 0);

    virtual int setupLayout(void);

    void ShowUAV(bool value);

    int mapWidgetEditor_deleteOneWP(int index);
    int mapWidgetEditor_updateOneWP(const int& row, const MapPointData& oneWP);
    int mapWidgetEditor_upOneWP(int row);
    int mapWidgetEditor_downOneWP(int row);
    void AddTableView(ShowCurrentWayPoints* editor, QSplitter* splitr);
signals:
    void signal_sendWP(const QList<ProtocolDataType>& WPItems);
    void signal_getWP();
    void signal_mapWidgetEditor_clearShowList(const QList<MapPointData>&);
    void signal_mapEditor_UpdateMovableWP(const int&, const MapPointData&);
    void signal_UpdateVehicleHome(internals::PointLatLng pos,int height);
    void signal_UpdateMapType(core::MapType::Types);
public slots:
    void upload_slots(void);
    void download_slots();
    void save_click();
    void open_load_click();

    void moveUAVStateChanged(bool state);

    void mapWidget_zoomChanged(int newZoom);
    void mapWidget_mousePressEvent(QMouseEvent *event);

    void moveAirLineStateChanged(bool state);

    void SetViewCenterModeChanged(bool state);

    void receiveWPDatas(QList<MapPointData> WPList);

    void receiveOneWPData_slots(const MapPointData& oneWPD);

    void initSetHome(internals::PointLatLng pos, double height);

    void homeData_edited(QString text);

    void homeData_editingFinished();

    void update_currentPos(internals::PointLatLng cpt);

    void showWP(const QList<MapPointData>& dataList);

    void UpdateUAVState(internals::PointLatLng pos,double direction,int height);

    void changedMapType(int index);

    void OffsetChanged();

    void CacheOffMap();

    void mapEdit_AutoPlan_slots();

    void mapEdit_AutoPlan_enableButtons_slots();

    void mapEdit_generateBoundary_slots(const QList<MapPointData>&);

    void mapEdit_generateWayPoints_slots(const APInfo&);

    void mapEdit_generateTableDatas_slots();

    void mapEdit_clearAutoPlanPoint_slots(const bool&);

    void mapEdit_isEnableClick_slots(const bool& );

    void mapEdit_ereseOnePoint_slots(const int&);

private:
    void generateLocalWayPointList(QList<ProtocolDataType>& dataList, \
                                   const QString& strLineData);
    void setupMapType_list();
    void setMapType();
    void InitMapName();
    void GenerateRealPlanList(const QList<MapPointData>& datas, const APInfo& info, QList<MapPointData>& dataList);
    void InitOriginOfCoor(const QList<MapPointData>& datas, struct map_projection_reference_s* ref);
    int LngLatToCoordinate(struct map_projection_reference_s* ref, const double& angle, const double& lng, const double& lat, double& x, double& y);
    COPointData getMaxXPoint(QList<COPointData>& srcList);
    int generateCOPlanList(const QList<COPointData>& srcList, const double& disx, const double& disy,  QList<COPointData>& dstList);
    int generateCOPlanList(const QList<COPointData>& srcList, const COPointData& homeCO, const double& disx, const double& disy,  QList<COPointData>& dstList);
    int CoordinateToLngLat(struct map_projection_reference_s* ref, const double& angle, const double& x, const double& y, double& lng, double& lat);
    bool isNew(const QList<COPointData>& srcList, const COPointData& item);
    bool LineIntersection(const COPointData& stPoint, const COPointData& endPoint, const double& y, COPointData& point);
    int generatePointsOfLineY(const QList<COPointData>& srcList, const double& y, const double& dis, QList<COPointData>& dstList);
    int getYSection(const QList<COPointData>& srcList, double& st, double& end);
    bool reenterAngle(const QList<COPointData>& srcList, int& reAngleIndex, bool& isReAngle);
    double DistanceOfTwoPoints(const QList<COPointData>& srcList, COPointData& stPoint, COPointData& endPoint, bool& isCrossReAngle);
    double DistanceOfTwoPoints(const QList<COPointData>& srcList, COPointData& stPoint, COPointData& endPoint);
    int disableOtherBtn();
    int enableOtherBtn();
    void mapEdit_clearBoundary();
    void mapEdit_clearBoundaryPoints();
    void mapEdit_clearBoundaryTable();
protected:
    QSettings                   *m_conf;
    MapWidget                   *m_mapWidget;
    QPushButton                 *m_btn_upload;
    QPushButton                 *m_btn_download;
    QPushButton                 *m_btn_savelocal;
    QPushButton                 *m_btn_openlocal;
    QPushButton                 *m_btn_downloadMap;
    QPushButton                 *m_btn_autoPlan;

    QLabel*                     m_lbl_lng;
    QLabel*                     m_lbl_lat;
    QLabel*                     m_lbl_alt;

    QLineEdit                   *m_ledt_lng;
    QLineEdit                   *m_ledt_lat;
    QLineEdit                   *m_ledt_alt;

    QLabel*                     m_cursorLabel;
    QLabel*                     m_lbl_timinglng;
    QLabel*                     m_lbl_timinglat;

    QLabel*                     m_homeDataLabel;
    QLineEdit                   *m_ledt_timinglng;
    QLineEdit                   *m_ledt_timinglat;

    QCheckBox                   *m_cbMoveUAV;
    QCheckBox                   *m_cbAirLine;
    QLabel*                     m_mapTypeLabel;
    QComboBox                   *m_cboMapType;
    mapcontrol::UAVItem         *m_uav;
    MapPointData                m_homeData;
    bool                        m_isNeedUpdating;
    bool                        m_isInitHome;
    int                         m_editType;
    core::MapType::Types        m_mapType;
    QMap<int, QString>          m_mapName;
    QGridLayout*                m_mainLayout;
    ShowCurrentWayPoints*       m_table_editor;

    QLabel*                     m_offset_setting;

    QLabel*                     m_lbl_offset_Lat;
    QLineEdit*                  m_tbx_offset_Lat;
    QLabel*                     m_lbl_offset_Lon;
    QLineEdit*                  m_tbx_offset_Lon;

    QLabel*                     m_view_setting;
    QCheckBox*                  m_SetUavasCenter;
    CacheMapManager*            m_cacheMapManager;
    AutoPlan*                   m_autoPlan;
    QList<MapPointData>         m_boundaryDatas;
    QList<MapPointData>         m_autoPlanWayPointsList;
    bool                        m_isReAngle;
    int                         m_reAngleIndex;
    bool                        m_isFinished;
};

#endif // QTESTWIN_H
