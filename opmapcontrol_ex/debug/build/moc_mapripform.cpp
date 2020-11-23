/****************************************************************************
** Meta object code from reading C++ file 'mapripform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mapwidget/mapripform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapripform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MapRipForm_t {
    QByteArrayData data[10];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapRipForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapRipForm_t qt_meta_stringdata_MapRipForm = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MapRipForm"
QT_MOC_LITERAL(1, 11, 13), // "SetPercentage"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "perc"
QT_MOC_LITERAL(4, 31, 11), // "SetProvider"
QT_MOC_LITERAL(5, 43, 4), // "prov"
QT_MOC_LITERAL(6, 48, 4), // "zoom"
QT_MOC_LITERAL(7, 53, 16), // "SetNumberOfTiles"
QT_MOC_LITERAL(8, 70, 5), // "total"
QT_MOC_LITERAL(9, 76, 6) // "actual"

    },
    "MapRipForm\0SetPercentage\0\0perc\0"
    "SetProvider\0prov\0zoom\0SetNumberOfTiles\0"
    "total\0actual"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapRipForm[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x0a /* Public */,
       4,    2,   32,    2, 0x0a /* Public */,
       7,    2,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    9,

       0        // eod
};

void MapRipForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MapRipForm *_t = static_cast<MapRipForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetPercentage((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 1: _t->SetProvider((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        case 2: _t->SetNumberOfTiles((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject MapRipForm::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MapRipForm.data,
      qt_meta_data_MapRipForm,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MapRipForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapRipForm::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MapRipForm.stringdata0))
        return static_cast<void*>(const_cast< MapRipForm*>(this));
    return QWidget::qt_metacast(_clname);
}

int MapRipForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
