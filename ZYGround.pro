#-------------------------------------------------
#
# Project created by XINGJIWEI 2016-04-24
#
#-------------------------------------------------
#CONFIG += exceptions
CONFIG +=  c++11
QT  +=core\
     gui\
     network\
     sql\
     opengl\
     svg\
     serialport\
     concurrent\
     xml\
     positioning

#qtHaveModule(opengl): QT += opengl

#�������ܵı�������
#LIBS    += -lOle32
#DEFINES += QGC_SPEECH_ENABLED
#DEFINES += QGC_NOTIFY_TUNES_ENABLED

DEFINES   += ZYFlightMode_ENABLED
#DEFINES  += ZYDataAnalasis_ENABLED
#DEFINES  += AIRSENSOR_ENABLED
#DEFINES  += AERIALVideo_ENABLED

#ϵ�����ֵĴ�����δ�ϲ�����Ҫ������
DEFINES   += MOORING_ENABLED


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets\
                                        multimedia\
                                        multimediawidgets

TARGET = ZYGroundApp
TEMPLATE = app

OPMAPCONTROL_DIR = opmapcontrol_ex
COMM_DIR = connections
CONTROL_DIR = controls

INCLUDEPATH +=  $$OPMAPCONTROL_DIR \
                $$OPMAPCONTROL_DIR/core \
                $$OPMAPCONTROL_DIR/internals \
                $$OPMAPCONTROL_DIR/mapwidget



INCLUDEPATH += ./vehicles/mavlink/include/mavlink/v1.0

    INCLUDEPATH += \
        SDL\include

    LIBS += \
        -L$$IN_PWD\SDL\lib\x64 \
        -lSDL



##########VLC###########
#LIBS       += -lVLCQtCore -lVLCQtWidgets
#LIBS       += -L$$IN_PWD/vlc/lib -lVLCQtCore -lVLCQtWidgets
#INCLUDEPATH += $$IN_PWD/vlc/include
##########VLC###########

