/*============================================================================*/
/* Constants.hpp -- add constants into main dictionary at startup             */
/*============================================================================*/
#pragma once

//-----------------------------------------------------------------------------

namespace STORE
{
  class Constants
  {
  public:
  static void vAddConstant(Stz* aName, i8 iVal);
  static void vAddConstant(Stz* aName, Stz* aValue);
  static void vAddConstantsArgs(void);
  static void vAddConstantsCpp(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
