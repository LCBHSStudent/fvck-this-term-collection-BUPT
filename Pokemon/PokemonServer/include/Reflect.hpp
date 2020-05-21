#ifndef REFLECT_H
#define REFLECT_H

#include <PreCompile.h>

template <class T>
class Singleton {
public:
    using handleType = std::unique_ptr<T>;
public:
    static handleType& GetInstance() {
        if (!ms_inst) {
            std::mutex m;
            std::lock_guard<std::mutex> lock(m);

            ms_inst.reset(new T); //不要写成了ms_inst->reset(new T);
        }
        return ms_inst;
    }

// 需要使用protected 关键字，不能使用private
protected:
    Singleton(){}
    Singleton(const Singleton& inst){}
    Singleton& operator=(const Singleton& inst){}

private:
    static handleType ms_inst;
};


// 下面的初始化需要加 typename
template<class T>
typename Singleton<T>::handleType Singleton<T>::ms_inst;

#endif // REFLECT_H
