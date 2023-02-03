#include "DCB.h"
#include "../mpx_supt.h"
#include <core/interrupts.h>
#include <core/tables.h>
#include <string.h>
#include <core/io.h>

DCB* dcb = NULL;

extern void reserved();
extern uint32_t serial_io_isr(void);

void (*oldfunc)(void) = reserved;


DCB* get_dcb(){
    return dcb;
}


/*
    typedef struct DCB{
        PCB* active_process;
        int status; // IDLE, READING, WRITING
        int open; // OPEN, CLOSE
        int* event_f;
        int chars_processed;
        int to_be_processed;
        char* buffer; // buffers in the IOCB
        char ring_buffer[RING_BUFFER_LEN];
        int ring_buf_full; // flag whether ring buffer is full or not
        int ring_buf_read_index; // index to read in ring buf
        int ring_buf_write_index; // index to insert in ring buf
    } DCB;
*/
DCB *create_DCB()
{
    DCB *d = (DCB*)sys_alloc_mem(sizeof(DCB));
    d->active_process = NULL;

    d->status = DCB_UNDEFINED;
    d->open = DCB_OPEN;

    d->event_f = NULL;
    d->chars_processed = 0;
    d->to_be_processed = 0;

    d->buffer = NULL;
    return d;
}

void free_DCB(DCB *d)
{
    sys_free_mem(d);
}

int com_open(int *eflag_p, int baud_rate)
{   
    //1.	Ensure that the parameters are valid, and that the device is not currently open. 
    if (dcb){return 103;} // Doesn't need opened
    if (!(baud_rate == 110 || baud_rate == 150 || baud_rate == 300 || baud_rate == 600 || baud_rate == 1200 || baud_rate == 2400 || baud_rate == 4800 || baud_rate == 9600 || baud_rate == 19200))
        return 102; // bad baud
    if (eflag_p == NULL){return 101;} // bad event flag
    
    //  2.	Initialize the DCB. In particular, this should include indicating that the device is open,
    //    saving a copy of the event flag pointer, and setting the initial device status to idle. In addition, the ring buffer parameters must be initialized.
    dcb = create_DCB();
    dcb->open = DCB_OPEN;
    dcb->event_f = eflag_p;
    dcb->status = DCB_IDLE;
    memset(dcb->ring_buffer, '\0', RING_BUFFER_LEN * sizeof(char));
    dcb->ring_buf_read_index = 0;
    dcb->ring_buf_write_index = 0;
    dcb->ring_buf_full = RING_BUF_EMPTY;

    // 3.	Save the address of the current interrupt handler, and install the new handler in the interrupt vector.
    //TODO, save old one.
    oldfunc = reserved;
    //idt_set_gate(0x24, (uint32_t)serial_io_isr, 0x08, 0x8e);

    // 4.	Compute the required baud rate divisor. 
    int baud_divisor = 115200 / baud_rate;

    // 5.	Store the value 0x80 in the Line Control Register.
    //  This allows the first two port addresses to access the Baud Rate Divisor register. 
    outb(COM1 + 3, 0x80);

    // 6.	Store the high order and low order bytes of the baud rate divisor into the MSB and LSB registers, respectively. 
    // TODO
    outb(COM1 + 0, (char)(baud_divisor&0xFF));
    outb(COM1 + 1, (char)((baud_divisor&0xFF00)>>8));

    // 7.	Store the value 0x03 in the Line Control Register. This sets the line characteristics to 8 data bits, 1 stop bit, and no parity. 
    //  It also restores normal functioning of the first two ports. 
    outb(COM1 + 3, 0x03);


    // 8.	Enable the appropriate level in the PIC mask register. 
    int mask;
    cli(); //disable interrupts
    mask = inb(PIC_MASK);
    mask = mask & ~0x10; // enable PIC priority 3 (Com1)
  //  mask = mask & ~(0x01 << 1); // enable PIC priority 3 (Com1) // TODO REMOVE THIS ONE?
    outb(PIC_MASK, mask);
    sti(); //enable interrupts

    outb(COM1 +2, 0xC7); //  enables fifo

    // 9.	Enable overall serial port interrupts by storing the value 0x08 in the Modem Control register. 
    outb(COM1 + 4, 0x08);

    // 10.	Enable input ready interrupts only by storing the value 0x01 in the Interrupt Enable register. 
    outb(COM1 + 1, 0x01);

    //TODO below need triggered for each block?
    dcb->chars_processed= 0;

    return 0;
}