HEADERS += \
    $$OPMAPCONTROL_DIR/opmapcontrol.h \
    $$OPMAPCONTROL_DIR/core/accessmode.h \
    $$OPMAPCONTROL_DIR/core/alllayersoftype.h \
    $$OPMAPCONTROL_DIR/core/cache.h \
    $$OPMAPCONTROL_DIR/core/cacheitemqueue.h \
    $$OPMAPCONTROL_DIR/core/debugheader.h \
    $$OPMAPCONTROL_DIR/core/diagnostics.h \
    $$OPMAPCONTROL_DIR/core/geodecoderstatus.h \
    $$OPMAPCONTROL_DIR/core/kibertilecache.h \
    $$OPMAPCONTROL_DIR/core/languagetype.h \
    $$OPMAPCONTROL_DIR/core/maptype.h \
    $$OPMAPCONTROL_DIR/core/memorycache.h \
    $$OPMAPCONTROL_DIR/core/opmaps.h \
    $$OPMAPCONTROL_DIR/core/placemark.h \
    $$OPMAPCONTROL_DIR/core/point.h \
    $$OPMAPCONTROL_DIR/core/providerstrings.h \
    $$OPMAPCONTROL_DIR/core/pureimage.h \
    $$OPMAPCONTROL_DIR/core/pureimagecache.h \
    $$OPMAPCONTROL_DIR/core/rawtile.h \
    $$OPMAPCONTROL_DIR/core/size.h \
    $$OPMAPCONTROL_DIR/core/tilecachequeue.h \
    $$OPMAPCONTROL_DIR/core/urlfactory.h \
    $$OPMAPCONTROL_DIR/internals/copyrightstrings.h \
    $$OPMAPCONTROL_DIR/internals/core.h \
    $$OPMAPCONTROL_DIR/internals/debugheader.h \
    $$OPMAPCONTROL_DIR/internals/loadtask.h \
    $$OPMAPCONTROL_DIR/internals/mousewheelzoomtype.h \
    $$OPMAPCONTROL_DIR/internals/pointlatlng.h \
    $$OPMAPCONTROL_DIR/internals/pureprojection.h \
    $$OPMAPCONTROL_DIR/internals/rectangle.h \
    $$OPMAPCONTROL_DIR/internals/rectlatlng.h \
    $$OPMAPCONTROL_DIR/internals/sizelatlng.h \
    $$OPMAPCONTROL_DIR/internals/tile.h \
    $$OPMAPCONTROL_DIR/internals/tilematrix.h \
    $$OPMAPCONTROL_DIR/mapwidget/gpsitem.h \
    $$OPMAPCONTROL_DIR/mapwidget/homeitem.h \
    $$OPMAPCONTROL_DIR/mapwidget/mapgraphicitem.h \
    $$OPMAPCONTROL_DIR/mapwidget/mapripform.h \
    $$OPMAPCONTROL_DIR/mapwidget/mapripper.h \
    $$OPMAPCONTROL_DIR/mapwidget/opmapwidget.h \
    $$OPMAPCONTROL_DIR/mapwidget/trailitem.h \
    $$OPMAPCONTROL_DIR/mapwidget/traillineitem.h \
    $$OPMAPCONTROL_DIR/mapwidget/uavitem.h \
    $$OPMAPCONTROL_DIR/mapwidget/uavmapfollowtype.h \
    $$OPMAPCONTROL_DIR/mapwidget/uavtrailtype.h \
    $$OPMAPCONTROL_DIR/mapwidget/waypointitem.h \
    $$OPMAPCONTROL_DIR/mapwidget/waypointlineitem.h \
    $$OPMAPCONTROL_DIR/mapwidget/omapconfiguration.h \
    $$OPMAPCONTROL_DIR/internals/projections/lks94projection.h \
    $$OPMAPCONTROL_DIR/internals/projections/mercatorprojection.h \
    $$OPMAPCONTROL_DIR/internals/projections/mercatorprojectionyandex.h \
    $$OPMAPCONTROL_DIR/internals/projections/platecarreeprojection.h \
    $$OPMAPCONTROL_DIR/internals/projections/platecarreeprojectionpergo.h\
    frmmain.h \
    $$CONTROL_DIR/iconhelper.h \
    $$CONTROL_DIR/frmmessagebox.h \
    $$CONTROL_DIR/myhelper.h \
    $$CONTROL_DIR/svgview.h \
    $$CONTROL_DIR/picturebox.h \
    $$CONTROL_DIR/PrimaryFlightDisplay.h \
    $$CONTROL_DIR/flightdatacontrol.h \
    $$CONTROL_DIR/MapWidget.h \
    $$CONTROL_DIR/UAS_types.h \
    $$CONTROL_DIR/MapWidgetContainer.h \
    $$CONTROL_DIR/MapWidgetEditor.h \
    $$CONTROL_DIR/myhelper.h\
    $$CONTROL_DIR/dataparameterview.h \
    $$CONTROL_DIR/inputdelegate.h\
    $$CONTROL_DIR/parametertableview.h\
    $$COMM_DIR/CommonStru.h \
    vehicles/vehiclemanager.h \
    vehicles/vehicle.h \
    vehicles/linkparser.h \
    vehicles/mavlinkparser.h \
    vehicles/speciallinkparser.h \
    mission/missionitem.h \
    mission/missionmanager.h \
    settingcontrols/settingmanager.h \
    loghelper/logger.h \
    $$COMM_DIR/seriallink.h \
    connections/linkmanager.h \
    connections/communicationinterface.h \
    connections/datastruct.h \
    vehicles/vehiclestate.h \
#    Voice/voicehelper.h\
#    Voice/QGCAudioWorker.h \
    connections/mavlinkmessageinterface.h \
    connections/mavlinkcommandexecutant.h \
    connections/mavlinkmissionexecutant.h\
    controls/buttondelegate.h \
    controllers/frmmaincontroller.h \
    controls/cmdwindow.h \
    controls/paramrightcontrol.h\
    connections/generalprotocolinterface.h \
    controllers/curvecontroller.h \
    controls/commoncontrol/croundprogressbar.h \
    controls/commoncontrol/loadingsplash.h \
    controls/Sensor/databoxresolver.h \
    controls/Sensor/sensordatamodel.h \
    controls/Sensor/SensorStruct.h \
    controls/Sensor/sensortableview.h \
    Utils/serialportfinder.h \
    Utils/logbroswer.h \
    loghelper/outputlogger.h \
    controls/mapcontrols/showcurrentwaypoints.h \
    controls/mapcontrols/wpdeletebtn.h\
    controls/mapcontrols/wpcmdcbo.h \
    controls/mapcontrols/wpmvbtn.h\
    nmea/nmeainterface.h \
    nmea/nmeathread.h \
    controllers/parametercontroller.h \
    controls/calibration/AccelCalibrationConfig.h \
