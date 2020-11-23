/****************************************************************************
 *
 *   (c) 2009-2016 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/


/// @file
///     @brief PX4 Firmware Upgrade UI
///     @author Don Gagne <don@thegagnes.com>

#include "FirmwareUpgradeController.h"
#include "Bootloader.h"
#include <QStandardPaths>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkProxy>
#include <QFileDialog>
#include <QDebug>
#include <qserialportinfo>

struct FirmwareToUrlElement_t {
    FirmwareUpgradeController::AutoPilotStackType_t    stackType;
    FirmwareUpgradeController::FirmwareType_t          firmwareType;
    FirmwareUpgradeController::FirmwareVehicleType_t   vehicleType;
    QString                                            url;
};

uint qHash(const FirmwareUpgradeController::FirmwareIdentifier& firmwareId)
{
    return static_cast<uint>(( firmwareId.autopilotStackType |
             (firmwareId.firmwareType << 8) |
             (firmwareId.firmwareVehicleType << 16) ));
}

/// @Brief Constructs a new FirmwareUpgradeController Widget. This widget is used within the PX4VehicleConfig set of screens.
FirmwareUpgradeController::FirmwareUpgradeController(void)
    :_downloadManager                  (nullptr)
    , _downloadNetworkReply             (nullptr)
    //, _statusLog                        (nullptr)
    , _selectedFirmwareType             (StableFirmware)
    , _image                            (nullptr)
    , _apmBoardDescriptionReplaceText   ("<APMBoardDescription>")
{

    _threadController = new PX4FirmwareUpgradeThreadController(this);
    Q_CHECK_PTR(_threadController);

    //connect(_threadController, &PX4FirmwareUpgradeThreadController::foundBoard,             this, &FirmwareUpgradeController::_foundBoard);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::noBoardFound,           this, &FirmwareUpgradeController::_noBoardFound);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::boardGone,              this, &FirmwareUpgradeController::_boardGone);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::foundBootloader,        this, &FirmwareUpgradeController::_foundBootloader);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::bootloaderSyncFailed,   this, &FirmwareUpgradeController::_bootloaderSyncFailed);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::error,                  this, &FirmwareUpgradeController::_error);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::updateProgress,         this, &FirmwareUpgradeController::_updateProgress);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::status,                 this, &FirmwareUpgradeController::_status);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::eraseStarted,           this, &FirmwareUpgradeController::_eraseStarted);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::eraseComplete,          this, &FirmwareUpgradeController::_eraseComplete);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::flashComplete,          this, &FirmwareUpgradeController::_flashComplete);
    connect(_threadController, &PX4FirmwareUpgradeThreadController::updateProgress,         this, &FirmwareUpgradeController::_updateProgress);
    
    connect(&_eraseTimer, &QTimer::timeout, this, &FirmwareUpgradeController::_eraseProgressTick);

    _initFirmwareHash();
    //_determinePX4StableVersion();

}

FirmwareUpgradeController::~FirmwareUpgradeController()
{
    //qgcApp()->toolbox()->linkManager()->setConnectionsAllowed();
}
void FirmwareUpgradeController::cancel(void)
{
    _eraseTimer.stop();
    _threadController->cancel();
}


void FirmwareUpgradeController::_noBoardFound(void)
{
    emit noBoardFound();
}

void FirmwareUpgradeController::_boardGone(void)
{
    emit boardGone();
}

/// @brief Called when the bootloader is connected to by the findBootloader process. Moves the state machine
///         to the next step.
void FirmwareUpgradeController::_foundBootloader(int bootloaderVersion, int boardID, int flashSize)
{
    _bootloaderFound = true;
    _bootloaderVersion = static_cast<uint32_t>(bootloaderVersion);
    _bootloaderBoardID = static_cast<uint32_t>(boardID);
    _bootloaderBoardFlashSize = static_cast<uint32_t>(flashSize);
    
    _appendStatusLog(tr("Connected to bootloader:"));
    _appendStatusLog(tr("  Version: %1").arg(_bootloaderVersion));
    _appendStatusLog(tr("  Board ID: %1").arg(_bootloaderBoardID));
    _appendStatusLog(tr("  Flash size: %1").arg(_bootloaderBoardFlashSize));
    _firmwareDownloadFinished("",_imageFileName);
}


/// @brief intializes the firmware hashes with proper urls.
/// This happens only once for a class instance first time when it is needed.
void FirmwareUpgradeController::_initFirmwareHash()
{
    //this->availableSerialPorts = this->availableSerialPorts();
}

/// @brief Called when the findBootloader process is unable to sync to the bootloader. Moves the state
///         machine to the appropriate error state.
void FirmwareUpgradeController::_bootloaderSyncFailed(void)
{
    _errorCancel("Unable to sync with bootloader.");
}

QHash<FirmwareUpgradeController::FirmwareIdentifier, QString>* FirmwareUpgradeController::_firmwareHashForBoardId(int boardId)
{
    QHash<FirmwareUpgradeController::FirmwareIdentifier, QString>* rgFirmware;

    switch (boardId) {
    case Bootloader::boardIDPX4Flow:
        rgFirmware = &_rgPX4FLowFirmware;
        break;
    case Bootloader::boardIDPX4FMUV2:
        rgFirmware = &_rgPX4FMUV2Firmware;
        break;
    case Bootloader::boardIDPX4FMUV3:
        rgFirmware = &_rgFMUV3Firmware;
        break;
    case Bootloader::boardIDPX4FMUV4:
        rgFirmware = &_rgFMUV4Firmware;
        break;
    case Bootloader::boardIDPX4FMUV4PRO:
        rgFirmware = &_rgFMUV4PROFirmware;
        break;
    case Bootloader::boardIDPX4FMUV5:
        rgFirmware = &_rgFMUV5Firmware;
        break;
    case Bootloader::boardIDAeroCore:
        rgFirmware = &_rgAeroCoreFirmware;
        break;
    case Bootloader::boardIDAUAVX2_1:
        rgFirmware = &_rgAUAVX2_1Firmware;
        break;
    case Bootloader::boardIDMINDPXFMUV2:
        rgFirmware = &_rgMindPXFMUV2Firmware;
        break;
    case Bootloader::boardIDTAPV1:
        rgFirmware = &_rgTAPV1Firmware;
        break;
    case Bootloader::boardIDASCV1:
        rgFirmware = &_rgASCV1Firmware;
        break;
    case Bootloader::boardIDCrazyflie2:
        rgFirmware = &_rgCrazyflie2Firmware;
        break;
    case Bootloader::boardIDOmnibusF4SD:
        rgFirmware = &_rgOmnibusF4SDFirmware;
        break;
    case Bootloader::boardIDFMUK66V3:
        rgFirmware = &_rgFMUK66V3Firmware;
        break;
    case Bootloader::boardID3DRRadio:
        rgFirmware = &_rg3DRRadioFirmware;
        break;
    default:
        rgFirmware = Q_NULLPTR;
    }

    if (rgFirmware) {
        _rgFirmwareDynamic = *rgFirmware;
    } else {
        _rgFirmwareDynamic.clear();
    }
/*
    // Special case handling for ArduPilot named ChibiOS board
    if (_foundBoardInfo.manufacturer() == QStringLiteral("ArduPilot") || _foundBoardInfo.manufacturer() == QStringLiteral("Hex/ProfiCNC")) {
        // Remove the ChibiOS by board id entries from the list
        for (const FirmwareIdentifier& firmwareId: _rgFirmwareDynamic.keys()) {
            switch (firmwareId.firmwareVehicleType) {
            case CopterChibiOSFirmware:
            case HeliChibiOSFirmware:
            case PlaneChibiOSFirmware:
            case RoverChibiOSFirmware:
            case SubChibiOSFirmware:
                _rgAPMChibiosReplaceNamedBoardFirmware.remove(firmwareId);
                break;
            default:
                break;
            }
        }

        // Add the ChibiOS by board description entries to the list
        for (const FirmwareIdentifier& firmwareId: _rgAPMChibiosReplaceNamedBoardFirmware.keys()) {
            QString namedUrl = _rgAPMChibiosReplaceNamedBoardFirmware[firmwareId];
            _rgFirmwareDynamic.insert(firmwareId, namedUrl.replace(_apmBoardDescriptionReplaceText, _foundBoardInfo.description().left(_foundBoardInfo.description().length() - 3)));
        }
    }
*/
    return &_rgFirmwareDynamic;
}

