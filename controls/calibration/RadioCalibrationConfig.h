/*===================================================================
APM_PLANNER Open Source Ground Control Station

(c) 2013 APM_PLANNER PROJECT <http://www.diydrones.com>

This file is part of the APM_PLANNER project

    APM_PLANNER is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    APM_PLANNER is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with APM_PLANNER. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/


#ifndef RADIOCALIBRATIONCONFIG_H
#define RADIOCALIBRATIONCONFIG_H

#include <QWidget>
#include <QTimer>
#include <QShowEvent>
#include <QHideEvent>
#include "ui_RadioCalibrationConfig.h"
#include "controllers/parametercontroller.h"

class QGCRadioChannelDisplay;


class RadioCalibrationConfig : public QWidget
{
    Q_OBJECT

    static const int RC_CHANNEL_PWM_MIN = 100; // Spektrum DX6i reports 898 on ch7 even though its 6 channels
    static const int RC_CHANNEL_PWM_MAX = 2100;
    static const int RC_CHANNEL_NUM_MAX = 8;
    static const int RC_CHANNEL_LOWER_CONTROL_CH_MAX = 4;
    
public:
    explicit RadioCalibrationConfig(QWidget *parent = 0);
    ~RadioCalibrationConfig();
    void initConnections();
protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
private slots:

    void remoteControlChannelRawChanged(int chan,uint16_t val);
    void parameterChanged(int uas, int component, QString parameterName, QVariant value);
    void guiUpdateTimerTick();
    void calibrateButtonClicked();

    void pitchClicked(bool state);
    void rollClicked(bool state);
    void yawClicked(bool state);
    void throttleClicked(bool state);

    void elevonsChecked(bool state);
    void elevonsReversed(bool state);
    void elevonsCh1Rev(bool state);
    void elevonsCh2Rev(bool state);
    void elevonOutput();

    void modeIndexChanged(int index);

private:
    void updateChannelReversalStates();
    void setParamChannelRev(const QString& param, bool state);
    void updateChannelRevState(QCheckBox *checkbox, int channelId);
    void readSettings();
    void writeSettings();
    bool isRadioControlActive();
    bool isInRange(double value, double min, double max);
    bool validRadioSettings();
    void _queryParam(ParamData& param,char* argName);
    bool _ParamExist(char* argName);

private:
    Ui::RadioCalibrationConfig ui;
    QList<uint16_t> rcMin;
    QList<uint16_t> rcMax;
    QList<uint16_t> rcTrim;
    QList<uint16_t> rcValue;

    QTimer *guiUpdateTimer;
    bool m_calibrationEnabled;

    int m_pitchChannel;
    int m_rollChannel;
    int m_yawChannel;
    int m_throttleChannel;

    QGCRadioChannelDisplay* m_pitchWidget;
    QGCRadioChannelDisplay* m_throttleWidget;
    QCheckBox* m_pitchCheckBox;
    QCheckBox* m_throttleCheckBox;
    int m_rcMode;

};

#endif // RADIOCALIBRATIONCONFIG_H