#    controls/calibration/CompassMotorCalibrationDialog.h \
    controls/calibration/QGCGeo.h \
    controls/calibration/RadioCalibrationConfig.h \
    controls/calibration/designer/QGCMouseWheelEventFilter.h \
    controls/calibration/designer/QGCRadioChannelDisplay.h \
    controls/calibration/calibrateform.h \
#    controls/calibration/dockcalibrateform.h \
    controls/flowlayout.h \
    controls/InfoPanel/infoview.h \
    controls/InfoPanel/infoviewitem.h \
    controls/statuscontrol/signalcontrol.h \
    controls/statuscontrol/satellitecontrol.h \
    controls/statuscontrol/batterycontrol.h \
    controls/calibration/APMCompassCal.h \
    controls/calibration/QGC.h \
    controls/calibration/compasscalibratecontrol.h \
    controls/calibration/esc_cali_en_from.h\
    Utils/zygroundglobalconfig.h \
#    controls/AerialVideo/commonvideoplayer.h \
    controls/mapcontrols/cachemapmanager.h \
    controls/mapcontrols/autoplan.h \
    opmapcontrol_ex/mapwidget/zoomChangeItem.h \
    controls/AerialVideo/camera.h \
    controls/customsettings.h \
    qwt/qwt_time_scale_draw.h \
    qwt/qwt_time_plot.h \
    controls/analisys/MAVLinkDecoder.h \
    controls/analisys/linechart/ChartPlot.h \
    controls/analisys/linechart/IncrementalPlot.h \
    controls/analisys/linechart/LinechartPlot.h \
    controls/analisys/linechart/Linecharts.h \
    controls/analisys/linechart/LinechartWidget.h \
    controls/analisys/linechart/Scrollbar.h \
    controls/analisys/linechart/ScrollZoomer.h \
    controls/analisys/linechartmainwindow.h \
    controls/analisys/LogCompressor.h \
    controls/analisys/MG.h \
    controllers/wpointscomunicationcontroller.h \
    controls/testform.h \
    controllers/showlogtable.h \
    controllers/runninglog.h \
    controls/calibration/accelcalibrationofindependent.h \
    controls/statuscontrol/refreshcontrol.h \
    controls/InfoPanel/statusmonitor.h \
    controls/InfoPanel/poweralarm.h \
    controllers/soundthread.h \
    controls/statuscontrol/warningform.h \
    controls/calibration/motortestform.h \
    controls/statuscontrol/joystickstate.h \
    controls/joystick/fancyjoystick.h \
    controls/joystick/joysticktoflycontroller.h \
    Utils/spdwidget.h \
    Utils/headingwidget.h \
    Utils/qimagelabel.h \
    connections/UDP/LinkConfiguration.h \
    connections/UDP/LinkInterface.h \
    connections/UDP/netcontrolto206.h \
    connections/UDP/QGC.h \
    connections/UDP/QGCConfig.h \
    connections/UDP/UDPLink.h \
    Utils/myserialport.h \
    controls/InfoPanel/critActInfo.h \
    controls/calibration/esccaliform.h \
    controls/joystick/zystick.h \
    messagetrans.h \
    controls/calibration/libeigen/ellipsoidmag.h \
    controls/firmware/Bootloader.h \
    controls/firmware/FirmwareImage.h \
    controls/firmware/FirmwareUpgradeController.h \
    controls/firmware/JsonHelper.h \
    controls/firmware/PX4FirmwareUpgradeThread.h \
    controls/firmwareform.h \
    controls/InfoPanel/infomonitor.h

