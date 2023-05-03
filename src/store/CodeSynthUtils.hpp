/*============================================================================*/
/* CodeSynthUtils.hpp -- Code synthesiser utilities                           */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "OpSketch.hpp"

namespace STORE {
  class CodeSynthUtils {
  public:
    static bool bExtractPL(File *pFile, OpSketch *pOpSketch,
                           Stz *szFunMkr,String *sCode, Column *pColumn, i8 iPayLoadIndex);

    static bool bExtractKey(File *pFile, OpSketch *pOpSketch,
                            Stz *szFunMkr, String *sCode, i8 iKeyType);

    static bool bExtractInflate(File *pFile, Stz *sColumn, Stz *sReturnToLabel,
                                Stz *szFunMkr, String *sCode);

  private:
    static void vReplaceColumnNo(Stz *sLine, Column *pColumn);

    static void vReplaceColumnName(Stz *sLine, Column *pColumn);

    static void vReplacePayloadIndex(Stz *sLine, i8 iPayloadIndex);

    static void vReplacePayloadBytes(Stz *sLine, OpSketch *pOpSketch);

    static void vReplaceStructTo(Stz *sLine, OpSketch *pOpSketch);

    static void vReplaceStructFrom(Stz *sLine, OpSketch *pOpSketch);

    static void vReplaceLabel(Stz *sLine, OpSketch *pOpSketch);

    static void vReplaceLabelNext(Stz *sLine, OpSketch *pOpSketch);

    static void vReplaceLabelLoop(Stz *sLine, OpSketch *pOpSketch);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
