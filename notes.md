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

## Chapter 14

In C, stack memory is managed by the compiler and heap memory is managed by the programmer.

If you need a value to be available after its initializing function returns, it must be static or live on the heap.

`malloc(sizeof(int))` - allocate space on the heap for an int, returning a pointer else NULL

`sizeof` is an operator, usually evaluated at compile-time.

`sizeof` on an array gives the array's size in bytes; on a pointer it gives the pointer's size.
So after `int *x = malloc(10 * sizeof(int))`, `sizeof(x)` is the pointer size (e.g. 8), not 40.

`free(ptr)` - free the heap memory allocated by the `malloc()` call which returned ptr

`free` takes no size argument: the allocator tracks the size of each allocation itself, often in a header just before the allocated memory.

`calloc(n, size)` - allocate and zero the memory.
`realloc(ptr, size)` - resize an allocation, preserving contents.

Common memory errors:
- forgetting to allocate memory, e.g. in `strcpy(dst, src)` where `dst` is unallocated
- not allocating enough memory
- uninitialized read
- memory leak
- dangling pointer (use-after-free)
- double free
- invalid free

Two levels of memory management:
- by the OS, giving memory to and reclaiming memory from a process
- by a process, allocating and freeing within the memory it's been given.

`malloc` and `free` are library calls, not system calls.

`malloc` makes a system call (e.g. brk(addr) or sbrk(delta)) only when the process needs to increase the heap segment size.

The program break is the end of a process's heap segment.

The brk(addr) system call sets the program break to addr.
The sbrk(delta) system call moves the program break by delta, returning the old break.

In short-lived programs memory leaks rarely cause trouble because the OS reclaims all a process's memory on exit.

cc -g  - produce debugging information in the OS's native format, usable by gdb
cc -Wall prog.c - compile prog.c, enabling core warnings

gdb a.out - run gdb against the a.out binary

run valgrind's memcheck against myprog - valgrind --leak-check=yes myprog
memcheck is the default tool
--leak-check=yes turns on the detailed memory leak detector
"It's worth fixing errors in the order they are reported, since later errors can be caused by earlier errors."

## Chapter 15

Static relocation was an early address translation mechanism, where a loader rewrites an executable's addresses before running it.
E.g. program's base is 3000, code has `movl 1000, %eax` then the loader rewrites it as `movl 4000, %eax`.

Two problems with static relocation:
- no protection against out-of-bounds addresses computed at runtime
- hard to move an address space mid-execution

Base and bounds (aka dynamic relocation) is an address translation mechanism, where all addresses referenced by a process are incremented by the base and checked against the bound.

In dynamic relocation, what happens if a translated address is out of bounds?
The hardware atomically jumps to a handler registered by the OS at boot time, which typically terminates the process.

The memory management unit (MMU) is the part of the processor that helps with address translation.

The base and bounds address translation mechanism requires two registers per CPU, to store the running process's base and bound.

Why is base and bounds cheap?
Just two operations (add base, compare bound), in hardware.

Given base and bounds, how can the OS relocate a process's memory?
While the process is paused, copy its memory then update its base.

## Chapter 16

A key issue with having a single base and bound for the running process:
- the OS must reserve one contiguous block [base, base + bound)
- a big bound means wasted space (internal fragmentation)
- a small bound risks running out of space

Visualize internal fragmentation:

```
0KB     ┌──────────────┐
        │     code     │
2KB     ├──────────────┤
        │     heap     │
4KB     ├──────────────┤
        │              │
        │              │
        │  (reserved   │
        │  but unused) │
        │              │
        │              │
510KB   ├──────────────┤
        │    stack     │
512KB   └──────────────┘
```

Segmentation is an address translation mechanism, where you store a base and bound _per segment_.

Two benefits of segmentation:
- supports sparse address spaces
- it allows for code sharing between processes

The term "segmentation fault" originates from a memory access on a segmented machine to an illegal address.

How does the hardware tell which segment an address is for?
Two approaches:
- explicit: encoded in reserved address bits
- implicit: based on the address's source, e.g. PC -> code, from frame pointer -> stack

Advantage of explicit versus implicit: addresses are self-describing.
Disadvantage of explicit versus implicit: costs address bits (e.g. 2 bits for 4 segments).

Use an extra register to flag whether a segment grows positive or negative.

Examples of how to calculate physical address:

CONTEXT

7 bit address space

seg high bit │ base │ limit │ growth
─────────────┼──────┼───────┼───────
0            │ 32   │ 20    │ +
1            │ 512  │ 20    │ -


GIVEN virtual address 108 what is physical address?
108 >= 64 so segment 1, offset 44
negative offset 44 - 64 = -20
-20 is within limit
so physical address is 512 - 20 = 492

GIVEN virtual address 97 what is physical address?
97 >= 64 so segment 1, offset 33
negative offset 33 - 64 = -31
magnitude -31 > 20 so segmentation violation

GIVEN virtual address 10 what is physical address?
10 < 64 so segment 0
10 < 20 so in bounds
so 10 + 32 = 42

GIVEN physical address 511 what is virtual address?
need negative offset -1
so offset 63
segment 1
so 64 + 63 = 127 i.e. 1111111

GIVEN physical address 40 what is virtual address?
need offset 8 onto base 32
so 8 

What are valid virtual addresses?
[0, 20) and [108, 128).

What are valid physical addresses?
[32, 52) and [492, 512)

To support sharing memory, we use a few more bits to mark segment permissions.
More work for hardware: it has to check if access is permitted.

A key issue with segmentation is external fragmentation: small holes of free space between segments.

Visualize external fragmentation:

```
0KB     ┌──────────────┐
        │  OS          │
8KB     ├──────────────┤
        │  (free)      │
14KB    ├──────────────┤
        │  P1 code     │
20KB    ├──────────────┤
        │  (free)      │
24KB    ├──────────────┤
        │  P2 heap     │
34KB    ├──────────────┤
        │  (free)      │
39KB    ├──────────────┤
        │  P1 stack    │
48KB    └──────────────┘
```
Two approaches to mitigating external fragmentation:
- compact, i.e. rearrange segments in memory to avoid small holes
- free-list management, i.e. allocate memory carefully to minimize small holes 

Kinds of fragmentation:
- internal: space which is mapped (by the OS) but unallocated (by the process)
- external: unmapped space between segments, enough in total but not enough individually for the request in hand