SOURCES += \
    $$OPMAPCONTROL_DIR/core/alllayersoftype.cpp \
    $$OPMAPCONTROL_DIR/core/cache.cpp \
    $$OPMAPCONTROL_DIR/core/cacheitemqueue.cpp \
    $$OPMAPCONTROL_DIR/core/diagnostics.cpp \
    $$OPMAPCONTROL_DIR/core/kibertilecache.cpp \
    $$OPMAPCONTROL_DIR/core/languagetype.cpp \
    $$OPMAPCONTROL_DIR/core/memorycache.cpp \
    $$OPMAPCONTROL_DIR/core/opmaps.cpp \
    $$OPMAPCONTROL_DIR/core/placemark.cpp \
    $$OPMAPCONTROL_DIR/core/point.cpp \
    $$OPMAPCONTROL_DIR/core/providerstrings.cpp \
    $$OPMAPCONTROL_DIR/core/pureimage.cpp \
    $$OPMAPCONTROL_DIR/core/pureimagecache.cpp \
    $$OPMAPCONTROL_DIR/core/rawtile.cpp \
    $$OPMAPCONTROL_DIR/core/size.cpp \
    $$OPMAPCONTROL_DIR/core/tilecachequeue.cpp \
    $$OPMAPCONTROL_DIR/core/urlfactory.cpp \
    $$OPMAPCONTROL_DIR/internals/core.cpp \
    $$OPMAPCONTROL_DIR/internals/loadtask.cpp \
    $$OPMAPCONTROL_DIR/internals/MouseWheelZoomType.cpp \
    $$OPMAPCONTROL_DIR/internals/pointlatlng.cpp \
    $$OPMAPCONTROL_DIR/internals/pureprojection.cpp \
    $$OPMAPCONTROL_DIR/internals/rectangle.cpp \
    $$OPMAPCONTROL_DIR/internals/rectlatlng.cpp \
    $$OPMAPCONTROL_DIR/internals/sizelatlng.cpp \
    $$OPMAPCONTROL_DIR/internals/tile.cpp \
    $$OPMAPCONTROL_DIR/internals/tilematrix.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/configuration.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/gpsitem.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/homeitem.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/mapgraphicitem.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/mapripform.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/mapripper.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/opmapwidget.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/trailitem.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/traillineitem.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/uavitem.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/waypointitem.cpp \
#    $$OPMAPCONTROL_DIR/mapwidget/waypointcircle.cpp \
    $$OPMAPCONTROL_DIR/mapwidget/waypointlineitem.cpp \
    $$OPMAPCONTROL_DIR/internals/projections/lks94projection.cpp \
    $$OPMAPCONTROL_DIR/internals/projections/mercatorprojection.cpp \
    $$OPMAPCONTROL_DIR/internals/projections/mercatorprojectionyandex.cpp \
    $$OPMAPCONTROL_DIR/internals/projections/platecarreeprojection.cpp \
    $$OPMAPCONTROL_DIR/internals/projections/platecarreeprojectionpergo.cpp\
    main.cpp\
    frmmain.cpp \
    $$CONTROL_DIR/iconhelper.cpp \
    $$CONTROL_DIR/frmmessagebox.cpp \
    $$CONTROL_DIR/svgview.cpp \
    $$CONTROL_DIR/picturebox.cpp \
    $$CONTROL_DIR/PrimaryFlightDisplay.cc \
    $$CONTROL_DIR/flightdatacontrol.cpp \
    $$CONTROL_DIR/MapWidget.cpp \
    $$CONTROL_DIR/UAS_types.cpp \
    $$CONTROL_DIR/MapWidgetContainer.cpp\
    $$CONTROL_DIR/MapWidgetEditor.cpp\
    $$CONTROL_DIR/dataparameterview.cpp \
    $$CONTROL_DIR/inputdelegate.cpp\
    $$CONTROL_DIR/parametertableview.cpp\
    vehicles/vehiclemanager.cpp \
    vehicles/vehicle.cpp \
    vehicles/linkparser.cpp \
    vehicles/mavlinkparser.cpp \
    vehicles/speciallinkparser.cpp \
    mission/missionitem.cpp \
    mission/missionmanager.cpp \
    settingcontrols/settingmanager.cpp \
    loghelper/logger.cpp \
    $$COMM_DIR/seriallink.cpp \
    connections/linkmanager.cpp \
    connections/communicationinterface.cpp \
#    Voice/voicehelper.cpp\
#    Voice/QGCAudioWorker.cpp \
    connections/mavlinkmessageinterface.cpp \
    connections/mavlinkcommandexecutant.cpp \
    connections/mavlinkmissionexecutant.cpp\
    controls/buttondelegate.cpp \
    controllers/frmmaincontroller.cpp \
    controls/paramrightcontrol.cpp\
    connections/generalprotocolinterface.cpp \
    controls/cmdwindow.cpp \
    controllers/curvecontroller.cpp \
    controls/commoncontrol/croundprogressbar.cpp \
    controls/commoncontrol/loadingsplash.cpp \
    controls/Sensor/databoxresolver.cpp \
    controls/Sensor/sensordatamodel.cpp \
    controls/Sensor/sensortableview.cpp \
    Utils/serialportfinder.cpp \
    Utils/logbroswer.cpp \
    loghelper/outputlogger.cpp \
    controls/mapcontrols/showcurrentwaypoints.cpp \
    controls/mapcontrols/wpdeletebtn.cpp \
    controls/mapcontrols/wpcmdcbo.cpp \
    controls/mapcontrols/wpmvbtn.cpp\
    nmea/nmeainterface.cpp \
    nmea/nmeathread.cpp \
    controllers/parametercontroller.cpp \
    controls/calibration/AccelCalibrationConfig.cc \
