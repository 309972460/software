/****************************************************************************
** Meta object code from reading C++ file 'deviceoffset.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "deviceoffset.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'deviceoffset.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_deviceOffset[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   13,   13,   13, 0x08,
      55,   13,   13,   13, 0x08,
      62,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_deviceOffset[] = {
    "deviceOffset\0\0,\0offSetValue(QString,QString)\0"
    "closeUI()\0pbOK()\0pbCancel()\0"
};

void deviceOffset::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        deviceOffset *_t = static_cast<deviceOffset *>(_o);
        switch (_id) {
        case 0: _t->offSetValue((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->closeUI(); break;
        case 2: _t->pbOK(); break;
        case 3: _t->pbCancel(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData deviceOffset::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject deviceOffset::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_deviceOffset,
      qt_meta_data_deviceOffset, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &deviceOffset::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *deviceOffset::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *deviceOffset::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_deviceOffset))
        return static_cast<void*>(const_cast< deviceOffset*>(this));
    return QDialog::qt_metacast(_clname);
}

int deviceOffset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void deviceOffset::offSetValue(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
