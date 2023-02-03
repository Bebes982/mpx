#include "mpx_supt.h"
#include <string.h>
#include <core/io.h>
#include "data-structures/PCB.h"
#include <core/syscall.h>

PCB* cop;

char* pcb_names[] = {"undefined", "user process", "system process", "blocked", "ready", "running", "suspended", "not suspended"};

PCB* comhand_pcb = NULL;

void showPCB(PCB* pcb){

    if(!pcb){
        char* message = "PCB not found.\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return;
    }
    
    char* message = "name: \t\t   ";
    int messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    message = pcb->name;
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    message = "\n";
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);


    message = "class: \t\t   ";
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    message = pcb_names[(unsigned int)pcb->process_class];
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    message = "\n";
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);


    message = "execution state:   ";
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    message = pcb_names[(unsigned int)pcb->execution_state];
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    message = "\n";
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);


    message = "dispatching state: ";
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    message = pcb_names[(unsigned int)pcb->dispatching_state];
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    message = "\n";
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);


    message = "priority: \t   ";
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    int pri = pcb->priority;
    message = "";
    itoa(pri, message);
    messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);

    int nl = 2;
        sys_req(WRITE,DEFAULT_DEVICE, "\n",&nl);

}

void showReadyProcesses(){
    int i;
    for(i = 0; i < PRIORITY_LEVELS && ready_queue[i]; i++){
        queue* q = ready_queue[i];
        qn* qn_;
        for(qn_ = q->front; qn_; qn_ = qn_->next){
            showPCB((PCB*)qn_->pcb);
            int nl = 2;
            sys_req(WRITE,DEFAULT_DEVICE, "\n",&nl);

        }
        q = suspended_ready_queue[i];
        for(qn_ = q->front; qn_; qn_ = qn_->next){
            showPCB((PCB*)qn_->pcb);
            int nl = 2;
            sys_req(WRITE,DEFAULT_DEVICE, "\n",&nl);
        }
        
        
    }
}

void showBlockedProcesses(){
    queue* q = blocked_queue[0];
    qn* qn_;
    for(qn_ = q->front; qn_; qn_ = qn_->next){
        showPCB((PCB*)qn_->pcb);
    
        int nl = 2;
        sys_req(WRITE,DEFAULT_DEVICE, "\n",&nl);
    }

    q = suspended_blocked_queue[0];
    for(qn_ = q->front; qn_; qn_ = qn_->next){
        showPCB((PCB*)qn_->pcb);
        
        int nl = 2;
        sys_req(WRITE,DEFAULT_DEVICE, "\n",&nl);
    }

    int nl = 2;
    sys_req(WRITE,DEFAULT_DEVICE, "\n",&nl);
}

void showAllProcesses(){
    showPCB(cop);
    int nl = 2;
    sys_req(WRITE,DEFAULT_DEVICE, "\n",&nl);
    showReadyProcesses();
    showBlockedProcesses();
}