#    controls/calibration/CompassConfig.cc \
#    controls/calibration/CompassMotorCalibrationDialog.cpp \
    controls/calibration/QGCGeo.cc \
    controls/calibration/RadioCalibrationConfig.cc \
    controls/calibration/designer/QGCMouseWheelEventFilter.cc \
    controls/calibration/designer/QGCRadioChannelDisplay.cpp \
    controls/calibration/calibrateform.cpp \
#    controls/calibration/calibrationconfighelper.cpp \
#    controls/calibration/dockcalibrateform.cpp \
    controls/flowlayout.cpp \
    controls/InfoPanel/infoview.cpp \
    controls/InfoPanel/infoviewitem.cpp \
    controls/statuscontrol/signalcontrol.cpp \
    controls/statuscontrol/satellitecontrol.cpp \
    controls/statuscontrol/batterycontrol.cpp \
    controls/calibration/APMCompassCal.cc \
    controls/calibration/QGC.cc \
    controls/calibration/compasscalibratecontrol.cpp \
#    controls/AerialVideo/commonvideoplayer.cpp \
    Utils/zygroundglobalconfig.cpp \
    controls/mapcontrols/cachemapmanager.cpp \
    controls/mapcontrols/autoplan.cpp \
    opmapcontrol_ex/mapwidget/zoomChangeitem.cpp \
    controls/AerialVideo/camera.cpp \
    controls/customsettings.cpp \
    qwt/qwt_time_scale_draw.cpp \
    qwt/qwt_time_plot.cpp \
    controls/analisys/MAVLinkDecoder.cc \
    controls/analisys/linechart/ChartPlot.cc \
    controls/analisys/linechart/IncrementalPlot.cc \
    controls/analisys/linechart/LinechartPlot.cc \
    controls/analisys/linechart/Linecharts.cc \
    controls/analisys/linechart/LinechartWidget.cc \
    controls/analisys/linechart/Scrollbar.cc \
    controls/analisys/linechart/ScrollZoomer.cc \
    controls/analisys/linechartmainwindow.cpp \
    controls/analisys/LogCompressor.cc \
    controllers/wpointscomunicationcontroller.cpp \
    controls/testform.cpp \
    controllers/showlogtable.cpp \
    controllers/runninglog.cpp \
    controls/calibration/accelcalibrationofindependent.cpp \
    controls/statuscontrol/refreshcontrol.cpp \
    controls/InfoPanel/statusmonitor.cpp \
    controls/InfoPanel/poweralarm.cpp \
    controllers/soundthread.cpp \
    controls/statuscontrol/warningform.cpp \
    controls/calibration/motortestform.cpp \
    controls/statuscontrol/joystickstate.cpp \
    controls/joystick/fancyjoystick.cpp \
    controls/joystick/joysticktoflycontroller.cpp \
    Utils/spdwidget.cpp \
    Utils/headingwidget.cpp \
    Utils/qimagelabel.cpp \
    connections/UDP/LinkConfiguration.cc \
    connections/UDP/netcontrolto206.cpp \
    connections/UDP/QGC.cc \
    connections/UDP/UDPLink.cc \
    Utils/myserialport.cpp \
    controls/InfoPanel/critActInfo.cpp \
    controls/calibration/esccaliform.cpp \
    controls/joystick/zystick.cpp \
    messagetrans.cpp \
    controls/calibration/libeigen/ellipsoidmag.cpp \
    controls/firmware/Bootloader.cc \
    controls/firmware/FirmwareImage.cc \
    controls/firmware/FirmwareUpgradeController.cc \
    controls/firmware/JsonHelper.cc \
    controls/firmware/PX4FirmwareUpgradeThread.cc \
    controls/firmwareform.cpp \
    controls/InfoPanel/infomonitor.cpp

QWTSRCDIR = qwt
DEPENDPATH += $$QWTSRCDIR
INCLUDEPATH += $$QWTSRCDIR

