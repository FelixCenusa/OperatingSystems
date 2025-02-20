# Lab 1: Processes, Threads, and Memory Systems

## TL;DR

- **Focus:** Processes, threads, and memory systems in operating systems.
- **Highlights:**
    - **Multithreading:** Squared thread IDs, synchronized transactions with semaphores, and deadlock-free dining philosophers.
    - **Parallel Processing:** Achieved 75% speedup in matrix multiplication and 70% speedup in matrix initialization/calculation.
    - **Memory Management:** Implemented FIFO, LRU, and Optimal page replacement algorithms.
- **Implementation:** Written in C with detailed performance analyses.

**PDF reports available in this repository**

This project is part of the Operating Systems course (DV1697/DV1698) by Felix Beniamin Cenusa and Yamen Chams (21‑11‑2024). It explores how processes and threads operate and interact, along with the impact of memory systems—especially virtual memory and page replacement algorithms—on overall system performance.

## Table of Contents

- [Project Overview](#project-overview)
- [Lab Tasks](#lab-tasks)
    - [Part 1: Processes and Threads](#part-1-processes-and-threads)
    - [Part 2: Memory Systems](#part-2-memory-systems)
- [Detailed Implementation](#detailed-implementation)
- [Performance Results](#performance-results)
- [Discussion and Reflections](#discussion-and-reflections)
- [Getting Started](#getting-started)
- [Contributors](#contributors)
- [Additional Resources](#additional-resources)

## Project Overview

This lab focuses on practical implementations and performance analysis of various systems programming tasks. It covers:

- **Multithreading:** Creating, synchronizing, and managing threads.
- **Parallel Processing:** Achieving significant speedups in computational tasks.
- **Memory Management:** Implementing and comparing page replacement algorithms (FIFO, LRU, and Optimal).

The project not only aims to solve specific problems but also deepens the understanding of how low-level system operations work.

## Lab Tasks

### Part 1: Processes and Threads

In this part, the focus is on understanding thread operations and the benefits of parallel execution.

- **Task 7: Squared Thread IDs**
    
    - **What We Did:**
        - Introduced a new variable `squaredId` in the `threadArgs` structure.
        - Each thread computes the square of its ID.
        - Used `pthread_join` to wait for all threads to complete and to collect their results.
- **Task 9: Data Integrity with Semaphores**
    
    - **What We Did:**
        - Created multiple threads to perform transactions.
        - Applied semaphore functions (`sem_init`, `sem_wait`, and `sem_post`) to lock the data during each transaction.
        - Ensured data integrity and atomicity in a manner similar to SQL transactions.
- **Task 11: Dining Philosophers and Deadlock Avoidance**
    
    - **What We Did:**
        - Implemented a strategy where each philosopher waits until both forks (or chopsticks) are available.
        - Ensured that both resources are locked simultaneously, effectively avoiding deadlocks.
- **Task 13: Parallel Matrix Multiplication**
    
    - **What We Did:**
        - Parallelized matrix multiplication.
        - **Performance:** Reduced execution time from 5.1 seconds (sequential) to 2.9 seconds, achieving a 75% speedup.
- **Task 14: Parallel Matrix Initialization and Calculation**
    
    - **What We Did:**
        - Parallelized both matrix initialization and computation.
        - **Performance:** Reduced execution time from 5.1 seconds (sequential) to 3.0 seconds, achieving a 70% speedup.
        - **Note:** The initialization task showed less benefit due to thread overhead; however, the multiplication portion greatly improved overall performance.

### Part 2: Memory Systems

This section demonstrates various page replacement strategies to handle virtual memory efficiently.

- **Task 17: FIFO Page Replacement**
    
    - **What We Did:**
        - Implemented the FIFO algorithm by reading a sample file and cycling through a fixed number of physical pages.
        - Incremented a counter (modulo the number of pages) to decide which page to replace.
        - **Observation:** The simple FIFO approach resulted in a high number of page faults.
- **Task 20: LRU Page Replacement**
    
    - **What We Did:**
        - Developed an LRU algorithm using a circular linked list.
        - When the memory is full, the least recently used page is replaced.
        - On access, a page’s “timer” is reset by moving it to the end of the list.
- **Task 23: Optimal (Belady’s) Page Replacement**
    
    - **What We Did:**
        - Implemented the Optimal algorithm with the aid of a linked list.
        - Added a function (`leastCommonInFuture`) to determine which page will not be used for the longest period in the future, then replaced that page.
        - Ensured the algorithm returns correct results when benchmarked against known outcomes.

## Detailed Implementation

The project is implemented in C with a focus on multithreading and memory management:

- **Thread Management:**
    
    - Threads are created using `pthread_create` and synchronized with `pthread_join`.
    - Semaphores are used to ensure that critical sections (e.g., during transactions) are executed atomically.
- **Memory Management:**
    
    - Three different page replacement strategies were implemented:
        - **FIFO:** A straightforward approach that can lead to many page faults.
        - **LRU:** Uses a circular linked list to track page usage and manage replacement.
        - **Optimal:** Employs predictive logic (via `leastCommonInFuture`) to select the page that will not be needed for the longest period.
    - The use of linked lists in LRU and Optimal strategies avoids the limitations of static array implementations.
- **Performance Metrics:**
    
    - Benchmarks illustrate the benefits of parallelization in reducing execution times.
    - Comparisons between sequential and parallel implementations provide insights into the trade-offs of thread overhead versus computational gains.

## Performance Results

- **Matrix Multiplication (Task 13):** 75% speedup (2.9 sec vs. 5.1 sec).
- **Matrix Initialization & Calculation (Task 14):** 70% speedup (3.0 sec vs. 5.1 sec).
- **Page Replacement Algorithms:**
    - All implementations produced correct outputs compared to expected benchmarks, though FIFO was less efficient due to its simplistic design.

## Discussion and Reflections

This lab was a valuable learning experience:

- **Learning Curve:**
    
    - Transitioning from C++ to C introduced challenges due to differences in syntax and memory management.
    - Working with low-level threading and synchronization deepened our understanding of process management.
- **Parallel Processing:**
    
    - Demonstrated significant performance gains through parallelization, even when considering the overhead of thread creation.
- **Memory Management:**
    
    - Implementing and comparing multiple page replacement algorithms provided practical insights into system performance trade-offs.
    - While simpler algorithms like FIFO are easier to implement, more sophisticated approaches (LRU and Optimal) yield better performance in demanding scenarios.

## Getting Started

To run the project locally:

1. **Clone the Repository:**
    
    bash
    
    Copy
    
    `git clone https://your-repo-url.git`
    
2. **Compile the Code:**
    
    - Ensure you have a C compiler with pthread support. For example:
        
        bash
        
        Copy
        
        `gcc -pthread -o lab1 main.c`
        
3. **Run the Executable:**
    
    bash
    
    Copy
    
    `./lab1`
    
4. **Review the PDF Report:**
    
    - For detailed explanations, performance analysis, and further insights, refer to the PDF reports available in the repository.

## Contributors

- **Felix Beniamin Cenusa**
- **Yamen Chams**

## Additional Resources

- Detailed PDF Report on Solutions for Part 1 and Part 2 _(update this link as necessary)_