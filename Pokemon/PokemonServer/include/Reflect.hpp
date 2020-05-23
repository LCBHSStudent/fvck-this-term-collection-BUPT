#ifndef REFLECT_H
#define REFLECT_H

#include <PreCompile.h>

template<typename ClassName, typename KeyType, typename FuncType>
struct FuncReflectHelper final {
public:
    explicit 
        FuncReflectHelper(QHash<KeyType, FuncType>& saver, KeyType&& key, FuncType func) {
            std::cout << "construct\n";
            saver[std::forward<KeyType>(key)] = func;
            this->~FuncReflectHelper();
        }
        ~FuncReflectHelper() {
            std::cout << "destruct\n";
        };
};

#endif // REFLECT_H
