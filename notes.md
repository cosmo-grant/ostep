# OSTEP Notes

## Chapter 6

Limited direct execution:
  - limited, because only the OS can execute certain instructions
  - direct, because all processes run directly on the CPU, not via an interpreter

A user process makes system calls to request the kernel perform privileged operations on its behalf.

To make a system call, a program executes the trap instruction, which atomically jumps into the kernel and switches to kernel mode.

After completing a system call, the OS executes a return-from-trap instruction, which atomically jumps back into the calling program and switches to user mode.

At boot time, the OS registers trap handler locations with the hardware.

How does the kernel control what code executes given a trap?
By registering trap handler locations with the hardware.

A user process typically informs the OS which system call it wants by putting the system call number in a well-known register or stack location.

The OS ensures it regains control via a timer interrupt, which jumps to an interrupt handler registered with the hardware at boot time.

C library functions which implement system calls are written in assembly: they put the necessary information (e.g. system call number) in well-known locations, execute the trap instruction, then retrieve the results from well-known locations.

Each process has a kernel stack, which is a region of kernel memory used by the OS and hardware when working on the process's behalf.

When context switching from process A to B, there are four saves/restores:
1. the hardware saves user registers to A's kernel stack
2. the OS saves kernel registers at point of switch() to A's process structure
3. the OS restores kernel registers from B's process structure
4. the hardware restores user registers from B's kernel stack

## Chapter 7

turnaround time = completion time - arrival time
response time = first scheduled time - arrival time

fifo = first in first out
sjf = shortest job first
psjf = preemptive shortest job first (shortest job first, re-assessing whenever a job arrives)
psjf aka shortest time-to-completion first
round-robin = run each job in turn for a time slice

Understanding metrics:

```
     0 1 2 3 4 5 6
A    x- -x
B      x  - -x
C        x    - -x
```

average turnaround time = (2 + 3 + 4) / 3 = 3
average response time = (0 + 1 + 2) / 3 = 1

FIFO gives poor turnaround time in convoys:

```
     0 1 2 3 4 5 6 7 8 9 10
A    x- - - - - - - -x
B    x                -x
C    x                  -x
```

average turnaround time = 9

SJF gives poor turnaround time in delayed convoys:

```
     0 1 2 3 4 5 6 7 8 9 10
A    x- - - - - - - -x
B      x              -x
C        x              -x
```

average turnaround time = 8

Visualize the schedule, assuming:
  SJF
  A arrives at 0, B at 1, C at 2.
  A takes 8, B takes 1, C takes 1.

```
     0 1 2 3 4 5 6 7 8 9 10
A    x- - - - - - - -x
B      x              -x
C        x              -x
```

Visualize the schedule, assuming:
  PSJF
  A arrives at 0, B at 1, C at 2.
  A takes 8, B takes 1, C takes 1.

```
     0 1 2 3 4 5 6 7 8 9 10
A    x-     - - - - - - -x
B      x-x
C        x-x
```
  
Visualize the schedule, assuming:
  PSJF
  A, B, C arrive at 0.
  All take 2.

```
     0 1 2 3 4 5 6
A    x- -x
B    x    - -x
C    x        - -x
```
 
Visualize the schedule, assuming:
  RR with time slice 1.
  A, B, C arrive at 0.
  All take 2.

```
     0 1 2 3 4 5 6
A    x-     -x
B    x  -     -x
C    x    -     -x
```

A key tension:
  SJF and similar are good for turnaround time, bad for response time.
  RR and similar are good for response time, bad for turnaround time.

PSJF is optimal for turnaround (assuming CPU only).

In round-robin scheduling, shortening the time slice decreases response time but increases context switch overhead.

## Chapter 8

Multi-Level Feedback Queue (MLFQ) is a scheduling approach which aims to balance turnaround and response time, without knowing a priori how long jobs take.

Rules:
1. New jobs go on the highest queue.
2. Round-robin jobs on the highest non-empty queue.
3. Demote a job one queue when it's used up its allotment on a queue.
4. Periodically promote all jobs to the highest queue.

Why 3? Prevent gaming the scheduler. 

Why 4? Prevent starvation. Cope if a CPU-heavy job becomes IO-heavy.

In an MLFQ, what's the risk of a rule like "reset a job's allotment if it gives up the CPU".
It allows gaming the scheduler: jobs can give up the CPU just before using up their allotment, to stay on the highest queue.

