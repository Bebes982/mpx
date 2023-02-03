#include <system.h>
#include "mcb.h"

MCB* HeadList;
MCB* TailList;

/*
  Procedure..: initializeHeap
  Description..: initializes the heap with the input being the size
  Params..: size_t size - unsigned integer of how big the heap should be
*/
void initializeHeap(size_t size);
