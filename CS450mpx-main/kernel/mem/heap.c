/*
  ----- heap.c -----

  Description..: Heap management procedures.
*/

#include <system.h>
#include <string.h>

#include <core/serial.h>
#include <mem/heap.h>
#include <mem/paging.h>

heap* kheap = 0; //kernel heap
heap* curr_heap = 0; //current heap

extern page_dir *kdir; //kernel page directory
extern void* end, _end, __end; //kernel end; defined in link.ld

//physical placement address
//current physical memory allocation address
uint32_t phys_alloc_addr = (uint32_t)&end;

uint32_t _kmalloc(uint32_t size, int page_align, uint32_t *phys_addr)
{
  uint32_t *addr;

  // Allocate on the kernel heap if one has been created
  if (kheap != 0){
    addr = (uint32_t*)alloc(size, kheap, page_align);
    if (phys_addr){
      page_entry *page = get_page((uint32_t)addr, kdir, 0);
      *phys_addr = (page->frameaddr*0x1000) + ((uint32_t)addr & 0xFFF);
    }
    return (uint32_t)addr;
  }
  // Else, allocate directly from physical memory
  else {
    if (page_align && (phys_alloc_addr & 0xFFFFF000)){
      phys_alloc_addr &= 0xFFFFF000;
      phys_alloc_addr += 0x1000;
    }
    addr = (uint32_t*)phys_alloc_addr;
    if (phys_addr){
      *phys_addr = phys_alloc_addr;
    }
    phys_alloc_addr += size;
    return (uint32_t)addr;
  }
}

uint32_t kmalloc(uint32_t size)
{
  return _kmalloc(size,0,0);
}

uint32_t alloc(uint32_t size, heap *h, int align)
{
  no_warn(size||align||h);
  static uint32_t heap_addr = KHEAP_BASE;

  uint32_t base = heap_addr;
  heap_addr += size;

  if (heap_addr > KHEAP_BASE + KHEAP_MIN)
    serial_println("Heap is full!");

  return base;
}

heap* make_heap(uint32_t base, uint32_t max, uint32_t min)
{  
  no_warn(base||max||min);
  return (heap*)kmalloc(sizeof(heap));
}
