#include "mcb.h"
#include "free_mem.h"
#include "initializeHeap.h"

// remove
#include "showmem.h"

int free_mem(void *block)
{

    MCB *temp = NULL;

    if (!block)
        return 1;

    // check to make sure there is a block with indicated address
    MCB *curr;
    for (curr = HeadList; curr; curr = curr->next)
    {
        if (curr->start_address == block)
        {
            if (curr->allocated == 0)
            {
                return 1; // cannot free already free mem
            }
            temp = curr;
            break;
        }
    }
    if (!temp)
    {
        return 1;
    }

    // frees mem, merges ajacent
    temp->allocated = 0;

    // check ajacent blocks
    // temp->prev consumes temp
    if (temp->prev != NULL && temp->prev->allocated == 0)
    {                                                                   // if adjacent, merge
        temp->prev->size = temp->prev->size + sizeof(MCB) + temp->size; // update size
        temp->prev->next = temp->next;                                  // remove
        temp->next->prev = temp->prev;

        if (!temp->next)
        {
            TailList = temp->prev;
        }
        temp = temp->prev;
    }
    // temp consumes temp->next
    if (temp->next != NULL && temp->next->allocated == 0)
    {                                                             // if ajacent, merge
        temp->size = temp->size + temp->next->size + sizeof(MCB); // update size
        temp->next = temp->next->next;
        if (!temp->next)
        {
            TailList = temp;
        }
        else
        {
            temp->next->next->prev = temp;
        }
    }

    return 0;
}
