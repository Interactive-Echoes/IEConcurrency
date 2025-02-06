// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#pragma once

#include "IEConcurrencyCommon.h"

template<typename T>
class IESpinOnWriteObject
{
public:
    explicit IESpinOnWriteObject(const T& ObjectValue = T()) : 
        m_ObjectStorage(std::make_unique<T>(ObjectValue)),
        m_Object(m_ObjectStorage.get())
    {
    }

    IESpinOnWriteObject(IESpinOnWriteObject&& Other) noexcept :
        m_ObjectStorage(std::move(Other.m_ObjectStorage)),
        m_Object(m_ObjectStorage.get())
    {
        Other.m_Object.store(nullptr);
    }

    IESpinOnWriteObject(const IESpinOnWriteObject&) = delete;
    IESpinOnWriteObject& operator=(const IESpinOnWriteObject&) = delete;

private:
    class ScopedLock
    {
    private:
        explicit ScopedLock(IESpinOnWriteObject<T>& SpinOnWriteObject) :
            m_SpinOnWriteObject(SpinOnWriteObject)
        {
        }
        ~ScopedLock()
        {
            m_SpinOnWriteObject.Unlock(*m_SpinOnWriteObject.m_ObjectStorage);
        }

        ScopedLock(ScopedLock&&) = delete;
        ScopedLock(const ScopedLock&) = delete;
        ScopedLock& operator=(const ScopedLock&) = delete;

    private:
        IESpinOnWriteObject<T>& m_SpinOnWriteObject;
        friend class IESpinOnWriteObject;
    };

public:
    struct LockedValue
    {
        const ScopedLock ScopedLock;
        const T& Value;
    };

public:
    const LockedValue LockForRead()
    {
        return LockedValue{ ScopedLock(*this), *m_Object.exchange(nullptr) };
    }

    void Write(const T& NewObject)
    {
        std::unique_ptr<const T> NewObjectStorage = std::make_unique<T>(NewObject);
        const T* Expected = m_ObjectStorage.get();
        const T* Desired = NewObjectStorage.get();
        while (!m_Object.compare_exchange_weak(Expected, Desired))
        {
            Expected = m_ObjectStorage.get();
        }
        m_ObjectStorage = std::move(NewObjectStorage);
    }

private:
    void Unlock(const T& Object)
    {
        m_Object.store(&Object);
    }

private:
    std::unique_ptr<const T> m_ObjectStorage;
    std::atomic<const T*> m_Object;
};