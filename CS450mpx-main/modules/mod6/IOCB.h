#ifndef IOCB_H
#define IOCB_H

#include <system.h>
#include "../data-structures/PCB.h"
#include "../mpx_supt.h"

#define READ_OP 0
#define WRITE_OP 1

typedef struct IOCB{
   PCB* pcb;
   int code;
   int id;
   char* b_ptr;
   int* c_ptr;
} IOCB;

#endif