*This project has been created as part of the 42 curriculum by imelero-*

# Codexion

## Description

Codexion is a multithreaded concurrency simulation inspired by scheduling and resource allocation problems. The project models a system of independent “coders” competing for shared limited resources called “dongles” (mutex-protected objects).

Each coder follows a cyclic workflow:
- acquire two dongles
- compile (critical section using both dongles)
- release dongles
- debug (no dongles required)
- refactor (no dongles required)
- repeat until completion or burnout

The simulation supports two scheduling policies:
- FIFO (First In, First Out)
- EDF (Earliest Deadline First)

The goal of the project is to correctly manage concurrency, avoid race conditions, prevent deadlocks, and ensure accurate timing-based lifecycle control for each coder.

---

## Instructions

### Compilation

```
make
```

This will generate the executable:

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

---

### Arguments

- number_of_coders: number of threads and dongles in the system
- time_to_burnout: maximum time (ms) a coder can go without starting a new compile
- time_to_compile: time spent compiling (requires 2 dongles)
- time_to_debug: time spent debugging
- time_to_refactor: time spent refactoring
- number_of_compiles_required: number of successful compilations before stopping
- dongle_cooldown: cooldown time before a released dongle becomes available again
- scheduler: scheduling policy (fifo or edf)

---

## Blocking cases handled

The implementation explicitly addresses the following concurrency problems:

- Deadlock prevention:
  - Ordered locking strategy (even/odd acquisition order)
  - Avoiding circular wait conditions (Coffman condition mitigation)

- Starvation prevention:
  - EDF scheduling prioritizes coders with the closest burnout deadline
  - Ensures long-waiting coders eventually acquire resources

- Precise burnout detection:
  - Central monitor thread tracks last compile timestamps
  - Immediate simulation stop on violation

- Race condition handling:
  - All shared state (state, last_compile, compiles) protected by mutexes

- Log serialization:
  - Print operations are implicitly serialized via mutex-protected state checks (and/or dedicated print mutex if used)

- Resource cooldown handling:
  - Dongles become unavailable for a fixed cooldown period after release

---

## Thread synchronization mechanisms

The project uses pthread-based synchronization primitives:

### pthread_mutex_t

Used to protect:

- Dongles (each dongle has its own mutex)
- Global simulation state (params->mutex)
- Shared coder state (last_compile, compiles)
- Prints

### Coordination strategy

- Dongle mutexes ensure exclusive access to shared resources
- Global mutex ensures safe reads/writes of simulation state
- Monitor thread safely observes coders without data races

### Example race condition prevention

Without mutex:
- two coders could simultaneously “acquire” the same dongle

With mutex:
- only one thread can lock a dongle at a time, ensuring exclusivity

---

## Resources

### References

- POSIX Threads (pthreads) documentation
- Operating Systems: Three Easy Pieces (OSTEP) — Concurrency chapter
- Scheduling algorithms:
  - FIFO scheduling theory
  - Earliest Deadline First (EDF)
- Coffman conditions (deadlock theory)

### AI usage

Artificial intelligence was used for:
- explanation of mutex behavior and race conditions
- documentation structuring (README formatting and required sections)