# cpp-systems-lab

this repository documents my journey in low-latency systems engineering. the focus is transitioning from high-level abstractions to understanding how c++ code interacts directly with hardware, including the cpu, memory, disk, and linux kernel.

## tools & environment

* **neovim**: basic navigation, insert/command modes for terminal-based editing.
* **tmux**: window and session management for an efficient compile/run cycle.
* **linux**: manipulating raw file descriptors, directory permissions, and kernel interfaces (/proc).

## repository structure

* **`fundamentals/`** — focus: transition to system development in c++, terminal tools, and performance-driven logic.
  * `login-system/`: creates a gatekeeper with attempt limits and handling of infinite loops.
  * `reverse-array/`: implements a high-performance array reversal algorithm by manipulating raw memory addresses directly, avoiding heap allocation (o(1) spatial complexity).
  * `asset-manager/`: buildss a financial asset management system using c++ class-based encapsulation, persisting state locally via flat-file .db, and manually reconstructing objects in memory.
  * `sys-monitor/`: a lightweight linux memory monitor that reads kernel data directly from /proc/meminfo, exposing it as a cli tool using professional build practices (-wall, -o3).



* **`storage/`** — focus: understanding how data is physically stored and retrieved from disk.
  * ***coming soon: log-structured storage engine***


* **`system-io/`** — focus: in-depth exploration of linux syscalls and filesystem behavior, bypassing standard c++ streams.
  * `sequential-vs-random-write/`: benchmarks the latency penalty of lseek by comparing sequential append operations versus random in-place updates.
  * `block-size-throughput/`: analyzes the impact of buffer size (4kb vs 1mb) on i/o throughput and syscall overhead.
  * `fsync-latency/`: investigates the cost of durability by measuring the performance gap between volatile page cache writes and hardware synchronized writes (fsync).
  * `mmap-editor/`: demonstrates memory-mapped file manipulation by creating a file, mapping it into ram with mmap(), writing data directly, and syncing changes back to disk.
  * `fd-stream/`: uses raw posix system calls (open, read, write, close) and file descriptors to read a file and output to stdout.
  * `getdents-syscall/`: explores filesystem internals by bypassing standard libraries and invoking the sys_getdents64 syscall directly, reading raw byte buffers and casting them into linux_dirent64 structs to extract inodes.
  * `dirent-classifier/`: extends the raw directory parser by evaluating the d_type field within the linux_dirent64 structure, dynamically classifying kernel objects into regular files, directories, and symlinks.
  * `direct-io-bypass/`: bypasses the linux page cache using the o_direct flag, requiring manual memory alignment via posix_memalign to satisfy hardware dma constraints and measuring raw i/o latency.
  * `mmap-msync/`: maps a file directly into virtual memory using mmap(), modifies data through raw pointer manipulation, and forces hardware synchronization using msync() to guarantee durability.
  * `mmap-vs-fsync/`: benchmarks the performance and latency differences across three distinct disk i/o strategies: standard page-cache buffered writes, highly durable write-throughs via fsync(), and zero-copy memory mapping.


