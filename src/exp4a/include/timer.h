#ifndef	_TIMER_H
#define	_TIMER_H

/* These are for "System Timer". See timer.c for details */
void timer_init ( void );
void handle_timer_irq ( void );

/* below are for Arm generic timers */
void generic_timer_init ( void );
void handle_generic_timer_irq ( void );

int find_mini_sleep();
void update_sleep();
void update_future(int new_future);

extern void gen_timer_init();
/* set timer to be fired after @interval System ticks */
extern void gen_timer_reset(int interval); 
extern int interval;

#endif  /*_TIMER_H */
