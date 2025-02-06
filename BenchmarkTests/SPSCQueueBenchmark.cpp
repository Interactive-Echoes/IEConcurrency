// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#include "IEConcurrencyBenchmarkFunctions.h"

/* -------------------------- SPSCQueueBenchmark -------------------------- */

// Set the size and type of elements to be used in the benchmarks. 
static constexpr size_t ELEMENT_TEST_SIZE = 1 << 20;
using ElementTestType = float;

// Set the following macros to 0/1 to enable/disable the corresponding benchmark tests:
// MEMORY_OPERATIONS_BENCHMARK: Measures memory allocation and deallocation performance.
// INTER_THREAD_LATENCY_BENCHMARK: Measures latency between producer and consumer threads.
#define MEMORY_OPERATIONS_BENCHMARK 1
#define INTER_THREAD_LATENCY_BENCHMARK 1

/*
    These benchmarks evaluate the performance of push and pop operations separately.
    Each iteration measures the time taken to either push or pop N elements, 
    where N is defined by the constant ELEMENT_TEST_SIZE.

    All benchmark functions are defined in the IEConcurrencyBenchmarkFunctions.h file.
*/
#if MEMORY_OPERATIONS_BENCHMARK
#define ARGS_B1 Arg(ELEMENT_TEST_SIZE)->Unit(benchmark::kMicrosecond)->UseManualTime()
BENCHMARK_TEMPLATE(BM_IESPSCQueue_Push,     ElementTestType)->ARGS_B1;
BENCHMARK_TEMPLATE(BM_BoostSPSCQueue_Push,  ElementTestType)->ARGS_B1;
BENCHMARK_TEMPLATE(BM_IESPSCQueue_Pop,      ElementTestType)->ARGS_B1;
BENCHMARK_TEMPLATE(BM_BoostSPSCQueue_Pop,   ElementTestType)->ARGS_B1;
#endif

/*
    These benchmarks measures sustained round-trip latency of the SPSC queue
    by performing multiple cycles of message passing.

    In each iteration, the queue processes N elements, where N is defined by the constant ELEMENT_TEST_SIZE.

    The test uses manual timing to precisely measure the duration of N complete cycles,
    excluding thread overhead.

    All benchmark functions are defined in the IEConcurrencyBenchmarkFunctions.h file.
*/
#if INTER_THREAD_LATENCY_BENCHMARK
#define ARGS_B2 Arg(ELEMENT_TEST_SIZE)->Unit(benchmark::kMicrosecond)->UseManualTime()
BENCHMARK_TEMPLATE(BM_IESPSCQueue_Latency,      ElementTestType)->ARGS_B2;
BENCHMARK_TEMPLATE(BM_BoostSPSCQueue_Latency,   ElementTestType)->ARGS_B2;
#endif

BENCHMARK_MAIN();