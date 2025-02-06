// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IEConcurrencyCommon.h"

template <typename T, typename Allocator = std::allocator<T>>
class IESPSCQueue : private Allocator
{
public:
    using ValueType = T;
    IESPSCQueue(size_t Size) :
        m_Capacity(Size + 1),
        m_Data(std::allocator_traits<Allocator>::allocate(*this, m_Capacity + 2 * m_PaddingElementsNum))
    {}
    IESPSCQueue(const IESPSCQueue&) = delete;
    IESPSCQueue& operator=(const IESPSCQueue&) = delete;
    ~IESPSCQueue()
    {
        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            for (std::size_t i = 0; i < m_Capacity - m_PaddingElementsNum; i++)
            {
                std::allocator_traits<Allocator>::destroy(*this, m_Data + m_PaddingElementsNum + i);
            }
        }
        std::allocator_traits<Allocator>::deallocate(*this, m_Data, m_Capacity + 2 * m_PaddingElementsNum);
    }

    template <typename... Args>
    bool Push(Args&&... _Args)
    {
        if (m_Num.load(std::memory_order_acquire) >= m_Capacity)
        {
            return false;
        }

        std::allocator_traits<Allocator>::construct(*this, m_Data + m_BackIndex + m_PaddingElementsNum, std::forward<Args>(_Args)...);
        m_BackIndex = m_BackIndex == m_Capacity ? 0 : m_BackIndex + 1;
        m_Num.fetch_add(1, std::memory_order_release);
        return true;
    }

    bool Pop(T& Element)
    {
        if (m_Num.load(std::memory_order_acquire) == 0)
        {
            return false;
        }

        Element = std::move(m_Data[m_FrontIndex + m_PaddingElementsNum]);
        m_FrontIndex = m_FrontIndex == m_Capacity ? 0 : m_FrontIndex + 1;
        m_Num.fetch_sub(1, std::memory_order_release);
        return true;
    }

    std::optional<T> Pop()
    {
        T Element;
        if (Pop(Element))
        {
            return std::optional<T>(Element);
        }
        return std::nullopt;
    }

    bool IsEmpty() const
    {
        return m_Num.load(std::memory_order_acquire) == 0;
    }

private:
    static constexpr size_t m_PaddingElementsNum = (IE_CACHE_LINE_SIZE - 1) / sizeof(T) + 1;
    const size_t m_Capacity;
    const typename std::allocator_traits<Allocator>::pointer m_Data;

    alignas(IE_CACHE_LINE_SIZE) size_t m_FrontIndex = 0;
    alignas(IE_CACHE_LINE_SIZE) size_t m_BackIndex = 0;
    alignas(IE_CACHE_LINE_SIZE) std::atomic<size_t> m_Num{ 0 };
};