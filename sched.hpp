#pragma once
#include "env.hpp"
#include "sched_class.hpp"

#if SITL == ON
#include "sched_pc.hpp"
static OS::sched_pc_t sched_pc;
static OS::sched_abstract_t *sched = &sched_pc;
#else
#include "sched_stm32.hpp"
static OS::sched_stm32_t sched_stm32;
static OS::sched_abstract_t *sched = &sched_stm32;
#endif
