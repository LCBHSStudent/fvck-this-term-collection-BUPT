#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#ifdef _WIN32
    #pragma execution_character_set("utf-8")
    #pragma pack()
#endif

// ------ std Libraries includes
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
#include <iostream>
#include <typeinfo>
#include <functional>
#include <type_traits>
#include <string_view>

// ------ 别名域
using byte   = uint8_t;
using uint32 = uint32_t;
// ------ 不用using是为了后跟模板
#define uptr std::unique_ptr
#define sptr std::shared_ptr


// ------ 获取类型名
#define GET_CLASS_NAME(_class_) \
    QString(typeid(_class_).name())


// ------ 供 PROPERTY 使用的 get / set 函数族
// ------------Get
#define GET(_type_, _property_)  \
    inline _type_ get_##_property_() const noexcept { return m_##_property_; }
// ------------Set
#define SET(_type_, _property_) \
inline void set_##_property_(const _type_& var) { \
    m_##_property_ = var;\
}\
// ------------Property
#define PROPERTY(_type_, _property_name_) \
    GET(_type_, _property_name_) \
    SET(_type_, _property_name_) \
    

// 习惯
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

// 生成0~1随机数
#define RAND_OZ \
    s_distr(s_engine) \

// 概率百分比判断
#define JUDGE_OZ(_standard) \
    if(s_distr(s_engine) <= get_percent(_standard)) \
// ------end of random variables & func space

#define DEBUG_FLAG
#undef  DEBUG_FLAG

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
