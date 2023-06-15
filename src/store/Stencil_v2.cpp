/*============================================================================*/
/* Stencil_v2.cpp -- Op Stencil_v2                                            */
/*============================================================================*/

#include "BASE.h"

#include "Stencil_v2.hpp"
#include "Stream.hpp"
#include "StreamDef.hpp"
#include "Op.hpp"

#include "LogicUnit.hpp"
#include "TagConstants.h"

#include "Grid.hpp"
#include "Node.hpp"
#include "Index.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "IndexTree.hpp"
#include "Stage.hpp"
#include "IndexStream.hpp"

#include "CodeSynthUtils.hpp"
#include "eSK.h"
#include "eII.h"
#include "PipelineDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Stencil_v2,CN_STENCIL_V2)
__aNewFunction(Stencil_v2,CN_STENCIL_V2)
__vDeleteFunction(Stencil_v2,CN_STENCIL_V2)
__aFooterFunction(Stencil_v2)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stencil_v2::Stencil_v2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  m_szStageNm    = ""_Stz;

  m_xStageOps.m_iAggregOp    = 0;
  m_xStageOps.m_iFilterOp    = 0;
  m_xStageOps.m_iHashOp      = 0;
  m_xStageOps.m_iIndexJoinOp = 0;
  m_xStageOps.m_iIndexScanOp = 0;
  m_xStageOps.m_iKeyExistsOp = 0;
  m_xStageOps.m_iLimitOp     = 0;
  m_xStageOps.m_iOrdAggOp    = 0;
  m_xStageOps.m_iProjectOp   = 0;
  m_xStageOps.m_iScanOp      = 0;
  m_xStageOps.m_iSortOp      = 0;
  m_xStageOps.m_iTopOp       = 0;

  m_xStageOps.m_bScan           = false;
  m_xStageOps.m_bIndexScan      = false;

  m_xStageOps.m_bPayloadOp      = false;


  m_xStageOps.m_pHavingFilterOp = nullptr;
  m_xStageOps.m_pLimitOp        = nullptr;

  m_xStageOps.m_sLabelOrdAgg = ""_Stz;
  m_xStageOps.m_sHashExpr = ""_Stz;

  m_pOpSource    = nullptr;
  m_sHashExpr    = ""_Stz;

  try {
    m_pOpSource    = new CombinedSrc;
  }
  catch (LowMemory xLowMemory) {
    throw xLowMemory;
  }

  _checkInitialised_(CN_STENCIL_V2);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stencil_v2::~Stencil_v2(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  delete m_pOpSource;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair and delete the stencil                                              */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vRepair(Object* pObject)
{
  _static_

  Stencil_v2* pStencil_v2 = _pStencil_v2(pObject);

  // repair the stencil
  // ...
  
  // delete the stencil object
  delete pStencil_v2;
}

/*----------------------------------------------------------------------------*/
/* synthesise given pipeline                                                  */
/*----------------------------------------------------------------------------*/
eEC Stencil_v2::eSynthesise(PipelineDef *pPipelineDef) {
  _static_

  eEC eErrorCode;

  i8 nStages = pPipelineDef->nItems();

  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    StageDef *pStageDef = pPipelineDef->pGet(i_Stage);
    pStageDef->vDump();
  }

  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stencil_v2* pStencil = new Stencil_v2;
    StageDef *pStageDef = pPipelineDef->pGet(i_Stage);
    eErrorCode = pStencil->eSynthesise(pStageDef);
    if (eErrorCode != EC_NONE) {
      return eErrorCode;
    }

    // synthesise the code for the stage
    pStencil->vSynthesise(i_Stage);

    // add the synthesised code to the stageDef
    pStageDef->vCode(pStencil->szOpSource());

    delete pStencil;
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* synthesise given pipeline                                                  */
/*----------------------------------------------------------------------------*/
eEC Stencil_v2::eSynthesise(StageDef *pStageDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  s4K sLabelLoop;

  bool bInflateGenerated = false;
  bool bInflateCrw = false;
  bool bGeneratePotTub = true;
  bool bGeneratePot = false;

  s256 sHashExpr = ""_Stz;

  OpSketch *pOpSketch;

  OpDef* pOpDefScan = nullptr;

  eSR ePrevStencil_v2 = SR_NONE;
  eHK eHandlerKind = HK_NONE;

  sLabelLoop = ""_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect the validate information of the stage
  eEC eError = eValidateStage(pStageDef);
  if (eError != EC_NONE)
    return eError;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the stage name
  m_szStageNm = pStageDef->szStageNm();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // look for hashOP
  for (i8 i_Op = 0; i_Op < pStageDef->nItems(); i_Op++) {

    // get the opDef
    OpDef *pOpDef = pStageDef->pGet(i_Op);

    if (pOpDef->szOpNm()->bSame("hash"_Stz)) {

      // generate pot
      bGeneratePot = true;

      // get hash expr from OpDef
      m_sHashExpr = pOpDef->pArgDef_expr()->szArgVal();
    }
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each operator in the stageDef
  for (i8 i_Op = 0; i_Op < pStageDef->nItems(); i_Op++) {

    // get the opDef
    OpDef *pOpDef = pStageDef->pGet(i_Op);

    // skip scan OP for now (sketch later)
    if (pOpDef->szOpNm()->bSame("scan"_Stz)) {
      bInflateCrw = true;
      pOpDefScan = pOpDef;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflateCrw before handling the current operator
    if (bInflateCrw) {

      RASSERT(!m_xStageOps.m_bIndexScan);

      eHandlerKind = HK_CRWBUFFER;
      sLabelLoop = "L_AdvanceNextCrwT"_Stz;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchInflateCrw(pStageDef, pOpDefScan, eHandlerKind, sLabelLoop);
      vAppend(pOpSketch);

      bInflateCrw = false;
      bInflateGenerated = true;
      ePrevStencil_v2 = SR_IT;
    }

    // generate code for indexScan OP
    if (pOpDef->szOpNm()->bSame("indexScan"_Stz)) {

      // either scan / indexScan should be there
      RASSERT(!bInflateCrw);

      eHandlerKind = HK_INDEXBUFFER;
      sLabelLoop = "L_AdvanceNextIndexT"_Stz;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchIndexScan(pStageDef, pOpDef, eHandlerKind, sLabelLoop);
      vAppend(pOpSketch);

      bInflateGenerated = true;
      ePrevStencil_v2 = SR_IT;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflateRow before handling the current operator
    if (!bInflateGenerated) {

      RASSERT(!m_xStageOps.m_bIndexScan);

      eHandlerKind = HK_ROWBUFFER;
      sLabelLoop = "L_AdvanceNextRowT"_Stz;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchInflateRow(pStageDef, pOpDef, eHandlerKind, sLabelLoop);
      vAppend(pOpSketch);

      bInflateGenerated = true;
      ePrevStencil_v2 = SR_IT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOP
    if (pOpDef->szOpNm()->bSame("aggreg"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchAggreg(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      ePrevStencil_v2 = SR_AT;

      OpSketch *pAggOpSketch = pOpSketch;

      pOpSketch = pSketchEos(pStageDef, HK_EOS, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchPotTub(pAggOpSketch, HK_EOS, ePrevStencil_v2, sLabelLoop, bGeneratePot, sHashExpr);
      vAppend(pOpSketch);

      bGeneratePotTub = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOpDef->szOpNm()->bSame("filter"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchFilter(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOp
    if (pOpDef->szOpNm()->bSame("gbyAgg"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchGbyAgg(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      ePrevStencil_v2 = SR_PL;

      OpSketch *pGbyAggOpSketch = pOpSketch;

      pOpSketch = pSketchEos(pStageDef, HK_EOS, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchPotTub(pGbyAggOpSketch, HK_EOS, ePrevStencil_v2, sLabelLoop, bGeneratePot, sHashExpr);
      pOpSketch->vIndexDef(pGbyAggOpSketch->m_pIndexDef);
      pOpSketch->vCopyColsMap(pGbyAggOpSketch->m_pColsMap);
      vAppend(pOpSketch);

      bGeneratePotTub = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the hashOp
    if (pOpDef->szOpNm()->bSame("hash"_Stz)) {

      // already handled

      continue;
    }

    // generate code for indexScan OP
    if (pOpDef->szOpNm()->bSame("indexJoin"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchIndexJoin(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      sLabelLoop = "L_AdvanceNextRowJ"_Stz;

      ePrevStencil_v2 = SR_TJ;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the limitOP
    if (pOpDef->szOpNm()->bSame("limit"_Stz)) {

      //pCombined->aCombinedT()->m_iLimitRows = 1;

      // limit after a payload will move to EOSHandler
      if (m_xStageOps.m_bPayloadOp) {
        continue;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchLimit(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the projectOP
    if (pOpDef->szOpNm()->bSame("project"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchProject(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      ePrevStencil_v2 = SR_PT;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the semiJoinOP
    if (pOpDef->szOpNm()->bSame("keyExists"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchSemiJoin(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the antiJoinOP
    if (pOpDef->szOpNm()->bSame("keyNotExists"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchAntiJoin(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the sort OP
    if (pOpDef->szOpNm()->bSame("sort"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchSort(pStageDef, pOpDef, eHandlerKind, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      ePrevStencil_v2 = SR_PL;

      OpSketch *pSortOpSketch = pOpSketch;

      pOpSketch = pSketchEos(pStageDef, HK_EOS, ePrevStencil_v2, sLabelLoop);
      vAppend(pOpSketch);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchPotTub(pSortOpSketch, HK_EOS, ePrevStencil_v2, sLabelLoop,bGeneratePot, sHashExpr);
      pOpSketch->vIndexDef(pSortOpSketch->m_pIndexDef);
      pOpSketch->vCopyColsMap(pSortOpSketch->m_pColsMap);
      vAppend(pOpSketch);

      bGeneratePotTub = false;
      continue;
    }

  }

  OpSketch* pLastOpSketch = pOpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the POT/TUB if not already generated
  if ( bGeneratePotTub ) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // sketch pot/tub
    pOpSketch = pSketchPotTub(pLastOpSketch, eHandlerKind, ePrevStencil_v2, sLabelLoop, bGeneratePot, sHashExpr);
    vAppend(pOpSketch);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // sketch EOS handler function
    pOpSketch = pSketchEos(pStageDef, HK_EOS, ePrevStencil_v2, sLabelLoop);
    vAppend(pOpSketch);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sketch StreamDef function
  pOpSketch = pSketchStreamDef(pLastOpSketch, HK_STREAMDEF);
  vAppend(pOpSketch);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  vTrace("OpSketch's generated for stage %s to be synthesised: \n"_Stz, pStageDef->szStageNm());

  for (i8 i_OpSketch = 0; i_OpSketch < nItems(); i_OpSketch++) {
    OpSketch* pOpSketch = pGet(i_OpSketch);
    vTrace("  %s\n"_Stz, aSK[pOpSketch->m_eSketchKind]);
  }

  return EC_NONE;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* synthesizecode for all operators for given stage                           */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSynthesise(i8 i_Stage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  bool bPrependHeaders = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // synthesize the code for the combined operators
  if ( i_Stage == 0)
    bPrependHeaders = true;

    m_pOpSource->vSynthesizeCode(this, bPrependHeaders);
}

/*----------------------------------------------------------------------------*/
/* return op source code                                                      */
/*----------------------------------------------------------------------------*/
Stz* Stencil_v2::szOpSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);
  
  char* azCode = m_pOpSource->azCode();
  
  return (Stz*)azCode;
}

/*----------------------------------------------------------------------------*/
/* return op stage name                                                       */
/*----------------------------------------------------------------------------*/
Stz* Stencil_v2::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL_V2,_FOOTER_);

  return (Stz*)m_szStageNm;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vGetNextLabel(Stz* sLabelNext, StageDef* pStageDef, i8 i_Op, Stz* sDefault) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  i8 iNextOp = i_Op + 1;

  if (iNextOp >= pStageDef->nItems()) {
    sLabelNext->vCopy(sDefault);
  }
  else {
    OpDef *pOpNext = pStageDef->pGet(iNextOp);

    if (pOpNext->szOpNm()->bSame("dynamic_v2"_Stz) || pOpNext->szOpNm()->bSame("hash"_Stz)) {
      // if nextOp is combined/hash, then next label is sDefault
      sLabelNext->vCopy(sDefault);
    }
    else {
      // form next label using next op
      sLabelNext->vSprint("L_%s_%d"_Stz, pOpNext->szOpNm(), iNextOp);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* set up for ordered aggregate                                               */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSetupOrdAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  bool bPayload = true;

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // create RowSingleAgg
//  pDynamic->aDynamicT()->m_xRowInfoAT.m_aRowSingle = new RowSingle(pStreamDefOut);
//  pDynamic->aDynamicT()->m_xRowInfoAT.m_aRowCellArray =
//      (byte*)RowSingle::aGetCellArray((RowSingle*)pDynamic->aDynamicT()->m_xRowInfoAT.m_aRowSingle,
//                                      &pDynamic->aDynamicT()->m_xRowInfoAT.m_acStrArea);
//  pDynamic->aDynamicT()->m_xRowInfoAT.m_acStrAreaSav = pDynamic->aDynamicT()->m_xRowInfoAT.m_acStrArea;

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);

  // do we have a payload ?
  if (pStreamDefOut->nColumns() == pIndexDef->nColumns())
    bPayload = false;

  // create the groupby index
  eKE eKind;
  pDynamic->aDynamicT()->m_xIndexInfoPL.m_aIndexDef = new IndexDef(pIndexDef);
  pDynamic->aDynamicT()->m_xIndexInfoPL.m_aIndexTree = IndexTree::aNewIndexTree(pIndexDef, &eKind);
  EntWrk::vEntKind((EntWrk*)pDynamic->aDynamicT()->m_xIndexInfoPL.m_aEntWrk, bPayload, pIndexDef->eKeyKind());

  // delete the indexdef in the LU of OP
  // not the correct place... will come back to it
  delete *pLogicUnit->m_xpIndexDef;
  pLogicUnit->m_xpIndexDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set up for sort                                                            */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSetupSort(Dynamic* pDynamic, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  eKE eKind;
  bool bPayload = true;

  // get the indexDef
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  IndexDef *pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // do we have a payload ?
  if (pStreamDefOut->nColumns() == pIndexDef->nColumns())
    bPayload = false;

  pDynamic->aDynamicT()->m_xIndexInfoPL.m_aIndexDef = new IndexDef(pIndexDef);
  pDynamic->aDynamicT()->m_xIndexInfoPL.m_aIndexTree = IndexTree::aNewIndexTree(pIndexDef, &eKind);
  EntWrk::vEntKind((EntWrk *) pDynamic->aDynamicT()->m_xIndexInfoPL.m_aEntWrk, bPayload, pIndexDef->eKeyKind());
  EntWrk::vEntDesc((EntWrk *) pDynamic->aDynamicT()->m_xIndexInfoPL.m_aEntWrk,pIndexDef);

  // delete the indexdef in the LU of OP
  // not the correct place... will come back to it
  delete *pLogicUnit->m_xpIndexDef;
  pLogicUnit->m_xpIndexDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* set up for top                                                             */
/*----------------------------------------------------------------------------*/
void Stencil_v2::vSetupTop(Dynamic* pDynamic, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  bool bPayload = true;

  // get the LogicUnit
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();

  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the IndexDef from the TopOp
  IndexDef* pIndexDef = _pIndexDef(*pLogicUnit->m_xpIndexDef);
  pDynamic->aDynamicT()->m_xIndexInfoPL.m_aIndexDef = new IndexDef(pIndexDef);

  // do we have a payload ?
  if (pStreamDefOut->nColumns() == pIndexDef->nColumns())
    bPayload = false;

  // create the indexTree
  eKE eKind;
  pDynamic->aDynamicT()->m_xIndexInfoPL.m_aIndexTree = IndexTree::aNewIndexTree(pIndexDef, &eKind);
  EntWrk::vEntKind((EntWrk *) pDynamic->aDynamicT()->m_xIndexInfoPL.m_aEntWrk, bPayload, pIndexDef->eKeyKind());

  // delete the indexdef in the LU of OP
  // not the correct place... will come back to it
  delete *pLogicUnit->m_xpIndexDef;
  pLogicUnit->m_xpIndexDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for sort OP                                       */
/*----------------------------------------------------------------------------*/
/*
void Stencil_v2::vSketchEosSort(Dynamic* pDynamic, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  CodeSynth_v2 *pCodeSynth;
  s256 szStageNm;
  s256 sLabelNext;
  s256 sLabel;
  i8   i_Op;
  eSR ePrevStencil_v2 = SR_NONE;

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(pDynamic->aDynamicT()->m_xIndexInfoPL.m_aIndexDef);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append code for EOSMARKER handling
  //vAppendFuncHeader3();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth_v2(pStreamDefOut,SR_PL);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth_v2(nullptr, pStreamDefOut, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(pIndexDef, nullptr, sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  Stz* sProjectExpr = ""_Stz;

  ePrevStencil_v2 = SR_PL;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payloadRow to Tub/Pot
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, ePrevStencil_v2, SR_NONE);

  if (m_xStageOps.m_pLimitOp) {
    // get limit from limitOp
    TagSet* pOpTags = m_xStageOps.m_pLimitOp->pOpTags();
    i8 iLimit = StrLib::iToI8((char*)(pOpTags->szValue(C_TAG_COUNT)));

    sLabel.vSprint("L_%s_%d"_Stz, m_xStageOps.m_pLimitOp->szOpNm(), m_xStageOps.m_pLimitOp->i_Op());
    vGetNextLabel(sLabelNext, pStream, m_xStageOps.m_pLimitOp->i_Op(), "L_AddRowToTubPot"_Stz);
    pCodeSynth->vSynthLimit(iLimit, sLabel, sLabelNext, "L_ReturnFinal"_Stz);
  }

  if (pDynamic->aDynamicT()->m_iPot)
   pCodeSynth->vSynthAllocPotRow(nullptr, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(nullptr, false, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz,""_Stz);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}
*/

/*----------------------------------------------------------------------------*/
/* sketch code for Eos fun3 for top OP                                        */
/*----------------------------------------------------------------------------*/
/*
void Stencil_v2::vSketchEosTop(Dynamic* pDynamic, Stream* pStream, Op* pOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s256 sLabelNext;
  CodeSynth_v2 *pCodeSynth;

  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);

  // get the IndexDef from the TopOp
  IndexDef *pIndexDef = _pIndexDef(pDynamic->aDynamicT()->m_aIndexDefPL);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append function header
  //vAppendFuncHeader3();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, SR_PL);
  pCodeSynth->vSynthStruct(0);
  m_pShapeEOS->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payload to buffer
  pCodeSynth = new CodeSynth_v2(nullptr, pStreamDefLft, nullptr, pPreExpStack, SR_NONE, SR_PL);

  vGetNextLabel(sLabelNext, pStream, pOp->i_Op(), "L_AddRowToTubPot"_Stz);

  pCodeSynth->vSynthEos(pIndexDef, nullptr, sLabelNext);
  m_pShapeEOS->vCodeSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // payloadRow to Tub/Pot
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, pStreamDefOut, nullptr, pPreExpStack, SR_PL, SR_NONE);

  if (pDynamic->aDynamicT()->m_iPot)
    pCodeSynth->vSynthAllocPotRow(nullptr, m_xStageOps.m_sHashExpr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);
  else
    pCodeSynth->vSynthAllocTubRow(nullptr, true, "L_AddRowToTubPot"_Stz, "L_AdvanceNextRow"_Stz, ""_Stz);

  m_pShapeEOS->vCodeSynthB(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append concluding code
  Fragments::vAppendConcludingFunc3(m_pConcludingEos->azCode());
}
*/

/*----------------------------------------------------------------------------*/
/* sketch code for indexJoin                                                  */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchIndexJoin(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch *pOpSketch;

  s256 sLabel;
  s256 sLabelNext;
  s256 sLabelOuterLoop;

  i8 i_Op = pOpDef->i_Op();

  // get join expr from OpDef
  Stz* sJoinExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;
  StreamDef *pStreamDefRht = nullptr; //pOpDef->m_pStreamDef_joinTable;

  // get the index
  Index* pIndex = pOpDef->m_pIndex_indexJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the join
  pOpSketch = new OpSketch(i_Op, sJoinExpr, eHandlerKind, SK_INDEXJOIN, ePrevStencil_v2, SR_TJ, II_JJ, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  sLabelOuterLoop = sLabelLoop;
  pOpSketch->vSetLabels(sLabel, sLabelNext, "L_AdvanceNextRowJ"_Stz, sLabelOuterLoop);

  // HACK-OPTIMISER
  // set the streamDefRht
  Table* pTable = pIndex->pTable();
  pOpSketch->vStreamDefRht(pTable->pStreamDef());

  // set the indexDef
  pOpSketch->vTable(pTable);
  pOpSketch->vIndex(pIndex);
  pOpSketch->vIndexDef(pIndex->pIndexDef());
  pOpSketch->vIndexPositionLower(IP_GE);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sJoinExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchIndexScan(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch* pOpSketch;

  s256 sLabel;
  s256 sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get indexScan expr from OpDef
  Stz* sIndexScanExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_emitted;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // get the index
  Index* pIndex = pOpDef->m_pIndex_indexScan;

  // create a new opSketch for this OP
  pOpSketch = new OpSketch(i_Op,sIndexScanExpr, eHandlerKind, SK_INDEXSCAN, SR_NONE, SR_IT, II_ST, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(),i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // set the table
  pOpSketch->vTable(pIndex->pTable());

  // set the index
  pOpSketch->vIndex(pIndex);

  // set the indexDef
  pOpSketch->vIndexDef(pIndex->pIndexDef());

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sIndexScanExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch inflate compact rows of crwBuffer)                                  */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchInflateCrw(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch* pOpSketch;

  s256 sLabel;
  s256 sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_emitted;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // get the columns required
  ColsReq* pColsReq = pOpDef->m_pColsReq;

  // create a new opSketch for this OP
  pOpSketch = new OpSketch(i_Op, ""_Stz, eHandlerKind, SK_INFLATE_CRW, SR_NONE, SR_IT, II_NONE, pStreamDefLft, pStreamDefOut);

  // make the labels
  sLabel.vSprint("L_InflateCrwT"_Stz);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // set the colsRequired
  pOpSketch->vCopyColsReq(pColsReq);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch inflate of row from rowBuffer                                       */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchInflateRow(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch *pOpSketch;

  s256 sLabel;
  s256 sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get the streamDefOut
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;

  // create a new opSketch for this OP
  pOpSketch = new OpSketch(i_Op, ""_Stz, eHandlerKind, SK_INFLATE_ROW, SR_NONE, SR_IT, II_NONE, pStreamDefLft, pStreamDefLft);

  // make the labels
  sLabel.vSprint("L_InflateRowT"_Stz);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch filter                                                              */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchFilter(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get filter expr from OpDef
  Stz* sFilterExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefOut
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(i_Op, sFilterExpr, eHandlerKind, SK_FILTER, ePrevStencil_v2, SR_NONE, II_NONE, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sFilterExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch eos                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchEos(StageDef* pStageDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  // create a new opSketch
  OpSketch* pOpSketch = new OpSketch(C_UNKNOWN, ""_Stz, eHandlerKind, SK_EOS, ePrevStencil_v2, SR_NONE, II_NONE, nullptr, nullptr);

  sLabel = ""_Stz;
  sLabelNext = ""_Stz;

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch semiJoin                                                            */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchSemiJoin(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch *pOpSketch;

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelOuterLoop;

  i8 i_Op = pOpDef->i_Op();

  // get join expr from OP
  Stz* sJoinExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get join constraint from OP
  Stz* sConstraintExpr = pOpDef->pArgDef_constraint()->szArgVal();

  // get the streamDefLft
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;

  //HACK-OPTIMISER
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_arriving; // pOpDef->m_pStreamDef_emitted;

  //HACK-OPTIMISER
  StreamDef *pStreamDefRht = nullptr; //pOpDef->m_pStreamDef_joinTable;

  // get the index
  Index *pIndex = pOpDef->m_pIndex_indexSemi;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the semi-join
  pOpSketch = new OpSketch(i_Op, sJoinExpr, eHandlerKind, SK_SEMIJOIN, ePrevStencil_v2, SR_IJ, II_JJ, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  //sLabelOuterLoop.vSprint("L_%s_END_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  //HACK-OPTIMISER
  // set the streamDefRht
  // pOpSketch->vStreamDefRht(pStreamDefRht);
  Table* pTable = pIndex->pTable();
  pOpSketch->vStreamDefRht(pTable->pStreamDef());

  // set the indexDef
  pOpSketch->vTable(pTable);
  pOpSketch->vIndex(pIndex);
  pOpSketch->vIndexDef(pIndex->pIndexDef());
  pOpSketch->vIndexPositionLower(IP_GE);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sJoinExpr);
  pOpSketch->vGenerateExprStacks(sConstraintExpr, true /* constraint */);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch limit                                                               */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchLimit(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get limit from limitOp
  Stz* sLimitExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  RASSERT(pStreamDefLft);

  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_arriving;
  RASSERT(pStreamDefOut);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(i_Op, sLimitExpr, eHandlerKind, SK_LIMIT, ePrevStencil_v2, SR_NONE, II_NONE, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch ordered aggregate                                                   */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchOrdAgg(Dynamic* pDynamic, Stream* pStream, Op* pOp,
                              eSR ePrevStencil_v2, Stz* sLabelLoop) {
/*
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;

  // get filter expr from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sProjectExpr = pOpTags->szValue(C_TAG_EXPR);

  // get the streamDefs
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefOut = _pStreamDef(*pLogicUnit->m_xpStreamDefOut);
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the indexDef
  IndexDef *pIndexDef = _pIndexDef(pDynamic->aDynamicT()->m_aIndexDefPL);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // create a new shape for this OP
  OpSketch *pShape = new OpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the streamDef (aggregate)
  pCodeSynth = new CodeSynth_v2(pStreamDefOut, SR_AT);
  pCodeSynth->vSynthStruct(0);
  pShape->vStructSynthA(pCodeSynth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the row projection
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefOut, pIndexDef, pPreExpStack, ePrevStencil_v2, SR_AT);

  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStream, i_Op, "L_AddRowToTubPot"_Stz);
  pCodeSynth->vSynthOrdAgg( sProjectExpr,
                            sLabel,
                            sLabelNext,
                            sLabelLoop);
  pShape->vCodeSynthA(pCodeSynth);

  //m_pPostStack = pCodeSynth->pPostStack();
  //pCodeSynth->vPostStack(nullptr);

  return pShape;
  */
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* sketch project expressions                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchAggreg(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s256 szStageNm;

  i8 i_Op = pOpDef->i_Op();

  // get aggregate expr from OpDef
  Stz* sAggregExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(i_Op, sAggregExpr, eHandlerKind, SK_AGGREG, ePrevStencil_v2, SR_AT, II_NONE, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sAggregExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch antiJoin                                                            */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchAntiJoin(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  OpSketch *pOpSketch;

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get join expr from OP
  Stz* sJoinExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get join constraint from OP
  Stz* sConstraintExpr = pOpDef->pArgDef_constraint()->szArgVal();

  // get semi-join expr from OP
  Stz* sFilterExpr = pOpDef->pArgDef_expr()->szArgVal(); // ????????

  // get the streamDefLft
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;

  //HACK-OPTIMISER
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_arriving; // pOpDef->m_pStreamDef_emitted;

  //HACK-OPTIMISER
  StreamDef *pStreamDefRht = nullptr; //pOpDef->m_pStreamDef_joinTable;

  // get the index
  Index *pIndex = pOpDef->m_pIndex_indexSemi;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the semi-join
  pOpSketch = new OpSketch(i_Op, sJoinExpr, eHandlerKind, SK_ANTIJOIN, ePrevStencil_v2, SR_IJ, II_JJ, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  //sLabelEnd.vSprint("L_%s_END_%d"_Stz, pOp->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AdvanceNextRow"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, "L_AdvanceNextRowJ"_Stz, sLabelLoop);

  //HACK-OPTIMISER
  // set the streamDefRht
  // pOpSketch->vStreamDefRht(pStreamDefRht);
  Table* pTable = pIndex->pTable();
  pOpSketch->vStreamDefRht(pTable->pStreamDef());

  // set the indexDef
  pOpSketch->vTable(pTable);
  pOpSketch->vIndex(pIndex);
  pOpSketch->vIndexDef(pIndex->pIndexDef());
  pOpSketch->vIndexPositionLower(IP_GE);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sJoinExpr);
  pOpSketch->vGenerateExprStacks(sConstraintExpr, true /* constraint */);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch project expressions                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchProject(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* sLabelLoop)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get project expr from OpDef
  Stz* sProjectExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(i_Op, sProjectExpr, eHandlerKind, SK_PROJECT, ePrevStencil_v2, SR_PT, II_NONE, pStreamDefLft, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // generate the expressißon stacks
  pOpSketch->vGenerateExprStacks(sProjectExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch project group by expressions                                        */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchGbyAgg(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind,
                                    eSR ePrevStencil_v2, Stz* sLabelLoop)
{

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s256 szStageNm;

  i8 i_Op = pOpDef->i_Op();

  // get the grouby aggregate expr from OpDef
  Stz* sGbyAggExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the columns required
  ColsReq* pColsMap = pOpDef->m_pColMap_gbyAgg;

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(i_Op, sGbyAggExpr, eHandlerKind, SK_GBYAGG, ePrevStencil_v2, SR_AT, II_PL, pStreamDefLft, pStreamDefOut);

  // set the labels in the opSketch
  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  // set the indexDef
  IndexDef *pIndexDef = pOpDef->m_pIndexDef_gbyAgg; //_pIndexDef(pDynamic->aDynamicT()->m_xIndexInfoPL.m_aIndexDef);
  pOpSketch->vIndexDef(pIndexDef);

  // set the payLoad bytes
  i8 nPayloadBytes = (pStreamDefOut->nColumns() - pIndexDef->nColumns()) * 8;
  pOpSketch->vPayloadBytes(nPayloadBytes);

  // set the colsMap (payload information)
  pOpSketch->vCopyColsMap(pColsMap);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sGbyAggExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch sort                                                                */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchSort(StageDef* pStageDef, OpDef* pOpDef, eHK eHandlerKind, eSR ePrevStencil_v2, Stz* szLabelLoop)
{

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;

  i8 i_Op = pOpDef->i_Op();

  // get sort expr from OpDef
  Stz* sSortExpr = pOpDef->pArgDef_expr()->szArgVal();

  //HACK-OPTIMISER
  // get the columns required
  ColsReq* pColsMap = nullptr; //pOpDef->m_pColMap_sort;

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(i_Op, sSortExpr, eHandlerKind, SK_SORT, ePrevStencil_v2, ePrevStencil_v2,  II_PL, pStreamDefLft, pStreamDefOut);

  // set the labels in the opSketch
  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);
  pOpSketch->vSetLabels(sLabel, ""_Stz, szLabelLoop);

  // set the indexDef
  IndexDef *pIndexDef = _pIndexDef(pOpDef->m_pIndexDef_sort);
  pOpSketch->vIndexDef(pIndexDef);

  // set the payLoad bytes
  i8 nPayloadBytes = (pStreamDefOut->nColumns() - pIndexDef->nColumns()) * 8;
  pOpSketch->vPayloadBytes(nPayloadBytes);

  // set the colsMap (payload information)
  pOpSketch->vCopyColsMap(pColsMap);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sSortExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch streamDef                                                           */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchStreamDef(OpSketch* pLastOp, eHK eHandlerKind) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  // get the streamDefOut
  StreamDef *pStreamDefOut = pLastOp->m_pStreamDefOut;
  RASSERT(pStreamDefOut);

  // create a new opSketch
  OpSketch* pOpSketch = new OpSketch(C_UNKNOWN, ""_Stz, eHandlerKind, SK_STREAMDEF, SR_NONE,  SR_NONE, II_NONE, pStreamDefOut, pStreamDefOut);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch top                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchTop(Dynamic* pDynamic, Op* pOp, eSR ePrevStencil_v2, Stz* sLabelLoop) {
/*
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOp->i_Op();
  CodeSynth_v2 *pCodeSynth;

  // create a new shape for this OP
  OpSketch *pShape = new OpSketch;

  // get opTags from OP
  TagSet* pOpTags = pOp->pOpTags();
  Stz* sKeyExpr = pOpTags->szValue(C_TAG_EXPR);

  i8 iTopCount = StrLib::iToI8((char*)(pOpTags->szValue(C_TAG_COUNT)));

  // get the streamDefOut
  LogicUnit *pLogicUnit = pOp->pOpLogicUnit();
  StreamDef *pStreamDefLft = _pStreamDef(*pLogicUnit->m_xpStreamDefLft);

  // get the IndexDef from the TopOp
  IndexDef* pIndexDef = _pIndexDef(pDynamic->aDynamicT()->m_aIndexDefPL);

  // get the aggregate type stack
  IntStack* pPreExpStack = _pIntStack(*pLogicUnit->m_xpPreExpStack);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the OP
  pCodeSynth = new CodeSynth_v2(pStreamDefLft, pStreamDefLft, pIndexDef, nullptr, ePrevStencil_v2, SR_NONE);
  sLabel.vSprint("L_%s_%d"_Stz, pOp->szOpNm(), i_Op);

  pCodeSynth->vSynthTop(sKeyExpr, iTopCount, sLabel, sLabelLoop);
  pShape->vCodeSynthB(pCodeSynth);

  return pShape;
  */
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* sketch tub                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil_v2::pSketchPotTub(OpSketch* pLastOp, eHK eHandlerKind, eSR ePrevStencil_v2,
                                 Stz* sLabelLoop, bool bGeneratePot, Stz* sHashExpr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  OpSketch* pOpSketch;

  // get the streamDefOut
  StreamDef *pStreamDefOut = pLastOp->m_pStreamDefOut;
  RASSERT(pStreamDefOut);

  // create a new opSketch
  if (bGeneratePot) {
    pOpSketch = new OpSketch(C_UNKNOWN, sHashExpr, eHandlerKind, SK_POT, ePrevStencil_v2, SR_PO,  II_PL, pStreamDefOut, pStreamDefOut);

    // generate the expression stack
    pOpSketch->vGenerateExprStacks(m_sHashExpr);
  }
  else {
    pOpSketch = new OpSketch(C_UNKNOWN, ""_Stz, eHandlerKind, SK_TUB, ePrevStencil_v2, SR_TO,  II_PL, pStreamDefOut, pStreamDefOut);
  }

  // set the labels in the opSketch
  sLabel = "L_AddRowToTubPot"_Stz;
  sLabelNext = ""_Stz;
  pOpSketch->vSetLabels(sLabel, sLabelNext, sLabelLoop);

  if (pLastOp->m_pInitStack)
    pOpSketch->vCopyInitStack(pLastOp->m_pInitStack);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* collect and validate the stage information of stage being synthesised      */
/*----------------------------------------------------------------------------*/
eEC Stencil_v2::eValidateStage(StageDef* pStageDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL_V2, _FOOTER_);

  i8 iOpsToCombine = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iterate the opDef's
  for (i8 i_Op = 0; i_Op < pStageDef->nItems(); i_Op++) {

    // get the op
    OpDef *pOpDef = pStageDef->pGet(i_Op);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // scanOp
    if (pOpDef->szOpNm()->bSame("scan"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // Should be the first OP after consumeOp
      if (i_Op != 0) {
        vTrace("Error: scanOP should be the first OP in the stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE SCAN-OP's
      if (m_xStageOps.m_iScanOp) {
        vTrace("Error: Cannot have multiple scan operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bScan = true;
      m_xStageOps.m_iScanOp++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // skip scanOPand count no of OPS to combine
    iOpsToCombine++;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // groupBy aggregate Op
    if (pOpDef->szOpNm()->bSame("gbyAgg"_Stz)) {

      m_xStageOps.m_iAggregOp++;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PAYLOAD OP's
      if (m_xStageOps.m_bPayloadOp) {
        vTrace("Error: Cannot have multiple combination of payload operators (gbyAgg/sort/top)"
               "in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bPayloadOp = true;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // filterOp
    if (pOpDef->szOpNm()->bSame("filter"_Stz)) {
      if (m_xStageOps.m_bPayloadOp)
        //m_xStageOps.m_pHavingFilterOp = pOp;
      m_xStageOps.m_iFilterOp++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // hashOp
    if (pOpDef->szOpNm()->bSame("hash"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE HASH OP's
      if (m_xStageOps.m_iHashOp) {
        vTrace("Error: Cannot have multiple hashOp's (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // Should be the last OP before CombinedOp
//      if (i_Op != pStageDef->nItems() - 3) {
//        vTrace("Error: HashOP should be the last OP before combinedOP (%s)!!!\n"_Stz,
//               m_szStageNm.azChar());
//        return EC_INVALID_PIPELINE;
//      }

      m_xStageOps.m_iHashOp++;

      // set hashRows in combined (needed at runtime)
      //pDynamic->aDynamicT()->m_iPot = 1;

      // get hash expr from OP
      //TagSet *pOpTags = pOp->pOpTags();
      //m_xStageOps.m_sHashExpr = pOpTags->szValue(C_TAG_EXPR);
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // indexJoinOp
    if (pOpDef->szOpNm()->bSame("indexJoin"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE INDEXJOIN OP's
      if (m_xStageOps.m_iIndexJoinOp) {
        vTrace("Error: Cannot have multiple indexJoinOP's (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // CANNOT HAVE INDEXJOIN OP wITH A KEYEXISTS OP
      if (m_xStageOps.m_iKeyExistsOp) {
        vTrace("Error: Cannot have indexJoinOP with an keyExistsOP (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iIndexJoinOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // indexScanOp
    if (pOpDef->szOpNm()->bSame("indexScan"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // Should be the first OP after consumeOp
      if (i_Op != 0) {
        vTrace("Error: indexScanOP should be the first OP in the stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE INDEXSCAN-OP's
      if (m_xStageOps.m_iIndexScanOp) {
        vTrace("Error: Cannot have multiple indexScan operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // set indexScan in combined (needed at runtime)
      //pDynamic->aDynamicT()->m_iIndexScan = 1;

      m_xStageOps.m_bIndexScan = true;
      m_xStageOps.m_iIndexScanOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // keyExistsOp
    if (pOpDef->szOpNm()->bSame("keyExists"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE KEYEXISTS-OP's
      if (m_xStageOps.m_iKeyExistsOp) {
        vTrace("Error: Cannot have multiple keyExists/keyNotExists operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // CANNOT HAVE KEYEXISTS WITH AN INDEXJOIN OP
      if (m_xStageOps.m_iIndexJoinOp) {
        vTrace("Error: Cannot have keyExists with an indexJoinOP (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iKeyExistsOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // keyNotExistsOp
    if (pOpDef->szOpNm()->bSame("antijoin"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE LIMIT-OP's
      if (m_xStageOps.m_iKeyExistsOp) {
        vTrace("Error: Cannot have multiple keyExists/keyNotExists operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iKeyExistsOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // limitOp
    if (pOpDef->szOpNm()->bSame("limit"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE LIMIT-OP's
      if (m_xStageOps.m_iLimitOp) {
        vTrace("Error: Cannot have multiple limit operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      //m_xStageOps.m_pLimitOp = pOp;
      m_xStageOps.m_iLimitOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // ordAggOp
    if (pOpDef->szOpNm()->bSame("ordAgg"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE LIMIT-OP's
      if (m_xStageOps.m_iOrdAggOp) {
        vTrace("Error: Cannot have multiple ordAgg operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }


      m_xStageOps.m_sLabelOrdAgg.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), pOpDef->i_Op());

      m_xStageOps.m_iOrdAggOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // projectOp
    if (pOpDef->szOpNm()->bSame("project"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PROJECT-OP's
      if (m_xStageOps.m_iProjectOp) {
        vTrace("Error: Cannot have multiple project operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // PROJECT-OP after AGGREG-OP
      if (m_xStageOps.m_iAggregOp) {
        vTrace("Error: Cannot have project operator after an aggregate operator in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_iProjectOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // aggreg
    if (pOpDef->szOpNm()->bSame("aggreg"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PAYLOAD OP's
      if (m_xStageOps.m_bPayloadOp) {
        vTrace("Error: Cannot have multiple combination of payload operators (aggreg/gbyAgg/sort/top) in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bPayloadOp = true;

      m_xStageOps.m_iAggregOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // sortOp
    if (pOpDef->szOpNm()->bSame("sort"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE SORT-OP's
      if (m_xStageOps.m_iSortOp) {
        vTrace("Error: Cannot have multiple sort operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PAYLOAD OP's
      if (m_xStageOps.m_bPayloadOp) {
        vTrace("Error: Cannot have multiple combination of payload operators (aggregate/sort/top) in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bPayloadOp = true;
      m_xStageOps.m_iSortOp++;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // topOp
    if (pOpDef->szOpNm()->bSame("top"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE TOP-OP's
      if (m_xStageOps.m_iTopOp) {
        vTrace("Error: Cannot have multiple top operators in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());
        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // TOP with ORDAGG
      if (m_xStageOps.m_iOrdAggOp) {
        vTrace("Error: Cannot have top with OrdAggOP the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MULTIPLE PAYLOAD OP's
      if (m_xStageOps.m_bPayloadOp) {
        vTrace("Error: Cannot have multiple combination of payload operators (gbyAgg/sort/top) in the same stage (%s)!!!\n"_Stz,
               m_szStageNm.azChar());

        return EC_INVALID_PIPELINE;
      }

      m_xStageOps.m_bPayloadOp = true;
      m_xStageOps.m_iTopOp++;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // any OP's to combine ?
  if (iOpsToCombine == 0) {
    vTrace("Error: Zero OP's to combine (%s)!!!\n"_Stz,
           m_szStageNm.azChar());
    return EC_INVALID_PIPELINE;
  }

  return EC_NONE;
}

//*============================================================================*/
/* end                                                                         */
//*============================================================================*/
