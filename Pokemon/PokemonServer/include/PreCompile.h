#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#include <map>
#include <array>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <cstdint>
#include <typeinfo>
#include <functional>
#include <type_traits>
#include <string_view>

using byte   = uint8_t;
using uint32 = uint32_t;

#define GET_CLASS_NAME(_class_) \
    QString(typeid(_class_).name())

#define GET(_type_, _property_)  \
    _type_ get_##_property_() const noexcept { return m_##_property_; }

#define MALLOC(_size_, _type_) \
    static_cast<type*>(malloc(_size_ * sizeof(_type_)))

#define FUNCTION 
#define RESOURCE 

#include <QHash>
#include <QDebug>
#include <QObject>
#include <QSqlError>
#include <QTcpSocket>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>

#endif // PRECOMPILE_H
