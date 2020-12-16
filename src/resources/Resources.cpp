/*============================================================================*/
/* Resources.cpp - Resources namespace functions                              */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Resources::vCheckAssumptions(void)
{
  ALWAYS(sizeof(struct sockaddr_in) <= (8 * 16));
  
  // pool
  
  //ASSERT(sizeof(Object) + sizeof(bn)*2 + sizeof(void*) == sizeof(PoolBuffer));
  //ASSERT(sizeof(PoolBuffer) + sizeof(void*)*2          == sizeof(HeapBuffer));
  
  ALWAYS(sizeof(HeapBuffer) == 1 * sizeof(CacheLine));
  
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_TINY_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_SMALL_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_MEDIUM_OBJECT));
  ALWAYS(CacheLine::bMultipleCacheLines(C_SIZE_LARGE_OBJECT));

  HeapMem::vValidateEnums();
  
  
  // shared memory
  
  
  // alignment
 
  ALWAYS(sizeof(SingleWord) == 8);
  ALWAYS(sizeof(SingleWord) == C_BYTES_PER_WORD);
  ALWAYS(sizeof(DoubleWord) == 2 * C_BYTES_PER_WORD);
  ALWAYS(sizeof(QuadWord)   == 4 * C_BYTES_PER_WORD);
  ALWAYS(sizeof(CacheLine)  == C_BYTES_PER_CACHE_LINE);
  
  // locking
  
  
  // host
  
  ALWAYS(Process::bTestPid(Process::iGetPid()));

  
  // library
  
  ALWAYS(8 == sizeof(Dtm));

  
  // focals
  
  
  // enums
  
  
  // esses
  
  
  // correctness
  
  ALWAYS(sizeof(u2)  == sizeof(eCN));
    
  ALWAYS(8 == sizeof(Header));
  ALWAYS(8 == sizeof(Object));
  ALWAYS(16 == sizeof(Footer));
  

  ALWAYS(8 == sizeof(Gid));
  
  ALWAYS(8 == sizeof(Bcd));
  ALWAYS(8 == sizeof(Bol));
  ALWAYS(8 == sizeof(Dte));
  ALWAYS(8 == sizeof(Edt));
  ALWAYS(8 == sizeof(Flt));
  ALWAYS(8 == sizeof(Int));
  ALWAYS(8 == sizeof(Ivl));
  ALWAYS(8 == sizeof(Tme));
  ALWAYS(8 == sizeof(Tsp));
  
  
  // class data
  
  
  // strings
  
  
  // assembler
  
  
  // base
  
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
