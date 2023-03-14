/*============================================================================*/
/* CodeSynthUtils.hpp -- Code synthesiser utilities                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CodeSynthUtils
  {
  public:
    static bool bExtract(File* pFile, Stz* szFunMkr, Stz* szStartMkr, Stz* szEndMkr, String* sCode);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
