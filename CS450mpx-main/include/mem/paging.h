#ifndef _PAGING_H
#define _PAGING_H

#include <system.h>

#define PAGE_SIZE 0x1000

/*
  Page entry structure
  Describes a single page in memory
*/
typedef struct {
  uint32_t present   : 1;
  uint32_t writeable : 1;
  uint32_t usermode  : 1;
  uint32_t accessed  : 1;
  uint32_t dirty     : 1;
  uint32_t reserved  : 7;
  uint32_t frameaddr : 20;
} page_entry;

/*
  Page table structure
  Contains 1024 pages/frames
*/
typedef struct {
  page_entry pages[1024];
} page_table;

/*
  Page directory structure
  Limited to 1024 tables for now
*/
typedef struct {
  page_table *tables[1024];
  uint32_t tables_phys[1024];
} page_dir;

/*
  Procedure..: set_bit
  Description..: Marks a page frame bit as in use (1).
*/
void set_bit(uint32_t addr);

/*
  Procedure..: clear_bit
  Description..: Marks a page frame bit as free (0).
*/
void clear_bit(uint32_t addr);

/*
  Procedure..: get_bit
  Description..: Checks if page frame is in use.
*/
uint32_t get_bit(uint32_t addr);

/*
  Procedure..: first_free
  Description..: Finds the first free page frame.
*/
uint32_t first_free();

/*
  Procedure..: init_paging
  Description..: Initializes the kernel page directory and 
    initial kernel heap area. Performs identity mapping of
    the kernel frames such that the virtual addresses are
    equivalent to the physical addresses.
*/
void init_paging();

/*
  Procedure..: load_page_dir
  Description..: Sets a page directory as the current
    directory and enables paging via the cr0 register.
    The cr3 register enables address translation from
    linear to physical addresses.
    http://en.wikipedia.org/wiki/Control_register#Control_registers_in_x86_series
*/
void load_page_dir(page_dir *new_page_dir);

/*
  Procedure..: get_page
  Description..: Finds and returns a page, allocating a new 
   page table if necessary.
*/
page_entry* get_page(uint32_t addr, page_dir *dir, int make_table);

/*
  Procedure..: new_frame
  Description..: Marks a frame as in use in the frame bitmap,
    sets up the page, and saves the frame index in the page.
*/
void new_frame(page_entry* page);

#endif