/// @brief Prompts the user to select a firmware file if needed and moves the state machine to the next state.
void FirmwareUpgradeController::_getFirmwareFile(FirmwareIdentifier firmwareId)
{
    QHash<FirmwareIdentifier, QString>* prgFirmware = _firmwareHashForBoardId(static_cast<int>(_bootloaderBoardID));
    
    if (!prgFirmware && firmwareId.firmwareType != CustomFirmware) {
        _errorCancel(tr("Attempting to flash an unknown board type, you must select 'Custom firmware file'"));
        return;
    }
    
    if (firmwareId.firmwareType == CustomFirmware) {
        _firmwareFilename = QFileDialog::getOpenFileName(nullptr,                                                                // Parent to main window
                                                            tr("Select Firmware File"),                                             // Dialog Caption
                                                            QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),    // Initial directory
                                                            tr("Firmware Files (*.px4 *.apj *.bin *.ihx)"));                        // File filter
    } else {
        if (prgFirmware->contains(firmwareId)) {
            _firmwareFilename = prgFirmware->value(firmwareId);
        } else {
            _errorCancel(tr("Unable to find specified firmware download location"));
            return;
        }
    }
    
    if (_firmwareFilename.isEmpty()) {
        _errorCancel(tr("No firmware file selected"));
    } else {
        //_downloadFirmware();
    }
}
void FirmwareUpgradeController::flashFirmware(QString fileName,QString portName,QString baud,bool isUSB)
{
    //_threadController->openBootloader(portName);
    _appendStatusLog("flashing ---",0);
    qDebug()<<fileName<<";"<<portName<<";"<<baud<<";"<<isUSB;
    if(fileName=="")
    {
        emit infoMsg("flash image file is empty!");
        return;
    }
    else if(portName=="")
    {
        emit infoMsg("serial port is empty!");
        return;
    }
    _imageFileName = fileName;
    QUrl filePath(fileName);
    if(filePath.isLocalFile())
    {
        _imageFileName = filePath.toLocalFile();
    }
    emit infoMsg("try to flashing,image file:"+fileName+", port:"+portName+", baudrate:"+baud+", USB:"+isUSB);
    _portName =  portName;
    _threadController->openBootloader(portName,isUSB);
}

