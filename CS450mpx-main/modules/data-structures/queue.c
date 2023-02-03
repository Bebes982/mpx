#include "queue.h"
#include "../mpx_supt.h"


qn* create_q_node(void* pcb){
    qn* qn_;
    qn_ = sys_alloc_mem(sizeof(qn));
    qn_->next = NULL;
    qn_->prev = NULL;
    qn_->pcb = pcb;
    return qn_;
}


queue* create_queue(){
    queue* q; 
    q= (queue*)sys_alloc_mem(sizeof(queue));
    q->front = NULL;
    q->end = NULL;
    return q;
}

//does not free the items held in the queue. (PCBs)
void free_qn(qn* qn_){
    sys_free_mem(qn_);
}

//does not free the items in the queue nodes (PCBs)
void free_queue(queue* q){
    sys_free_mem(q);
}


void enqueue(queue* q, void* pcb){
    qn* new_qn = create_q_node(pcb);

    if(!q->front){
        q->end = new_qn;
        q->front = new_qn;
    }
    else{
        q->end->next = new_qn;
        new_qn->prev = q->end;
        q->end = new_qn;
    }
}

void* dequeue(queue* q){
    if(!q || !q->front)
        return NULL;
    qn* qn_ = q->front;
    q->front = q->front->next;
    if(q->front == NULL)
        q->end = NULL;
    void* ret = qn_->pcb;
    free_qn(qn_);
    return ret;
}
