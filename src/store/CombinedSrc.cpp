
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
  m_pCommonCode     = nullptr;

  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_COMBINEDSRC);
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

  // close the files

  if (m_pFileHeader && m_pFileHeader->bIsOpen())
    m_pFileHeader->vClose();

  if (m_pFunctionHeader && m_pFunctionHeader->bIsOpen())
    m_pFunctionHeader->vClose();

  if (m_pFunctionFooter && m_pFunctionFooter->bIsOpen())
    m_pFunctionFooter->vClose();

  if (m_pCommonCode && m_pCommonCode->bIsOpen())
    m_pCommonCode->vClose();

  delete(m_pFileHeader);
  delete(m_pFunctionHeader);
  delete(m_pFunctionFooter);
  delete(m_pCommonCode);
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

  File* pCodeFile = new File(this);

  bool bOpened;

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

        // open the aggregate code fragments file
        bOpened = pCodeFile->bOpenRead("../src/store/Fragments/Aggreg.cpp"_Stz);
        RASSERT ( bOpened );

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "iAggregBegin"_Stz, pCodeFile);

        vAppendCodeFragment(pOpSketch, "vAggregInitBegin"_Stz, pCodeFile);
        vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
        vAppendCodeFragment(pOpSketch, "iAggregInitEnd"_Stz, pCodeFile);

        vAppendCodeFragment(pOpSketch, "vAggregColsBegin"_Stz, pCodeFile);
        vGenerateExprCode(pOpSketch->m_pExprStack, "    "_Stz);
        vAppendCodeFragment(pOpSketch, "iAggregColsEnd"_Stz, pCodeFile);

        // close the aggregate code fragments file
        pCodeFile->vClose();

        break;

      case SK_FILTER:
        // open the filter code fragments file
        bOpened = pCodeFile->bOpenRead("../src/store/Fragments/Filter.cpp"_Stz);
        RASSERT ( bOpened );

        vAppendCodeFragment(pOpSketch, "vFilterBegin"_Stz, pCodeFile);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        vAppendCodeFragment(pOpSketch, "iFilterEnd"_Stz, pCodeFile);

        // close the filter code fragments file
        pCodeFile->vClose();
        break;

      case SK_GBYAGG:

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "iGbyAggBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pGbyStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        vAppendCodeFragment(pOpSketch, "vGbyAggFindKey"_Stz);
        if (pOpSketch->m_nPayloadBytes)
          vAppendCodeFragment(pOpSketch, "vGbyAggCreatePayLoad"_Stz);

        if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT) {
          vAppendCodeFragment(pOpSketch, "iGbyAggInsertIntKey"_Stz);
          vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
          vAppendCodeFragment(pOpSketch, "iGbyAggIntKeyEnd"_Stz);
        }
        else {
          vAppendCodeFragment(pOpSketch, "iGbyAggInsertVchKey"_Stz);
          vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
          vAppendCodeFragment(pOpSketch, "iGbyAggGenKeyEnd"_Stz);
        }
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);

        break;

      case SK_PROJECT:

        // open the project code fragments file
        bOpened = pCodeFile->bOpenRead("../src/store/Fragments/Project.cpp"_Stz);
        RASSERT ( bOpened );

        vAppendStencilTo(pOpSketch);

        vAppendCodeFragment(pOpSketch, "vProjectBegin"_Stz, pCodeFile);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "iProjectEnd"_Stz, pCodeFile);

        // close the project code fragments file
        pCodeFile->vClose();

        break;

      case SK_INDEXSCAN:
        // open the indexScan code fragments file
        bOpened = pCodeFile->bOpenRead("../src/store/Fragments/IndexScan.cpp"_Stz);
        RASSERT ( bOpened );
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // new INDEXBUFFER handler
        vAppendFunctionHeader(eCurrentHandler, pStencil->szStageNm());

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vIndexScanBegin"_Stz, pCodeFile);
        vGenerateExprCode(pOpSketch->m_pUpperStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        vAppendCodeFragment(pOpSketch, "vIndexUpperCheck"_Stz, pCodeFile);
        vAppendCodeFragment(pOpSketch, "vIndexDereference"_Stz);
        vAppendCodeInflateCols(pOpSketch, "L_RetT_ColIS"_Stz, "vInflateCrwCol"_Stz);
        vAppendCodeFragment(pOpSketch, "vIndexScanReturn"_Stz, pCodeFile);

        // close the indexScan code fragments file
        pCodeFile->vClose();

        break;

      case SK_INDEXJOIN:
        // open the indexJoin code fragments file
        bOpened = pCodeFile->bOpenRead("../src/store/Fragments/IndexJoin.cpp"_Stz);
        RASSERT ( bOpened );

        vAppendStencilTo(pOpSketch);
        if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT)
          vAppendCodeFragmentKey(pOpSketch, "vIndexJoinIntKey"_Stz, pCodeFile, (i8)KE_INT);
        else
          vAppendCodeFragmentKey(pOpSketch, "vIndexJoinGenKey"_Stz, pCodeFile, (i8)KE_GENERAL);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        if (pOpSketch->m_pExprStack->nValues()) {
          vAppendCodeFragment(pOpSketch, "vIndexJoinPositionLower"_Stz, pCodeFile);
        }
        else {
          // cross join
          vAppendCodeFragment(pOpSketch, "vIndexJoinPositionBegin"_Stz, pCodeFile);
        }
        vAppendCodeFragment(pOpSketch, "vIndexJoinContinue"_Stz, pCodeFile);
        //vAppendCodeFragment(pOpSketch, "vIndexUpperCheck"_Stz, pCodeFile);
        vAppendCodeCopyCols(pOpSketch, "vCopyVchCol"_Stz);
        vAppendCodeFragment(pOpSketch, "vIndexJoinDereference"_Stz, pCodeFile);
        vAppendCodeInflateCols(pOpSketch, "L_RetT_ColIS"_Stz, "vInflateCrwCol"_Stz);
        vAppendCodeFragment(pOpSketch, "vIndexJoinReturn"_Stz, pCodeFile);

        // close the indexJoin code fragments file
        pCodeFile->vClose();

        break;

      case SK_INFLATE_CRW:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // new CRWBUFFER handler
        vAppendFunctionHeader(eCurrentHandler, pStencil->szStageNm());

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vInflateCrwBegin"_Stz);
        vAppendCodeInflateCols(pOpSketch, "L_RetT_ColT"_Stz, "vInflateCrwCol"_Stz);
        vAppendCodeFragment(pOpSketch, "vInflateCrwReturn"_Stz);
        break;

      case SK_INFLATE_ROW:
        // open the inflateRow code fragments file
        bOpened = pCodeFile->bOpenRead("../src/store/Fragments/InflateRow.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // new ROWBUFFER handler
        vAppendFunctionHeader(eCurrentHandler, pStencil->szStageNm());

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vInflateRowBegin"_Stz, pCodeFile);
        vAppendCodeInflateRowCols(pOpSketch, "vInflateRowCol"_Stz, pCodeFile);

        // close the inflateRow code fragments file
        pCodeFile->vClose();

        break;

      case SK_SORT:

        //vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "iSortBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);

        if (pOpSketch->m_nPayloadBytes)
          vAppendCodeFragment(pOpSketch, "vGbyAggCreatePayLoad"_Stz);

        if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT) {
          vAppendCodeFragment(pOpSketch, "iGbyAggInsertIntKey"_Stz);
        }
        else {
          vAppendCodeFragment(pOpSketch, "iGbyAggInsertVchKey"_Stz);
        }
        vAppendCodePopulatePayloadCols(pOpSketch);

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
            vAppendCodeCopyCols(pOpSketch, "vTubVchCol"_Stz);
          }

        }
        else {
          // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
          // TUB in CRW/ROW/INDEX handler
          vAppendCodeFragment(pOpSketch, "iTubBegin"_Stz);
          vAppendCodeCopyCols(pOpSketch, "vTubVchCol"_Stz);
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

  delete pCodeFile;

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
void CombinedSrc::vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction, File* pCodeFile)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  if ( ! pCodeFile )
    pCodeFile = m_pCommonCode;

  pCodeFile->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(pCodeFile, pOpSketch, szFunction, sCode, nullptr, C_UNKNOWN);
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
  m_pCommonCode->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pCommonCode, pOpSketch, szFunction, sCode, pColumn, C_UNKNOWN);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeFragmentPL(OpSketch* pOpSketch, Stz* szFunction, Column* pColumn,
                                      i8 iPayLoadIndex)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  m_pCommonCode->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pCommonCode, pOpSketch, szFunction, sCode, pColumn,
                                        iPayLoadIndex);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeFragmentKey(OpSketch* pOpSketch, Stz* szFunction, File* pCodeFile, i8 iKeyType)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  pCodeFile->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(pCodeFile, pOpSketch, szFunction, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment for inflate columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeInflateCols(OpSketch* pOpSketch, Stz* sReturnPrefix, Stz* szFunction)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s256 sReturnToColumn;
  s16KB sCode;
  bool bExtracted;

  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefLft->nColumns(); i_Col++) {

    Column* pColumn = pOpSketch->m_pStreamDefLft->pGetColumn(i_Col);
    StrLib::vSprint(sReturnToColumn, "%s%lld"_Stz, sReturnPrefix, i_Col+1);

    sCode = ""_Stz;
    m_pCommonCode->vSeekStart();

    bExtracted = CodeSynthUtils::bExtractInflate(m_pCommonCode, pColumn, sReturnToColumn, szFunction, sCode);
    RASSERT(bExtracted);
    StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
  }
}

