#include "settingmanager.h"
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>
#include <QTextStream>
#include <QDomNode>
#include <QTextCodec>

static SettingManager* m_settingManager=NULL;

SettingManager::SettingManager(QObject *parent) : QObject(parent)
{
     m_plist=new QList<ParamData>();
     m_full_plist=new QList<ParamData>();
     m_types[1]=QString::fromLocal8Bit("8位无符号整型");
     m_types[2]=QString::fromLocal8Bit("8位整型");
     m_types[3]=QString::fromLocal8Bit("16位无符号整型");
     m_types[4]=QString::fromLocal8Bit("16位整型");
     m_types[5]=QString::fromLocal8Bit("32位无符号整型");
     m_types[6]=QString::fromLocal8Bit("32位整型");
     m_types[7]=QString::fromLocal8Bit("64位无符号整型");
     m_types[8]=QString::fromLocal8Bit("64位整型");
     m_types[9]=QString::fromLocal8Bit("32位浮点型");
     m_types[10]=QString::fromLocal8Bit("64位双精度浮点型");
     m_types[11]=QString::fromLocal8Bit("枚举结束标志");

     m_settingManager=this;
}

SettingManager::~SettingManager()
{
   delete m_plist;
}

SettingManager* SettingManager::Instance()
{
  if(m_settingManager==NULL)
      m_settingManager=new SettingManager();
  return m_settingManager;
}

QList<ParamData>* SettingManager::LoadSettings(QString file_name)
{
  QList<ParamData>*  temp=new QList<ParamData>();
    if(m_plist)
    m_plist->clear();

    if(file_name.isEmpty())

        return NULL;

    QFile file(file_name);

    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::information(NULL, QString("title"), QString("open error!"));

        return NULL;
    }

    QTextStream vReadStream(&file);
    QTextCodec *vCodec = QTextCodec::codecForName("GBK");
    vReadStream.setCodec(vCodec);
    QString vXmlDataStr = vReadStream.readAll();
    file.close();


    QDomDocument document;



    if(!document.setContent(vXmlDataStr.toLocal8Bit()))
    {
        QMessageBox::information(NULL, QString("title"), QString("parse file failed "));
        return NULL;
    }
    if(document.isNull())
    {
        QMessageBox::information(NULL, QString("title"), QString("document is null!"));
        return NULL;
    }
    QDomElement root = document.documentElement();
    QDomNode settingItem = root.firstChild();
    while(!settingItem.isNull())
    {
        ParamData t_data;
        t_data.key=settingItem.toElement().attribute("Name");
        if(settingItem.toElement().attribute("ZhName")!=NULL||settingItem.toElement().attribute("ZhName")!="")
        {
           t_data.ZhName=settingItem.toElement().attribute("ZhName");
        }
        t_data.count=settingItem.toElement().attribute("Count").toInt();
        int type_code=settingItem.toElement().attribute("Type").toInt();
        t_data.type_Zh=m_types[type_code];
        t_data.value=settingItem.toElement().attribute("Value");
        t_data.type=type_code;
        t_data.IsVisible=settingItem.toElement().attribute("IsVisible");
        t_data.desc=settingItem.toElement().attribute("Desc");
        m_plist->append(t_data);
        temp->append(t_data);
        settingItem=settingItem.nextSibling();
    }




    return temp;

}

void SettingManager::SaveSetting(QList<ParamData>* plist,QString filePath)
{
    m_full_plist->clear();
     m_full_plist->append(*plist);
    QDomDocument doc;

    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"gb2312\"");

    doc.appendChild(instruction);

    QDomElement root = doc.createElement("Parameters");

    doc.appendChild(root);
    QList<ParamData>::const_iterator m_begin=plist->begin();

    for(m_begin;m_begin!=plist->end();m_begin++)
    {

        QDomElement setItem = doc.createElement("SettingItem");

        setItem.setAttribute("Name", (*m_begin).key);
        setItem.setAttribute("Value",(*m_begin).value);
        setItem.setAttribute("Type", (*m_begin).type);
        setItem.setAttribute("Count", (*m_begin).count);
        setItem.setAttribute("Desc", (*m_begin).desc);
        setItem.setAttribute("IsVisible", "True");
        root.appendChild(setItem);
    }
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))

    return ;

    QTextStream out(&file);

    out.setCodec("GBK");

    doc.save(out,4,QDomNode::EncodingFromTextStream);

    file.close();

}

QList<ParamData>* SettingManager::getParametersFromControl()
{
      return m_plist;
}

 QList<ParamData>* SettingManager::getFullParameters()
 {
    return m_full_plist;
 }

QList<ParamData>* SettingManager::LoadPartSettings(QString config,QString filePath)
{
  QList<ParamData>* fullargs= LoadSettings(filePath); //param
  //QList<ParamData>* partargs= LoadSettings(config); //display
  if(!fullargs)
      return NULL;

  QList<ParamData>* displayList=new QList<ParamData>();
//  for(int i=0;i<partargs->size();i++)
//  {
//      for(int j=0;j<fullargs->size();j++)
//      {
//          if(partargs->at(i).key==fullargs->at(j).key)
//          {
//              //fullargs->at(j).desc.sprintf("%s",partargs->at(i).desc.toLocal8Bit().data());
//              //memccpy()
//              displayList->append(fullargs->at(j));
//          }
//      }
//  }
 QList<ParamData>::iterator itr= fullargs->begin();

 for(;itr!=fullargs->end();itr++)
 {
     QList<ParamData>::iterator full_itr= fullargs->begin();
     for(;full_itr!=fullargs->end();full_itr++)
     {
        if((*itr).key==(*full_itr).key)
        {
            (*full_itr).ZhName=(*itr).key;
            (*full_itr).desc=(*itr).desc;
            displayList->append(*full_itr);
        }
     }
 }

  delete fullargs;
  //delete partargs;

  return displayList;
}


QList<MultiData>* SettingManager::getMultilang()
{
    QList<MultiData>* mylist=new QList<MultiData>();
    QFile file("Data/EnToZH.xml");

    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::information(NULL, QString("title"), QString("open error!"));

        return NULL;
    }

    QTextStream vReadStream(&file);
    QTextCodec *vCodec = QTextCodec::codecForName("GBK");
    vReadStream.setCodec(vCodec);
    QString vXmlDataStr = vReadStream.readAll();
    file.close();


    QDomDocument document;



    if(!document.setContent(vXmlDataStr.toLocal8Bit()))
    {
        QMessageBox::information(NULL, QString("title"), QString("parse file failed "));
        return NULL;
    }
    if(document.isNull())
    {
        QMessageBox::information(NULL, QString("title"), QString("document is null!"));
        return NULL;
    }
    QDomElement root = document.documentElement();
    QDomNode settingItem = root.firstChild();
    while(!settingItem.isNull())
    {
        MultiData t_data;
        t_data.En=settingItem.toElement().attribute("En");
        t_data.Zh_CN=settingItem.toElement().attribute("Zh");
        mylist->append(t_data);
        settingItem=settingItem.nextSibling();
    }
    return mylist;
}



bool SettingManager::_ParamExist(char* argName)
{
    QList<ParamData>* temp= getFullParameters();
    for(int i=0;i<temp->size();i++)
    {
          if(temp->at(i).key==QString::fromLocal8Bit(argName))
          {
            return true;
          }
    }
     return false;
}
