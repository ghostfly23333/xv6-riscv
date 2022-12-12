// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
};

#define SEM_MAX_NUM 128

struct sem
{
  struct spinlock lock;   
  int resource_count;
  int *procs;
  int allocated;
};
extern int sem_in_use;
extern struct sem sems[SEM_MAX_NUM];
