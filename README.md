# 🧵 Codexion

## Description

**Codexion** is a concurrency simulation written in C, inspired by the classical **Dining Philosophers** problem. Instead of philosophers and forks, it models a circular workspace where multiple **Coders** (POSIX threads) compete for a limited number of **USB Dongles** (shared resources protected by mutexes) in order to compile their work.

If a Coder goes too long without compiling, they burn out — and the simulation ends.

The project is a deep dive into systems programming fundamentals: thread lifecycle management, deadlock prevention, starvation avoidance, and scheduling policies.

---

## ✨ What it covers

- POSIX thread creation and lifecycle management
- Mutex-based synchronization of shared resources
- Deadlock prevention via Even/Odd acquisition ordering (breaks circular wait)
- Starvation avoidance with two scheduling policies: **FIFO** and **EDF**
- Dongle cooldown enforcement with temporal correctness
- Thread-safe monitoring and coordinated simulation termination
- Serialized logging with zero interleaved output

---

## 🚀 Quick Start

### Compile

```bash
make
```

### Run

```bash
./codexion <coders> <t_burnout> <t_compile> <t_debug> <t_refactor> <n_compiles> <cooldown> <scheduler>
```

**Example:**

```bash
./codexion 5 800 200 200 200 7 100 fifo
```

---

## ⚙️ Arguments

| Argument | Description | Example |
|----------|-------------|---------|
| `coders` | Number of coder threads (and dongles) | `5` |
| `t_burnout` | Max time (ms) a coder can survive without compiling | `800` |
| `t_compile` | Time (ms) spent compiling — requires 2 dongles | `200` |
| `t_debug` | Time (ms) spent debugging — no dongles held | `200` |
| `t_refactor` | Time (ms) spent refactoring — no dongles held | `200` |
| `n_compiles` | Simulation ends when all coders reach this compile count | `7` |
| `cooldown` | Time (ms) a dongle stays unavailable after use | `100` |
| `scheduler` | Arbitration policy: `fifo` or `edf` | `fifo` |

---

## 🔄 Coder Lifecycle

```
         ┌─────────────────────────────────┐
         │                                 │
         ▼                                 │
    [THINKING]  ──► wait for 2 dongles ──► [COMPILING] ──► release dongles
         ▲                                     │
         │                                     ▼
         └──────── [DEBUGGING / REFACTORING] ◄─┘

    Monitor thread: continuously checks time since last compile
    If elapsed > t_burnout → sets termination flag → all threads exit cleanly
```

---

## 🛡️ Concurrency Challenges Solved

### 1. Deadlock Prevention

Deadlock requires four Coffman conditions: mutual exclusion, hold-and-wait, no preemption, and circular wait. Since mutual exclusion is unavoidable (dongles are exclusive), the implementation eliminates **circular wait** using an **Even/Odd acquisition strategy**:

- Even-indexed coders pick up the **left dongle first**
- Odd-indexed coders pick up the **right dongle first**

This deterministic ordering breaks cyclic dependencies at the structural level.

### 2. Starvation Prevention

Two scheduling policies are available:

**FIFO** — Fairness by arrival order. Coders waiting longest get priority.

**EDF (Earliest Deadline First)** — Dynamically prioritizes the coder whose `last_compile_start` timestamp is oldest. Reduces the risk of indefinite postponement for time-critical threads.

### 3. Dongle Cooldown

Each dongle enforces a cooldown period after use, protected by `cldw_lock` and tracked via `available_time`. Prevents premature reuse without introducing race conditions.

### 4. Burnout Detection

A dedicated **Monitor thread** continuously audits coder states:
- Locks `compile_mutex` to safely read `last_compile_start`
- Compares elapsed time against `t_burnout`
- Sets `monitor_flag` (protected by `death_lock`) to trigger clean shutdown

Guarantees no duplicate termination triggers and no inconsistent reads during shutdown.

### 5. Log Serialization

All console output is protected by `write_lock`. No interleaved messages, no garbled logs — deterministic output regardless of thread scheduling.

---

## 🔒 Synchronization Architecture

The locking strategy is segmented to avoid unnecessary bottlenecks:

**Global control (`s_data`)**

| Mutex | Purpose |
|-------|---------|
| `threads_lock` | Barrier ensuring all threads start simultaneously |
| `write_lock` | Serializes console output |
| `death_lock` | Protects the `monitor_flag` termination signal |

**Per-dongle (`s_dongle`)**

| Mutex | Purpose |
|-------|---------|
| `dngl` | Exclusive physical possession of a dongle |
| `cldw_lock` | Protects `available_time` for temporal checks |
| `queue_lock` | Guards scheduling queues (`first_queue`, `second_queue`) |

**Per-coder (`s_coder`)**

| Mutex | Purpose |
|-------|---------|
| `compile_mutex` | Protects `last_compile` and `times_compiled` — read safely by the Monitor |

---

## 📁 Project Structure

```
Threads/
├── coders/         # Source files: coder threads, monitor, scheduling logic
├── Makefile
└── README.md
```

---

## 📋 Requirements

- C compiler with POSIX support (`gcc` / `clang`)
- Linux or macOS (POSIX threads)
- `make`

---

## 📚 References

- [Introduction to Threads (video)](https://www.youtube.com/watch?v=zfI78DmjIhY)
- [Dining Philosophers Problem explained (video)](https://www.youtube.com/watch?v=ZdBCdoAJdmw)
- [POSIX Threads Programming — LLNL](https://hpc-tutorials.llnl.gov/posix/)