/// @brief Updates the progress indicator while downloading
void FirmwareUpgradeController::_firmwareDownloadProgress(qint64 curr, qint64 total)
{
    // Take care of cases where 0 / 0 is emitted as error return value
    if (total > 0) {
        //_progressBar->setProperty("value", static_cast<float>(curr) / static_cast<float>(total));
    }
}

/// @brief Called when the firmware download completes.
void FirmwareUpgradeController::_firmwareDownloadFinished(QString remoteFile, QString localFile)
{
    Q_UNUSED(remoteFile);

    _appendStatusLog(tr("Download complete"));
    
    FirmwareImage* image = new FirmwareImage(this);
    
    connect(image, &FirmwareImage::statusMessage, this, &FirmwareUpgradeController::_status);
    connect(image, &FirmwareImage::errorMessage, this, &FirmwareUpgradeController::_error);
    
    if (!image->load(localFile, _bootloaderBoardID)) {
        _errorCancel(tr("Image load failed"));
        return;
    }
    
    // We can't proceed unless we have the bootloader
    if (!_bootloaderFound) {
        _errorCancel(tr("Bootloader not found"));
        return;
    }
    
    if (_bootloaderBoardFlashSize != 0 && image->imageSize() > _bootloaderBoardFlashSize) {
        _errorCancel(tr("Image size of %1 is too large for board flash size %2").arg(image->imageSize()).arg(_bootloaderBoardFlashSize));
        return;
    }

    _threadController->flash(image);
}

/// @brief Called when an error occurs during download
void FirmwareUpgradeController::_firmwareDownloadError(QString errorMsg)
{
    _errorCancel(errorMsg);
}

/// @brief returns firmware type as a string
QString FirmwareUpgradeController::firmwareTypeAsString(FirmwareType_t type) const
{
    switch (type) {
    case StableFirmware:
        return "stable";
    case DeveloperFirmware:
        return "developer";
    case BetaFirmware:
        return "beta";
    default:
        return "custom";
    }
}

