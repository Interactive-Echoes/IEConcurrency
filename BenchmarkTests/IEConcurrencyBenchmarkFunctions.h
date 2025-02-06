// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include <chrono>
#include <thread>

#include "benchmark/benchmark.h"
#include "boost/lockfree/spsc_queue.hpp"
#include "boost/lockfree/stack.hpp"

#include "IEConcurrency.h"

template<typename ElementType>
static void BM_IESPSCQueue_Push(benchmark::State& state)
{
    const unsigned int N = state.range(0);
    for (auto _ : state)
    {
        IESPSCQueue<ElementType> Queue(N);
        auto Start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++)
        {
            benchmark::DoNotOptimize(Queue.Push(ElementType()));
        }
        auto End = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);
        state.SetIterationTime(Elapsed.count());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N * state.iterations());
}

template<typename ElementType>
static void BM_BoostSPSCQueue_Push(benchmark::State& state)
{
    const unsigned int N = state.range(0);
    for (auto _ : state)
    {
        boost::lockfree::spsc_queue<ElementType> Queue(N);
        auto Start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++)
        {
            benchmark::DoNotOptimize(Queue.push(ElementType()));
        }
        auto End = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);
        state.SetIterationTime(Elapsed.count());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N * state.iterations());
}

template<typename ElementType>
static void BM_IESPSCQueue_Pop(benchmark::State& state)
{
    const unsigned int N = state.range(0);
    for (auto _ : state)
    {
        IESPSCQueue<ElementType> Queue(N);
        for (int i = 0; i < N; ++i)
        {
            Queue.Push(ElementType());
        }
        auto Start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++)
        {
            ElementType Element;
            Queue.Pop(Element);
            benchmark::DoNotOptimize(Element);
        }
        auto End = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);
        state.SetIterationTime(Elapsed.count());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N * state.iterations());
}

template<typename ElementType>
static void BM_BoostSPSCQueue_Pop(benchmark::State& state)
{
    const unsigned int N = state.range(0);
    for (auto _ : state)
    {
        boost::lockfree::spsc_queue<ElementType> Queue(N);
        for (int i = 0; i < N; ++i)
        {
            Queue.push(ElementType());
        }
        auto Start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N; i++)
        {
            ElementType Element;
            Queue.pop(Element);
            benchmark::DoNotOptimize(Element);
        }
        auto End = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);
        state.SetIterationTime(Elapsed.count());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N * state.iterations());
}

template<typename ElementType>
static void BM_IESPSCQueue_Latency(benchmark::State& state)
{
    const unsigned int N = state.range(0);
    IESPSCQueue<ElementType> Queue1(N), Queue2(N);
    
    for (auto _ : state)
    {
        std::thread Thread = std::thread([&]
        {
            for (int i = 0; i < N; i++)
            {
                ElementType Element;
                benchmark::DoNotOptimize(Element);
                while (!Queue1.Pop(Element)) {}
                while (!Queue2.Push(Element)) {}
            }
        });
        
        auto Start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N; i++)
        {
            while (!Queue1.Push(ElementType())) {}
            ElementType Element;
            benchmark::DoNotOptimize(Element);
            while (!Queue2.Pop(Element)) {}
        }

        auto End = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);
        state.SetIterationTime(Elapsed.count());

        Thread.join();
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N * state.iterations());
}

template<typename ElementType>
static void BM_BoostSPSCQueue_Latency(benchmark::State& state)
{
    const unsigned int N = state.range(0);
    boost::lockfree::spsc_queue<ElementType> Queue1(N), Queue2(N);
    
    for (auto _ : state)
    {
        std::thread Thread = std::thread([&]
        {
            for (int i = 0; i < N; i++)
            {
                ElementType Element;
                benchmark::DoNotOptimize(Element);
                while (!Queue1.pop(&Element, 1)) {}
                while (!Queue2.push(Element)) {}
            }
        });
        
        auto Start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < N; i++)
        {
            while (!Queue1.push(ElementType())) {}
            ElementType Element;
            benchmark::DoNotOptimize(Element);
            while (!Queue2.pop(Element)) {}
        }
        
        auto End = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(End - Start);
        state.SetIterationTime(Elapsed.count());

        Thread.join();
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(N * state.iterations());
}