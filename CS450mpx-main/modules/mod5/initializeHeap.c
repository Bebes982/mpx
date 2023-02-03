#include <mem/heap.h>
#include "../mpx_supt.h"
#include "initializeHeap.h"

#include "showmem.h"

void initializeHeap(size_t size){
    HeadList = NULL;
    TailList = NULL;
    
    uint32_t startAddr =  kmalloc(size + sizeof(MCB));
    HeadList = (void*)startAddr;
    HeadList->size = size;
    HeadList->start_address = (void*)startAddr + sizeof(MCB);
    HeadList->prev = NULL;
    HeadList->next = NULL;   
    HeadList->allocated = FALSE;

    TailList = HeadList;
}
