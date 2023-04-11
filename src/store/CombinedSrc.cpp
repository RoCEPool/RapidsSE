
/*============================================================================*/
/* CombinedSrc.cpp -- Code Synthesizer                                        */
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

  m_pFileHeader     = nullptr;
  m_pFunctionHeader = nullptr;
  m_pFunctionFooter = nullptr;
  m_pFileCode       = nullptr;

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
  s256 sFunSignature;

  bool bExtracted;
  i8 nFileSize;
  bool bResult;
  bool bEmptyEosHandler = false;

  OpSketch* pEOSSketch = nullptr;

  eSK eDrivingStencil = SK_NONE;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Open the code fragment files
  bool bOpen = bOpenCodeFiles();
  if ( ! bOpen ) {
    return;
  }

  StrLib::vAppendString(&m_azCodeCrs, "#define WITH_TINYC");

  // read the file header in
  nFileSize = m_pFileHeader->iSize();
  m_pFileHeader->nReadBytes((void*)sCode.azChar(), nFileSize);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

  for (i8 iOpSketch = 0; iOpSketch < pStencil->nItems(); iOpSketch++) {

    OpSketch* pOpSketch = pStencil->pGet(iOpSketch);

    switch(pOpSketch->eSketchKind()) {

      case SK_EOS:
        pEOSSketch = pOpSketch;
        break;

      case SK_AGGREG:

        vAppendStencilTo(pOpSketch);

        vAppendCodeFragment(pOpSketch, "iAggregBegin"_Stz);

        vAppendCodeFragment(pOpSketch, "vAggregInitBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
        vAppendCodeFragment(pOpSketch, "iAggregInitEnd"_Stz);

        vAppendCodeFragment(pOpSketch, "vAggregColsBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pExprStack, "    "_Stz);
        vAppendCodeFragment(pOpSketch, "iAggregColsEnd"_Stz);

        break;

      case SK_FILTER:
        vAppendCodeFragment(pOpSketch, "vFilterBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        vAppendCodeFragment(pOpSketch, "iFilterEnd"_Stz);
        break;

      case SK_PROJECT:

        vAppendStencilTo(pOpSketch);

        vAppendCodeFragment(pOpSketch, "vProjectBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "iProjectEnd"_Stz);

        break;

      case SK_INFLATE_CRW:
        eDrivingStencil = SK_INFLATE_CRW;

        vAppendFunctionHeader(pOpSketch, "__FUNCTION_CRW__"_Stz, 0, pStencil->szStageNm());

        StrLib::vAppendString(&m_azCodeCrs, pOpSketch->m_sStencilTo.azChar());
        vAppendCodeFragment(pOpSketch, "vInflateCrwBegin"_Stz);

        vAppendCodeInflateCols(pOpSketch, "vInflateCrwCol"_Stz);

        break;

      case SK_TUB:
        if (pOpSketch->m_eHandlerKind == HK_EOS) {
          switch (eDrivingStencil) {

            case SK_INFLATE_CRW:
              vAppendFunctionFooter(pOpSketch, "vInflateCrwEnd"_Stz);
              break;

            default:
              NEVER_REACH;
          }

          vAppendFunctionHeader(pOpSketch, "__FUNCTION_EOS__"_Stz, 3, pStencil->szStageNm());

          vAppendStencilTo(pOpSketch);

          vAppendCodeFragment(pOpSketch, "iEosTubBegin"_Stz);
          vAppendCodeFragment(pOpSketch, "iEosTubBody"_Stz);

        }
        else {
          bEmptyEosHandler = true;

          vAppendStencilTo(pOpSketch);

          vAppendCodeFragment(pOpSketch, "iTubBegin"_Stz);
        }

        vAppendCodeTubCols(pOpSketch);

        break;

      default:
        NEVER_REACH
    }

  }

  if ( ! bEmptyEosHandler) {
    vAppendFunctionFooter(nullptr, "iEosEnd"_Stz);
  }

  if (bEmptyEosHandler) {

    switch (eDrivingStencil) {

      case SK_INFLATE_CRW:
        vAppendFunctionFooter(nullptr, "vInflateCrwEnd"_Stz);
        break;

      default:
        NEVER_REACH;
    }

    // generate the code for the EOS handler
    sCode = ""_Stz;
    m_pFunctionHeader->vSeekStart();
    bExtracted = CodeSynthUtils::bExtract(m_pFunctionHeader, pEOSSketch, "__FUNCTION_EOS_EMPTY__"_Stz, "marker-start"_Stz,
                                          "marker-end"_Stz, sCode);
    RASSERT(bExtracted);
    sFunSignature.vSprint("iFun3_%s"_Stz, pStencil->szStageNm());
    bResult = StrLib::bReplace(sCode.azChar(), "__FUNCTION_EOS_EMPTY__", sFunSignature.azChar());
    StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
  }

  vTrace("Synthesized Code: \n %s \n"_Stz, m_caCode);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Close the code fragment files
  vCloseCodeFiles();
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
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction, Column* pColumn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  m_pFileCode->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFileCode, pOpSketch, szFunction,
                                        "marker-start"_Stz, "marker-end"_Stz, sCode, pColumn);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment for inflate columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeInflateCols(OpSketch* pOpSketch, Stz* szFunction)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s256 sColumn;
  s256 sReturnToColumn;
  s16KB sCode;
  bool bExtracted;

  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefLft->nColumns(); i_Col++) {

    StrLib::vSprint(sColumn, "%lld"_Stz, i_Col);
    StrLib::vSprint(sReturnToColumn, "L_RetT_Col%lld"_Stz, i_Col);

    sCode = ""_Stz;
    m_pFileCode->vSeekStart();

    bExtracted = CodeSynthUtils::bExtractInflate(m_pFileCode, sColumn, sReturnToColumn, szFunction,
                                                 "marker-start"_Stz, "marker-end"_Stz, sCode);
    RASSERT(bExtracted);
    StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
  }

  vAppendCodeFragment(pOpSketch, "vInflateCrwReturn"_Stz);

}

/*----------------------------------------------------------------------------*/
/* append function footer fragment                                            */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFunctionFooter(OpSketch* pOpSketch, Stz* szFunction)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  s256 sFunSignature;
  bool bExtracted;
  bool bResult;

  sCode = ""_Stz;
  m_pFunctionFooter->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionFooter, pOpSketch, szFunction, "marker-start"_Stz, "marker-end"_Stz, sCode);
  RASSERT(bExtracted);

  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append function header fragment                                            */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFunctionHeader(OpSketch* pOpSketch, Stz* szFunction, i8 iFunction, Stz* szStageNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  s256 sFunSignature;
  bool bExtracted;
  bool bResult;

  sCode = ""_Stz;
  m_pFunctionHeader->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionHeader, pOpSketch, szFunction, "marker-start"_Stz, "marker-end"_Stz, sCode);
  RASSERT(bExtracted);

  sFunSignature.vSprint("iFun%d_%s"_Stz, iFunction,szStageNm);
  bResult = StrLib::bReplace(sCode.azChar(), (char*)szFunction, sFunSignature.azChar());
  RASSERT(bResult);

  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}


