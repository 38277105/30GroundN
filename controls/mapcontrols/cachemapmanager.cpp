#include "cachemapmanager.h"
#include "ui_cachemapmanager.h"
CacheMapManager::CacheMapManager():
    ui(new Ui::CacheMapManager)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    m_fileName = "cacheMapRecord.xml";
    ReadRecordXml();
}

CacheMapManager::~CacheMapManager()
{
    delete ui;
    for(QList<CacheMapInfoData*>::iterator iter = m_recordList.begin(); iter != m_recordList.end(); iter++){
        delete *iter;
        *iter = NULL;
    }
}

void CacheMapManager::initZooms(){

}

void CacheMapManager::createRecord(){

}
void CacheMapManager::UpdateRecordXml(){

}

int  CacheMapManager::ReadRecordXml(){
    QFile file(m_fileName);
    CMDataType dtype;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        CacheMapInfoData *record;
        QXmlStreamReader reader(&file);
        while (!reader.atEnd()&& !reader.hasError()){
            reader.readNext();
            if(reader.isStartElement()){
                if(reader.name() == "Record"){
                    record = new CacheMapInfoData;
                }
                if(reader.name() == "LeftUp"){
                    dtype = LEFTUP;
                }
                if(reader.name() == "LeftDown"){
                    dtype = LEFTDOWN;
                }
                if(reader.name() == "RightUp"){
                    dtype = RIGHTUP;
                }
                if(reader.name() == "RightDown"){
                    dtype = RIGHTDOWN;
                }
                if(reader.name() == "Name"){
                    record->strName = reader.readElementText();
                }
                if(reader.name() == "Zoom"){
                    record->strZoom = reader.readElementText();
                }
                if(reader.name() == "Lat" && dtype == LEFTUP){
                    record->strLeftUpLat = reader.readElementText();
                }
                if(reader.name() == "Lng" && dtype == LEFTUP){
                    record->strLeftUpLng = reader.readElementText();
                }
                if(reader.name() == "Lat" && dtype == LEFTDOWN){
                    record->strLeftDownLat = reader.readElementText();
                }
                if(reader.name() == "Lng" && dtype == LEFTDOWN){
                    record->strLeftDownLng = reader.readElementText();
                }
                if(reader.name() == "Lat" && dtype == RIGHTUP){
                    record->strRightUpLat = reader.readElementText();
                }
                if(reader.name() == "Lng" && dtype == RIGHTUP){
                    record->strRightUpLng = reader.readElementText();
                }
                if(reader.name() == "Lat" && dtype == RIGHTDOWN){
                    record->strRightDownLat = reader.readElementText();
                }
                if(reader.name() == "Lng" && dtype == RIGHTDOWN){
                    record->strRightDownLng = reader.readElementText();
                }
            }else if(reader.isEndElement()){
                if(reader.name() == "Record"){
                    m_recordList.push_back(record);
                }
            }
        }
    }else{
        qDebug()<<"read inbox file error...";
    }
    file.close();
    return true;
}

int  CacheMapManager::WriteRecordXml(){
    QFile file(m_fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.setCodec("gb18030");
        writer.writeStartElement("DATA");
        for(QList<CacheMapInfoData*>::iterator iter = m_recordList.begin(); iter != m_recordList.end(); iter++){
            writer.writeStartElement("Record");
            writer.writeTextElement("Name", (*iter)->strName);
            writer.writeTextElement("Zoom", (*iter)->strZoom);
            writer.writeStartElement("LeftUp");
            writer.writeTextElement("Lat", (*iter)->strLeftUpLat);
            writer.writeTextElement("Lng", (*iter)->strLeftUpLng);
            writer.writeEndElement();

            writer.writeStartElement("LeftDown");
            writer.writeTextElement("Lat", (*iter)->strLeftDownLat);
            writer.writeTextElement("Lng", (*iter)->strLeftDownLng);
            writer.writeEndElement();

            writer.writeStartElement("RightUp");
            writer.writeTextElement("Lat", (*iter)->strRightUpLat);
            writer.writeTextElement("Lng", (*iter)->strRightUpLng);
            writer.writeEndElement();

            writer.writeStartElement("RightDown");
            writer.writeTextElement("Lat", (*iter)->strRightDownLat);
            writer.writeTextElement("Lng", (*iter)->strRightDownLng);
            writer.writeEndElement();

            writer.writeEndElement();
        }
        writer.writeEndElement();
        writer.writeEndDocument();
    }else{
        qDebug() << "write inbox file error...";
    }
    file.close();
    return true;
}

int  CacheMapManager::UpdateShowData(int index){
    if(m_recordList.isEmpty() || index >= m_recordList.size()){
        return true;
    }
    ui->m_lineEdit_leftUpLng->setText(m_recordList[index]->strLeftUpLng);
    ui->m_lineEdit_leftUpLat->setText(m_recordList[index]->strLeftUpLat);
    ui->m_lineEdit_leftDownLng->setText(m_recordList[index]->strLeftDownLng);
    ui->m_lineEdit_leftDownLat->setText(m_recordList[index]->strLeftDownLat);
    ui->m_lineEdit_rightUpLng->setText(m_recordList[index]->strRightUpLng);
    ui->m_lineEdit_rightUpLat->setText(m_recordList[index]->strRightUpLat);
    ui->m_lineEdit_rightDownLng->setText(m_recordList[index]->strRightDownLng);
    ui->m_lineEdit_rightDownLat->setText(m_recordList[index]->strRightDownLat);
    return true;
}
