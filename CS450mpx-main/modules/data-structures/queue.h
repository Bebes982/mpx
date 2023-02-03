#ifndef MPX_QUEUE
#define MPX_QUEUE

typedef struct qn // queue node
{
    struct qn *next;
    struct qn *prev;
    void* pcb;
}qn;

qn* create_q_node(void* pcb);

typedef struct queue
{
    qn* front;
    qn* end;
} queue;

queue* create_queue();

void free_qn(qn* qn_);
void free_queue(queue* q);


void enqueue(queue* q, void* pcb);

// return PCB
void* dequeue(queue* q);

#endif