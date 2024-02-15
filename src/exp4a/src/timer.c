#include "utils.h"
#include "printf.h"
#include "sched.h"
#include "peripherals/timer.h"
#include "timer.h"

#ifdef USE_QEMU
int interval = (1 << 26); // xzl: around 1 sec
#else
int interval = 1 * 1000 * 1000; // xzl: around 1 sec
#endif

/* 	These are for Arm generic timers. 
	They are fully functional on both QEMU and Rpi3.
	Recommended.
*/
void generic_timer_init ( void )
{
	gen_timer_init();
}

int find_mini_sleep(){
		int global_min = 1000;
		struct task_struct * q;
		for(int i = 0; i < NR_TASKS; i++){
			q = task[i];
			if(q && q->state == TASK_WAIT && q->sleep_time > 0 && q->sleep_time < global_min){
				global_min = q->sleep_time;
			}
			
		}
		return global_min;
}

void update_future(int new_future){
	struct task_struct * p;
	for(int i = 0; i < NR_TASKS; i++){
		p = task[i];
		if(p && p->state == TASK_WAIT){
			p->future_time = new_future;
		}
	}
}

void update_sleep(){
	struct task_struct * p;
	for(int i = 0; i < NR_TASKS; i++){
		p = task[i];
		if(p && p->state == TASK_WAIT){
			if(p->future_time <= p->sleep_time){
				p->sleep_time = p->sleep_time - p->future_time;
				// printf("updated in %u ticks\n\r", p->sleep_time);

			}
		}
	}

}

void handle_generic_timer_irq( void ) 
{
	// TODO: In order to implement sleep(t), you should calculate @interval based on t, 
	// instead of having a fixed @interval which triggers periodic interrupts
	struct task_struct * p;

	update_sleep();

	//check previous sleep() request
	for(int i = 0; i < NR_TASKS; i++){
		p = task[i];
		if(p && p->state == TASK_WAIT && p->sleep_time == 0){
			p->state = TASK_RUNNING;
		}
	}

	int next_sleep_time = find_mini_sleep();
	update_future(next_sleep_time);
	gen_timer_reset(next_sleep_time * interval);	
	printf(" Timer interrupt received. next in %u ticks\n\r", next_sleep_time);
}