HEADERS += $$QWTSRCDIR/qwt.h \
           $$QWTSRCDIR/qwt_abstract_legend.h \
           $$QWTSRCDIR/qwt_abstract_scale.h \
           $$QWTSRCDIR/qwt_abstract_scale_draw.h \
           $$QWTSRCDIR/qwt_abstract_slider.h \
           $$QWTSRCDIR/qwt_analog_clock.h \
           $$QWTSRCDIR/qwt_arrow_button.h \
           $$QWTSRCDIR/qwt_clipper.h \
           $$QWTSRCDIR/qwt_color_map.h \
           $$QWTSRCDIR/qwt_compass.h \
           $$QWTSRCDIR/qwt_compass_rose.h \
           $$QWTSRCDIR/qwt_counter.h \
           $$QWTSRCDIR/qwt_curve_fitter.h \
           $$QWTSRCDIR/qwt_dial.h \
           $$QWTSRCDIR/qwt_dial_needle.h \
           $$QWTSRCDIR/qwt_dyngrid_layout.h \
           $$QWTSRCDIR/qwt_event_pattern.h \
           $$QWTSRCDIR/qwt_global.h \
           $$QWTSRCDIR/qwt_graphic.h \
           $$QWTSRCDIR/qwt_interval.h \
           $$QWTSRCDIR/qwt_knob.h \
           $$QWTSRCDIR/qwt_legend.h \
           $$QWTSRCDIR/qwt_legend_data.h \
           $$QWTSRCDIR/qwt_legend_label.h \
           $$QWTSRCDIR/qwt_magnifier.h \
           $$QWTSRCDIR/qwt_math.h \
           $$QWTSRCDIR/qwt_null_paintdevice.h \
           $$QWTSRCDIR/qwt_painter.h \
           $$QWTSRCDIR/qwt_painter_command.h \
           $$QWTSRCDIR/qwt_panner.h \
           $$QWTSRCDIR/qwt_picker.h \
           $$QWTSRCDIR/qwt_picker_machine.h \
           $$QWTSRCDIR/qwt_pixel_matrix.h \
           $$QWTSRCDIR/qwt_plot.h \
           $$QWTSRCDIR/qwt_plot_canvas.h \
           $$QWTSRCDIR/qwt_plot_curve.h \
           $$QWTSRCDIR/qwt_plot_dict.h \
           $$QWTSRCDIR/qwt_plot_grid.h \
           $$QWTSRCDIR/qwt_plot_item.h \
           $$QWTSRCDIR/qwt_plot_layout.h \
           $$QWTSRCDIR/qwt_plot_magnifier.h \
           $$QWTSRCDIR/qwt_plot_marker.h \
           $$QWTSRCDIR/qwt_plot_panner.h \
           $$QWTSRCDIR/qwt_plot_picker.h \
           $$QWTSRCDIR/qwt_plot_rasteritem.h \
           $$QWTSRCDIR/qwt_plot_scaleitem.h \
           $$QWTSRCDIR/qwt_plot_seriesitem.h \
           $$QWTSRCDIR/qwt_plot_spectrogram.h \
           $$QWTSRCDIR/qwt_plot_svgitem.h \
           $$QWTSRCDIR/qwt_plot_zoomer.h \
           $$QWTSRCDIR/qwt_point_mapper.h \
           $$QWTSRCDIR/qwt_point_data.h \
           $$QWTSRCDIR/qwt_raster_data.h \
           $$QWTSRCDIR/qwt_round_scale_draw.h \
           $$QWTSRCDIR/qwt_scale_div.h \
           $$QWTSRCDIR/qwt_scale_draw.h \
           $$QWTSRCDIR/qwt_scale_engine.h \
           $$QWTSRCDIR/qwt_scale_map.h \
           $$QWTSRCDIR/qwt_scale_widget.h \
           $$QWTSRCDIR/qwt_series_data.h \
           $$QWTSRCDIR/qwt_slider.h \
           $$QWTSRCDIR/qwt_spline.h \
           $$QWTSRCDIR/qwt_symbol.h \
           $$QWTSRCDIR/qwt_text.h \
           $$QWTSRCDIR/qwt_text_engine.h \
           $$QWTSRCDIR/qwt_text_label.h \
           $$QWTSRCDIR/qwt_thermo.h \
           $$QWTSRCDIR/qwt_transform.h \
           $$QWTSRCDIR/qwt_wheel.h \
           $$QWTSRCDIR/qwt_widget_overlay.h