int com_close(void)
{
    // 1.	Ensure that the port is currently open.
    if (dcb->open == DCB_CLOSE){ return 201; }

    //2.	Clear the open indicator in the DCB. 
    dcb->open = DCB_CLOSE;

    //3.	Disable the appropriate level in the PIC mask register. 
    int mask;
    cli(); //disable interrupts
    mask = inb(PIC_MASK);
    mask = mask | (0x10);
    outb(PIC_MASK, mask);
    sti(); //enable interrupts

    // 4.	Disable all interrupts in the ACC by loading zero values to the Modem Status register and the Interrupt Enable register. 
    outb(COM1 + 4, 0x00);
    outb(COM1 + 1, 0x00);

    // 5.	Restore the original saved interrupt vector. 
        // TODO. get actual original
    idt_set_gate(0x30, (uint32_t)oldfunc, 0x08, 0x8e);

    // TODO what value?
    return 0;
}


int com_read(char *buf_p, int *count_p)
{
    // 1. Validate the supplied parameters. 
    if (!buf_p) { return 302;}
    if (!count_p || *count_p <= 0){return 303;}

    //2. Ensure that the port is open, and the status is idle. 
    if (dcb->open != DCB_OPEN){return 301;}
    if (dcb->status != DCB_IDLE){ return 304; }

 
    // 3. Initialize the input buffer variables (not the ring buffer!) 
    // and set the status to reading. 
    dcb->buffer = buf_p;
    dcb->status = DCB_READING; 

    dcb->to_be_processed = *count_p;

    // 4. Clear the caller's event flag. 
    *dcb->event_f = EVENT_CLEAR;

    // 5. copy chars from ring buff to requestor's buff
    // until ring buff empty, requested count reached, cr enter code found
    // remove from ring buffer - interrupts should be disabled during copying
    cli(); //disable interrupts
    while( dcb->ring_buf_full != RING_BUF_EMPTY 
            && dcb->to_be_processed > 0
            && dcb->ring_buffer[dcb->ring_buf_read_index] != '\r'){
        // add to buf_p
        *(buf_p + dcb->ring_buf_read_index * sizeof(char)) = dcb->ring_buffer[dcb->ring_buf_read_index];
        dcb->chars_processed++;
        dcb->to_be_processed--;
        // increment read index, update full flag if necessary
        dcb->ring_buf_read_index = (dcb->ring_buf_read_index + 1) % RING_BUFFER_LEN;
        if(dcb->ring_buf_read_index == dcb->ring_buf_write_index ){
            dcb->ring_buf_full = RING_BUF_EMPTY;
        }
    }
    sti(); //enable interrupts


    // 6.	If more characters are needed, return. If the block is complete, continue with step 7. 

    if(dcb->to_be_processed > 0){
        // need more chars
        return 0;
    }

    // 7.	Reset the DCB status to idle, set the event flag, and return the actual count to the requestor's variable. 
    dcb->status = DCB_IDLE;
    *dcb->event_f = EVENT_SET;
    *count_p = dcb->chars_processed;
    return 0;
}
int com_write(char *buf_p, int *count_p){

    // 1.	Ensure that the input parameters are valid. 
    if (!buf_p){return 302;}
    if (!count_p || *count_p <= 0){return 303;}

    // 2.	Ensure that the port is currently open and idle. 
    if (dcb->open != DCB_OPEN){return 301;}
    if (dcb->status != DCB_IDLE){return 304;}

    // 3.	Install the buffer pointer and counters in the DCB, and set the current status to writing. 
    dcb->buffer = buf_p;
    dcb->status = DCB_WRITING;

    // 4.	Clear the caller's event flag. 
    *dcb->event_f = EVENT_CLEAR;

    // 5.	Get the first character from the requestor's buffer and store it in the output register
    outb(COM1, *(buf_p));
    dcb->to_be_processed = *count_p - 1;
    dcb->chars_processed++;
    outb(COM1 + 1, inb(COM1 + 1) | 0x02); // enables PIC interrupt register for COM1
    *count_p = dcb->chars_processed;

    //TODO: what value goes here?
    return 0;
}