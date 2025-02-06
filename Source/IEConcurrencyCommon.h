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

#ifdef __cpp_lib_hardware_interference_size
    #define IE_CACHE_LINE std::hardware_destructive_interference_size
#else
    #define IE_CACHE_LINE 64
#endif