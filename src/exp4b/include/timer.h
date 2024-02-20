#ifndef	_TIMER_H
#define	_TIMER_H

void timer_init ( void );
void handle_timer_irq ( void );

void generic_timer_init ( void );
void handle_generic_timer_irq ( void );

extern unsigned long start_tick;
extern unsigned long frequency;

extern void gen_timer_init();
extern void gen_timer_reset(int interval);
extern unsigned int get_CNTPCT_EL0();
extern unsigned long get_time_ms();

#endif  /*_TIMER_H */
