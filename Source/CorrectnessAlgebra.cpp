/*============================================================================*/
/* CorrectnessAlgebra.cpp -- Correctness Algebra namespace functions          */
/*============================================================================*/

#include "CorrectnessAlgebra.h"

/*----------------------------------------------------------------------------*/
/* things to check at boot time                                               */
/*----------------------------------------------------------------------------*/
void CorrectnessAlgebra::vCheckAssumptions(void)
{
  ASSERT(sizeof(u2)  == sizeof(eCN));
  
  //ASSERT(4  == sizeof(Class));
  //ASSERT(8  == sizeof(Magic));
  
  ASSERT(8 == sizeof(Object));
  ASSERT(8  == sizeof(Footer));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