* **`processes/`** — focus: inter-process communication, scheduling, and process lifecycle.
  * `fork-generator/`: explores basic process management by creating multiple child processes with fork(), while the parent monitors and collects termination status using waitpid() in non-blocking mode.
  * `fork-exec-shell/`: implements a minimal interactive shell loop that parses user input, forks a child process, and replaces its memory image using execvp.
  * `pipe-latency/`: measures ipc latency using two unidirectional pipes between a parent and child, performing a high-frequency ping-pong of single-byte messages to time round-trip duration.
  * `signal-ipc-pipeline/`: integrates pipes for one-way task transmission and signals (sigterm) to trigger a graceful child process shutdown, ensuring proper zombie reaping.
  * `fork-vs-thread/`: benchmarks the time overhead and memory footprint (rss) of spawning 10,000 processes via fork() compared to 10,000 posix threads, utilizing getrusage() for precise kernel-level metrics.
  * `race-condition/`: demonstrates thread interference and lost updates by having multiple pthreads increment a shared global counter concurrently without any synchronization.
  * `mutex-lock/`: mitigates the data race using pthread_mutex, ensuring thread safety while benchmarking the time and context-switch overhead introduced by kernel-level blocking.
  * `atomic-operations/`: implements lock-free synchronization using std::atomic, comparing the performance gain and lower latency of hardware-level memory barriers versus traditional system call mutexes.
  * `condition-variable/`: introduces thread synchronization using std::condition_variable and std::unique_lock to manage a basic unbounded producer-consumer workflow.
  * `bounded-buffer/`: implements the classic bounded-buffer problem using dual condition variables to prevent memory overflow and tightly coordinate thread pacing.
  * `thread-safe-queue/`: encapsulates synchronization primitives into a cohesive c++ class, building a reusable thread-safe queue data structure for robust concurrent state management.
  * `thread-pool-pattern/`: constructs a multi-worker thread pool utilizing the custom thread-safe queue, implementing the poison pill technique for graceful shutdown and benchmarking concurrent throughput.
  * `getdents-thread-pool/`: integrates raw linux filesystem syscalls (sys_getdents64) with a concurrent thread pool, using a producer thread to rapidly traverse directories and multiple consumer threads to process inodes concurrently via a thread-safe queue.
  * `mmap-ipc-signal/`: demonstrates inter-thread communication and synchronization using shared memory mapping (mmap). a producer thread modifies the mapped file and forces hardware sync (msync), utilizing standard condition variables to safely signal a waiting consumer thread to read the updated memory.
  * `sync-overhead-bench/`: benchmarks the extreme latency penalty introduced by mutex locks during high-frequency variable increments compared to an unsynchronized data race.
  * `hardware-reordering/`: demonstrates cpu out-of-order execution by showing how hardware can reorder independent loads and stores, even when using an asm volatile compiler barrier, leading to unexpected memory states.
  * `sequential-consistency/`: solves the hardware reordering problem by utilizing std::atomic with memory_order_seq_cst, enforcing strict global memory ordering and preventing cpu instruction reordering.

* **`memory/`** — focus: memory allocation, heap management, raw pointers, and execution faults.
  * `custom-malloc/`: implements a minimal userspace heap allocator using sbrk() to request memory from the kernel, tracking blocks with a linked list of headers for first-fit recycling.
  * `pointer-arithmetic/`: demonstrates manual heap allocation using new/delete[], utilizing raw pointers to reverse an array in-place.
  * `leaks/`: simulates a memory leak by overriding a pointer before freeing its block, creating unreachable memory (detected via valgrind).
  * `double-free/`: causes a runtime abort and core dump by calling delete twice on the same pointer, corrupting the internal cache of the memory allocator.
  * `dangling-pointer/`: triggers an invalid write error by attempting to assign a value to a memory address that has already been released back to the heap.
  * `null-deref/`: intentionally passes a null pointer to a function that dereferences it, generating a segmentation fault for gdb stack trace analysis.
  * `pagemap/`: translates a virtual memory address to its corresponding physical address by reading /proc/self/pagemap, illustrating page frame calculation.
  * `manual-reallocation/`: illustrates dynamic array mechanics by allocating a larger heap block, copying old data, and safely freeing previous memory to prevent internal fragmentation.


* **`experiments/`** — focus: unstructured sandbox for testing new concepts.
  * `scratch/`: temporary files and quick syntax checks.
  * `quick-tests/`: mini-benchmarks for specific functions.
    * `pipe-flood/`: continuously writes and reads from a pipe to observe basic i/o synchronization and buffer behavior.
  * `one-off/`: isolated single-file scripts that do not fit a larger module.
    * `nonblocking-wait/`: tests concurrent parent-child execution by polling the child's status using waitpid with the wnohang flag.
    * `sigint-handler/`: simple isolated script replacing the default ctrl+c termination with a custom sigint signal handler for graceful exits.
    * `parallel-mmap-scanner/`: an advanced capstone experiment combining process creation (fork), raw directory parsing (getdents64), and zero-copy file mapping (mmap). implements data parallelism by spawning multiple threads to scan isolated memory chunks concurrently, utilizing atomic counters and getrusage for system-level benchmarking.


## how to run

each project folder contains self-contained experiments. standard compilation follows this format:

```bash
g++ -wall -wextra -o3 folder-name/main.cpp -o app_name
./app_name
