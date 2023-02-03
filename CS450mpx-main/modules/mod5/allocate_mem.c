#include <system.h>
#include "initializeHeap.h"
#include "mcb.h"
#include "../mpx_supt.h"

// remove
#include "showmem.h"

// allocates memory from the heap
// returns void * pointer to allocated mem, or null on failure
// first-fit method
uint32_t allocate_mem(uint32_t len){
    if(len == 0){
        char* message = "Cannot allocate memory of size 0.\n";
        int messageSize = (int)sizeof(message);
        sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
        return 0;
    }
    
    // iterate through mcbs
    MCB* curr;
    for(curr = HeadList; curr; curr = curr->next){
        if(curr->size < len || curr->allocated){
            continue;
        }
            
        curr->allocated = TRUE;

        //create mcb for new
        if(curr->size - len <= sizeof(MCB)){
            return (uint32_t) curr->start_address;
        }

        MCB* new = curr->start_address + len;
        
        new->size = curr->size - (len + sizeof(MCB));
        curr->size = len;

        new->next = curr->next;
        new->prev = curr;
        new->allocated = FALSE;

        curr->next = new;
        new->start_address = ((void*)new) + sizeof(MCB);

        // modify list
        if(new->next == NULL){
            TailList = new;
        }
        
        return (uint32_t) curr->start_address;
    }
    char* message = "No memory available.\n";
    int messageSize = (int)sizeof(message);
    sys_req(WRITE, DEFAULT_DEVICE, message, &messageSize);
    return 0; // no memory available
}