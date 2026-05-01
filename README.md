# Codexion

##  Description

Codexion is a concurrency simulation written in C, inspired by the classical Dining Philosophers problem.
The project models a circular working environment where multiple Coders (threads) compete for a limited number of USB Dongles (shared resources protected by mutexes) in order to compile their work.

The primary goal of this project is to master:
- POSIX thread creation and management  
- Safe synchronization of shared resources  
- Deadlock prevention strategies  
- Starvation avoidance mechanisms  
- Thread-safe monitoring and coordinated termination  
- Implementation of scheduling policies (FIFO and EDF)


## Instructions
### Compilation
```
make
```
### Execution
```
./codexion <coders> <t_burnout> <t_compile> <t_debug> <t_refactor> <n_compiles> <cooldown> <scheduler>
```
| Argument | Description | Example |
| :--- | :--- | :--- |
| **coders** | Total number of coders (threads) and dongles. | `5` |
| **t_burnout** | Time (ms) a coder can survive without compiling. | `800` |
| **t_compile** | Time (ms) spent compiling (holding 2 dongles). | `200` |
| **t_debug** | Time (ms) spent debugging (no dongles). | `200` |
| **t_refactor** | Time (ms) spent refactoring (no dongles). | `200` |
| **n_compiles** | Simulation stops if all coders reach this count. | `7` |
| **cooldown** | Time (ms) a dongle remains unavailable after use. | `100` |
| **scheduler** | Arbitration policy: `fifo` or `edf`. | `fifo` |

## Resources

During this project, I encountered several challenges such as:

- Deadlocks
- Arbitration issues
- Dongle cooldowns

I relied on AI assistance to better understand basic concepts such as threads, routines, and deadlocks.

I also watched videos to deepen my understanding of threads, such as:
- https://www.youtube.com/watch?v=zfI78DmjIhY
- https://www.youtube.com/watch?v=ZdBCdoAJdmw


## Blocking Cases Handled

This implementation explicitly addresses the main classical concurrency issues:

1. Deadlock Prevention (Coffman Conditions)

Deadlock requires four conditions: mutual exclusion, hold-and-wait, no preemption, and circular wait.
While mutual exclusion is unavoidable (dongles are exclusive resources), the implementation eliminates circular wait using an Even/Odd acquisition strategy:
Even coders pick up the left dongle first.
Odd coders pick up the right dongle first.
This deterministic ordering breaks cyclic dependencies and prevents structural deadlocks.

2. Starvation Prevention

Two arbitration policies are implemented:
FIFO (First-In, First-Out) – Ensures fairness by respecting arrival order.
EDF (Earliest Deadline First) – Dynamically prioritizes the coder whose last_compile_start timestamp is oldest.
EDF reduces the risk of indefinite postponement for time-critical threads.

3. Cooldown Handling

Each dongle enforces a cooldown period after use.
Protected by cldw_lock
Checked via available_cldw

Prevents premature reuse

This guarantees temporal correctness without introducing race conditions.

4. Precise Burnout Detection

A dedicated Monitor thread continuously checks coder states:
Locks compile_mutex to read last_compile_start
Compares elapsed time against t_burnout

Sets monitor_flag (protected by death_lock) when termination is required

This ensures:
- No inconsistent reads
- No duplicate termination triggers
- No race conditions during shutdown

5. Log Serialization

All console output is protected by write_lock, ensuring:

No interleaved messages
Deterministic output
Thread-safe logging


## Thread Synchronization Mechanisms
The solution utilizes a segmented locking strategy to manage shared resources without bottlenecks:

1. Global Control (s_data)
threads_lock: Act as a synchronization barrier to ensure all coder threads and the monitor start the simulation simultaneously.

write_lock: Serializes console output to maintain readable and ordered logs.

death_lock: Protects the monitor_flag boolean(is int), which signals the end of the simulation (either by burnout or completion).

2. Resource Mutexes (s_dongle): 
dngl: Represents the exclusive physical possession of a dongle.

cldw_lock: Protects the available_time timestamp for safe temporal checks.

queue_lock: Critical for scheduling logic; it protects the explicit waiting slots (first_queue, second_queue) to prevent race conditions during resource arbitration.

3. Coder Mutexes (s_coder)
compile_mutex: Protects the coder's internal statistics (last_compile and times_compiled). This allows the Monitor thread to safely audit the coder's state without causing data races.
