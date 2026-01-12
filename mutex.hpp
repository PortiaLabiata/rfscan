#pragma once
#include "env.hpp"

#if SITL == ON
#include "mutex_pc.hpp"
namespace OS { using mutex_t = OS::mutex_pc_t; }
#else
#include "mutex_stm32.hpp"
namespace OS { using mutex_t = OS::mutex_stm32_t; }
#endif
