/****************************************************************************
** Meta object code from reading C++ file 'FinestraPrincipale.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "FinestraPrincipale.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FinestraPrincipale.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FinestraPrincipale[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x0a,
      38,   19,   19,   19, 0x0a,
      56,   19,   19,   19, 0x0a,
      74,   19,   19,   19, 0x0a,
      97,   19,   91,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FinestraPrincipale[] = {
    "FinestraPrincipale\0\0aggiungiOggetto()\0"
    "effettuaPuntata()\0richiediOggetti()\0"
    "ottieniOggetto()\0void*\0riceviServizi(void*)\0"
};

void FinestraPrincipale::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FinestraPrincipale *_t = static_cast<FinestraPrincipale *>(_o);
        switch (_id) {
        case 0: _t->aggiungiOggetto(); break;
        case 1: _t->effettuaPuntata(); break;
        case 2: _t->richiediOggetti(); break;
        case 3: _t->ottieniOggetto(); break;
        case 4: { void* _r = _t->riceviServizi((*reinterpret_cast< void*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< void**>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FinestraPrincipale::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FinestraPrincipale::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FinestraPrincipale,
      qt_meta_data_FinestraPrincipale, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FinestraPrincipale::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FinestraPrincipale::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FinestraPrincipale::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FinestraPrincipale))
        return static_cast<void*>(const_cast< FinestraPrincipale*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FinestraPrincipale::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
