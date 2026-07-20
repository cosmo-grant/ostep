# Chapter 5

## 1

a

a
└── b

a
└── b
└── c

a
└── b

a
└── b
└── d

a
└── b
└── d
└── e

## 2

As fork_percentage increases, the final tree will be wider and deeper.
I'm not sure what "angle" from a to expect for large fork_percentage.
On the one hand, a gets lots of chances to fork, since always around.
So expect a big angle.
On the other hand, after a few steps, a is one process among many.
So expect a small angle.

## 3

Yes.
Comparing tree 1 and tree 2, if a process has vanished, the action was that that process exited.
If a process has appeared, the action was that that process's parent forked.

## 4

By default orphaned processes become children of the root.
With `-R`, they become children of their grandparent.

>In a Unix-like operating system any orphaned process will be immediately adopted by an implementation-defined system process: the kernel sets the parent to this process. This operation is called re-parenting and occurs automatically. Even though technically the process has a system process as its parent, it is still called an orphan process since the process that originally created it no longer exists. In other systems orphaned processes are immediately terminated by the kernel. Most Unix systems have historically used init as the system process to which orphans are reparented, but in modern DragonFly BSD, FreeBSD, and Linux systems, an orphan process may be reparented to a "subreaper" process instead of init.

## 6

Given number of actions and initial and final tree, can you tell what happened?

You can't tell order.

You might end up with just a, after 100 actions, which leaves wide open what happened.
(E.g. maybe a forked and then its child exited, 50 times.
Or maybe we got a1 - ... - a51 after all forks then back to a1 after 50 exits.)

In which cases _can_ you tell?
If we ended up a1 - ... - a101, then all forks of youngest process.
More generally, if we have n+1 processes after n actions then all forks.
