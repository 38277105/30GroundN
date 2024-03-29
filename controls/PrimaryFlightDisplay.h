#ifndef PRIMARYFLIGHTDISPLAY_H
#define PRIMARYFLIGHTDISPLAY_H

#include <QWidget>
#include <QPen>
#include <QTimer>
//#include "UASInterface.h"

class PrimaryFlightDisplay : public QWidget
{
    Q_OBJECT
public:
    PrimaryFlightDisplay(int width = 640, int height = 480, QWidget* parent = NULL);
    ~PrimaryFlightDisplay();

public slots:
    /** @brief Attitude from main autopilot / system state */
    void updateAttitude( double roll, double pitch, double yaw, quint64 timestamp);
    /** @brief Attitude from one specific component / redundant autopilot */
    void updateAttitude( int component, double roll, double pitch, double yaw, quint64 timestamp);

    void speedChanged(double groundspeed, double airspeed);
    void altitudeChanged( double altitudeAMSL, double altitudeRelative, double climbRate);
    void updateNavigationControllerErrors(double altitudeError, double speedError, double xtrackError);

    void uasTextMessage(int uasid, int componentid, int severity, QString text);

    void preArmMessageTimeout();

//    /** @brief Set the currently monitored UAS */
//    //void addUAS(UASInterface* uas);
//    void forgetUAS(UASInterface* uas);
//    void setActiveUAS(UASInterface* uas);

protected:
    enum Layout {
        COMPASS_INTEGRATED,
        COMPASS_SEPARATED               // For a very high container. Feature panels are at bottom.
    };

    enum Style {
        NO_OVERLAYS,                    // Hzon not visible through tapes nor through feature panels. Frames with margins between.
        OVERLAY_HORIZONTAL,             // Hzon visible through tapes and (frameless) feature panels.
        OVERLAY_HSI                     // Hzon visible through everything except bottom feature panels.
    };

    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *e);

    // from HUD.h:

    /** @brief Preferred Size */
    QSize sizeHint() const;
    /** @brief Start updating widget */
    void showEvent(QShowEvent* event);
    /** @brief Stop updating widget */
    void hideEvent(QHideEvent* event);

    // dongfang: We have no context menu. Viewonly.
    // void contextMenuEvent (QContextMenuEvent* event);

    // dongfang: What is that?
    // dongfang: OK it's for UI interaction. Presently, there is none.
    void createActions();

signals:
    void visibilityChanged(bool visible);

private:
    /*
    enum AltimeterMode {
        PRIMARY_MAIN_GPS_SUB,   // Show the primary alt. on tape and GPS as extra info
        GPS_MAIN                // Show GPS on tape and no extra info
    };

    enum AltimeterFrame {
        ASL,                    // Show ASL altitudes (plane pilots' normal preference)
        RELATIVE_TO_HOME        // Show relative-to-home altitude (copter pilots)
    };

    enum SpeedMode {
        PRIMARY_MAIN_GROUND_SUB,// Show primary speed (often airspeed) on tape and groundspeed as extra
        GROUND_MAIN             // Show groundspeed on tape and no extra info
    };
    */

    /*
     * There are at least these differences between airplane and copter PDF view:
     * - Airplane show absolute altutude in altimeter, copter shows relative to home
     */
    bool shouldDisplayNavigationData();

    void drawTextCenter(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawTextLeftCenter(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawTextRightCenter(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawTextCenterBottom(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawTextCenterTop(QPainter& painter, QString text, float fontSize, float x, float y);
    void drawAIGlobalFeatures(QPainter& painter, QRectF mainArea, QRectF paintArea);
    void drawAIAirframeFixedFeatures(QPainter& painter, QRectF area);
    void drawPitchScale(QPainter& painter, QRectF area, float intrusion, bool drawNumbersLeft, bool drawNumbersRight);
    void drawRollScale(QPainter& painter, QRectF area, bool drawTicks, bool drawNumbers);
    void drawAIAttitudeScales(QPainter& painter, QRectF area, float intrusion);
    void drawAICompassDisk(QPainter& painter, QRectF area, float halfspan);
    void drawSeparateCompassDisk(QPainter& painter, QRectF area);

    void drawAltimeter(QPainter& painter, QRectF area, float altitudeRelative, float altitudeAMSL, float vv);
    void drawVelocityMeter(QPainter& painter, QRectF area, float speed, float secondarySpeed);
    void fillInstrumentBackground(QPainter& painter, QRectF edge);
    void fillInstrumentOpagueBackground(QPainter& painter, QRectF edge);
    void drawInstrumentBackground(QPainter& painter, QRectF edge);

    /* This information is not currently included. These headers left in as a memo for restoration later.
    void drawLinkStatsPanel(QPainter& painter, QRectF area);
    void drawSysStatsPanel(QPainter& painter, QRectF area);
    void drawMissionStatsPanel(QPainter& painter, QRectF area);
    void drawSensorsStatsPanel(QPainter& painter, QRectF area);
    */

    void doPaint();

    //UASInterface* uas;          ///< The uas currently monitored

    /*
    AltimeterMode altimeterMode;
    AltimeterFrame altimeterFrame;
    SpeedMode speedMode;
    */

    bool preArmCheckFailure;
    QString preArmCheckMessage;
    QTimer *preArmMessageTimer;

    bool didReceivePrimaryAltitude;
    bool didReceivePrimarySpeed;

    float roll;
    float pitch;
    float heading;

    float m_altitudeRelative;
    float m_altitudeAMSL;

    // APM: GPS and baro mix above home (GPS) altitude. This value comes from the GLOBAL_POSITION_INT message.
    // Do !!!NOT!!! ever do altitude calculations at the ground station. There are enough pitfalls already.
    // If the MP "set home altitude" button is migrated to here, it must set the UAS home altitude, not a GS-local one.
    float aboveHomeAltitude;

    float m_groundspeed;
    float m_airspeed;
    float m_climbRate;

    float navigationAltitudeError;
    float navigationSpeedError;
    float navigationCrosstrackError;
    float navigationTargetBearing;

    Layout layout;      // The display layout.
    Style style;        // The AI style (tapes translucent or opague)

    // TODO: Use stylesheet colors?
    QColor redColor;
    QColor amberColor;
    QColor greenColor;

    qreal lineWidth;
    qreal fineLineWidth;

    qreal smallTextSize;
    qreal mediumTextSize;
    qreal largeTextSize;

    // Globally used stuff only.
    QPen instrumentEdgePen;
    QBrush instrumentBackground;
    QBrush instrumentOpagueBackground;

    QFont font;

    QTimer* refreshTimer;       ///< The main timer, controls the update rate

    static const int tickValues[];
    static const QString compassWindNames[];

    static const int updateInterval = 250;
};

#endif // PRIMARYFLIGHTDISPLAY_H
