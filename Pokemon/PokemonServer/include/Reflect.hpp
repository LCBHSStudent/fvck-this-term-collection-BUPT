#ifndef REFLECT_H
#define REFLECT_H

#include <PreCompile.h>

/**
 * @param   ClassName 启用反射的类名 
 * @param   KeyType   索引值类型（一般为string或int）
 * @param   FuncType  反射调用的函数类型  
 */
template<typename ClassName, typename KeyType, typename FuncType>
struct FuncReflectHelper final {
public:
    explicit 
        FuncReflectHelper(QHash<KeyType, FuncType>& saver, KeyType&& key, FuncType func) {
            saver[std::forward<KeyType>(key)] = func;
            this->~FuncReflectHelper();
        }
        ~FuncReflectHelper() {}
    // 存储类型名
    using className = ClassName;
};

#endif // REFLECT_H
