#ifndef MPX_SYSCALL
#define MPX_SYSCALL

#include <system.h>
#include "../modules/data-structures/PCB.h"
#include "../modules/mpx_supt.h"

//GLOBAL CURRENTLY OPERATING PROCESS
extern PCB* cop;
uint32_t* sys_call(context* c);

void serial_port_interrupt();
void read_interrupt();
void write_interrupt();
void line_status_interrupt();
void modem_interrupt();
#endif