/*----------------------------------------------------------------------------*/
/* append code fragment for inflate columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeInflateRowCols(OpSketch* pOpSketch, Stz* szFunction, File* pCodeFile)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s256 sReturnToColumn;
  s16KB sCode;
  bool bExtracted;

  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefLft->nColumns(); i_Col++) {

    Column *pColumn = pOpSketch->m_pStreamDefLft->pGetColumn(i_Col);

    if (pColumn->eColumnType() == CT_VARCHAR ||
        pColumn->eColumnType() == CT_CHAR) {
      sCode = ""_Stz;
      pCodeFile->vSeekStart();

      bExtracted = CodeSynthUtils::bExtractInflate(pCodeFile, pColumn, ""_Stz, szFunction, sCode);
      RASSERT(bExtracted);
      StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* append code fragment for populating payload columns                        */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodePopulatePayloadCols(OpSketch* pOpSketch)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  s256 sColumn;
  i8 i_DupCtr = 1;
  i8 iPayLoad = 0;

  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefOut->nColumns(); i_Col++) {

    Column *pColumn = pOpSketch->m_pStreamDefOut->pGetColumn(i_Col);

    // is the column part of the aggregate group by key ?
    Column* pIndexColumn = pOpSketch->m_pIndexDef->pFindColumn(i_Col);

    if (pIndexColumn) {
      continue;
    }

    // make the column name
    vMakeColumnNmStruct(pOpSketch->m_pStreamDefOut, sColumn, i_DupCtr, i_Col);

    if ( pColumn->eColumnType() == CT_VARCHAR ||
         pColumn->eColumnType() == CT_CHAR ) {

      sCode = ""_Stz;
      m_pCommonCode->vSeekStart();

      vAppendCodeFragmentPL(pOpSketch, "vPopulateVchPL"_Stz, pColumn, iPayLoad);
      StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
    }
    else {
      sCode = ""_Stz;
      m_pCommonCode->vSeekStart();

      vAppendCodeFragmentPL(pOpSketch, "vPopulateIntPL"_Stz, pColumn, iPayLoad);
      StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
    }
    iPayLoad++;
  }
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
  sCode = ""_Stz;
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
      szFunction = "vInflateRowEnd";
      break;
    default:
      NEVER_REACH;
  }
  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  m_pFunctionFooter->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionFooter, nullptr, szFunction, sCode, nullptr, C_UNKNOWN);
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
    case HK_ROWBUFFER:
      szFunction = "__FUNCTION_ROW__";
      iFunction = 1;
      break;
    case HK_INDEXBUFFER:
      szFunction = "__FUNCTION_INDEX__";
      iFunction = 2;
      break;
    case HK_EOS:
      szFunction = "__FUNCTION_EOS__";
      iFunction = 3;
      break;
    default:
      NEVER_REACH;
  }

  sCode = ""_Stz;
  m_pFunctionHeader->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionHeader, nullptr, szFunction, sCode, nullptr, C_UNKNOWN);
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
/* append code fragment for copying columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeCopyCols(OpSketch* pOpSketch, Stz* szFunction)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);


  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefLft->nColumns(); i_Col++) {

    m_pCommonCode->vSeekStart();

    Column *pColumn = pOpSketch->m_pStreamDefLft->pGetColumn(i_Col);

    switch (pColumn->eColumnType()) {

      case CT_VARCHAR:
        vAppendCodeFragment(pOpSketch, szFunction, pColumn);
        if (pOpSketch->m_eStencilTo == SR_TB)
          vAppendCodeFragment(pOpSketch, "vTubRowStr"_Stz, pColumn);
        break;

      default:
        vAppendCodeFragment(pOpSketch, "vCopyCol"_Stz, pColumn);
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

  //s256 sColNm;
  //i8 i_DupCtr = 1;

  for (i8 i_Col = 0; i_Col < pIndexDef->nColumns(); i_Col++) {

    sCode = ""_Stz;
    m_pCommonCode->vSeekStart();

    // get the index columns
    Column* pIndexColumn = pIndexDef->pGetColumn(i_Col);

    // get the steamDef column No maching the indexColumn
    i8 iStreamDefCol = pIndexDef->iGetStreamDefCol(pIndexColumn->iColumnNo());

    // make the column name
    //vMakeColumnNmStruct(pStreamDefOut, sColNm, i_DupCtr, iStreamDefCol);

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
        vAppendCodeFragmentPL(pOpSketch, "vPayLoadToTubVchCol"_Stz, pColumn, iPayLoad);
        break;

      default:
        vAppendCodeFragmentPL(pOpSketch, "vPayLoadToTubCol"_Stz, pColumn, iPayLoad);
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
  m_pCommonCode->vClose();

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
  m_pCommonCode = new File(this);
  bOpened = m_pCommonCode->bOpenRead("../src/store/Fragments/Code.cpp"_Stz);
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
