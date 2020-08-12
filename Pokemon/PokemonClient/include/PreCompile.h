#ifndef PRECOMPILE_H
#define PRECOMPILE_H

// ------ std include 域
#include <map>
#include <array>
#include <string>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <cstdint>
#include <iostream>
#include <typeinfo>
#include <iostream>
#include <functional>
#include <type_traits>
#include <string_view>

// ------ 别名定义域
using byte   = uint8_t;
using uint32 = uint32_t;

// ------ 宏域
#define GET_CLASS_NAME(_class_) \
    QString(typeid(_class_).name())
// ------ 供设定PROPERTY使用的宏 用于定义函数
#define GET(_type_, _property_)  \
    _type_ get_##_property_() const noexcept { return m_##_property_; }
// ------ 习惯
#define MALLOC(_size_, _type_) \
    static_cast<type*>(malloc(_size_ * sizeof(_type_)))
// ------ 定义标志符（类似"slots"）
#define FUNCTION 
#define RESOURCE 

// ------ QT include 域
#include <QHash>
#include <QtQml>
#include <QDebug>
#include <QObject>
#include <QQuickItem>
#include <QTcpSocket>
#include <QHostAddress>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtConcurrent/QtConcurrent>

#endif // PRECOMPILE_H
