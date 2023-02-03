#ifndef io_scheduler_H
#define io_scheduler_H

#include "../mpx_supt.h"
#include "../data-structures/PCB.h"
#include "DCB.h"
#include "IOCB.h"

typedef struct io_scheduler{
    queue* com1_queue;
    PCB* active_pcb;
    DCB* com1;
    int* event_ptr;
    int event_flag;
} io_scheduler;

void initialize_io_scheduler();

PCB* process_next_iocb();

int enqueue_iocb(IOCB* iocb);

int* get_event_flag();

#endif