SOURCES += $$QWTSRCDIR/qwt_abstract_legend.cpp \
           $$QWTSRCDIR/qwt_abstract_scale.cpp \
           $$QWTSRCDIR/qwt_abstract_scale_draw.cpp \
           $$QWTSRCDIR/qwt_abstract_slider.cpp \
           $$QWTSRCDIR/qwt_analog_clock.cpp \
           $$QWTSRCDIR/qwt_arrow_button.cpp \
           $$QWTSRCDIR/qwt_clipper.cpp \
           $$QWTSRCDIR/qwt_color_map.cpp \
           $$QWTSRCDIR/qwt_compass.cpp \
           $$QWTSRCDIR/qwt_compass_rose.cpp \
           $$QWTSRCDIR/qwt_counter.cpp \
           $$QWTSRCDIR/qwt_curve_fitter.cpp \
           $$QWTSRCDIR/qwt_dial.cpp \
           $$QWTSRCDIR/qwt_dial_needle.cpp \
           $$QWTSRCDIR/qwt_dyngrid_layout.cpp \
           $$QWTSRCDIR/qwt_event_pattern.cpp \
           $$QWTSRCDIR/qwt_graphic.cpp \
           $$QWTSRCDIR/qwt_interval.cpp \
           $$QWTSRCDIR/qwt_knob.cpp \
           $$QWTSRCDIR/qwt_legend.cpp \
           $$QWTSRCDIR/qwt_legend_data.cpp \
           $$QWTSRCDIR/qwt_legend_label.cpp \
           $$QWTSRCDIR/qwt_magnifier.cpp \
           $$QWTSRCDIR/qwt_math.cpp \
           $$QWTSRCDIR/qwt_null_paintdevice.cpp \
           $$QWTSRCDIR/qwt_painter.cpp \
           $$QWTSRCDIR/qwt_painter_command.cpp \
           $$QWTSRCDIR/qwt_panner.cpp \
           $$QWTSRCDIR/qwt_picker.cpp \
           $$QWTSRCDIR/qwt_picker_machine.cpp \
           $$QWTSRCDIR/qwt_pixel_matrix.cpp \
           $$QWTSRCDIR/qwt_plot.cpp \
           $$QWTSRCDIR/qwt_plot_axis.cpp \
           $$QWTSRCDIR/qwt_plot_canvas.cpp \
           $$QWTSRCDIR/qwt_plot_curve.cpp \
           $$QWTSRCDIR/qwt_plot_dict.cpp \
           $$QWTSRCDIR/qwt_plot_grid.cpp \
           $$QWTSRCDIR/qwt_plot_item.cpp \
           $$QWTSRCDIR/qwt_plot_layout.cpp \
           $$QWTSRCDIR/qwt_plot_magnifier.cpp \
           $$QWTSRCDIR/qwt_plot_marker.cpp \
           $$QWTSRCDIR/qwt_plot_panner.cpp \
           $$QWTSRCDIR/qwt_plot_picker.cpp \
           $$QWTSRCDIR/qwt_plot_rasteritem.cpp \
           $$QWTSRCDIR/qwt_plot_scaleitem.cpp \
           $$QWTSRCDIR/qwt_plot_seriesitem.cpp \
           $$QWTSRCDIR/qwt_plot_spectrogram.cpp \
           $$QWTSRCDIR/qwt_plot_svgitem.cpp \
           $$QWTSRCDIR/qwt_plot_xml.cpp \
           $$QWTSRCDIR/qwt_plot_zoomer.cpp \
           $$QWTSRCDIR/qwt_point_mapper.cpp \
           $$QWTSRCDIR/qwt_point_data.cpp \
           $$QWTSRCDIR/qwt_raster_data.cpp \
           $$QWTSRCDIR/qwt_round_scale_draw.cpp \
           $$QWTSRCDIR/qwt_scale_div.cpp \
           $$QWTSRCDIR/qwt_scale_draw.cpp \
           $$QWTSRCDIR/qwt_scale_engine.cpp \
           $$QWTSRCDIR/qwt_scale_map.cpp \
           $$QWTSRCDIR/qwt_scale_widget.cpp \
           $$QWTSRCDIR/qwt_series_data.cpp \
           $$QWTSRCDIR/qwt_slider.cpp \
           $$QWTSRCDIR/qwt_spline.cpp \
           $$QWTSRCDIR/qwt_symbol.cpp \
           $$QWTSRCDIR/qwt_text.cpp \
           $$QWTSRCDIR/qwt_text_engine.cpp \
           $$QWTSRCDIR/qwt_text_label.cpp \
           $$QWTSRCDIR/qwt_thermo.cpp \
           $$QWTSRCDIR/qwt_transform.cpp \
           $$QWTSRCDIR/qwt_wheel.cpp \
           $$QWTSRCDIR/qwt_widget_overlay.cpp