/*----------------------------------------------------------------------------*/
/* append stencil to                                                          */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendStencilTo(OpSketch* pOpSketch) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  StrLib::vAppendString(&m_azCodeCrs, pOpSketch->m_sStencilTo.azChar());

}

/*----------------------------------------------------------------------------*/
/* append code fragment for tub columns                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeTubCols(OpSketch* pOpSketch)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefLft->nColumns(); i_Col++) {

    sCode = ""_Stz;
    m_pFileCode->vSeekStart();

    Column *pColumn = pOpSketch->m_pStreamDefLft->pGetColumn(i_Col);

    switch (pColumn->eColumnType()) {

      case CT_VARCHAR:
        vAppendCodeFragment(pOpSketch, "vTubVchCol"_Stz, pColumn);
//        bExtracted = CodeSynthUtils::bExtract(m_pFileCode, pOpSketch, "vTubVchCol"_Stz, "marker-start"_Stz,
//                                              "marker-end"_Stz, sCode, pColumn);
//        RASSERT(bExtracted);
//        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
        break;

      default:
        vAppendCodeFragment(pOpSketch, "vTubCol"_Stz, pColumn);
//        bExtracted = CodeSynthUtils::bExtract(m_pFileCode, pOpSketch, "vTubCol"_Stz, "marker-start"_Stz,
//                                              "marker-end"_Stz, sCode, pColumn);
//        RASSERT(bExtracted);
//        StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
        break;
    }
  }

}

/*----------------------------------------------------------------------------*/
/* close the code fragment files                                             */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vCloseCodeFiles(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

  // close the files
  m_pFileHeader->vClose();
  m_pFunctionHeader->vClose();
  m_pFileCode->vClose();

}

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

/*----------------------------------------------------------------------------*/
/* print the synthesized code                                                 */
/*----------------------------------------------------------------------------*/
bool CombinedSrc::bOpenCodeFiles(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  bool  bOpened;

  // open the file header file
  m_pFileHeader = new File(this);
  bOpened = m_pFileHeader->bOpenRead("../src/store/Fragments/fileHeader.cpp"_Stz);
  if ( ! bOpened )
    return false;

  // open the function headers file
  m_pFunctionHeader = new File(this);
  bOpened = m_pFunctionHeader->bOpenRead("../src/store/Fragments/functionHeaders.cpp"_Stz);
  if ( ! bOpened )
    return false;

  // open the code fragments file
  m_pFileCode = new File(this);
  bOpened = m_pFileCode->bOpenRead("../src/store/Fragments/Code.cpp"_Stz);
  if ( ! bOpened )
    return false;

  // open the function footer file
  m_pFunctionFooter = new File(this);
  bOpened = m_pFunctionFooter->bOpenRead("../src/store/Fragments/functionFooters.cpp"_Stz);
  if ( ! bOpened )
    return false;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
