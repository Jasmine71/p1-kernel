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

void handle_generic_timer_irq( void ) 
{
	// TODO: In order to implement sleep(t), you should calculate @interval based on t, 
	// instead of having a fixed @interval which triggers periodic interrupts
	struct task_strcuts * p;
	int mini_sleep_time = 1000;
	int prev_sleep_time = 0;
	for(int i = 0; i < NR_TASKS; i++){
		p = task[i];
		if(p && p->state == TASK_WAIT && p->sleep_time < mini_sleep_time){
			mini_sleep_time = p->sleep_time - prev_sleep_time;	
		}
	}
	prev_sleep_time = mini_sleep_time;
	gen_timer_reset(mini_sleep_time);	
	printf("Timer interrupt received. next in %u ticks\n\r", interval);
}
