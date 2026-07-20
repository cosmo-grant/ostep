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
arrives:      A  B  C
running:      A     B    C
              |--|--|----|----|
              0  1  2    4    6
```

average turnaround time = (2 + 3 + 4) / 3 = 3
average response time = (0 + 1 + 2) / 3 = 1

FIFO gives poor turnaround time in convoys:

```
arrived:      ABC
running:      A         B C
completed:              A B C
              |---------|-|-|
              0         8 9 10
```

average turnaround time = 9

SJF gives poor turnaround time in delayed convoys:

```
arrived:      A B C
running:      A         B C
completed:              A B C
              |-|-|-----|-|-|
              0 1 2     8 9 10
```

average turnaround time = 8

Visualize the schedule, assuming:
  SJF
  A arrives at 0, B at 1, C at 2.
  A takes 8, B takes 1, C takes 1.

```
arrived:      A B C
running:      A         B C
completed:              A B C
              |-|-|-----|-|-|
              0 1 2     8 9 10
```

Visualize the schedule, assuming:
  PSJF
  A arrives at 0, B at 1, C at 2.
  A takes 8, B takes 1, C takes 1.

```
arrived:      A B C
running:      A B C A
completed:        B C       A
              |-|-|-|-------|
              0 1 2 3       10
```
  
Visualize the schedule, assuming:
  PSJF
  A, B, C arrive at 0.
  All take 2.

```
arrived:      ABC
scheduled:    A   B   C
completed:        A   B   C
              |---|---|---|
              0   2   4   6
```
 
Visualize the schedule, assuming:
  RR with time slice 1.
  A, B, C arrive at 0.
  All take 2.

```
arrived:      ABC
running:      A B C A B C
completed:            A B C
              |-|-|-|-|-|-|
              0 1 2 3 4 5 6
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
