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
                           Stz *szFunMkr,String *sCode, Column *pColumn, i8 iIndexColumn);

    static bool bExtractInflate(File *pFile, Column* pColumn, Stz *sReturnToLabel,
                                Stz *szFunMkr, String *sCode);

  private:
    static bool bGenerateExprStack(Stz* sLine, String* sCode, OpSketch* pOpSketch);

    static void vReplaceColumnNo(Stz *sLine, Column *pColumn);
    static void vReplaceColumnName(Stz *sLine, Column *pColumn);
    static bool bReplaceIndexInfo(Stz *sLine, Stz *sReplacedLine, OpSketch *pOpSketch);
    static void vReplaceIndexPosition(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceInsertEnt(Stz* sLine, OpSketch* pOpSketch);
    static void vReplaceKeyKind(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceNumColumns(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceOperatorName(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceIndexColumn(Stz *sLine, i8 iIndexColumn);
    static void vReplacePayloadBytes(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceStructTo(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceStructFrom(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceReturnToLabel(Stz *sLine, Stz* sReturnToLabel);
    static void vReplaceLabel(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceLabelIndex(Stz *sLine, OpSketch *pOpSketch);
		static void vReplaceLabelJoinNulls(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceLabelNext(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceLabelLoop(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceColumnsInflated(Stz *sLine, OpSketch *pOpSketch);
    static void vReplaceColumnsToInflate(Stz *sLine, OpSketch *pOpSketch);
    static bool bReplaceRowInfoFrom(Stz* sLine, Stz* sReplacedLine, OpSketch* pOpSketch);
    static bool bReplaceRowInfoTo(Stz* sLine, Stz* sReplacedLine, OpSketch* pOpSketch);
    static void vReplaceTubPot(Stz* sLine, OpSketch* pOpSketch);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
