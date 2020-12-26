/*============================================================================*/
/* Arguments.cpp - Arguments namespace                                        */
/*============================================================================*/

#include "Arguments.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void Arguments::vCheckAssumptions(void)
{
  ALWAYS(CacheLine::bMultipleCacheLines(sizeof(Operand)));
  ALWAYS(CacheLine::bMultipleCacheLines(sizeof(LinkedBuffer)));

  ALWAYS(sizeof(ForthCell)   == 8);
  
  ALWAYS(sizeof(Dic)     < C_SIZE_SMALL_OBJECT);
  
  ALWAYS(sizeof(Dtm)    == sizeof(SingleWord));

  ALWAYS(sizeof(u4) == sizeof(ColPix));
  
  
  
  ALWAYS(sizeof(Operand)      == sizeof(CacheLine));
  ALWAYS(sizeof(LinkedBuffer) == 2 * sizeof(CacheLine));
  
  ALWAYS(sizeof(StreamDef)    < C_SIZE_LARGE_OBJECT);
  
  ALWAYS(sizeof(Tx)           < C_SIZE_SMALL_OBJECT);
  ALWAYS(sizeof(TxSet)        < C_SIZE_SMALL_OBJECT);
  
  
  ALWAYS(sizeof(Rrf) == 8);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
