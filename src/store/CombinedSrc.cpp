
/*============================================================================*/
/* CombinedSrc.cpp -- Combined source                                         */
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
/* form column name from streamDef for given ordinal column                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vMakeColumnNmStruct(StreamDef* pStreamDef, s256& sColNm, i8& i_DupCtr, i8 i_Column)
{
  _static_

  s256 sUpper;

  // get the column from the streamDef
  Column* pColumn = pStreamDef->pGetColumn(i_Column);

  sUpper = pColumn->szColNm();
  StrLib::vToUpper((char *) sUpper.azChar());

  if (pStreamDef->bIsDuplicateName(pColumn))
    sColNm.vSprint("%s%lld.m_iAsI8"_Stz, sUpper.azChar(), i_DupCtr++);
  else
    sColNm.vSprint("%s.m_iAsI8"_Stz, sUpper.azChar());

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Synthesize code for the combined operators                                 */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vSynthesizeCode(Stencil_v2* pStencil)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

  eHK ePreviousHandler = HK_NONE;
  eHK eCurrentHandler  = HK_NONE;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Open the code fragment files
  bool bOpen = bOpenCodeFiles();
  if ( ! bOpen ) {
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append the file headers
  vAppendFileHeaders();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sketch code for each operator
  for (i8 iOpSketch = 0; iOpSketch < pStencil->nItems(); iOpSketch++) {

    OpSketch* pOpSketch = pStencil->pGet(iOpSketch);

    eCurrentHandler = pOpSketch->m_eHandlerKind;

    switch(pOpSketch->eSketchKind()) {

      case SK_EOS:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // end previous handler
        vAppendFunctionFooter(ePreviousHandler);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // begin new EOS handler
        vAppendFunctionHeader(eCurrentHandler, pStencil->szStageNm());
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

      case SK_GBYAGG:

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "iGbyAggBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pGbyStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        vAppendCodeFragment(pOpSketch, "vGbyAggFindKey"_Stz);
        if (pOpSketch->m_nPayloadBytes)
          vAppendCodeFragment(pOpSketch, "vGbyAggCreatePayLoad"_Stz);
        vAppendCodeFragment(pOpSketch, "iGbyAggInsertKey"_Stz);
        vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
        if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT)
          vAppendCodeFragment(pOpSketch, "iGbyAggIntKeyEnd"_Stz);
        else
          vAppendCodeFragment(pOpSketch, "iGbyAggGenKeyEnd"_Stz);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);

        break;

      case SK_PROJECT:

        vAppendStencilTo(pOpSketch);

        vAppendCodeFragment(pOpSketch, "vProjectBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "iProjectEnd"_Stz);
        break;

      case SK_INDEXSCAN:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // new INDEXBUFFER handler
        vAppendFunctionHeader(eCurrentHandler, pStencil->szStageNm());

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vIndexScanBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pUpperStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        vAppendCodeFragment(pOpSketch, "vIndexScanContinue"_Stz);
        break;

      case SK_INFLATE_CRW:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // new CRWBUFFER handler
        vAppendFunctionHeader(eCurrentHandler, pStencil->szStageNm());

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vInflateCrwBegin"_Stz);
        vAppendCodeInflateCols(pOpSketch, "vInflateCrwCol"_Stz);
        break;

      case SK_TUB:

        vAppendStencilTo(pOpSketch);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // TUB in EOS handler
        if (pOpSketch->m_eHandlerKind == HK_EOS) {
          if (pOpSketch->m_eStencilFrom == SR_PL) {
            if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT)
              vAppendCodeFragment(pOpSketch, "vEosPLIntKeyTraversal"_Stz);
            else
              vAppendCodeFragment(pOpSketch, "vEosPLGenKeyTraversal"_Stz);
            vAppendCodeFragment(pOpSketch, "iEosPayLoadTub"_Stz);
            vAppendCodeKeyToTubCols(pOpSketch);
            vAppendCodePayLoadToTubCols(pOpSketch);
            vAppendCodeFragment(pOpSketch, "vEosPayLoadTubEnd"_Stz);
          }
          else {
            vAppendCodeFragment(pOpSketch, "iEosTubBegin"_Stz);
            vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
            vAppendCodeFragment(pOpSketch, "iEosTubBody"_Stz);
            vAppendCodeTubCols(pOpSketch);
          }

        }
        else {
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          // TUB in CRW/ROW/INDEX handler
          vAppendCodeFragment(pOpSketch, "iTubBegin"_Stz);
          vAppendCodeTubCols(pOpSketch);
        }

        break;

      default:
        NEVER_REACH
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // save previous handler
    ePreviousHandler = eCurrentHandler;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end EOS handler
  RASSERT(eCurrentHandler == HK_EOS);
  vAppendFunctionFooter(eCurrentHandler);

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
void CombinedSrc::vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  m_pFileCode->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFileCode, pOpSketch, szFunction,
                                        "marker-start"_Stz, "marker-end"_Stz, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

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
                                        "marker-start"_Stz, "marker-end"_Stz, sCode, pColumn, C_UNKNOWN);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction, Column* pColumn, i8 iPayLoadIndex)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  m_pFileCode->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFileCode, pOpSketch, szFunction,
                                        "marker-start"_Stz, "marker-end"_Stz, sCode, pColumn, iPayLoadIndex);
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
/* append file headers                                                        */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFileHeaders(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  i8 nFileSize;
  s16KB sCode;

  StrLib::vAppendString(&m_azCodeCrs, "#define WITH_TINYC");

  // read the file header in
  nFileSize = m_pFileHeader->iSize();
  m_pFileHeader->nReadBytes((void *) sCode.azChar(), nFileSize);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append function footer fragment                                            */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFunctionFooter(eHK eHandlerKind)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s256 szFunction;

  switch(eHandlerKind) {
    case HK_CRWBUFFER:
      szFunction = "vInflateCrwEnd";
      break;
    case HK_EOS:
      szFunction = "iEosEnd";
      break;
    case HK_INDEXBUFFER:
      szFunction = "vIndexScanEnd";
      break;
    case HK_ROWBUFFER:
      RASSERT(0);
      break;
    default:
      NEVER_REACH;
  }
  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  m_pFunctionFooter->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionFooter, nullptr, szFunction, "marker-start"_Stz, "marker-end"_Stz, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);

  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append function header fragment                                            */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFunctionHeader(eHK eHandlerKind, Stz* szStageNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  s256 sFunSignature;
  bool bExtracted;
  bool bResult;
  i8 iFunction;

  s256 szFunction;

  switch(eHandlerKind) {
    case HK_CRWBUFFER:
      szFunction = "__FUNCTION_CRW__";
      iFunction = 0;
      break;
    case HK_EOS:
      szFunction = "__FUNCTION_EOS__";
      iFunction = 3;
      break;
    case HK_INDEXBUFFER:
      szFunction = "__FUNCTION_INDEX__";
      iFunction = 2;
      break;
    case HK_ROWBUFFER:
      RASSERT(0);
      break;
    default:
      NEVER_REACH;
  }

  sCode = ""_Stz;
  m_pFunctionHeader->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionHeader, nullptr, szFunction, "marker-start"_Stz, "marker-end"_Stz, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);

  sFunSignature.vSprint("iFun%d_%s"_Stz, iFunction,szStageNm);
  bResult = StrLib::bReplace(sCode.azChar(), szFunction.azChar(), sFunSignature.azChar());
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
        break;

      default:
        vAppendCodeFragment(pOpSketch, "vTubCol"_Stz, pColumn);
        break;
    }
  }

}

