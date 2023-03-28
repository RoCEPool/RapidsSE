/*============================================================================*/
/* CodeSynthUtils.hpp -- Code synthesiser utilities                           */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "OpSketch.hpp"

namespace STORE
{
  class CodeSynthUtils {
  public:
    static bool bExtract(File *pFile, OpSketch* pOpSketch,
                         Stz *szFunMkr, Stz *szStartMkr,
                         Stz *szEndMkr, String *sCode);

    static bool bExtractInflate(File *pFile, Stz *sColumn, Stz *sReturnToLabel,
                                Stz *szFunMkr, Stz *szStartMkr,
                                Stz *szEndMkr, String *sCode);
  };

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
