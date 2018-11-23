/*============================================================================*/
/* Axioms.h -- Axioms namespace                                               */
/*============================================================================*/

#pragma once

#include "Prompt.h"

//-----------------------------------------------------------------------------

namespace Axioms
{
  void vCheckAssumptions(void);
  
  void vAddConstant(Stz* aName, i8 iVal);
  void vAddConstant(Stz* aName, Stz* aValue);
  void vAddConstants(void);
}

using namespace Axioms;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