OPMAPCUSTOM= opmapcustom
DEPENDPATH += $$OPMAPCUSTOM
INCLUDEPATH += $$OPMAPCUSTOM

HEADERS+=   $$OPMAPCUSTOM/homeeditor.h\
            $$OPMAPCUSTOM/opmap_statusbar_widget.h\
            $$OPMAPCUSTOM/opmap_zoom_slider_widget.h\
            $$OPMAPCUSTOM/opmapgadgetwidget.h

SOURCES +=  $$OPMAPCUSTOM/homeeditor.cpp\
            $$OPMAPCUSTOM/opmap_statusbar_widget.cpp\
            $$OPMAPCUSTOM/opmap_zoom_slider_widget.cpp\
            $$OPMAPCUSTOM/opmapgadgetwidget.cpp



FORMS       += $$OPMAPCONTROL_DIR/mapwidget/mapripform.ui\
                frmmain.ui \
                $$CONTROL_DIR/frmmessagebox.ui\
                $$CONTROL_DIR/dataparameterview.ui \
                $$OPMAPCUSTOM/homeeditor.ui\
                $$OPMAPCUSTOM/opmap_statusbar_widget.ui\
                $$OPMAPCUSTOM/opmap_widget.ui\
                $$OPMAPCUSTOM/opmap_zoom_slider_widget.ui \
                controls/mainwindow.ui \
                controls/cmdwindow.ui \
                controls/paramrightcontrol.ui \
                controls/paramrightcontrol.ui \
#    controls/calibration/CompassConfig.ui \
#    controls/calibration/CompassMotorCalibrationDialog.ui
    controls/calibration/AccelCalibrationConfig.ui \
#    controls/calibration/CompassConfig.ui \
#    controls/calibration/CompassMotorCalibrationDialog.ui \
    controls/calibration/RadioCalibrationConfig.ui \
    controls/calibration/calibrateform.ui \
#    controls/calibration/dockcalibrateform.ui \
    controls/statuscontrol/signalcontrol.ui \
    controls/statuscontrol/satellitecontrol.ui \
    controls/statuscontrol/batterycontrol.ui \
    controls/calibration/compasscalibratecontrol.ui \
    controls/mapcontrols/cachemapmanager.ui \
    controls/mapcontrols/autoplan.ui \
    controls/AerialVideo/camera.ui \
    controls/customsettings.ui \
    controls/analisys/Linechart.ui \
    controls/testform.ui \
    controls/calibration/accelcalibrationofindependent.ui \
    controls/statuscontrol/refreshcontrol.ui \
    controls/InfoPanel/statusmonitor.ui \
    controls/statuscontrol/warningform.ui \
    controls/calibration/motortestform.ui \
    controls/statuscontrol/joystickstate.ui \
    controls/InfoPanel/critActInfo.ui \
    controls/calibration/esccaliform.ui \
    controls/firmwareform.ui \
    controls/InfoPanel/infomonitor.ui


#########################
ALGLibDir=controls/calibration/alglib
INCLUDEPATH += $$ALGLibDir
HEADERS +=     $$ALGLibDir/ap.h \
    $$ALGLibDir/alglibinternal.h\
    $$ALGLibDir/interpolation.h \
    $$ALGLibDir/optimization.h \
    $$ALGLibDir/linalg.h \
    $$ALGLibDir/alglibmisc.h \
    $$ALGLibDir/diffequations.h \
    $$ALGLibDir/integration.h \
    $$ALGLibDir/solvers.h \
    $$ALGLibDir/specialfunctions.cpp

SOURCES +=     $$ALGLibDir/ap.cpp \
    $$ALGLibDir/alglibinternal.cpp \
    $$ALGLibDir/interpolation.cpp \
    $$ALGLibDir/optimization.cpp \
    $$ALGLibDir/linalg.cpp \
    $$ALGLibDir/alglibmisc.cpp \
    $$ALGLibDir/diffequations.cpp \
    $$ALGLibDir/integration.cpp \
    $$ALGLibDir/solvers.cpp \
    $$ALGLibDir/specialfunctions.cpp
DEFINES += NOMINMAX
#########################

RESOURCES   += $$OPMAPCONTROL_DIR/mapwidget/mapresources.qrc\
               rc.qrc

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin


win32:RC_FILE=main.rc
win32:QMAKE_CXXFLAGS += /bigobj

QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG

#RESOURCES += \
#    rc.qrc

