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

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
