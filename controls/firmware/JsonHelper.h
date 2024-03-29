﻿/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#ifndef JsonHelper_H
#define JsonHelper_H

#include <QJsonObject>
#include <QVariantList>
#include <QGeoCoordinate>

//class QmlObjectListModel;

class JsonHelper
{
public:
    /// Determines is the specified data is a json file
    /// @return true: file is json, false: file is not json
    static bool isJsonFile(const QByteArray&    bytes,          ///< json bytes
                           QJsonDocument&       jsonDoc,        ///< returned json document
                           QString&             errorString);   ///< error on parse failure

    /// Saves the standard file header the json object
    static void saveQGCJsonFileHeader(QJsonObject&      jsonObject, ///< root json object
                                      const QString&    fileType,   ///< file type for file
                                      int               version);   ///< version number for file

    /// Validates the standard parts of a QGC json file:
    ///     jsonFileTypeKey - Required and checked to be equal to expectedFileType
    ///     jsonVersionKey - Required and checked to be below supportedMajorVersion, supportedMinorVersion
    ///     jsonGroundStationKey - Required and checked to be string type
    /// @return false: validation failed, errorString set
    static bool validateQGCJsonFile(const QJsonObject&  jsonObject,             ///< json object to validate
                                    const QString&      expectedFileType,       ///< correct file type for file
                                    int                 minSupportedVersion,    ///< minimum supported version
                                    int                 maxSupportedVersion,    ///< maximum supported major version
                                    int                 &version,               ///< returned file version
                                    QString&            errorString);           ///< returned error string if validation fails

    /// Validates that the specified keys are in the object
    /// @return false: validation failed, errorString set
    static bool validateRequiredKeys(const QJsonObject& jsonObject, ///< json object to validate
                                     const QStringList& keys,       ///< keys which are required to be present
                                     QString& errorString);         ///< returned error string if validation fails

    /// Validates the types of specified keys are in the object
    /// @return false: validation failed, errorString set
    static bool validateKeyTypes(const QJsonObject& jsonObject,         ///< json object to validate
                                 const QStringList& keys,               ///< keys to validate
                                 const QList<QJsonValue::Type>& types,  ///< required type for each key, QJsonValue::Null specifies double with possible NaN
                                 QString& errorString);                 ///< returned error string if validation fails

    typedef struct {
        const char*         key;        ///< json key name
        QJsonValue::Type    type;       ///< required type for key, QJsonValue::Null specifies double with possible NaN
        bool                required;   ///< true: key must be present
    } KeyValidateInfo;

    static bool validateKeys(const QJsonObject& jsonObject, const QList<KeyValidateInfo>& keyInfo, QString& errorString);

    /// Loads a QGeoCoordinate
    ///     Stored as array [ lat, lon, alt ]
    /// @return false: validation failed
    static bool loadGeoCoordinate(const QJsonValue& jsonValue,              ///< json value to load from
                                  bool              altitudeRequired,       ///< true: altitude must be specified
                                  QGeoCoordinate&   coordinate,             ///< returned QGeoCordinate
                                  QString&          errorString,            ///< returned error string if load failure
                                  bool              geoJsonFormat = false); ///< if true, use [lon, lat], [lat, lon] otherwise

    /// Saves a QGeoCoordinate
    ///     Stored as array [ lat, lon, alt ]
    static void saveGeoCoordinate(const QGeoCoordinate& coordinate,     ///< QGeoCoordinate to save
                                  bool                  writeAltitude,  ///< true: write altitude to json
                                  QJsonValue&           jsonValue);     ///< json value to save to

    /// Loads a QGeoCoordinate
    ///     Stored as array [ lon, lat, alt ]
    /// @return false: validation failed
    static bool loadGeoJsonCoordinate(const QJsonValue& jsonValue,          ///< json value to load from
                                      bool              altitudeRequired,   ///< true: altitude must be specified
                                      QGeoCoordinate&   coordinate,         ///< returned QGeoCordinate
                                      QString&          errorString);       ///< returned error string if load failure

    /// Saves a QGeoCoordinate
    ///     Stored as array [ lon, lat, alt ]
    static void saveGeoJsonCoordinate(const QGeoCoordinate& coordinate,     ///< QGeoCoordinate to save
                                      bool                  writeAltitude,  ///< true: write altitude to json
                                      QJsonValue&           jsonValue);     ///< json value to save to

      /// Loads a list of QGeoCoordinates from a json array
    /// @return false: validation failed
    static bool loadGeoCoordinateArray(const QJsonValue&    jsonValue,              ///< json value which contains points
                                       bool                 altitudeRequired,       ///< true: altitude field must be specified
                                       QVariantList&        rgVarPoints,            ///< returned points
                                       QString&             errorString);           ///< returned error string if load failure
    static bool loadGeoCoordinateArray(const QJsonValue&        jsonValue,          ///< json value which contains points
                                       bool                     altitudeRequired,   ///< true: altitude field must be specified
                                       QList<QGeoCoordinate>&   rgPoints,           ///< returned points
                                       QString&                 errorString);       ///< returned error string if load failure

    /// Saves a list of QGeoCoordinates to a json array
    static void saveGeoCoordinateArray(const QVariantList&  rgVarPoints,            ///< points to save
                                       bool                 writeAltitude,          ///< true: write altitide value
                                       QJsonValue&          jsonValue);             ///< json value to save to
    static void saveGeoCoordinateArray(const QList<QGeoCoordinate>& rgPoints,       ///< points to save
                                       bool                         writeAltitude,  ///< true: write altitide value
                                       QJsonValue&                  jsonValue);     ///< json value to save to

    static bool parseEnum(const QJsonObject& jsonObject, QStringList& enumStrings, QStringList& enumValues, QString& errorString, QString valueName = QString());

    /// Returns NaN if the value is null, or if not, the double value
    static double possibleNaNJsonValue(const QJsonValue& value);

    static const char* jsonVersionKey;
    static const char* jsonGroundStationKey;
    static const char* jsonGroundStationValue;
    static const char* jsonFileTypeKey;

private:
    static QString _jsonValueTypeToString(QJsonValue::Type type);
    static bool _loadGeoCoordinate(const QJsonValue&    jsonValue,
                                   bool                 altitudeRequired,
                                   QGeoCoordinate&      coordinate,
                                   QString&             errorString,
                                   bool                 geoJsonFormat);
    static void _saveGeoCoordinate(const QGeoCoordinate&    coordinate,
                                   bool                     writeAltitude,
                                   QJsonValue&              jsonValue,
                                   bool                     geoJsonFormat);

    static const char*  _enumStringsJsonKey;
    static const char*  _enumValuesJsonKey;
};

#endif
