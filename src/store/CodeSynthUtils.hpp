/*============================================================================*/
/* CodeSynthUtils.hpp -- Code synthesiser utilities                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class CodeSynthUtils {
  public:
    static bool bExtract(File *pFile, Stz *sLabel, Stz *sLabelNext, Stz *sLabelLoop,
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
