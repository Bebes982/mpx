#include "data-structures/PCB.h"
#include "mpx_supt.h"
#include <string.h>
#include <core/syscall.h>

void suspendPCB(char* name){
    PCB* pcb = find_pcb(name);
    if(pcb != NULL){
        if(pcb->process_class == PCB_SYSTEM_PROCESS){
            char message[] = {"You may not suspend a system process.\n"};
            int messageSize = (int)sizeof(message);
            sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
            return;
        }
       if(pcb->dispatching_state != PCB_SUSPENDED){
            remove_pcb(pcb);
            pcb->dispatching_state = PCB_SUSPENDED;
            insert_pcb(pcb);
        }
    }
    else{
        if(0 == strcmp(name, cop->name)){
            char message[] = {"You cannot do this to the currently operating process.\n"};
            int messageSize = (int)sizeof(message);
            sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
            return;
        }
        char* message = "Could not find process with that name\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
    }
}

void resumePCB(char* name){
    PCB* pcb = find_pcb(name);
    if(pcb != NULL){
        if(pcb->dispatching_state != PCB_NOT_SUSPENDED){
            remove_pcb(pcb);
            pcb->dispatching_state = PCB_NOT_SUSPENDED;
            insert_pcb(pcb);
        }
    }
    else{
        if(0 == strcmp(name, cop->name)){
            char message[] = {"You cannot do this to the currently operating process.\n"};
            int messageSize = (int)sizeof(message);
            sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
            return;
        }
        char* message = "Could not find process with that name\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
    }
}

void setPriority(char* name, int priority){
    if(priority < 0 || priority>= PRIORITY_LEVELS){
        char* message = "Invalid priority level.\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
    }

    PCB* pcb = find_pcb(name);

    if(0 == strcmp(name, cop->name)){
            pcb = cop;
            cop->priority = priority;
            return;
    }

    if(pcb != NULL){
        remove_pcb(pcb);
        pcb->priority = priority;
        insert_pcb(pcb);
    }
    else{
        
        char* message = "Could not find process with that name.\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
    }
}
void deletePCB(char* name){
    PCB* pcbPtr = find_pcb(name);
    if (pcbPtr == NULL){
        if(0 == strcmp(name, cop->name)){
            char message[] = {"You cannot do this to the currently operating process.\n"};
            int messageSize = (int)sizeof(message);
            sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
            return;
        }
        char message[] = {"Invalid Process Name"};
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }
    if(pcbPtr->process_class == PCB_SYSTEM_PROCESS){
        char message[] = {"You may not delete a system process.\n"};
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }
    if(pcbPtr->dispatching_state == PCB_NOT_SUSPENDED){
        char message[] = {"The process is not suspended. Try suspending it first.\n"};
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }

    remove_pcb(pcbPtr);
    free_pcb(pcbPtr);
}