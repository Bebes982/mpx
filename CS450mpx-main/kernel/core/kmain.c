/*
  ----- kmain.c -----

  Description..: Kernel main. The first function called after
      the bootloader. Initialization of hardware, system
      structures, devices, and initial processes happens here.

      Initial Kernel -- by Forrest Desjardin, 2013,
      Modifications by:    Andrew Duncan 2014,  John Jacko 2017
              Ben Smith 2018, and Alex Wilson 2019
*/

#include <stdint.h>
#include <string.h>
#include <system.h>

#include <core/io.h>
#include <core/serial.h>
#include <core/tables.h>
#include <core/interrupts.h>
#include <mem/heap.h>
#include <mem/paging.h>

#include "modules/mpx_supt.h"
#include "modules/commandHandler.h"
#include "modules/loadr3.h"
#include "modules/data-structures/PCB.h"
#include "modules/mod5/initializeHeap.h"
#include "modules/mod5/allocate_mem.h"

#include "modules/mod6/io_scheduler.h"
#include "modules/versionAndShutdown.h"

// TODO: remove
#include "modules/data-structures/queue.h"
#include "modules/PCBCommands.h"
#include "modules/alarm.h"
#include "modules/show.h"
#include "modules/mod5/showmem.h"
#include "modules/mod5/free_mem.h"

void kmain(void)
{
  extern uint32_t magic;
  // Uncomment if you want to access the multiboot header
  // extern void *mbd;
  // char *boot_loader_name = (char*)((long*)mbd)[16];

  mpx_init(IO_MODULE);
  initializeHeap(50000);
  sys_set_malloc(allocate_mem);
  sys_set_free(free_mem);

  // 0) Initialize Serial I/O
  // functions to initialize serial I/O can be found in serial.c
  // there are 3 functions to call

  init_serial(COM1);
  set_serial_out(COM1);
  set_serial_in(COM1);

  klogv("Starting MPX boot sequence...");
  klogv("Initialized serial I/O on COM1 device...");

  // 1) Initialize the support software by identifying the current
  //     MPX Module.  This will change with each module.
  // you will need to call mpx_init from the mpx_supt.c



  // 2) Check that the boot was successful and correct when using grub
  // Comment this when booting the kernel directly using QEMU, etc.
  if (magic != 0x2BADB002)
  {
    // kpanic("Boot was not error free. Halting.");
  }

  // 3) Descriptor Tables -- tables.c
  //  you will need to initialize the global
  // this keeps track of allocated segments and pages
  klogv("Initializing descriptor tables...");
  init_gdt();
  init_idt();

  // 4)  Interrupt vector table --  tables.c
  // this creates and initializes a default interrupt vector table
  // this function is in tables.c
  
  cli();
  init_irq();
  init_pic();
  sti();
  klogv("Interrupt vector table initialized!");

  // 5) Virtual Memory -- paging.c  -- init_paging
  //  this function creates the kernel's heap
  //  from which memory will be allocated when the program calls
  // sys_alloc_mem UNTIL the memory management module  is completed
  // this allocates memory using discrete "pages" of physical memory
  // NOTE:  You will only have about 70000 bytes of dynamic memory
  //
  klogv("Initializing virtual memory...");
  init_paging();

  // 6) Call YOUR command handler -  interface method
  klogv("Transferring control to commhand...");
  
  init_PCB_queue(); 

    // int bla = 0;
    //  while(!bla){
    //  bla = bla;
    //  } 

  shutting_down = 0;
  initialize_io_scheduler();
  
  klogv("Initialized io scheduler");


  PCB *ComHandProc = setup_pcb("ComHandProc", PCB_SYSTEM_PROCESS, 1);
  initializeRegs2(ComHandProc, comHand);
  insert_pcb(ComHandProc);

  PCB *IdleProc = setup_pcb("idle", PCB_SYSTEM_PROCESS, 9);
  initializeRegs(IdleProc, idle);
  insert_pcb(IdleProc);

  // start executing processes
  while(!shutting_down){
    sti();
    sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }



  // 7) System Shutdown on return from your command handler
  klogv("Starting system shutdown procedure...");

  /* Shutdown Procedure */
  klogv("Shutdown complete. You may now turn off the machine. (QEMU: C-a x)");
  hlt();
}
