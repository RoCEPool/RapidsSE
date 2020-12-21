/*============================================================================*/
/* DataBuffers.cpp - DataBuffers namespace functions                          */
/*============================================================================*/

#include "DataBuffers.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void DataBuffers::vCheckAssumptions(void)
{
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

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