/*----------------------------------------------------------------------------*/
/* append code fragment for index key columns to tub                          */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeKeyToTubCols(OpSketch* pOpSketch)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  IndexDef* pIndexDef = pOpSketch->m_pIndexDef;
  StreamDef* pStreamDefOut = pOpSketch->m_pStreamDefOut;

  s256 sColNm;
  i8 i_DupCtr = 1;

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {

    sCode = ""_Stz;
    m_pFileCode->vSeekStart();

    // get the index columns
    Column* pIndexColumn = pIndexDef->pGetColumn(i_Col);

    // get the steamDef column No maching the indexColumn
    i8 iStreamDefCol = pIndexDef->iGetStreamDefCol(pIndexColumn->iColumnNo());

    // make the column name
    vMakeColumnNmStruct(pStreamDefOut, sColNm, i_DupCtr, iStreamDefCol);

    Column *pColumn = pStreamDefOut->pGetColumn(iStreamDefCol);

    if (pIndexDef->eKeyKind() == KE_GENERAL) {
      switch (pColumn->eColumnType()) {

        case CT_VARCHAR:
        case CT_CHAR:
          vAppendCodeFragment(pOpSketch, "vPLVchKeyToTub"_Stz, pColumn);
          break;

        default:
          RASSERT(0);
          break;
      }
    }
    else {
      vAppendCodeFragment(pOpSketch, "vPayLoadKeyToTub"_Stz, pColumn);
    }

  }

}

/*----------------------------------------------------------------------------*/
/* append code fragment for payload columns to tub                            */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodePayLoadToTubCols(OpSketch* pOpSketch)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  IndexDef* pIndexDef = pOpSketch->m_pIndexDef;
  StreamDef* pStreamDefOut = pOpSketch->m_pStreamDefOut;

  s256 sColNm;
  i8 i_DupCtr = 1;
  i8 iPayLoad = 0;
  i8 nColumns = pStreamDefOut->nColumns();
  i8 iIndexColumns = pIndexDef->nColumns();

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++, iIndexColumns++) {

    Column *pColumn = pStreamDefOut->pGetColumn(i_Col);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is column in the key ?
    Column* pIndexColumn = pIndexDef->pFindColumn(pColumn->iColumnNo());

    if (pIndexColumn)
      continue;

    // make the column name
    vMakeColumnNmStruct(pStreamDefOut, sColNm, i_DupCtr, i_Col);

    switch (pColumn->eColumnType()) {

      case CT_VARCHAR:
      case CT_CHAR:
        vAppendCodeFragment(pOpSketch, "vPayLoadToTubVchCol"_Stz, pColumn);
        break;

      default:
        vAppendCodeFragment(pOpSketch, "vPayLoadToTubCol"_Stz, pColumn, iPayLoad);
        break;
    }
    iPayLoad++;
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
