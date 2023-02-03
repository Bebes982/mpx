#ifndef DCB_H
#define DCB_H
#include <system.h>
#include "../data-structures/PCB.h"

// HARDWARE IO ADDRESSES
#ifndef COM1
    #define COM1 0x3f8
    #define COM2 0x2f8
    #define COM3 0x3e8
    #define COM4 0x2e8
#endif
#define PIC_MASK 0x21

// DCB ENUMS
#define DCB_UNDEFINED 0
#define DCB_IDLE 1
#define DCB_READING 2
#define DCB_WRITING 3
#define DCB_OPEN 4
#define DCB_CLOSE 5
#define EVENT_CLEAR 0
#define EVENT_SET 1
#define RING_BUF_FULL 8
#define RING_BUF_NOT_FULL 9
#define RING_BUF_EMPTY 10

// BUF LEN
#define RING_BUFFER_LEN 32

typedef struct DCB{
    PCB* active_process;

    int status; // IDLE, READING, WRITING
    int open; // OPEN, CLOSE

    int* event_f;
    int chars_processed;
    int to_be_processed;
    //int current_bytes_processed; // current number of byres 
    //int& bytes_processed; //the pointer passed to sysreq that is to be set at the end

    char* buffer; // buffers in the IOCB
    
    char ring_buffer[RING_BUFFER_LEN];
    int ring_buf_full; // flag whether ring buffer is full or not
    int ring_buf_read_index; // index to read in ring buf
    int ring_buf_write_index; // index to insert in ring buf
} DCB;


DCB* create_DCB();
void free_DCB(DCB* d);

DCB* get_dcb();

int com_open(int *eflag_p, int baud_rate);
int com_close(void);
int com_read(char *buf_p, int *count_p);
int com_write(char* buf_p, int *count_p);


#endif