#ifndef	_IRQ_H
#define	_IRQ_H

void enable_interrupt_controller( void );

void irq_vector_init( void );
void enable_irq( void );
void disable_irq( void );
void task_to();
void task_from();

#endif  /*_IRQ_H */
