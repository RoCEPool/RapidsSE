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

    static void vReplaceColumnNo(String* sLine, Column *pColumn);
    static void vReplaceColumnName(String* sLine, Column *pColumn);
    static bool bReplaceIndexInfo(String* sLine, String* sReplacedLine, OpSketch *pOpSketch);
    static void vReplaceIndexPosition(String* sLine, OpSketch *pOpSketch);
    static void vReplaceInsertEnt(String* sLine, OpSketch* pOpSketch);
    static void vReplaceKeyKind(String* sLine, OpSketch *pOpSketch);
    static void vReplaceNumColumns(String* sLine, OpSketch *pOpSketch);
    static void vReplaceOperatorName(String* sLine, OpSketch *pOpSketch);
    static void vReplaceIndexColumn(String* sLine, i8 iIndexColumn);
    static void vReplacePayloadBytes(String* sLine, OpSketch *pOpSketch);
    static void vReplaceStructTo(String* sLine, OpSketch *pOpSketch);
    static void vReplaceStructFrom(String* sLine, OpSketch *pOpSketch);
    static void vReplaceReturnToLabel(String* sLine, Stz* sReturnToLabel);
    static void vReplaceLabel(String* sLine, OpSketch *pOpSketch);
    static void vReplaceLabelIndex(String* sLine, OpSketch *pOpSketch);
		static void vReplaceLabelJoinNulls(String* sLine, OpSketch *pOpSketch);
    static void vReplaceLabelNext(String* sLine, OpSketch *pOpSketch);
    static void vReplaceLabelLoop(String* sLine, OpSketch *pOpSketch);
    static void vReplaceColumnsInflated(String* sLine, OpSketch *pOpSketch);
    static void vReplaceColumnsToInflate(String* sLine, OpSketch *pOpSketch);
    static bool bReplaceRowInfoFrom(String* sLine, String* sReplacedLine, OpSketch* pOpSketch);
    static bool bReplaceRowInfoTo(String* sLine, String* sReplacedLine, OpSketch* pOpSketch);
    static void vReplaceTubPot(String* sLine, OpSketch* pOpSketch);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
