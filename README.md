<div align="center">
  <picture>
    <source media="(prefers-color-scheme: light)" srcset="https://github.com/mozahzah/IECore/raw/master/Resources/IE-Brand-Kit/IE-Logo-Alt-NoBg.png">
    <source media="(prefers-color-scheme: dark)" srcset="https://github.com/mozahzah/IECore/raw/master/Resources/IE-Brand-Kit/IE-Logo-NoBg.png">
  <img alt="IELogo" width="128">
  </picture>
</div>

# IEConcurrency
IE's high-performance concurrency library providing modern concurrent data structures and other syncronization primitives.

> [!IMPORTANT] 
> For installation and integration instructions, please refer to the Wiki here: [IEConcurrency Wiki](https://github.com/Interactive-Echoes/IEConcurrency/wiki)

## Included syncronization primitives and concurrent data atructures

- **IESpinOnWriteObject**  
A templated class providing lock-free and wait-free read access to an object, with spinlock for writes. Ideal for real-time applications like audio processing, where the audio thread needs fast, non-blocking reads, and the UI thread can handle spinlocks for syncronized writes.
- **IESPSCQueue**  
A lock-free and wait-free single-producer single-consumer (SPSC) FIFO Queue concurrent data structure, designed with fully padded access to prevent false sharing. By utilizing only a single atomic element size counter for synchronization, the IESPSCQueue outperforms Boost library's spsc_queue implementation.
- **IESPMCQueue**  
A lock-free single-producer multi-consumer (SPMC) FIFO Queue concurrent data structure. The producer operates in a lock-free and wait-free manner, while consumers are lock-free but rely on a spinlock for synchronization. The structure is fully padded to avoid false sharing and use a single atomic size counter along with an atomic read flag to manage consumer synchronization.

## Repository Structure
This repository is organized across two main branches:

### Master Branch
The master branch contains the core library implementation, designed to be easily integrated into your applications. This is the branch you should use when adding IEConcurrency as a dependency to your project or looking for the latest stable release.

### Benchmark Branch
The benchmark branch contains the performance benchmarking infrastructure using Google Benchmark framework. Use this branch when:
- Comparing IEConcurrency performance against other libraries
- Running performance tests
- Contributing new benchmarks
- Analyzing or reporting performance metrics
- Contributing new features or modifications
> [!NOTE]
> The benchmark branch serves as both the development and benchmarking branch.  
> For more information on how to benchmark and contribute, visit the [Benchmarking and Development Wiki](https://github.com/Interactive-Echoes/IEConcurrency/wiki/Benchmarking-and-Development).

## Third-Party Libraries for Benchmarking Performance
- [Google-Benchmark](https://github.com/google/benchmark)
- [Boost](https://github.com/boostorg/boost.git)

## Contribution
Contributors are welcomed to this open-source project. Any feedback or assistance, whether in coding, packaging, documentation, design, or testing, is greatly appreciated. 

## License
This work is licensed under the [SPDX-License-Identifier: GPL-2.0-only](./LICENSE).
