#pragma once
#include "env.hpp"
#include "osal_class.hpp"

#if SITL
#include "osal_pc.hpp"
static OSAL::osal_pc_t osal_pc;
static OSAL::osal_backend_t *osal = &osal_pc;
#else
#include "osal_stm32.hpp"
static OSAL::osal_stm32_t osal_stm32;
static OSAL::osal_backend_t *osal = &osal_stm32;
#endif
