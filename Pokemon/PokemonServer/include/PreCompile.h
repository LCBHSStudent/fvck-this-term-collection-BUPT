#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <typeinfo>
#include <type_traits>
#include <string_view>

using byte   = uint8_t;
using uint32 = uint32_t;

#define GET_CLASS_NAME(_class_) \
    QString(typeid(_class_).name())

#define GET(_type_, _property_)  \
    _type_ get_##_property_() { return m_##_property_; }

#define FUNCTION 
#define RESOURCE 

#include <QDebug>
#include <QObject>
#include <QSqlError>
#include <QTcpSocket>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QCoreApplication>

// #define 



#endif // PRECOMPILE_H
