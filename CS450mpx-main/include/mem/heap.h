#ifndef _HEAP_H
#define _HEAP_H
#include <system.h>

/* Kernel heap */
#define TABLE_SIZE 0x1000
#define KHEAP_BASE 0xD000000
#define KHEAP_MIN  0x10000
#define KHEAP_SIZE 0x1000000

/* Heap allocation header */
typedef struct {
  int size;
  int index_id;
} header;

typedef struct {
  header head;
} footer;

typedef struct {
  int size;
  int empty;
  uint32_t block;
} index_entry;

/* Kernel heap index table */
typedef struct {
  index_entry table[TABLE_SIZE];
  int id;
} index_table;

/* Heap structure */
typedef struct {
  index_table index;
  uint32_t base;
  uint32_t max_size;
  uint32_t min_size;
} heap;

/*
  Procedure..: _kmalloc
  Description..: Base-level kernel memory allocation routine. Used to
      provide page alignment and access physical addresses of allocations.
      Called by kmalloc with align=0, physical_address=0.
*/
uint32_t _kmalloc(uint32_t size, int align, uint32_t *phys_addr);

/*
  Procedure..: kmalloc
  Description..: Standard kernel memory allocation rountine. Use this unless you
      need to specify alignment or obtain a physical address. Calls _kmalloc.
*/
uint32_t kmalloc(uint32_t size);

/*
  Procedure..: kfree
  Description..: Free kernel memory.
*/
uint32_t kfree();

/*
  Procedure..: init_kheap
  Description..: Initialize the kernel heap, and set it as the current heap.
*/
void init_kheap();

/*
  Procedure..: alloc
  Description..: Allocate some memory using the given heap. Can specify page-alignment.
*/
uint32_t alloc(uint32_t size, heap *hp, int align);

/*
  Procedure..: make_heap
  Description..: Create a new heap.
  Parameters..: base - physical start address of the heap
                max  - maximum size the heap may grow to
		min  - minimum/initial size
*/
heap* make_heap(uint32_t base, uint32_t max, uint32_t min);

#endif
