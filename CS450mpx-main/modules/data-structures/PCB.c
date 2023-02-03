#include "PCB.h"
#include "../mpx_supt.h"
#include "string.h"
#include "queue.h"

#define PCB_N_QUEUES 5 // number of queues inside all_queues
queue** all_queues[] = {ready_queue, blocked_queue, suspended_ready_queue, suspended_blocked_queue, other_queue};
char* queue_names[] = {"ready", "blocked", "suspended_ready", "suspended_blocked", "other"};

char _PCB_queue_is_initialized = 0;


// initiliaze array of queues defined in PCB.h
void init_PCB_queue(){
    int i,k;
    for(i = 0; i<PRIORITY_LEVELS; i++){
        ready_queue[i] = create_queue();
        suspended_ready_queue[i] = create_queue();
    } 

    // this is done because these queues are FIFO and do not depend on priority level
    blocked_queue[0] = create_queue();
    suspended_blocked_queue[0] = create_queue();
    other_queue[0] = create_queue();
    for(k = 1; k<PRIORITY_LEVELS; k++){
        blocked_queue[k] = blocked_queue[0];
        suspended_blocked_queue[k] = suspended_blocked_queue[0];
        other_queue[k] = other_queue[0];
    }    
    _PCB_queue_is_initialized = 1;
}

//gets the queue that pcb should go into
queue* get_queue(PCB* pcb){
    queue **queue_type = NULL;

    if(pcb->execution_state == PCB_READY){
        if(pcb->dispatching_state == PCB_SUSPENDED){
            queue_type = suspended_ready_queue;
        }
        else if(pcb->dispatching_state == PCB_NOT_SUSPENDED){
            queue_type = ready_queue;
        } else{
            queue_type = other_queue;
        }
        
    }
    else if(pcb->execution_state == PCB_BLOCKED){
        if(pcb->dispatching_state == PCB_SUSPENDED){
            queue_type = suspended_blocked_queue;
        }
        else if(pcb->dispatching_state == PCB_NOT_SUSPENDED){
            queue_type = blocked_queue;
        } else{
            queue_type = other_queue;
        }
    }
    else{
        queue_type = other_queue;
    }

    if(queue_type)
        return queue_type[pcb->priority];
    return NULL;
}


/*
    Uses sys_alloc_mem() to allocate memory for a new PCB, possibly including the stack, and perform reasonable initialization
    • Parameters: None
    • Returns: A pointer to a newly allocated PCB on success, NULL on error during allocation or initialization
*/
PCB* allocate_pcb(){
    PCB* pcb = (PCB*)sys_alloc_mem(sizeof(PCB));
    
    if(pcb){
        memset(pcb->name, '\0', PCB_max_name_len*(sizeof(char)+1));
        pcb->priority = -1;
        pcb->process_class = PCB_UNDEFINED;
        pcb->execution_state = PCB_UNDEFINED;
        pcb->dispatching_state = PCB_UNDEFINED;
        memset(pcb->stack, 0, PCB_stack_size*sizeof(unsigned char));
        pcb->stack_base = 0;
        pcb->stack_top = 0;
    }

    return pcb;
}

/*
    • Uses sys_free_mem() to free all memory associated with a PCB, including the stack
    • Parameters: A pointer to the PCB to free
    • Returns:A code indicating success or error
*/
char free_pcb(PCB* pcb){
    return sys_free_mem(pcb);
}

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
PCB* setup_pcb(char* name, char process_class, int priority){
    if(strlen(name) > PCB_max_name_len || !name || !name[0]) // process name too long or null
        return NULL;
    else if(process_class != PCB_USER_PROCESS && process_class != PCB_SYSTEM_PROCESS) //invalid process class
        return NULL;
    else if(priority < 0 || priority >= PRIORITY_LEVELS) // invalid priority
        return NULL;

    PCB* pcb = allocate_pcb();
    if(pcb){
        strcpy(pcb->name, name);
        pcb->process_class = process_class;
        pcb->priority = priority;
        pcb->execution_state = PCB_READY;
        pcb->dispatching_state = PCB_NOT_SUSPENDED;
        pcb->stack_base = pcb->stack;
        pcb->stack_top = pcb->stack_base + PCB_stack_size - sizeof(context);
    }
    return pcb;
}

/*
    • Searches all process queues for a process with the provided name
    • Parameters: The name of the process to find
    • Returns: A pointer to the found PCB on success, NULL if the provided name was not found
*/
PCB* find_pcb(char* name){
    if(!_PCB_queue_is_initialized || !name || !name[0]) //invalid search
        return NULL;
    
    
    int q_i;
    for(q_i = 0; q_i < PCB_N_QUEUES; q_i++){
        queue** q_type = all_queues[q_i];
        int i;
        for(i =0; i<PRIORITY_LEVELS; i++){
            queue* q = q_type[i];
            qn* qn_; 
            for(qn_ = q->front; qn_; qn_ = qn_->next){
                if(0 == strcmp(((PCB*)qn_->pcb)->name, name)){
                    return (PCB*) qn_->pcb;
                }
            }
        }
    }
    return NULL;
}

/*
    • Inserts a PCB into the appropriate queue, based on state and (if Ready) priority
    • Parameters: A pointer to the PCB to enqueue
    • Returns:None
*/
void insert_pcb(PCB* pcb){
    if(!_PCB_queue_is_initialized){
        init_PCB_queue();
    }
    queue* q = get_queue(pcb);
    if(q)
        enqueue(q, pcb);
}

/*
    • Removes a PCB from its current queue
    • Parameters: A pointer to the PCB to unqueue
    • Returns: A success or error code
*/
char remove_pcb(PCB* pcb){
    if(!_PCB_queue_is_initialized || !pcb)
        return 0;
    queue* q = get_queue(pcb);
    qn* _qn;
    for(_qn = q->front; _qn; _qn = _qn->next){
        if(pcb == _qn->pcb){
            
            if(_qn == q->front){
                q->front = _qn->next;
            }
            if(_qn == q->end){
                q->end = _qn->prev;
            }

            if(_qn->next){
                _qn->next->prev = _qn->prev;
            }
            if(_qn->prev){
                _qn->prev->next = _qn->next;
            }
            free_qn(_qn);
            return 1;
        }
    }
    return 0;
}


// pass in ready_queue, blocked_queue, suspended_queue
PCB* dequeue_next_pcb(queue** q){
    if(!_PCB_queue_is_initialized)
        return NULL;
    int i;
    for(i = 0; i<PRIORITY_LEVELS; i++){
        PCB* ret = dequeue(q[i]);
        if(ret)
            return ret;
    }

    return NULL;
}

