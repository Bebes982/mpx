#ifndef MPX_PCB
#define MPX_PCB

#include "queue.h"
#include <system.h>

#define PCB_UNDEFINED 0

// Process Class
#define PCB_USER_PROCESS 1
#define PCB_SYSTEM_PROCESS 2

// Execution State
#define PCB_BLOCKED 3
#define PCB_READY 4
#define PCB_RUNNING 5

//Dispatching State
#define PCB_SUSPENDED 6
#define PCB_NOT_SUSPENDED 7

//Array sizes
#define PCB_max_name_len 16
#define PCB_stack_size 1024
#define PRIORITY_LEVELS 10

queue* ready_queue[PRIORITY_LEVELS];
queue* blocked_queue[PRIORITY_LEVELS];
queue* suspended_ready_queue[PRIORITY_LEVELS];
queue* suspended_blocked_queue[PRIORITY_LEVELS];
queue* other_queue[PRIORITY_LEVELS]; // a catch-all queue

typedef struct context{
    #define reg uint32_t
    reg gs, fs, es, ds;
    reg edi, esi, ebp, esp, ebx, edx, ecx, eax;
    reg eip, cs, eflags;
}context;

typedef struct PCB{
    char name[PCB_max_name_len+1];
    int priority;
    char process_class;
    char execution_state;
    char dispatching_state;
    unsigned char stack[PCB_stack_size];
    unsigned char mem2[128];
    unsigned char* stack_base;
    unsigned char* stack_top;
} PCB;


/*
    Uses sys_alloc_mem() to allocate memory for a new PCB, possibly
    including the stack, and perform reasonable initialization
    • Parameters:
        None
    • Returns:
        A pointer to a newly allocated PCB on success
        NULL on error during allocation or initialization
*/
PCB* allocate_pcb();

/*
    • Uses sys_free_mem() to free all memory associated with a PCB, including
    the stack
    • Parameters:
        A pointer to the PCB to free
    • Returns:
        A code indicating success or error
*/
char free_pcb(PCB* pcb);

/*
    • Allocates (via allocate pcb()) a new PCB, initializes it with data
    provided, and sets the state to Ready, Not-suspended
    • Parameters:
        The process name
        The process class (user application or system process)
        The process priority (0-9)
    • Returns:
        A pointer to the initialized PCB on success
        NULL on error allocating, initializing, or invalid parameter
*/
PCB* setup_pcb(char* name, char process_class, int priority);

/*
    • Searches all process queues for a process with the provided name
    • Parameters:
        The name of the process to find
    • Returns:
        A pointer to the found PCB on success
        NULL if the provided name was not found
*/
PCB* find_pcb(char* name);

/*
    • Inserts a PCB into the appropriate queue, based on state and (if Ready)
    priority
    • Parameters:
        A pointer to the PCB to enqueue
    • Returns:
        None
*/
void insert_pcb(PCB* pcb);

/*
    • Removes a PCB from its current queue
    • Parameters:
        A pointer to the PCB to unqueue
    • Returns:
        A success or error code
*/
char remove_pcb(PCB* pcb);

PCB* dequeue_next_pcb(queue** q);

void print_all();

void init_PCB_queue();



#endif