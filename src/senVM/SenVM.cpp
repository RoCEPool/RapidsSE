/*============================================================================*/
/* SenVM.cpp - SenVM namespace functions                                      */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void SenVM::vCheckAssumptions(void)
{
  ALWAYS(sizeof(AnyVal) == 16);
  ALWAYS(sizeof(AnyVal) == sizeof(IntVal));
  ALWAYS(sizeof(AnyVal) == sizeof(FipVal));

  ALWAYS(sizeof(ForthCell)   == 8);
  
  ALWAYS(sizeof(Dic)     < C_SIZE_SMALL_OBJECT);
  
  ALWAYS(sizeof(Dtm)    == sizeof(SingleWord));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
