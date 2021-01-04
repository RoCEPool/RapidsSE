/*============================================================================*/
/* STORE.cpp -- STORE namespace functions                                     */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"

#include "AttachMap.hpp"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void STORE::vCheckAssumptions(void)
{
  ALWAYS(CacheLine::bMultipleCacheLines(sizeof(Operand)));
  ALWAYS(CacheLine::bMultipleCacheLines(sizeof(LinkedBuffer)));

  ALWAYS(sizeof(CodeCell)   == 8);
  
  ALWAYS(sizeof(Dic)     < C_SIZE_SMALL_OBJECT);
  
  ALWAYS(sizeof(Dtm)    == sizeof(SingleWord));

  ALWAYS(sizeof(u4) == sizeof(ColPix));
  
  
  
  ALWAYS(sizeof(Operand)      == sizeof(CacheLine));
  ALWAYS(sizeof(LinkedBuffer) == 2 * sizeof(CacheLine));
  
  ALWAYS(sizeof(StreamDef)    < C_SIZE_LARGE_OBJECT);
  
  ALWAYS(sizeof(Tx)           < C_SIZE_SMALL_OBJECT);
  ALWAYS(sizeof(TxSet)        < C_SIZE_SMALL_OBJECT);
  
  
  ALWAYS(sizeof(Rrf) == 8);

  
  ALWAYS(C_LOAD_READCHUNK < C_WSP_CSVBUFFER);
  
  ALWAYS(CacheLine::bMultipleCacheLines(sizeof(DataBuffer)));
  
  ALWAYS(sizeof(FreeItem) <= sizeof(Ent));
  ALWAYS(sizeof(NEO)      == C_NEO_SIZE);
  
  // dont change without correcting Ent
  ALWAYS(6 == C_MAX_LVL_LEVELS);

  ALWAYS(sizeof(HeadLine) == sizeof(SingleWord));
  ALWAYS(sizeof(TailLine) == sizeof(SpaceUnit));

#if defined(__ENT_HEADER)
  ALWAYS(40 == sizeof(Ent));
  ALWAYS(48 == sizeof(Ent_gen));
  ALWAYS(40 == sizeof(Ent_int));
#else
  ALWAYS(32 == sizeof(Ent));
  ALWAYS(40 == sizeof(Ent_gen));
  ALWAYS(32 == sizeof(Ent_int));
#endif

  
  ALWAYS(sizeof(Plan)      < C_SIZE_MEDIUM_OBJECT);


  ALWAYS(sizeof(AttachMap) < C_SIZE_POOL_BUFFER);
  
  
  ALWAYS(sizeof(Thread) < C_SIZE_SMALL_OBJECT);
  
  ALWAYS(sizeof(Queue)    < C_SIZE_SMALL_OBJECT);
  ALWAYS(sizeof(Task)     < C_SIZE_MEDIUM_OBJECT);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
