// SPDX-License-Identifier: GPL-2.0-only
// Copyright © Interactive Echoes. All rights reserved.
// Author: mozahzah

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "IEConcurrency.h"

static void Demo_IESpinOnWriteObject()
{
    /*
        This demo demonstrates how the Reader thread blocks the Writer thread when accessing the shared data (IESpinOnWriteObjectstd::string).
        During each read operation, the Writer thread attempts to write new data but is blocked until the Reader completes its task.
        After the first read operation, the Writer is able to update the data, and in the second read operation, the updated data is successfully retrieved.
        Notably, the read operation is both lock-free and wait-free.
    */
    std::printf("\nIESpinOnWriteObject Demo Started.\n\n");

    IESpinOnWriteObject<std::string> Data(std::string("[OLD DATA]"));
    const int ReaderSimulatedWorkTime = 2000;
    
    std::thread Reader([&Data](int SimulatedWorkTime)
        {
            for (int i = 0; i < 2; i++)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Add small delay between reads
                IESpinOnWriteObject<std::string>::LockedValue LockedData = Data.LockForRead();
                std::printf("Reader thread: Reading Locked Data %s\n", LockedData.Value.c_str());
                std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
                std::printf("Reader thread: Unlocked Data\n");
            }

        }, ReaderSimulatedWorkTime);

    std::this_thread::sleep_for(std::chrono::milliseconds(400)); // Add small delay before writing new data.
    std::thread Writer([&Data]()
        {
            std::printf("Writer thread: Waiting to Write New Data...\n");
            std::string NewData = std::string("[NEW DATA]");
            Data.Write(NewData);
            std::printf("Writer thread: New Data Written %s\n", NewData.c_str());
        });

    Writer.join();
    Reader.join();

    std::printf("\nIESpinOnWriteObject Demo Finished.\n");
}

static void Demo_IESPSCQueue()
{
    /*
        This demo demonstrates the behavior of the IESPSCQueue<int> in a producer-consumer scenario.
        A Producer thread generates data and attempts to push it into the queue, retrying if the queue is full.
        Meanwhile, a Consumer thread retrieves and processes data from the queue.
        The Producer and Consumer run concurrently, with the Producer generating items and the Consumer consuming them at different simulated work intervals.
        The demo highlights the synchronization between the threads and the queue’s handling of push and pop operations.
        The demo concludes once all items are processed.
    */
    std::printf("\nIESPSCQueue Demo Started.\n\n");

    IESPSCQueue<int> Queue(5);
    std::atomic<bool> bHasFinished{ false };
    const int ProducerSimulatedWorkTime = 100;
    const int ConsumerSimulatedWorkTime = 300;

    std::thread Producer([&](int SimulatedWorkTime)
    {
        for (int i = 0; i < 10; ++i)
        {
            while (!Queue.Push(i))
            {
                std::printf("Producer thread: Queue is full, retrying to push %d ...\n", i);
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
            }
            std::printf("Producer thread: Produced %d\n", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
        }
        bHasFinished.store(true, std::memory_order_relaxed);
    }, ProducerSimulatedWorkTime);
    
    std::thread Consumer([&](int SimulatedWorkTime)
    {
        for (int i = 0; i < 10; ++i)
        {
            std::optional<decltype(Queue)::ValueType> Value = Queue.Pop();
            if (Value)
            {
                std::printf("Consumer thread: Consumed %d\n", *Value);
            }
            else if (!bHasFinished.load(std::memory_order_relaxed))
            {
                std::printf("Consumer thread: Queue is empty, waiting for data...\n");
            }
            else
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
        }
    }, ConsumerSimulatedWorkTime);

    Producer.join();
    Consumer.join();

    std::printf("\nIESPSCQueue Demo Finished.\n");
}

static void Demo_IESPMCQueue()
{
    /*
        This demo demonstrates the behavior of the IESPMCQueue<int> in a producer-consumers scenario.
        A Producer thread generates data and attempts to push it into the queue, retrying if the queue is full.
        Meanwhile, a Consumer thread retrieves and processes data from the queue.
        The Producer and Consumers run concurrently, with the Producer generating items and the Consumers consuming them at different simulated work intervals.
        The demo highlights the synchronization between the threads and the queue’s handling of push and pop operations.
        The demo concludes once all items are processed.
    */
    std::printf("\nIESPMCQueue Demo Started.\n\n");

    IESPMCQueue<int> Queue(5);
    std::atomic<bool> bHasFinished{ false };
    const int ProducerSimulatedWorkTime = 100;
    const int Consumer1SimulatedWorkTime = 400;
    const int Consumer2SimulatedWorkTime = 250;

    std::thread Producer([&](int SimulatedWorkTime)
    {
        for (int i = 0; i < 10; ++i)
        {
            while (!Queue.Push(i))
            {
                std::printf("Producer thread: Queue is full, retrying to push %d\n", i);
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
            }
            std::printf("Producer thread: Produced %d\n", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
        }
        bHasFinished.store(true, std::memory_order_relaxed);
    }, ProducerSimulatedWorkTime);
    
    std::thread Consumer1([&](int SimulatedWorkTime)
    {
        for (int i = 0; i < 10; ++i)
        {
            std::optional<decltype(Queue)::ValueType> Value = Queue.Pop();
            if (Value)
            {
                std::printf("Consumer thread 1: Consumed %d\n", *Value);
            }
            else if (!bHasFinished.load(std::memory_order_relaxed))
            {
                std::printf("Consumer thread 1: Queue is empty, waiting for data...\n");
            }
            else
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
        }
    }, Consumer1SimulatedWorkTime);

    std::thread Consumer2([&](int SimulatedWorkTime)
    {
        for (int i = 0; i < 10; ++i)
        {
            std::optional<decltype(Queue)::ValueType> Value = Queue.Pop();
            if (Value)
            {
                std::printf("Consumer thread 2: Consumed %d\n", *Value);
            }
            else if (!bHasFinished.load(std::memory_order_relaxed))
            {
                std::printf("Consumer thread 2: Queue is empty, waiting for data...\n");
            }
            else
            {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(SimulatedWorkTime));
        }
    }, Consumer2SimulatedWorkTime);

    Producer.join();
    Consumer1.join();
    Consumer2.join();

    std::printf("\nIESPMCQueue Demo Finished.\n");
}

int main()
{
    /* Comment/Uncomment Demo to run */
    Demo_IESpinOnWriteObject();
    Demo_IESPSCQueue();
    Demo_IESPMCQueue();
    
    return 0;
}