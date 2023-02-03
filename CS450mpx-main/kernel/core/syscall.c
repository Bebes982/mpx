
#include <core/syscall.h>
#include <system.h>
#include "modules/data-structures/PCB.h"
#include "modules/mod6/io_scheduler.h"
#include "modules/mod6/IOCB.h"

#include "modules/mod6/DCB.h"
#include <core/io.h>


DCB* _dcb = NULL;

// Interrupt causes
#define NOT_SERIAL   0b001
#define MODEM_STATUS 0b000
#define OUTPUT       0b010
#define INPUT        0b100
#define LINE_STATUS  0b110

context* original_context = NULL;

PCB* cop = NULL;

uint32_t* sys_call(context* c){
    PCB *next_process = dequeue_next_pcb( ready_queue);

    // check event flag
    int* event_ptr = get_event_flag();
    if(*event_ptr == 1){ // should be multiple to check
        // completion sequence

        // initiate next iocb transfer from queue
        PCB* finished_pcb = process_next_iocb();

        // return pcb to ready state and queue
        // should reference pcb associated with event_flag, not cop
        if(finished_pcb){
            remove_pcb(finished_pcb);
            finished_pcb->execution_state = PCB_READY;
            insert_pcb(finished_pcb);
        }
        
        *event_ptr = 0;
    }


    if(original_context == NULL){
        original_context = c;
    }
    else if(!next_process){ // there are no processes at all
        return (uint32_t*) original_context;
    }

    if(!cop){ //there is no cop
        cop = next_process;
        cop->execution_state = PCB_READY;
        return (uint32_t*) cop->stack_top;
    }

    if (params.op_code == IDLE)
    {
        cop->stack_top = (unsigned char*)c;

        if(next_process){
            cop->execution_state = PCB_READY;
            insert_pcb(cop);
        }
        else return (uint32_t*) cop->stack_top;
    }
    else if(params.op_code == EXIT){
        free_pcb(cop);
    }
    else if (params.op_code == READ || params.op_code == WRITE){
        // update execution state
        cop->stack_top = (unsigned char*)c;
        cop->execution_state = PCB_BLOCKED;
        insert_pcb(cop);

        // pass to io scheduler
        IOCB* iocb = (IOCB*)sys_alloc_mem(sizeof(IOCB*));

        iocb->code = params.op_code;
        iocb->id = params.device_id;
        iocb->b_ptr = params.buffer_ptr;
        iocb->c_ptr = params.count_ptr;

        iocb->pcb = cop;

        enqueue_iocb(iocb);
    }
  
  if (next_process != NULL){
     cop = next_process;
     cop->execution_state = PCB_RUNNING;
     return (uint32_t*) cop->stack_top;
  }

    else return (uint32_t*)original_context;
}

context* original_context_serial = NULL;

void  serial_port_interrupt(){ // first level interrupt
    _dcb = get_dcb();

    if(!_dcb)
        return;

    // 1.	If the port is not open, clear the interrupt and return. 
    if(_dcb->open != DCB_OPEN){
        outb(COM1 + 2, inb(COM1 + 2) & ~0x06); //  clear bits 2 and 3 
        return;
    }

    // 2.	Read the Interrupt ID register to determine the exact cause of the interrupt. 
    //  Bit 0 must be a 0 if the interrupt was actually caused by the serial port

    char interrupt_cause = inb(COM1+2) & 0b111;

    // 3.	Call the appropriate second-level handler. 
    if(interrupt_cause & NOT_SERIAL){
        return;
    }
    if(interrupt_cause == MODEM_STATUS){
        modem_interrupt();
    }
    if(interrupt_cause == OUTPUT){
        write_interrupt();
    }
    if(interrupt_cause == INPUT){
        read_interrupt();
    }
    if(interrupt_cause == LINE_STATUS){
        line_status_interrupt();
    }

    // 4. Clear the interrupt by sending EOI to the PIC command register. 
    outb(0x20,0x20);
    return;
}


// input interrupt handler
void read_interrupt(){
    _dcb = get_dcb();
    // 1.	Read a character from the input register. 
    char input = inb(COM1);
    _dcb->chars_processed++; 
    _dcb->to_be_processed--;   

    // 2.	If the current status is not reading, store the character in the ring buffer. 
    //      If the buffer is full, discard the character. 
    //      In either case return to the first-level handler. Do not signal completion. 

    if(_dcb->status != DCB_READING){ // If the current status is not reading,
        if(_dcb->ring_buf_full == RING_BUF_FULL){ //If the buffer is full, discard the character.
            return; // return to the first-level handler. Do not signal completion.
        }
        else{ // store the character in the ring buffer.
            _dcb->ring_buffer[_dcb->ring_buf_write_index] = input;
            _dcb->ring_buf_write_index = (_dcb->ring_buf_write_index + 1) % RING_BUFFER_LEN;
            _dcb->ring_buf_full = RING_BUF_NOT_FULL;
            if(_dcb->ring_buf_write_index == _dcb->ring_buf_read_index){
                _dcb->ring_buf_full = RING_BUF_FULL;
            }
            return; // return to the first-level handler. Do not signal completion.
        }
    }
    // 3. current status is reading - store char is requestor's input buffer
    if(_dcb->status == DCB_READING){
        *(_dcb->buffer + _dcb->chars_processed * sizeof(char)) = input;
    }
    // 4.	If the count is not completed and the character is not CR, return. Do not signal completion. 
    // 5. Otherwise, the transfer has completed. Set the status to idle. Set the event flag and return the requestor's count value. 
    if(_dcb->to_be_processed > 0 && input != '\r'){
        return;
    }else{
        _dcb->status = DCB_IDLE;
        *_dcb->event_f = EVENT_SET;
        return;
    }
    
} 


//output interrupt handler
void write_interrupt(){
    _dcb = get_dcb();
    // 1.	If the current status is not writing, ignore the interrupt and return. 
    if(_dcb->status != DCB_WRITING){return;}

    // 2.	Otherwise, if the count has not been exhausted, get the next character from the requestor's output buffer
    // and store it in the output register. Return without signaling completion. 
    if(_dcb->to_be_processed != 0){        
        if(*(_dcb->buffer + sizeof(char) * _dcb->chars_processed) != '\0'){
            outb(COM1, *(_dcb->buffer + sizeof(char) * _dcb->chars_processed));
            _dcb->to_be_processed--;
            _dcb->chars_processed++;
        }
        else{
            return;
        }        
    }

    // 3.	Otherwise, all characters have been transferred. Reset the status to idle. 
    //  Set the event flag and return the count value. Disable write interrupts by clearing bit 1 in the interrupt enable register. 
    if(_dcb->to_be_processed == 0){ 
        outb(COM1 + 1, inb(COM1 + 1) & ~0x02); 
        _dcb->status = DCB_IDLE;
        *_dcb->event_f = EVENT_SET;
    }

}
void line_status_interrupt(){
    inb(COM1 + 5);
    return;
}
void modem_interrupt(){
    inb(COM1 + 6);
    return;
}


