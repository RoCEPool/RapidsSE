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

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
