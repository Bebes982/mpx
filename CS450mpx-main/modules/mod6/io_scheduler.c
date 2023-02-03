#include "io_scheduler.h"
#include "DCB.h"
#include "IOCB.h"
#include "../data-structures/queue.h"

io_scheduler* sched = NULL;


void initialize_io_scheduler(){
    // initialize vars
    io_scheduler* scheduler = (io_scheduler*)sys_alloc_mem(sizeof(io_scheduler));
    scheduler->active_pcb = NULL;
    scheduler->com1_queue = create_queue();

    scheduler->event_flag = 0;
    scheduler->event_ptr = &scheduler->event_flag;

    // initialize dcb
    com_open(scheduler->event_ptr, 1200);

    scheduler->com1 = get_dcb();

    sched = scheduler;
}


int enqueue_iocb(IOCB* iocb){
    // verify params - oper is read/write, device recognized, oper legal and for specified device
    if(iocb->code != READ && iocb->code != WRITE){
        return 1;
    }
    if(iocb->b_ptr == NULL){
        return 1;
    }
    if(iocb->c_ptr == NULL || *iocb->c_ptr < 0){
        return 1;
    }
    // if(iocb->params->device_id != COM1 ){
    //     return 1;
    // }
    
    // check status of relevent dcb
    // if idle, process, else add to queue
    if(sched->com1->status == DCB_IDLE){
        // start process
        // call appropriate driver (dcb?) - needs pcb*, device id (ex com1?), oper code (read/write?)
        sched->active_pcb = iocb->pcb;
        if(iocb->code == READ){
            com_read(iocb->b_ptr, iocb->c_ptr);
        }else if(iocb->code == WRITE){
            com_write(iocb->b_ptr, iocb->c_ptr);
        }
    }else{
        // add to correct queue
        enqueue(sched->com1_queue, iocb);
    }
    // return to sys call
    return 0;
}

PCB* process_next_iocb(){
    // set iocb->active_pcb to null
    PCB* finished = sched->active_pcb;
    sched->active_pcb = NULL;
    
    // initiate next iocb transfer from queue
    if(sched->com1->status == DCB_IDLE){
        // dequeue next iocb and set it to active
        IOCB* iocb = dequeue(sched->com1_queue);
        if(iocb == NULL){
            return finished;
        }

        sched->active_pcb = iocb->pcb;
        
        // initiate operation
        if(iocb->code == READ){
            com_read(iocb->b_ptr, iocb->c_ptr);
        }else if(iocb->code == WRITE){
            com_write(iocb->b_ptr, iocb->c_ptr);
        }
    }else{
        return NULL; // should be idle but isn't
    }

    return finished;
}

int* get_event_flag(){
    return sched->event_ptr;
}