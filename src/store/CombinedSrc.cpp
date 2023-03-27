
/*============================================================================*/
/* CombinedSrc.cpp -- Code Synthesizer                                          */
/*============================================================================*/

#include "BASE.h"

#include "Stream.hpp"
#include "StreamDef.hpp"
#include "TokenVec.hpp"
#include "Token.hpp"
#include "LogicUnit.hpp"
#include "Index.hpp"
#include "SN_Dte.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Bol.hpp"
#include "SN_Flt.hpp"
#include "SN_Bcd.hpp"
#include "eFM.h"
#include "CodeSynthUtils.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CombinedSrc,CN_COMBINEDSRC)
__aNewFunction(CombinedSrc,CN_COMBINEDSRC)
__vDeleteFunction(CombinedSrc,CN_COMBINEDSRC)
__aFooterFunction(CombinedSrc)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CombinedSrc::CombinedSrc(void)
: Object(CN_COMBINEDSRC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);
  
#ifdef WITH_DEBUG_VERSION
  MemLib::vSet(m_caCode,0xFF,C_FLATTEN_SOURCE_V2,CN_COMBINEDSRC);
#endif

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_COMBINEDSRC);
#endif

  m_caCode[0]   = '\0';
  m_azCodeCrs   = m_caCode;

  _checkInitialised_(CN_COMBINEDSRC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CombinedSrc::~CombinedSrc(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Synthesize code for the combined operators                                 */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vSynthesizeCode(Stencil_v2* pStencil)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

  s16KB sCode;
  File* pFileHeader;
  File* pFunctionHeader;
  File* pFileCode;
  bool  bOpened;
  bool bExtracted;
  i8 nFileSize;

  // open the file header file
  pFileHeader = new File(this);
  bOpened = pFileHeader->bOpenRead("../src/store/Fragments/fileHeader.cpp"_Stz);
  RASSERT(bOpened);

  // open the function headers file
  pFunctionHeader = new File(this);
  bOpened = pFunctionHeader->bOpenRead("../src/store/Fragments/functionHeaders.cpp"_Stz);
  RASSERT(bOpened);

  // open the code fragments file
  pFileCode = new File(this);
  bOpened = pFileCode->bOpenRead("../src/store/Fragments/Code.cpp"_Stz);
  RASSERT(bOpened);

  // read the file header in
  nFileSize = pFileHeader->iSize();
  pFileHeader->nReadBytes((void*)sCode.azChar(), nFileSize);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

  for (i8 iOpSketch = 0; iOpSketch < pStencil->nItems(); iOpSketch++) {

    OpSketch* pOpSketch = pStencil->pGet(iOpSketch);

    switch(pOpSketch->eSketchKind()) {

      case SK_FILTER:
        sCode = ""_Stz;
        pFileCode->vSeekStart();
        bExtracted = CodeSynthUtils::bExtract(pFileCode, pOpSketch->m_sLabel, pOpSketch->m_sLabelNext, pOpSketch->m_sLabelLoop, "vFilterBegin"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
        RASSERT(bExtracted);
        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

        sCode = ""_Stz;
        pFileCode->vSeekStart();
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        bExtracted = CodeSynthUtils::bExtract(pFileCode, ""_Stz, ""_Stz, ""_Stz, "vExprCheck"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
        RASSERT(bExtracted);
        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

        sCode = ""_Stz;
        bExtracted = CodeSynthUtils::bExtract(pFileCode, pOpSketch->m_sLabel, pOpSketch->m_sLabelNext, pOpSketch->m_sLabelLoop, "iFilterEnd"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
        RASSERT(bExtracted);
        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

        break;

      case SK_PROJECT:
        sCode = ""_Stz;
        pFileCode->vSeekStart();
        bExtracted = CodeSynthUtils::bExtract(pFileCode, pOpSketch->m_sLabel, pOpSketch->m_sLabelNext, pOpSketch->m_sLabelLoop, "vProjectBegin"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
        RASSERT(bExtracted);
        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

        sCode = ""_Stz;
        pFileCode->vSeekStart();
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        bExtracted = CodeSynthUtils::bExtract(pFileCode, ""_Stz, ""_Stz, ""_Stz, "vExprCheck"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
        RASSERT(bExtracted);
        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

        break;

      case SK_INFLATE_CRW:

        sCode = ""_Stz;
        pFunctionHeader->vSeekStart();
        bExtracted = CodeSynthUtils::bExtract(pFunctionHeader, pOpSketch->m_sLabel, pOpSketch->m_sLabelNext, pOpSketch->m_sLabelLoop, "vInflateCrw"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
        RASSERT(bExtracted);
        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

        StrLib::vAppendString(&m_azCodeCrs, pOpSketch->m_sStencil.azChar());

        sCode = ""_Stz;
        pFileCode->vSeekStart();
        bExtracted = CodeSynthUtils::bExtract(pFileCode, pOpSketch->m_sLabel, pOpSketch->m_sLabelNext, pOpSketch->m_sLabelLoop, "vInflateCrwBegin"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
        RASSERT(bExtracted);
        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

        for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefLft->nColumns(); i_Col++) {

          s256 sColumn;
          s256 sReturnToColumn;

          StrLib::vSprint(sColumn,"%lld"_Stz,i_Col);
          StrLib::vSprint(sReturnToColumn,"L_RetT_Col%lld"_Stz,i_Col);

          sCode = ""_Stz;
          pFileCode->vSeekStart();

          if (i_Col < pOpSketch->m_pStreamDefLft->nColumns() - 1) {
            bExtracted = CodeSynthUtils::bExtractInflate(pFileCode, sColumn, sReturnToColumn, "vInflateCrwCol"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
            RASSERT(bExtracted);
            StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
          }
          else {
            bExtracted = CodeSynthUtils::bExtract(pFileCode, pOpSketch->m_sLabel, pOpSketch->m_sLabelNext, pOpSketch->m_sLabelLoop, "vInflateCrwColLast"_Stz, "marker-start"_Stz, "marker-end"_Stz, sCode);
            RASSERT(bExtracted);
            StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
          }

        }

        break;

      default:
        NEVER_REACH
    }
  }

  StrLib::vAppendString(&m_azCodeCrs, "}");

  vTrace("Synthesized Code: \n %s \n"_Stz, m_caCode);

  // close the files
  pFileHeader->vClose();
  pFunctionHeader->vClose();
  pFileCode->vClose();
}

/*----------------------------------------------------------------------------*/
/* print the synthesized code                                                 */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vPrint(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  vTrace("%s\n"_Stz,m_caCode);
}

/*----------------------------------------------------------------------------*/
/* return source code                                                         */
/*----------------------------------------------------------------------------*/
  char *CombinedSrc::azCode(void) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

    return m_caCode;
  }

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
  void CombinedSrc::vReset(void) {
    _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

    MemLib::vZero(m_caCode, sizeof(m_caCode), CN_COMBINEDSRC);
  }

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate code for the expression                                           */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vGenerateExprCode(TypedStack* pTypedStack, Stz* sIndent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s4K sCopy;

  i8 nExpr = pTypedStack ? pTypedStack->nValues() - 1 : C_UNKNOWN ;

  TypeValueArea* pTypedCell;
  while ( nExpr >= 0) {
    pTypedCell = pTypedStack->aPeekItem(nExpr--);
    Stz* sExpr = pTypedCell->szStz();
    sCopy.vSprint("\n%s%s"_Stz, sIndent, sExpr->azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
