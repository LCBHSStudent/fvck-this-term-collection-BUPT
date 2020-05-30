#ifndef REFLECT_H
#define REFLECT_H

#include <PreCompile.h>

template<typename ClassName, typename KeyType, typename FuncType>
struct FuncReflectHelper final {
public:
    explicit 
        FuncReflectHelper(QHash<KeyType, FuncType>& saver, KeyType&& key, FuncType func) {
            saver[std::forward<KeyType>(key)] = func;
            this->~FuncReflectHelper();
        }
        ~FuncReflectHelper() {}
};

#endif // REFLECT_H
