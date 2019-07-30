/*============================================================================*/
/* Constants.hpp -- add constants into main dictionary at startup             */
/*============================================================================*/

#pragma once

#include "StorageEngine.h"

//-----------------------------------------------------------------------------

namespace StorageEngine
{
  class Constants
  {
  public:
  static void vAddConstant(Stz* aName, i8 iVal);
  static void vAddConstant(Stz* aName, Stz* aValue);
  static void vAddConstants(void);
  };
}

using namespace Axioms;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
