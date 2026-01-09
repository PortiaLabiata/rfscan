#pragma once
#include "env.hpp"
#include "osal_class.hpp"

#if SITL
#include "disp_pc.hpp"
static OSAL::disp_pc_t disp_pc;
static OSAL::disp_backend_t *disp = &disp_pc;
#else
#include "disp_stm32.hpp"
static OSAL::disp_stm32_t disp_stm32;
static OSAL::disp_backend_t *disp = &disp_stm32;
#endif
