/*============================================================================*/
/* CodeSynthUtils.hpp -- Code synthesiser utilities                           */
/*============================================================================*/
#pragma once

#include "BASE.h"
#include "OpSketch.hpp"

namespace STORE {
  class CodeSynthUtils {
  public:
    static bool bExtract(File *pFile, OpSketch *pOpSketch,
                           Stz *szFunMkr,String *sCode, Column *pColumn, i8 iPayLoadIndex);

    static bool bExtractInflate(File *pFile, Column* pColumn, Stz *sReturnToLabel,
                                Stz *szFunMkr, String *sCode);

  private:
    static void vReplaceColumnNo(Stz *sLine, Column *pColumn);
    static void vReplaceColumnName(Stz *sLine, Column *pColumn);
    static bool bReplaceIndexInfo(Stz *sLine, Stz *sReplacedLine, OpSketch *pOpSketch);
    static void vReplaceIndexPosition(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceKeyKind(Stz *sLine, OpSketch *pOpSketch);
    static void vReplacePayloadIndex(Stz *sLine, i8 iPayloadIndex);
    static void vReplacePayloadBytes(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceStructTo(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceStructFrom(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceReturnToLabel(Stz *sLine, Stz* sReturnToLabel);
    static void vReplaceLabel(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceLabelIndex(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceLabelNext(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceLabelLoop(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceColumnsInflated(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceColumnsToInflate(Stz *sLine, OpSketch *pOpSketch);
    static bool bReplaceRowInfoFrom(Stz* sLine, Stz* sReplacedLine, OpSketch* pOpSketch);
    static bool bReplaceRowInfoTo(Stz* sLine, Stz* sReplacedLine, OpSketch* pOpSketch);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
