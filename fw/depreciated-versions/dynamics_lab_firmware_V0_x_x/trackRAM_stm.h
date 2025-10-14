/* trackRAM_stm

https://github.com/stm32duino/STM32Examples/blob/main/examples/Benchmarking/MemoryAllocationStatistics/MemoryAllocationStatistics.ino

 This sketch employs mallinfo() to retrieve memory allocation
  statistics before and after allocating and freeing blocks of
  memory. The statistics are displayed on Serial.

  Creation 14 Feb 2018
  by Frederic Pillon

  This example code is in the public domain.

  Based on example from:
  http://man7.org/linux/man-pages/man3/mallinfo.3.html





*/


#include <malloc.h>

extern "C" char *sbrk(int i);
/* Use linker definition */
extern char _end;
extern char _sdata;
extern char _estack;
extern char _Min_Stack_Size;

static char *ramstart = &_sdata;
static char *ramend = &_estack;
static char *minSP = (char*)(ramend - &_Min_Stack_Size);

#define NUM_BLOCKS 100
#define BLOCK_SIZE 4

void display_mallinfo(void){
  char *heapend = (char*)sbrk(0);
  char * stack_ptr = (char*)__get_MSP();
  struct mallinfo mi = mallinfo();

  Serial.print(F("Total non-mmapped bytes (arena):       "));
  Serial.println(mi.arena);
  Serial.print(F("# of free chunks (ordblks):            "));
  Serial.println(mi.ordblks);
  Serial.print(F("# of free fastbin blocks (smblks):     "));
  Serial.println(mi.smblks);
  Serial.print(F("# of mapped regions (hblks):           "));
  Serial.println(mi.hblks);
  Serial.print(F("Bytes in mapped regions (hblkhd):      "));
  Serial.println(mi.hblkhd);
  Serial.print(F("Max. total allocated space (usmblks):  "));
  Serial.println(mi.usmblks);
  Serial.print(F("Free bytes held in fastbins (fsmblks): "));
  Serial.println(mi.fsmblks);
  Serial.print(F("Total allocated space (uordblks):      "));
  Serial.println(mi.uordblks);
  Serial.print(F("Total free space (fordblks):           "));
  Serial.println(mi.fordblks);
  Serial.print(F("Topmost releasable block (keepcost):   "));
  Serial.println(mi.keepcost);

  Serial.print(F("RAM Start at:       0x"));
  Serial.println((unsigned long)ramstart, HEX);
  Serial.print(F("Data/Bss end at:    0x"));
  Serial.println((unsigned long)&_end, HEX);
  Serial.print(F("Heap end at:        0x"));
  Serial.println((unsigned long)heapend, HEX);
  Serial.print(F("Stack Ptr end at:   0x"));
  Serial.println((unsigned long)stack_ptr, HEX);
  Serial.print(F("RAM End at:         0x"));
  Serial.println((unsigned long)ramend, HEX);

  Serial.print(F("Heap RAM Used:      "));
  Serial.println(mi.uordblks);
  Serial.print(F("Program RAM Used:   "));
  Serial.println(&_end - ramstart);
  Serial.print(F("Stack RAM Used:     "));
  Serial.println(ramend - stack_ptr);
  Serial.print(F("Estimated Free RAM: "));
  Serial.println(((stack_ptr < minSP) ? stack_ptr : minSP) - heapend + mi.fordblks);
}