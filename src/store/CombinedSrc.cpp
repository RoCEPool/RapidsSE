
/*============================================================================*/
/* CombinedSrc.cpp -- Combined source                                         */
/*============================================================================*/

#include "BASE.h"

#include "Stream.hpp"
#include "StreamDef.hpp"
#include "LogicUnit.hpp"
#include "Index.hpp"
#include "SN_Dte.hpp"
#include "SN_Tme.hpp"
#include "SN_Tsp.hpp"
#include "SN_Bol.hpp"
#include "SN_Flt.hpp"
#include "SN_Bcd.hpp"
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

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caCode,sizeof(m_caCode),CN_COMBINEDSRC);
#endif

  m_azCodeCrs   = m_caCode;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Open the common code fragment files
  bool bOpen = bOpenCodeFiles();
  if ( ! bOpen ) {
    return;
  }

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Close the common code fragment files
  vCloseCodeFiles();

  delete(m_pFileHeader);
  delete(m_pFunctionHeader);
  delete(m_pFunctionFooter);
  delete(m_pCommonCode);
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
void CombinedSrc::vSynthesizeCode(Stencil_v2* pStencil, bool bFirstStage, bool bLastStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COMBINEDSRC,_FOOTER_);

  bool bOpened;
  bool bHasIndexInflate = false;

  eHK ePreviousHandler = HK_NONE;
  eHK eCurrentHandler  = HK_NONE;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // new the file pointers
  File* pOperatorFile = new File(this);
  File* pIndexFile    = new File(this);
  File* pInflateFile  = new File(this);
  File* pFilterFile   = new File(this);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append the file headers if firstStage
  if (bFirstStage)
    vAppendFileHeaders();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate code for each operator
  for (i8 iOpSketch = 0; iOpSketch < pStencil->nItems(); iOpSketch++) {

    OpSketch* pOpSketch = pStencil->pGet(iOpSketch);

    eCurrentHandler = pOpSketch->m_eHandlerKind;

    switch(pOpSketch->eSketchKind()) {

      case SK_ANTIJOIN:
      case SK_SEMIJOIN:

        bHasIndexInflate = true;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the index, indexJoin, inflate  and filter code fragments file
        bOpened = pIndexFile->bOpenRead("../src/store/Fragments/Index.cpp"_Stz);
        RASSERT ( bOpened );

        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/IndexJoin.cpp"_Stz);
        RASSERT ( bOpened );

        bOpened = pFilterFile->bOpenRead("../src/store/Fragments/Filter.cpp"_Stz);
        RASSERT ( bOpened );

        bOpened = pInflateFile->bOpenRead("../src/store/Fragments/InflateCrw.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator begin
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        vAppendStencilTo(pOpSketch);
        //vAppendCodeFragment(pOpSketch, "vJoinBegin"_Stz, pOperatorFile);
        vAppendCodeFragment(pOpSketch, "vOperatorBegin"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // initialize with key kind
        if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT)
          vAppendCodeFragment(pOpSketch, "vIndexJoinIntKey"_Stz, pOperatorFile);
        else
          vAppendCodeFragment(pOpSketch, "vIndexJoinGenKey"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // generate expressions for populating the key
        RASSERT(pOpSketch->m_pExprStack->nValues());
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // position at lower bound
        vAppendCodeFragment(pOpSketch, "vIndexPositionLower"_Stz, pIndexFile);
        vAppendCodeFragment(pOpSketch, "vIndexJoinContinue"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // dereference the join
        vAppendCodeFragment(pOpSketch, "vIndexJoinDereference"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // inflate the right table columns
        vAppendCodeInflateRightCols(pOpSketch, "L_RetT_ColIJ"_Stz, pInflateFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // generate code for the constraint
        vGenerateExprCode(pOpSketch->m_pConstraintStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        vAppendCodeFragment(pOpSketch, "vFilterEnd"_Stz, pFilterFile);

        vAppendCodeFragment(pOpSketch, "vIndexJoinReturn01"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // anti/semi-join end
        if (pOpSketch->eSketchKind() == SK_ANTIJOIN)
          vAppendCodeFragment(pOpSketch, "vAntiJoinEnd"_Stz, pOperatorFile);
        else
          vAppendCodeFragment(pOpSketch, "vSemiJoinEnd"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the indexJoin code fragments file
        pInflateFile->vClose();
        pOperatorFile->vClose();
        pIndexFile->vClose();
        pFilterFile->vClose();

        break;

      case SK_EOS:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // end previous handler (by appending its footer)
        vAppendFunctionFooter(pOpSketch, ePreviousHandler, bHasIndexInflate);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // begin new EOS handler (by appending its function header)
        vAppendFunctionHeader(pOpSketch, eCurrentHandler, pStencil->szStageNm(), bHasIndexInflate);
        break;

      case SK_AGGREG:

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the aggregate code fragments file
        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/Aggreg.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vOperatorBegin"_Stz);

        vAppendCodeFragment(pOpSketch, "vAggregInitBegin"_Stz, pOperatorFile);
        vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
        vAppendCodeFragment(pOpSketch, "iAggregInitEnd"_Stz, pOperatorFile);

        vAppendCodeFragment(pOpSketch, "vAggregColsBegin"_Stz, pOperatorFile);
        vGenerateExprCode(pOpSketch->m_pExprStack, "    "_Stz);
        vAppendCodeFragment(pOpSketch, "iAggregColsEnd"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the aggregate code fragments file
        pOperatorFile->vClose();

        break;

      case SK_FILTER:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the filter code fragments file
        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/Filter.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        vAppendCodeFragment(pOpSketch, "vFilterBegin"_Stz, pOperatorFile);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
        vAppendCodeFragment(pOpSketch, "vFilterEnd"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the filter code fragments file
        pOperatorFile->vClose();
        break;

      case SK_GBYAGG:

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the gbyagg code fragments file
        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/GbyAgg.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        // stencil to
        vAppendStencilTo(pOpSketch);

        // create indexDef and indexTree
        vAppendCodeFragment(pOpSketch, "iGbyAggBegin"_Stz, pOperatorFile);
        vAppendCodeFragment(pOpSketch, "vIndexDefBegin"_Stz);
        vAppendCodeIndexCols(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vIndexDefContinue"_Stz);

        // populate the payload key
        vAppendCodeFragment(pOpSketch, "iResetPLKey"_Stz);
        vGenerateExprCode(pOpSketch->m_pGbyStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);

        // find the key
        vAppendCodeFragment(pOpSketch, "vGbyAggFindKey"_Stz, pOperatorFile);

        // create the payload
        if (pOpSketch->m_nPayloadBytes)
          vAppendCodeFragment(pOpSketch, "vCreatePayLoad"_Stz);

        // insert key
        if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT) {
          vAppendCodeFragment(pOpSketch, "iIndexInsertIntKey"_Stz);
          vAppendCodeFragment(pOpSketch, "vIndexInsertContinue"_Stz);
          vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
          vAppendCodeFragment(pOpSketch, "iGbyAggIntKeyEnd"_Stz, pOperatorFile);
        }
        else {
          vAppendCodeFragment(pOpSketch, "iIndexInsertVchKey"_Stz);
          vAppendCodeFragment(pOpSketch, "vIndexInsertContinue"_Stz);
          vGenerateExprCode(pOpSketch->m_pInitStack, "    "_Stz);
          vAppendCodeFragment(pOpSketch, "iGbyAggGenKeyEnd"_Stz, pOperatorFile);
        }

        // populate the payload
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the gbyAgg code fragments file
        pOperatorFile->vClose();
        break;

        break;

      case SK_ORDAGG:
        RASSERT(0);
        break;

      case SK_PROJECT:

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the project code fragments file
        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/Project.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        vAppendStencilTo(pOpSketch);

        vAppendCodeFragment(pOpSketch, "vOperatorBegin"_Stz);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "iProjectEnd"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the project code fragments file
        pOperatorFile->vClose();

        break;

      case SK_INDEXSCAN:

        bHasIndexInflate = true;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the indexScan code fragments file
        bOpened = pIndexFile->bOpenRead("../src/store/Fragments/Index.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the index code fragments file
        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/IndexScan.cpp"_Stz);
        RASSERT ( bOpened );
        
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // new INDEXBUFFER handler
        vAppendFunctionHeader(pOpSketch, eCurrentHandler, pStencil->szStageNm(), bHasIndexInflate);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        vAppendStencilTo(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vIndexScanBegin"_Stz, pOperatorFile);

        // No lower bound
        if (pOpSketch->m_eLowerIP == IP_NONE) {
          vAppendCodeFragment(pOpSketch, "vIndexPositionBegin"_Stz, pIndexFile);
        }
        else {
          vGenerateExprCode(pOpSketch->m_pLowerStack, "  "_Stz);
          vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
          vAppendCodeFragment(pOpSketch, "vIndexPositionLower"_Stz, pIndexFile);
        }

        if (pOpSketch->m_eUpperIP != IP_NONE) {
          vGenerateExprCode(pOpSketch->m_pUpperStack, "  "_Stz);
          vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
          vAppendCodeFragment(pOpSketch, "vIndexPositionUpper"_Stz, pIndexFile);
          vAppendCodeFragment(pOpSketch, "vIndexDereferenceUB"_Stz, pIndexFile);
        }
        else {
          vAppendCodeFragment(pOpSketch, "vIndexDereferenceLB"_Stz, pIndexFile);
        }

        // open the inflateCrw code fragments file
        bOpened = pInflateFile->bOpenRead("../src/store/Fragments/InflateCrw.cpp"_Stz);
        RASSERT ( bOpened );
        vAppendCodeInflateCols(pOpSketch, "L_RetT_ColIS"_Stz, pInflateFile);

        vAppendCodeFragment(pOpSketch, "vIndexReturn"_Stz, pIndexFile);

        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the indexScan code fragments file
        pInflateFile->vClose();
        pOperatorFile->vClose();
        pIndexFile->vClose();

        break;

      case SK_INDEXJOIN:

        bHasIndexInflate = true;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the indexJoin code fragments file
        bOpened = pIndexFile->bOpenRead("../src/store/Fragments/Index.cpp"_Stz);
        RASSERT ( bOpened );

        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/IndexJoin.cpp"_Stz);
        RASSERT ( bOpened );

        bOpened = pInflateFile->bOpenRead("../src/store/Fragments/InflateCrw.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        vAppendStencilTo(pOpSketch);
        //vAppendCodeFragment(pOpSketch, "vJoinBegin"_Stz, pOperatorFile);
        vAppendCodeFragment(pOpSketch, "vOperatorBegin"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // initialize with key kind
        if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT)
          vAppendCodeFragment(pOpSketch, "vIndexJoinIntKey"_Stz, pOperatorFile);
        else
          vAppendCodeFragment(pOpSketch, "vIndexJoinGenKey"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // generate expressions for populating the key
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // posisition the index
        if (pOpSketch->m_pExprStack->nValues()) {
          vAppendCodeFragment(pOpSketch, "vIndexPositionLower"_Stz, pIndexFile);

          if (pOpSketch->m_bLeftOuter) {
            vAppendCodeFragment(pOpSketch, "vIndexJoinContinueLO0"_Stz, pOperatorFile);
            vAppendCodeFragment(pOpSketch, "vIndexJoinContinueLO1"_Stz, pOperatorFile);
            vAppendCodeCopyCols(pOpSketch, "vCopyVchCol"_Stz);
            vAppendCodeNullRightCols(pOpSketch, "vCopyNulls"_Stz);
            vAppendCodeFragment(pOpSketch, "vIndexJoinContinueLO2"_Stz, pOperatorFile);

            vAppendCodeFragment(pOpSketch, "vIndexJoinContinueLO3"_Stz, pOperatorFile);
            vAppendCodeFragment(pOpSketch, "vIndexJoinContinueLO1"_Stz, pOperatorFile);
            vAppendCodeCopyCols(pOpSketch, "vCopyVchCol"_Stz);
            vAppendCodeNullRightCols(pOpSketch, "vCopyNulls"_Stz);
            vAppendCodeFragment(pOpSketch, "vIndexJoinContinueLO2"_Stz, pOperatorFile);
            vAppendCodeCopyCols(pOpSketch, "vCopyVchCol"_Stz);
          }
          else {
            vAppendCodeFragment(pOpSketch, "vIndexJoinContinue"_Stz, pOperatorFile);
            vAppendCodeCopyCols(pOpSketch, "vCopyVchCol"_Stz);
          }
        }
        else {
          // cross join
          vAppendCodeFragment(pOpSketch, "vIndexCrossPositionBegin"_Stz, pIndexFile);
          vAppendCodeFragment(pOpSketch, "vIndexCrossJoinContinue"_Stz, pOperatorFile);
          vAppendCodeCopyCols(pOpSketch, "vCopyVchCol"_Stz);
        }



        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // dereference the join
        vAppendCodeFragment(pOpSketch, "vIndexJoinDereference"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // inflate the right table columns
        vAppendCodeInflateRightCols(pOpSketch, "L_RetT_ColIJ"_Stz, pInflateFile);

        if (pOpSketch->m_pConstraintStack->nValues()) {
          vAppendCodeFragment(pOpSketch, "vIndexJoinReturn01"_Stz, pOperatorFile);
          vGenerateExprCode(pOpSketch->m_pConstraintStack, "  "_Stz);
          vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);
          vAppendCodeFragment(pOpSketch, "vFilterEndIndexJoin"_Stz, pOperatorFile);
          vAppendCodeFragment(pOpSketch, "vIndexJoinReturn02"_Stz, pOperatorFile);
        }
        else {
          vAppendCodeFragment(pOpSketch, "vIndexJoinReturn"_Stz, pOperatorFile);
        }


        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the indexJoin code fragments file
        pInflateFile->vClose();
        pOperatorFile->vClose();
        pIndexFile->vClose();

        break;

      case SK_INFLATE_CRW:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the inflateCrw code fragments file
        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/InflateCrw.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // new CRWBUFFER handler
        vAppendFunctionHeader(pOpSketch, eCurrentHandler, pStencil->szStageNm(), bHasIndexInflate);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // append to stencil
        vAppendStencilTo(pOpSketch);

        vAppendCodeFragment(pOpSketch, "vInflateCrwBegin"_Stz, pOperatorFile);
        vAppendCodeInflateCols(pOpSketch, "L_RetT_ColT"_Stz, pOperatorFile);
        vAppendCodeFragment(pOpSketch, "vInflateCrwReturn"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the inflateCrw code fragments file
        pOperatorFile->vClose();
        break;

      case SK_INFLATE_ROW:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the inflateRow code fragments file
        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/InflateRow.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // new ROWBUFFER handler
        vAppendFunctionHeader(pOpSketch, eCurrentHandler, pStencil->szStageNm(), bHasIndexInflate);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // append to stencil
        vAppendStencilTo(pOpSketch);

        vAppendCodeFragment(pOpSketch, "vInflateRowBegin"_Stz, pOperatorFile);
        vAppendCodeInflateRowCols(pOpSketch, "vInflateRowCol"_Stz, pOperatorFile);
        vAppendCodeFragment(pOpSketch, "vInflateRowReturn"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the inflateRow code fragments file
        pOperatorFile->vClose();

        break;

      case SK_LIMIT:
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // open the limit code fragments file
        bOpened = pOperatorFile->bOpenRead("../src/store/Fragments/Limit.cpp"_Stz);
        RASSERT ( bOpened );

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        vAppendCodeFragment(pOpSketch, "vLimit"_Stz, pOperatorFile);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // close the inflateRow code fragments file
        pOperatorFile->vClose();

        break;

      case SK_SORT:
      case SK_TOP:

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        vAppendCodeFragment(pOpSketch, "iSortBegin"_Stz);
        vAppendCodeFragment(pOpSketch, "iResetPLKey"_Stz);
        vAppendCodeFragment(pOpSketch, "vIndexDefBegin"_Stz);
        vAppendCodeIndexCols(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vIndexDefContinue"_Stz);
        vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);
        vAppendCodeFragment(pOpSketch, "vExprCheck"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // create the pay load
        if (pOpSketch->m_nPayloadBytes)
          vAppendCodeFragment(pOpSketch, "vCreatePayLoad"_Stz);

        if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT) {
          vAppendCodeFragment(pOpSketch, "iIndexInsertIntKey"_Stz);
        }
        else {
          vAppendCodeFragment(pOpSketch, "iIndexInsertVchKey"_Stz);
        }
        vAppendCodePopulatePayloadCols(pOpSketch);
        vAppendCodeFragment(pOpSketch, "vCloseBraces"_Stz);
        break;

      case SK_STREAMDEF:

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // end previous handler function
        vAppendFunctionFooter(pOpSketch, ePreviousHandler, bHasIndexInflate);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // begin new StreamDef handler function
        vAppendFunctionHeader(pOpSketch, eCurrentHandler, pStencil->szStageNm(), bHasIndexInflate);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // StreamDefOut generation
        vAppendCodeFragment(pOpSketch, "iStreamDefBegin"_Stz);
        vAppendCodePixCols(pOpSketch, bLastStage );
        vAppendCodeFragment(pOpSketch, "vStreamDefEnd"_Stz);
        break;

      case SK_TUB:
      case SK_POT:

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // operator stub
        vAppendCodeFragment(pOpSketch, "vOperatorStub"_Stz);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // to stencil
        vAppendStencilTo(pOpSketch);

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // TUB in EOS handler
        if (pOpSketch->m_eHandlerKind == HK_EOS) {
          if (pOpSketch->m_eStencilFrom == SR_PL) {
            vAppendCodeFragment(pOpSketch, "iTubBegin"_Stz);
            vAppendCodePixCols(pOpSketch);
            vAppendCodeFragment(pOpSketch, "vTubRowPix"_Stz);
            if (pOpSketch->m_pIndexDef->eKeyKind() == KE_INT)
              vAppendCodeFragment(pOpSketch, "vEosPLIntKeyTraversal"_Stz);
            else
              vAppendCodeFragment(pOpSketch, "vEosPLGenKeyTraversal"_Stz);

            if ( ! pOpSketch->m_sLimitExpr.bSame(""_Stz)) {
              vAppendCodeFragment(pOpSketch, "vEosLimit"_Stz);
            }

            vAppendCodeFragment(pOpSketch, "iEosPayLoadTub"_Stz);
            vAppendCodeToTubCols(pOpSketch);
            vAppendCodeFragment(pOpSketch, "vEosPayLoadTubEnd"_Stz);
          }
          else {
            vAppendCodeFragment(pOpSketch, "iTubBegin"_Stz);
            vAppendCodePixCols(pOpSketch);
            vAppendCodeFragment(pOpSketch, "vTubRowPix"_Stz);
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
          vAppendCodePixCols(pOpSketch);
          vAppendCodeFragment(pOpSketch, "vTubRowPix"_Stz);

          // generate code for the hash value
          if (pOpSketch->eSketchKind() == SK_POT)
            vGenerateExprCode(pOpSketch->m_pExprStack, "  "_Stz);

          vAppendCodeFragment(pOpSketch, "iTubContinue"_Stz);
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
  // end StreamDef handler
  RASSERT(eCurrentHandler == HK_STREAMDEF);
  vAppendFunctionFooter(nullptr, eCurrentHandler, bHasIndexInflate);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete the code file pointers
  delete pOperatorFile;
  delete pIndexFile;
  delete pInflateFile;
  delete pFilterFile;
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
    m_azCodeCrs   = m_caCode;
  }

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction, File* pOperatorFile)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  if ( ! pOperatorFile )
    pOperatorFile = m_pCommonCode;

  pOperatorFile->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(pOperatorFile, pOpSketch, szFunction, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeFragment(OpSketch* pOpSketch, Stz* szFunction, Column* pColumn, File* pOperatorFile)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  if ( ! pOperatorFile )
    pOperatorFile = m_pCommonCode;

  pOperatorFile->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(pOperatorFile, pOpSketch, szFunction, sCode, pColumn, C_UNKNOWN);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment                                                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeFragmentPL(OpSketch* pOpSketch, Stz* szFunction, Column* pColumn,
                                      i8 iIndexColumn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  m_pCommonCode->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pCommonCode, pOpSketch, szFunction, sCode, pColumn,
                                        iIndexColumn);
  RASSERT(bExtracted);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append code fragment for inflate columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeInflateCols(OpSketch* pOpSketch, Stz* sReturnPrefix, File* pOperatorFile)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s256 sReturnToColumn;
  s16KB sCode;
  bool bExtracted;
  bool bOpened = false;

  i8 nCols = 0;
  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefLft->nColumns(); i_Col++) {

//    if (nCols > pOpSketch->m_pColsReq->m_nCols) {
//      break;
//    }

    nCols++;

    Column* pColumn = pOpSketch->m_pStreamDefLft->pGetColumn(i_Col);

    StrLib::vSprint(sReturnToColumn, "%s%lld"_Stz, sReturnPrefix, i_Col+1);

//    if (pOpSketch->m_pColsReq->m_axCols[i_Col+1].m_i_EmittedCol == C_UNKNOWN) {
//      sCode = ""_Stz;
//      pOperatorFile->vSeekStart();
//
//      bExtracted = CodeSynthUtils::bExtractInflate(pOperatorFile, pColumn, sReturnToColumn, "vSkipCrwCol"_Stz, sCode);
//      RASSERT(bExtracted);
//      StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
//      continue;
//    }

    sCode = ""_Stz;
    pOperatorFile->vSeekStart();

    bExtracted = CodeSynthUtils::bExtractInflate(pOperatorFile, pColumn, sReturnToColumn, "vInflateCrwCol"_Stz, sCode);
    RASSERT(bExtracted);
    StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
  }

/*
  i8 nColsReq = 0;
  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefLft->nColumns(); i_Col++) {

    Column* pColumn = pOpSketch->m_pStreamDefLft->pGetColumn(i_Col);

    StrLib::vSprint(sReturnToColumn, "%s%lld"_Stz, sReturnPrefix, i_Col+1);

    if (pOpSketch->bColReq(i_Col+1)) {
      nColsReq++;
      sCode = ""_Stz;
      pOperatorFile->vSeekStart();

      bExtracted = CodeSynthUtils::bExtractInflate(pOperatorFile, pColumn, sReturnToColumn, "vInflateCrwCol"_Stz, sCode);
      RASSERT(bExtracted);
      StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
    }
    else {
      sCode = ""_Stz;
      pOperatorFile->vSeekStart();

      bExtracted = CodeSynthUtils::bExtractInflate(pOperatorFile, pColumn, sReturnToColumn, "vSkipCrwCol"_Stz, sCode);
      RASSERT(bExtracted);
      StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
      continue;
    }

    if (nColsReq >= pOpSketch->m_pColsReq->m_nCols)
      return;
  }
*/
}

/*----------------------------------------------------------------------------*/
/* append code fragment for inflate columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeInflateRightCols(OpSketch* pOpSketch, Stz* sReturnPrefix, File* pOperatorFile)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s256 sReturnToColumn;
  s16KB sCode;
  bool bExtracted;
  bool bOpened = false;

  for (i8 i_Col = 0; i_Col < pOpSketch->m_pStreamDefRht->nColumns(); i_Col++) {

    Column* pColumn = pOpSketch->m_pStreamDefRht->pGetColumn(i_Col);
    StrLib::vSprint(sReturnToColumn, "%s%lld"_Stz, sReturnPrefix, i_Col+1);

    sCode = ""_Stz;
    pOperatorFile->vSeekStart();

    bExtracted = CodeSynthUtils::bExtractInflate(pOperatorFile, pColumn, sReturnToColumn, "vInflateCrwCol"_Stz, sCode);
    RASSERT(bExtracted);
    StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
  }

}


/*----------------------------------------------------------------------------*/
/* append code fragment for inflate columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeInflateRowCols(OpSketch* pOpSketch, Stz* szFunction, File* pOperatorFile)
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
      pOperatorFile->vSeekStart();

      bExtracted = CodeSynthUtils::bExtractInflate(pOperatorFile, pColumn, ""_Stz, szFunction, sCode);
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
  i8 iPayLoad = 0;

  i8 nColumns = pOpSketch->m_pStreamDefLft->nColumns();
  RASSERT(nColumns == pOpSketch->m_pColsMap->m_nCols);

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    i8 i_Cell = pOpSketch->m_pColsMap->m_axCols[i_Col].m_i_Cell;
    i8 i_EmittedCol = pOpSketch->m_pColsMap->m_axCols[i_Col].m_i_EmittedCol;
    Column *pColumn = pOpSketch->m_pStreamDefLft->pGetColumn(i_EmittedCol-1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is column in the payload ?
    bool bIsPayload = pOpSketch->bIsPayload(i_Col);

//    // is the column part of the aggregate group by key ?
//    Column* pIndexColumn = pOpSketch->m_pIndexDef->pFindColumn(i_Col);
//
//    if (pIndexColumn) {
//      continue;
//    }

    if ( ! bIsPayload ) {
      continue;
    }

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
  s4K sCode;

  // zero the string area as reading into it and will not be null terminated
  sCode.vZero();

  StrLib::vAppendString(&m_azCodeCrs, "#define WITH_CLANG");

  // read the file header in
  sCode = ""_Stz;
  nFileSize = m_pFileHeader->iSize();
  m_pFileHeader->nReadBytes((void *) sCode.azChar(), nFileSize);
  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append function footer fragment                                            */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFunctionFooter(OpSketch* pOpSketch, eHK eHandlerKind, bool &bHasIndexInflate)
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
      if (bHasIndexInflate)
        szFunction = "vInflateRowIndexEnd";
      else
        szFunction = "vInflateRowEnd";
      break;
    case HK_STREAMDEF:
      szFunction = "iStreamDefEnd";
      break;
    default:
      NEVER_REACH;
  }
  s16KB sCode;
  bool bExtracted;

  sCode = ""_Stz;
  m_pFunctionFooter->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionFooter, pOpSketch, szFunction, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);

  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
  bHasIndexInflate = false;
}

/*----------------------------------------------------------------------------*/
/* append function header fragment                                            */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendFunctionHeader(OpSketch* pOpSketch, eHK eHandlerKind, Stz* szStageNm, bool bHasIndexInflate )
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  s256 sFunSignature;
  s256 sFunctionPrefix;
  bool bExtracted;
  bool bResult;

  s256 szFunction;

  switch(eHandlerKind) {
    case HK_CRWBUFFER:
      szFunction = "__FUNCTION_CRW__";
      sFunSignature = "iCrwHandler_";
      break;
    case HK_EOS:
      szFunction = "__FUNCTION_EOS__";
      sFunSignature = "iEosHandler_";
      break;
    case HK_INDEXBUFFER:
      szFunction = "__FUNCTION_INDEX__";
      sFunSignature = "iIndexHandler_";
      break;
    case HK_ROWBUFFER:
      szFunction = "__FUNCTION_ROW__";
      sFunSignature = "iRowHandler_";
      break;
    case HK_STREAMDEF:
      szFunction = "__FUNCTION_STREAMDEF__";
      sFunSignature = "iStreamDef_";
      break;
    default:
      NEVER_REACH;
  }

  // append header

  sCode = ""_Stz;
  m_pFunctionHeader->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionHeader, nullptr, szFunction, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);

  sFunSignature.vConcat(szStageNm);
  bResult = StrLib::bReplace(sCode.azChar(), szFunction.azChar(), sFunSignature.azChar());
  RASSERT(bResult);

  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

  if (eHandlerKind == HK_EOS || eHandlerKind == HK_STREAMDEF)
    return;

  // append vSetColumns

  sCode = ""_Stz;
  m_pFunctionHeader->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionHeader, pOpSketch, "vSetColumns"_Stz, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);

  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());

  // append function continue

  switch(eHandlerKind) {
    case HK_CRWBUFFER:
      szFunction = "vCrwContinue";
      break;
    case HK_ROWBUFFER:
      if (bHasIndexInflate)
        szFunction = "vRowIndexContinue";
      else
        szFunction = "vRowContinue";
      break;
    case HK_INDEXBUFFER:
      szFunction = "vIndexContinue";
      break;
    case HK_EOS:
      szFunction = "__FUNCTION_EOS__";
      break;
    case HK_STREAMDEF:
      szFunction = "__FUNCTION_STREAMDEF__";
      break;
    default:
      NEVER_REACH;
  }

  sCode = ""_Stz;
  m_pFunctionHeader->vSeekStart();
  bExtracted = CodeSynthUtils::bExtract(m_pFunctionHeader, nullptr, szFunction, sCode, nullptr, C_UNKNOWN);
  RASSERT(bExtracted);

  StrLib::vAppendString(&m_azCodeCrs, sCode.azChar());
}

/*----------------------------------------------------------------------------*/
/* append stencil to                                                          */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendStencilTo(OpSketch* pOpSketch) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;
  File* pStencilFile = new File(this);
  StreamDef* pStreamDef;

  bool bOpened = pStencilFile->bOpenRead("../src/store/Fragments/Stencil.cpp"_Stz);
  RASSERT ( bOpened );

  if (pOpSketch->m_eSketchKind == SK_SEMIJOIN || pOpSketch->m_eSketchKind == SK_ANTIJOIN) {
    pStreamDef = pOpSketch->m_pStreamDefRht;
  }
  else {
    pStreamDef = pOpSketch->m_pStreamDefOut;
  }

  sCode = ""_Stz;
  pStencilFile->vSeekStart();
  vAppendCodeFragment(pOpSketch, "vStencilToBegin"_Stz, pStencilFile);

  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++) {

    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    sCode = ""_Stz;
    pStencilFile->vSeekStart();
    vAppendCodeFragment(pOpSketch, "vStencilToCol"_Stz, pColumn, pStencilFile);
  }

  sCode = ""_Stz;
  pStencilFile->vSeekStart();
  vAppendCodeFragment(pOpSketch, "vStencilToEnd"_Stz, pStencilFile);

  // close the stencil code fragments file
  pStencilFile->vClose();
  delete pStencilFile;
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

        if (pOpSketch->m_eStencilTo == SR_TO)
          vAppendCodeFragment(pOpSketch, "vTubVchCol"_Stz, pColumn);
        else
          vAppendCodeFragment(pOpSketch, szFunction, pColumn);
        break;

      default:
        vAppendCodeFragment(pOpSketch, "vCopyCol"_Stz, pColumn);
        break;
    }
  }

}

/*----------------------------------------------------------------------------*/
/* append code fragment for copying columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodePixCols(OpSketch* pOpSketch, bool bLastStage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s256 sCopy;
  i8 nColumns = pOpSketch->m_pStreamDefOut->nColumns();

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    m_pCommonCode->vSeekStart();

    Column *pColumn = pOpSketch->m_pStreamDefOut->pGetColumn(i_Col);

    if (bLastStage)
      StrLib::vRemoveHead(pColumn->szColNm()->azChar(), '_');

    if (i_Col == nColumns - 1)
      sCopy.vSprint("      {%lld, \"%s\", %lld, %lld, %lld, %s, %s, %s}\n };\n"_Stz,
                    pColumn->iColumnNo(),
                    pColumn->szColNm(),
                    pColumn->eColumnType(),
                    pColumn->iColumnLength(),
                    pColumn->iColumnScale(),
                    pColumn->bIsNullable() ? "true" : "false",
                    pColumn->bIsPtnKey() ? "true" : "false",
                    pColumn->bIsAscending() ? "true" : "false");
    else
      sCopy.vSprint("      {%lld, \"%s\", %lld, %lld, %lld, %s, %s, %s},\n"_Stz,
                    pColumn->iColumnNo(),
                    pColumn->szColNm(),
                    pColumn->eColumnType(),
                    pColumn->iColumnLength(),
                    pColumn->iColumnScale(),
                    pColumn->bIsNullable() ? "true" : "false",
                    pColumn->bIsPtnKey() ? "true" : "false",
                    pColumn->bIsAscending() ? "true" : "false");
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }

}

/*----------------------------------------------------------------------------*/
/* append code fragment for copying columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeIndexCols(OpSketch* pOpSketch)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s256 sCopy;
  i8 nColumns = pOpSketch->m_pIndexDef->nColumns();

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    m_pCommonCode->vSeekStart();

    Column *pColumn = pOpSketch->m_pIndexDef->pGetColumn(i_Col);

    if (i_Col == nColumns - 1)
      sCopy.vSprint("      {%lld, \"%s\", %lld, %lld, %lld, %s, %s, %s}\n   };\n"_Stz,
                    pColumn->iColumnNo(),
                    pColumn->szColNm(),
                    pColumn->eColumnType(),
                    pColumn->iColumnLength(),
                    pColumn->iColumnScale(),
                    pColumn->bIsNullable() ? "true" : "false",
                    pColumn->bIsPtnKey() ? "true" : "false",
                    pColumn->bIsAscending() ? "true" : "false");
    else
      sCopy.vSprint("      {%lld, \"%s\", %lld, %lld, %lld, %s, %s, %s},\n"_Stz,
                    pColumn->iColumnNo(),
                    pColumn->szColNm(),
                    pColumn->eColumnType(),
                    pColumn->iColumnLength(),
                    pColumn->iColumnScale(),
                    pColumn->bIsNullable() ? "true" : "false",
                    pColumn->bIsPtnKey() ? "true" : "false",
                    pColumn->bIsAscending() ? "true" : "false");
    StrLib::vAppendString(&m_azCodeCrs, sCopy.azChar());
  }

}
/*----------------------------------------------------------------------------*/
/* append code fragment for copying columns                                   */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeNullRightCols(OpSketch* pOpSketch, Stz* szFunction)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  i8 nLeftColumns = pOpSketch->m_pStreamDefLft->nColumns();
  i8 nColumns = pOpSketch->m_pStreamDefOut->nColumns();

  for (i8 i_Col = nLeftColumns; i_Col < nColumns; i_Col++) {
    m_pCommonCode->vSeekStart();
    Column *pColumn = pOpSketch->m_pStreamDefOut->pGetColumn(i_Col);
    vAppendCodeFragment(pOpSketch, szFunction, pColumn);
  }

}

/*----------------------------------------------------------------------------*/
/* append code fragment for key, payload columns to tub                       */
/*----------------------------------------------------------------------------*/
void CombinedSrc::vAppendCodeToTubCols(OpSketch* pOpSketch) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_COMBINEDSRC, _FOOTER_);

  s16KB sCode;

  bool bExtracted;

  IndexDef* pIndexDef = pOpSketch->m_pIndexDef;
  StreamDef *pStreamDefOut = pOpSketch->m_pStreamDefOut;

  s256 sColNm;
  i8 i_DupCtr = 1;
  i8 nColumns = pStreamDefOut->nColumns();

  RASSERT(nColumns == pOpSketch->m_pColsMap->m_nCols);

  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {

    i8 i_Cell = pOpSketch->m_pColsMap->m_axCols[i_Col].m_i_Cell;
    i8 i_EmittedCol = pOpSketch->m_pColsMap->m_axCols[i_Col].m_i_EmittedCol;
    Column *pColumn = pStreamDefOut->pGetColumn(i_EmittedCol-1);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // is column in the payload ?
    bool bIsPayload = pOpSketch->bIsPayload(i_Col);

    if (bIsPayload) {

      // make the column name
      //vMakeColumnNmStruct(pStreamDefOut, sColNm, i_DupCtr, i_Cell);

    switch (pColumn->eColumnType()) {

      case CT_VARCHAR:
      case CT_CHAR:
        vAppendCodeFragmentPL(pOpSketch, "vPayLoadToTubVchCol"_Stz, pColumn, i_Cell-1);
        break;

      default:
        vAppendCodeFragmentPL(pOpSketch, "vPayLoadToTubCol"_Stz, pColumn, i_Cell-1);
        break;
    }
  }

    else {
      sCode = ""_Stz;
      m_pCommonCode->vSeekStart();

      // get the index columns
      //Column* pIndexColumn = pIndexDef->pGetColumn(i_Col);

      // get the steamDef column No maching the indexColumn
      //i8 iStreamDefCol = pIndexDef->iGetStreamDefCol(pIndexColumn->iColumnNo());

      // make the column name
      //vMakeColumnNmStruct(pStreamDefOut, sColNm, i_DupCtr, iStreamDefCol);

      //Column *pColumn = pStreamDefOut->pGetColumn(iStreamDefCol);

      if (pIndexDef->eKeyKind() == KE_GENERAL) {
        switch (pColumn->eColumnType()) {

          case CT_VARCHAR:
          case CT_CHAR:
            vAppendCodeFragmentPL(pOpSketch, "vPLVchKeyToTub"_Stz, pColumn, i_Cell-1);
            break;

          default:
            vAppendCodeFragment(pOpSketch, "vPLIntKeyToTub"_Stz, pColumn);
            break;
        }
      }
      else {
        vAppendCodeFragment(pOpSketch, "vPayLoadKeyToTub"_Stz, pColumn);
      }
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
  m_pFunctionFooter->vClose();
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
  bOpened = m_pCommonCode->bOpenRead("../src/store/Fragments/CommonCode.cpp"_Stz);
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
