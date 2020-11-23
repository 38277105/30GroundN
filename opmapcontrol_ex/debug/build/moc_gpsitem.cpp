/****************************************************************************
** Meta object code from reading C++ file 'gpsitem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mapwidget/gpsitem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gpsitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_mapcontrol__GPSItem_t {
    QByteArrayData data[9];
    char stringdata0[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mapcontrol__GPSItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mapcontrol__GPSItem_t qt_meta_stringdata_mapcontrol__GPSItem = {
    {
QT_MOC_LITERAL(0, 0, 19), // "mapcontrol::GPSItem"
QT_MOC_LITERAL(1, 20, 18), // "UAVReachedWayPoint"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 14), // "waypointnumber"
QT_MOC_LITERAL(4, 55, 13), // "WayPointItem*"
QT_MOC_LITERAL(5, 69, 8), // "waypoint"
QT_MOC_LITERAL(6, 78, 19), // "UAVLeftSafetyBouble"
QT_MOC_LITERAL(7, 98, 22), // "internals::PointLatLng"
QT_MOC_LITERAL(8, 121, 8) // "position"

    },
    "mapcontrol::GPSItem\0UAVReachedWayPoint\0"
    "\0waypointnumber\0WayPointItem*\0waypoint\0"
    "UAVLeftSafetyBouble\0internals::PointLatLng\0"
    "position"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mapcontrol__GPSItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x06 /* Public */,
       6,    1,   29,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void mapcontrol::GPSItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GPSItem *_t = static_cast<GPSItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UAVReachedWayPoint((*reinterpret_cast< const int(*)>(_a[1])),(*reinterpret_cast< WayPointItem*(*)>(_a[2]))); break;
        case 1: _t->UAVLeftSafetyBouble((*reinterpret_cast< const internals::PointLatLng(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< WayPointItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GPSItem::*_t)(int const & , WayPointItem * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GPSItem::UAVReachedWayPoint)) {
                *result = 0;
            }
        }
        {
            typedef void (GPSItem::*_t)(internals::PointLatLng const & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GPSItem::UAVLeftSafetyBouble)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject mapcontrol::GPSItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_mapcontrol__GPSItem.data,
      qt_meta_data_mapcontrol__GPSItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *mapcontrol::GPSItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mapcontrol::GPSItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_mapcontrol__GPSItem.stringdata0))
        return static_cast<void*>(const_cast< GPSItem*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< GPSItem*>(this));
    if (!strcmp(_clname, "org.qt-project.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< GPSItem*>(this));
    return QObject::qt_metacast(_clname);
}

int mapcontrol::GPSItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void mapcontrol::GPSItem::UAVReachedWayPoint(int const & _t1, WayPointItem * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void mapcontrol::GPSItem::UAVLeftSafetyBouble(internals::PointLatLng const & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
