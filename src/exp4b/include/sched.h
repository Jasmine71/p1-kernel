#ifndef _SCHED_H
#define _SCHED_H

#define THREAD_CPU_CONTEXT			0 		// offset of cpu_context in task_struct 

#ifndef __ASSEMBLER__

#define THREAD_SIZE				4096

#define NR_TASKS				64 
#define NUM_TRACES              50

#define FIRST_TASK task[0]
#define LAST_TASK task[NR_TASKS-1]

#define TASK_RUNNING				0

extern struct task_struct *current;
extern struct task_struct * task[NR_TASKS];
// extern struct trace_log *log;

// extern struct context_switch *cur_entry;
extern int nr_tasks;
extern int index_log;


struct cpu_context {
	unsigned long x19;
	unsigned long x20;
	unsigned long x21;
	unsigned long x22;
	unsigned long x23;
	unsigned long x24;
	unsigned long x25;
	unsigned long x26;
	unsigned long x27;
	unsigned long x28;
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
};

struct task_struct {
	struct cpu_context cpu_context;
	long state;	
	long counter; /* countdown for scheduling. higher value means having run for less. recharged in schedule(). decremented in timer_tick(). always non negative */
	long priority;
	long preempt_count;
	long pid;
	long first_time; //0 first time, 1 not first time
	unsigned long init_sp;
	unsigned long init_pc;
};

struct context_switch{
	unsigned long cur_time; //ms
	int id_from;
	int id_to;
	unsigned long pc_from;
	unsigned long pc_to;
	unsigned long sp_from;
	unsigned long sp_to;
};

// struct trace_log{
// 	struct context_switch * trace[NUM_TRACES];
// 	int index;
// };

extern void sched_init(void);
extern void schedule(void);
extern void timer_tick(void);
extern void preempt_disable(void);
extern void preempt_enable(void);
extern void switch_to(struct task_struct* next);
extern void cpu_switch_to(struct task_struct* prev, struct task_struct* next);
extern int getpid();
extern unsigned long get_pc(void);
extern unsigned long get_sp(void);
extern struct context_switch log[NUM_TRACES];
extern void check_sched_time();

#define INIT_TASK \
/*cpu_context*/	{ {0,0,0,0,0,0,0,0,0,0,0,0,0}, \
/* state etc */	0,0,1, 0,0,1,0,0 \
}

#endif
#endif