/// @brief Signals completion of one of the specified bootloader commands. Moves the state machine to the
///         appropriate next step.
void FirmwareUpgradeController::_flashComplete(void)
{
    delete _image;
    _image = nullptr;
    
    _appendStatusLog(tr("Upgrade complete"), true);
    _appendStatusLog("------------------------------------------", false);
    emit flashComplete();
    //qgcApp()->toolbox()->linkManager()->setConnectionsAllowed();
}

void FirmwareUpgradeController::_error(const QString& errorString)
{
    delete _image;
    _image = nullptr;
    
    _errorCancel(QString("Error: %1").arg(errorString));
}

void FirmwareUpgradeController::_status(const QString& statusString)
{
    _appendStatusLog(statusString);
}

/// @brief Updates the progress bar from long running bootloader commands
void FirmwareUpgradeController::_updateProgress(int curr, int total)
{
    emit updateProgress(static_cast<float>(curr) / static_cast<float>(total));
}

/// @brief Moves the progress bar ahead on tick while erasing the board
void FirmwareUpgradeController::_eraseProgressTick(void)
{
    _eraseTickCount++;
    emit updateProgress(static_cast<float>(_eraseTickCount*_eraseTickMsec) / static_cast<float>(_eraseTotalMsec));
}

/// Appends the specified text to the status log area in the ui
void FirmwareUpgradeController::_appendStatusLog(const QString& text, bool critical)
{

    
    QString varText;
    
    if (critical) {
        varText = QString("<font color=\"yellow\">%1</font>").arg(text);
    } else {
        varText = text;
    }
    emit infoMsg(varText);

}

void FirmwareUpgradeController::_errorCancel(const QString& msg)
{
    _appendStatusLog(msg, false);
    _appendStatusLog(tr("Upgrade cancelled"), true);
    _appendStatusLog("------------------------------------------", false);
    emit error();
    cancel();
    //qgcApp()->toolbox()->linkManager()->setConnectionsAllowed();
}

void FirmwareUpgradeController::_eraseStarted(void)
{
    // We set up our own progress bar for erase since the erase command does not provide one
    _eraseTickCount = 0;
    _eraseTimer.start(_eraseTickMsec);
}

void FirmwareUpgradeController::_eraseComplete(void)
{
    _eraseTimer.stop();
}


void FirmwareUpgradeController::_apmVersionDownloadFinished(QString remoteFile, QString localFile)
{
    //qCDebug(FirmwareUpgradeLog) << "Download complete" << remoteFile << localFile;

    // Now read the version file and pull out the version string

    QFile versionFile(localFile);
    versionFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&versionFile);
    QString versionContents = stream.readAll();

    QString version;
    QRegularExpression re("APMVERSION: (.*)$");
    QRegularExpressionMatch match = re.match(versionContents);
    if (match.hasMatch()) {
        version = match.captured(1);
    }

    if (version.isEmpty()) {
        qWarning() << "Unable to parse version info from file" << remoteFile;
        sender()->deleteLater();
        return;
    }

    // In order to determine the firmware and vehicle type for this file we find the matching entry in the firmware list

    QHash<FirmwareIdentifier, QString>* prgFirmware = _firmwareHashForBoardId(static_cast<int>(_bootloaderBoardID));

    QString remotePath = QFileInfo(remoteFile).path();
    for (FirmwareIdentifier firmwareId: prgFirmware->keys()) {
        if (remotePath == QFileInfo((*prgFirmware)[firmwareId]).path()) {
           // qCDebug(FirmwareUpgradeLog) << "Adding version to map, version:firwmareType:vehicleType" << version << firmwareId.firmwareType << firmwareId.firmwareVehicleType;
            _apmVersionMap[firmwareId.firmwareType][firmwareId.firmwareVehicleType] = version;
        }
    }

    emit apmAvailableVersionsChanged();
    sender()->deleteLater();
}

