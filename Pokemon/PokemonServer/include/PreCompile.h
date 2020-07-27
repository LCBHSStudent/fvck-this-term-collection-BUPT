#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#ifdef _WIN32
    #pragma execution_character_set("utf-8")
#endif

// ------std Libraries includes
#include <map>
#include <tuple>
#include <array>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <random>
#include <cstdint>
#include <cstdlib>
// #include <conio.h>
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

#define PROPERTY(_type_, _property_name_) \
    GET(_type_, _property_name_) \
    SET(_type_, _property_name_) \

#define MALLOC(_size_, _type_) \
    static_cast<type*>(malloc(_size_ * sizeof(_type_)))

// ------reserved for random components
extern std::random_device 
    s_rdev;
extern std::default_random_engine 
    s_engine;
extern std::uniform_real_distribution<float> 
    s_distr;

constexpr auto get_percent = [](int percentVal) {
    return static_cast<double>(percentVal) / 100.0f;};

#define RAND_OZ \
    s_distr(s_engine) \

#define JUDGE_OZ(_standard) \
    if(s_distr(s_engine) <= get_percent(_standard)) \
// ------end of random variables & func space

#define DEBUG_FLAG

#define FUNCTION 
#define RESOURCE 

// ------Qt Libraries includes
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