MLFQ uses priority as a learned proxy for job type: IO-heavy jobs sink slowly so get good response times; CPU-heavy jobs sink quickly, letting shorter jobs go first, so the system gets good average turnaround.

Some systems have a cli tool nice, which lets you run programs at altered scheduling priority.

## Chapter 9

A proportional share scheduler (aka fair-share scheduler) tries to ensure jobs get target percentages of CPU time.

Potential advantages of using randomness in decision procedures: avoid edge cases, lightweight, fast.

Lottery schedulers
- assign tickets to jobs (processes, users, etc.)
- periodically hold a lottery to select which job runs next

Ticket manipulations sometimes useful:
- currency: an job allocates tickets to sub-jobs (e.g. users to processes); the system converts to system-level ticket counts
- transfer: jobs can lend each other tickets (e.g. client to server, for the server to do work on the client's behalf)
- inflation: a job can give itself more tickets (e.g. in a cooperative setting, when the job judges it's for the best)

Lottery schedulers probably won't deliver exactly correct proportions, especially over short time scales, so Waldspurger invented stride schedulers.

Stride schedulers:
- assign tickets to jobs
- set strides inversely proportional to ticket counts
- each time a job runs, increment its counter ("pass") by stride
- at each time slice, pick the job with lowest pass

If A gets 100 tickets, B 50, C 250, then the strides are proportional to 5, 10, 2.

Given A has stride 10 and B 25:

```
A    B    runs
---------------
0    0    A
10   0    B
10   25   A
20   25   A
30   25   B
30   50   A
40   50   A
50   50   A
```

An advantage of stride over lottery schedulers: the shares are exactly correct (at the end of each cycle).
An advantage of lottery over stride schedulers: easier to fold in new jobs (no need to pick a pass).

"In a study of Google datacenters, Kanev et al. show that even after aggressive optimization, scheduling uses about 5% of overall datacenter CPU time." p105

From about 2007 to 2023, Linux used the Completely Fair Scheduler (CFS).

How CFS works, to a first approximation:
- maintain per-core runqueues of jobs as red-black trees, sorted by vruntime (i.e. CPU time / priority)
- at switch points, pick from the core's runqueue the job with lowest vruntime
- periodically load balance (i.e. redistribute threads across runqueues)

How does CFS prevent long-sleeping jobs from monopolizing the CPU when they wake up?
By setting an awakened job's vruntime to the runqueue's min.

How does CFS prevent a newly created job from monopolizing the CPU?
By setting its vruntime to the runqueue's max.


## Bouron et al, Battle of the Schedulers

Compares Linux's CFS versus FreeBSD's ULE.

Same:
- Designed to schedule many threads on multicore machines.
- Use per-core runqueues.
- On a context switch, a core only considers its local runqueue.
- Periodically and at select times, they load balance (i.e. redistribute threads across runqueues).

Different:
- ULE is lower LOC than CFS
- ULE runqueues are FIFO
- ULE tries to even out runqueue sizes; CFS tries to even out the "average amount of pending work".

## Chapter 13

A process's address space is its view of memory, an abstraction of physical memory.

A process's memory is divided into four segments:
  text, instructions
  data, static variables
  stack, call stack and local variables
  heap, available for dynamic allocation

Visualize a process's address space:

```
     0KB  +------------------+
          |       code       |
     1KB  +------------------+
          |       heap       |
          |        |         |
          |        v         |
          +------------------+
          |                  |
          |      (free)      |
          |                  |
          +------------------+
          |        ^         |
          |        |         |
          |       stack      |
    16KB  +------------------+
```


Heap and stack are placed at opposite ends of the address space so that they can both grow (towards the middle) without colliding.

Major goals of a virtual memory system: transparency, efficiency, protection.

Every address visible to a program (e.g. when you print a pointer) is a virtual address.

```c
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int x = 1;
  if (fork() == 0) {
    x = 2;
    printf("x=%d at %p\n", x, (void *)&x);
  } else {
    wait(NULL);
    printf("x=%d at %p\n", x, (void *)&x);
  }
}
```

```text
x=2 at 0x7ff7bfeff2ac
x=1 at 0x7ff7bfeff2ac
```

Same address, different values, which proves addresses are virtual.
