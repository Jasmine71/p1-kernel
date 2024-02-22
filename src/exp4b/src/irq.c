#include "utils.h"
#include "printf.h"
#include "timer.h"
#include "entry.h"
#include "peripherals/irq.h"
#include "sched.h"

const char *entry_error_messages[] = {
    "SYNC_INVALID_EL1t",
    "IRQ_INVALID_EL1t",		
    "FIQ_INVALID_EL1t",		
    "ERROR_INVALID_EL1T",		

    "SYNC_INVALID_EL1h",		
    "IRQ_INVALID_EL1h",		
    "FIQ_INVALID_EL1h",		
    "ERROR_INVALID_EL1h",		

    "SYNC_INVALID_EL0_64",		
    "IRQ_INVALID_EL0_64",		
    "FIQ_INVALID_EL0_64",		
    "ERROR_INVALID_EL0_64",	

    "SYNC_INVALID_EL0_32",		
    "IRQ_INVALID_EL0_32",		
    "FIQ_INVALID_EL0_32",		
    "ERROR_INVALID_EL0_32"	
};

void enable_interrupt_controller()
{
    // Enables Core 0 Timers interrupt control for the generic timer 
    put32(TIMER_INT_CTRL_0, TIMER_INT_CTRL_0_VALUE);
}

void show_invalid_entry_message(int type, unsigned long esr, unsigned long address)
{
    printf("%s, ESR: %x, address: %x\r\n", entry_error_messages[type], esr, address);
}

void handle_irq(void)
{
    // Each Core has its own pending local intrrupts register
    unsigned int irq = get32(INT_SOURCE_0);
    switch (irq) {
        case (GENERIC_TIMER_INTERRUPT):
            handle_generic_timer_irq();
            break;
        default:
            printf("Unknown pending irq: %x\r\n", irq);
    }
}
 void task_from(void){
    if(index_log < 50){
        struct context_switch *cur_entry = &log[index_log];
        unsigned long cur_ms = get_time_ms();
        int id_from = getpid();
        unsigned long sp_value = get_sp();
        unsigned long pc_value = get_pc();
        // printf("\n index %u record task from %u sp: %X pc:%X at time :%u\r\n", index_log, id_from, sp_value, pc_value, cur_ms);
        cur_entry->cur_time = cur_ms;
        cur_entry->id_from = id_from;
        cur_entry->sp_from = sp_value;
        cur_entry->pc_from = pc_value;
    } 

 }

 void task_to(void){
    if(index_log < 50){
        struct context_switch *cur_entry = &log[index_log];
        int id_to = getpid();
        unsigned long sp_value = get_sp();
        unsigned long pc_value = get_pc();
        // printf("\nindex %u record task to %u sp: %X pc:%X \r\n",index_log, id_to, sp_value, pc_value);
        cur_entry->id_to = id_to;
        cur_entry->sp_to = sp_value;
        cur_entry->pc_to = pc_value;
        index_log++;
    }
    
 }