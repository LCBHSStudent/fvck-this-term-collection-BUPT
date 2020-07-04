#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#pragma execution_character_set("utf-8")

#include <map>
#include <tuple>
#include <array>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <cstdint>
#include <cstdlib>
#include <conio.h>
#include <iostream>
#include <typeinfo>
#include <functional>
#include <type_traits>
#include <string_view>

using byte   = uint8_t;
using uint32 = uint32_t;

#define uptr std::unique_ptr
#define sptr std::shared_ptr

#define GET_CLASS_NAME(_class_) \
    QString(typeid(_class_).name())

#define GET(_type_, _property_)  \
    inline _type_ get_##_property_() const noexcept { return m_##_property_; }

#define SET(_type_, _property_) \
inline void set_##_property_(const _type_& var) { \
    m_##_property_ = var;\
}\

#define PROPERTY(_type_, _property_) \
    GET(_type_, _property_) \
    SET(_type_, _property_) \

#define MALLOC(_size_, _type_) \
    static_cast<type*>(malloc(_size_ * sizeof(_type_)))

#define DEBUG_FLAG

#define FUNCTION 
#define RESOURCE 

#include <QHash>
#include <QDebug>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QHostAddress>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>

#endif // PRECOMPILE_H
