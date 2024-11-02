#pragma once

#include "godotIncludes.hpp"
#include "godotUtils.hpp"
#include "log.h"
#include "math.hpp"
#include "timer.hpp"
#include "typedefs.hpp"

#define ASSERT(expr, msg)                                                                                              \
	{ __m_assert(#expr, expr, __FILE__, __LINE__, msg); }
#define ASSERT_NOTNULL(ptr) ASSERT(ptr != nullptr, "")
void __m_assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);