void FirmwareUpgradeController::setSelectedFirmwareType(FirmwareType_t firmwareType)
{
    _selectedFirmwareType = firmwareType;
    emit selectedFirmwareTypeChanged(_selectedFirmwareType);
    emit apmAvailableVersionsChanged();
}
QStringList FirmwareUpgradeController::availableSerialPorts(void)
{
    QStringList aPorts;
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts())
    {
        aPorts.append(qspinfo.portName());
    }
    //QStringListModel portsModel;
    //portsModel.setStringList(aPorts);
    return aPorts;
}
QStringList FirmwareUpgradeController::apmAvailableVersions(void)
{
    QStringList                     list;
    QList<FirmwareVehicleType_t>    vehicleTypes;

    // This allows us to force the order of the combo box display
    vehicleTypes << CopterChibiOSFirmware << HeliChibiOSFirmware << PlaneChibiOSFirmware << RoverChibiOSFirmware << SubChibiOSFirmware << CopterFirmware << HeliFirmware << PlaneFirmware << RoverFirmware << SubFirmware;

    _apmVehicleTypeFromCurrentVersionList.clear();

    for (FirmwareVehicleType_t vehicleType: vehicleTypes) {
        if (_apmVersionMap[_selectedFirmwareType].contains(vehicleType)) {
            QString version;

            switch (vehicleType) {
            case CopterFirmware:
                version = tr("NuttX - MultiRotor:");
                break;
            case HeliFirmware:
                version = tr("NuttX - Heli:");
                break;
            case CopterChibiOSFirmware:
                version = tr("ChibiOS- MultiRotor:");
                break;
            case HeliChibiOSFirmware:
                version = tr("ChibiOS - Heli:");
                break;
            case PlaneChibiOSFirmware:
            case RoverChibiOSFirmware:
            case SubChibiOSFirmware:
                version = tr("ChibiOS - ");
                break;
            default:
                version = tr("NuttX - ");
                break;
            }

            version += _apmVersionMap[_selectedFirmwareType][vehicleType];
            _apmVehicleTypeFromCurrentVersionList.append(vehicleType);

            list << version;
        }
    }

    return list;
}


void FirmwareUpgradeController::_px4ReleasesGithubDownloadFinished(QString remoteFile, QString localFile)
{
    /*
    Q_UNUSED(remoteFile);

    QFile jsonFile(localFile);
    if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCWarning(FirmwareUpgradeLog) << "Unable to open github px4 releases json file" << localFile << jsonFile.errorString();
        return;
    }
    QByteArray bytes = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError jsonParseError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
        qCWarning(FirmwareUpgradeLog) <<  "Unable to open px4 releases json document" << localFile << jsonParseError.errorString();
        return;
    }

    // Json should be an array of release objects
    if (!doc.isArray()) {
        qCWarning(FirmwareUpgradeLog) <<  "px4 releases json document is not an array" << localFile;
        return;
    }
    QJsonArray releases = doc.array();

    // The first release marked prerelease=false is stable
    // The first release marked prerelease=true is beta
    bool foundStable = false;
    bool foundBeta = false;
    for (int i=0; i<releases.count() && (!foundStable || !foundBeta); i++) {
        QJsonObject release = releases[i].toObject();
        if (!foundStable && !release["prerelease"].toBool()) {
            _px4StableVersion = release["name"].toString();
            emit px4StableVersionChanged(_px4StableVersion);
            //qCDebug(FirmwareUpgradeLog()) << "Found px4 stable version" << _px4StableVersion;
            foundStable = true;
        } else if (!foundBeta && release["prerelease"].toBool()) {
            _px4BetaVersion = release["name"].toString();
            emit px4StableVersionChanged(_px4BetaVersion);
            //qCDebug(FirmwareUpgradeLog()) << "Found px4 beta version" << _px4BetaVersion;
            foundBeta = true;
        }
    }

    if (!foundStable) {
        //qCDebug(FirmwareUpgradeLog()) << "Unable to find px4 stable version" << localFile;
    }
    if (!foundBeta) {
        //qCDebug(FirmwareUpgradeLog()) << "Unable to find px4 beta version" << localFile;
    }
    */
}

void FirmwareUpgradeController::_px4ReleasesGithubDownloadError(QString errorMsg)
{
    //qCWarning(FirmwareUpgradeLog) << "PX4 releases github download failed" << errorMsg;
}
