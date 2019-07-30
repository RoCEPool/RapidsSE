/*============================================================================*/
/* Constants.hpp -- add constants into main dictionary at startup             */
/*============================================================================*/

#pragma once

#include "Axioms.h"

//-----------------------------------------------------------------------------

namespace Axioms
{
  class Constants
  {
  static void vAddConstant(Stz* aName, i8 iVal);
  static void vAddConstant(Stz* aName, Stz* aValue);
  static void vAddConstants(void);
  };
}

using namespace Axioms;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
