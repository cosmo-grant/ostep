# Chapter 4

## 1

100%

In the simulation, each instruction either uses the CPU or issues IO.
In this example, no IO, so all CPU.
(The simulation doesn't model time spent switching or loading or anything like that.
We just model running instructions or waiting to run them.)

## 2

11 units

4 for Proc1 cpu.
1 for Proc2 to issue IO.
5 while Proc2 is blocked.
1 for Proc2 to handle IO.

## 3

7

1 for Proc1 to issue IO.
5 while Proc1 is blocked...
...during the first 4 of which Proc2 can complete.
1 for Proc1 to handle IO.

So, yes it makes a difference, because in this case there is a process which can use the cpu while the other process is blocked on IO.


## 4

11

1 for Proc1 to issue IO.
5 while Proc1 is blocked.
1 for Proc1 to handle IO.
4 for Proc2 cpu.

## 5

Same as 3

## 6

31

1 for Proc1 to issue IO.
5 for Proc2 to complete while Proc1 is blocked.
Not clear what runs next: Proc1 or Proc3?
I suppose Proc3.
5 for Proc3 to complete.
Not clear what runs next: Proc1 or Proc4?
I suppose Proc4.
5 for Proc4 to complete.
1 for Proc1 to handle IO.
2 * (1 + 5 + 1) = 14 for Proc1 to issue, wait for, handle two more IOs.

So the scheduling logic seems to be: when IO_RUN_LATER, chooses current _or new_ over io issuer.

Not effective.
More interleaving between IO and CPU would be better.

## 7

21

1 for Proc1 to issue IO.
5 for Proc2 to complete while Proc1 is blocked.
1 for Proc1 to handle IO.
1 for Proc1 to issue IO.
5 for Proc3 to complete while Proc1 is blocked.
1 for Proc1 to handle IO.
1 for Proc1 to issue IO.
5 for Proc4 to complete while Proc1 is blocked.
1 for Proc1 to handle IO.

100% CPU

Why run immediately a process which just completed an IO?
If it issued one, it's likely to issue another, and good to issue early, while there are still CPU-hungry processes we can run meanwhile.

## 8

```text
% uv run python process-run.py -s 1 -l 3:50,3:50
Produce a trace of what would happen when you run these processes:
Process 0
  cpu
  io
  io_done
  io
  io_done

Process 1
  cpu
  cpu
  cpu

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```

15

1 for proc0 cpu.
1 for proc0 to issue io.
5 for proc0 to wait for io, during first 3 of which proc1 can complete.
1 for proc0 to handle io.
1 + 5 + 1 = 7 for proc0 to issue, wait, handle io.

```text
% uv run python process-run.py -s 2 -l 3:50,3:50                                   master
Produce a trace of what would happen when you run these processes:
Process 0
  io
  io_done
  io
  io_done
  cpu

Process 1
  cpu
  io
  io_done
  io
  io_done

Important behaviors:
  System will switch when the current process is FINISHED or ISSUES AN IO
  After IOs, the process issuing the IO will run LATER (when it is its turn)
```

time    proc0       proc1
1       RUN:io      READY
2       BLOCKED     RUN:cpu
3       BLOCKED     RUN:io
4       BLOCKED     BLOCKED
5       BLOCKED     BLOCKED
6       BLOCKED     BLOCKED
7       RUN:io_done BLOCKED
8       RUN:io      BLOCKED
9       BLOCKED     RUN:io_done      
10      BLOCKED     RUN:io
11      BLOCKED     BLOCKED   
12      BLOCKED     BLOCKED   
13      BLOCKED     BLOCKED   
14      RUN:io_done BLOCKED   
15      RUN:cpu     BLOCKED
16      DONE        RUN:io_done


