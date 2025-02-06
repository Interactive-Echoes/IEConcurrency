// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include <atomic>
#include <cstdlib>
#include <memory>
#include <new>
#include <optional>
#include <type_traits>

using size_t = std::size_t;

#ifdef __cpp_lib_hardware_interference_size
    #define IE_CACHE_LINE_SIZE std::hardware_destructive_interference_size
#else
    #define IE_CACHE_LINE_SIZE 64
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define IE_LIKELY(x) __builtin_expect((x), 1)
    #define IE_UNLIKELY(x) __builtin_expect((x), 0)
#else
    #define IE_LIKELY(x) (x)
    #define IE_UNLIKELY(x) (x)
#endif