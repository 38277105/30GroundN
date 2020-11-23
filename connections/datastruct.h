#ifndef DATASTRUCT
#define DATASTRUCT
#include <stdint.h>
#include <QString>
typedef struct ParamStruct
{
   QString key;
   QString value;
   QString desc;
   QString type_Zh;
   uint8_t type;
   uint8_t count;
   QString IsVisible;
   bool    IsUpdated;
   QString ZhName;
}ParamData;

typedef struct MultiData
{
    QString En;//英文
    QString Zh_CN;//中文
}MultiData;

#endif // DATASTRUCT

