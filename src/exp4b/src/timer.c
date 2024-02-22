#include "utils.h"
#include "printf.h"
#include "sched.h"
#include "peripherals/timer.h"
#include "timer.h"
#include "irq.h"



#ifdef USE_QEMU
int interval = (1 << 26); // xzl: around 1 sec
#else
int interval = 1 * 1000 * 1000; // xzl: around 1 sec
#endif

unsigned long start_tick = 0;// count value not ms
unsigned long frequency = 0;


/* 	These are for Arm generic timer. 
	They are fully functional on both QEMU and Rpi3 
	Recommended.
*/
void generic_timer_init ( void )
{
	printf("interval is set to: %u\r\n", interval);
	gen_timer_init();
	gen_timer_reset(interval / 10);
}

void handle_generic_timer_irq( void ) 
{
	if(index_log == 50){
		//Todo: print and disable irq
		disable_irq();
		preempt_disable();
		for(int i = 0; i < 50; i++){
			struct context_switch *entry = &log[i];
			printf("%u from task%u (PC %x SP %x) to task%u (PC %x SP %x)\r\n", entry->cur_time,
			entry->id_from, entry->pc_from, entry->sp_from, entry->id_to,
			entry->pc_to, entry->sp_to);
		}
		index_log = 0;
		
		return;
	}
	gen_timer_reset(interval / 10);
    timer_tick();
}

/* 
	These are for "System Timer". They are NOT in use by this project. 
	I leave the code here FYI. 
	Rpi3: System Timer works fine. Can generate intrerrupts and be used as a counter for timekeeping.
	QEMU: System Timer can be used for timekeeping. Cannot generate interrupts. 
		You may want to adjust @interval as needed
	cf: 
	https://fxlin.github.io/p1-kernel/exp3/rpi-os/#fyi-other-timers-on-rpi3
*/
unsigned int curVal = 0;

void timer_init ( void )
{
	curVal = get32(TIMER_CLO);
	curVal += interval;
	put32(TIMER_C1, curVal);
}

void handle_timer_irq( void ) 
{
	curVal += interval;
	put32(TIMER_C1, curVal);
	put32(TIMER_CS, TIMER_CS_M1);
	timer_tick();
}

unsigned long get_time_ms(){
	unsigned long cur_tick = (unsigned long)get_CNTPCT_EL0();
	// printf("irq happen tick : %u \r\n", cur_tick);
	unsigned long diff = cur_tick - start_tick;
	if(frequency == 0){
		frequency = diff; //100 ms
	}
	return diff / frequency * 100;
}