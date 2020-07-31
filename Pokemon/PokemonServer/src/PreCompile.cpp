#include <PreCompile.h>

std::random_device 
    s_rdev = {};
// std::default_random_engine 
//    s_engine(s_rdev());
std::default_random_engine 
    s_engine(time(0));
std::uniform_real_distribution<float> 
    s_distr(0.0f, 1.0f);
