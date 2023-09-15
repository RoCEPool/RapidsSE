/*============================================================================*/
/* Stencil.cpp -- Op Stencil                                            */
/*============================================================================*/

#include "BASE.h"

#include "Stencil.hpp"
#include "Stream.hpp"
#include "StreamDef.hpp"
#include "Op.hpp"

#include "LogicUnit.hpp"

#include "Grid.hpp"
#include "Node.hpp"
#include "Index.hpp"
#include "Table.hpp"
#include "IndexTree.hpp"
#include "IndexStream.hpp"

#include "CodeSynthUtils.hpp"
#include "PipelineDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Stencil,CN_STENCIL)
__aNewFunction(Stencil,CN_STENCIL)
__vDeleteFunction(Stencil,CN_STENCIL)
__aFooterFunction(Stencil)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stencil::Stencil(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  m_szStageNm    = ""_Stz;
  m_pOpSource    = nullptr;
  m_eHandlerKind = HK_NONE;
  m_ePreviousStencil  = SR_NONE;
  m_sLabelLoop   = ""_Stz;
  
  try {
    m_pOpSource    = new CombinedSrc;
  }
  catch (LowMemory xLowMemory) {
    throw xLowMemory;
  }
  catch (InternalError xInternalError) {

    throw xInternalError;
  }

  _checkInitialised_(CN_STENCIL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stencil::~Stencil(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  delete m_pOpSource;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* synthesise given pipeline                                                  */
/*----------------------------------------------------------------------------*/
eEC Stencil::eSynthesise(PipelineDef *pPipelineDef) {
  _static_

  eEC eErrorCode;

  i8 nStages = pPipelineDef->nItems();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // synthesise the code for each stage
  for (i8 i_Stage = 0; i_Stage < nStages; i_Stage++) {
    Stencil* pStencil = new Stencil;
    StageDef *pStageDef = pPipelineDef->pGet(i_Stage);
    eErrorCode = pStencil->eSynthesise(pStageDef);
    if (eErrorCode != EC_NONE) {
      return eErrorCode;
    }

    // synthesise the code for the stage
    pStencil->vSynthesise(i_Stage == 0           ? true : false /* firstStage */,
                          i_Stage == nStages - 1 ? true : false /* lastStage */ );

    // add the synthesised code to the stageDef
    pStageDef->vCode(pStencil->szOpSource());

    delete pStencil;
  }

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* synthesise given pipeline                                                  */
/*----------------------------------------------------------------------------*/
eEC Stencil::eSynthesise(StageDef *pStageDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  bool bInflateGenerated = false;
  bool bGeneratePotTub = true;
  bool bGeneratePot = false;
  bool bPayload = false;

  s256 sHashExpr = ""_Stz;
  s256 sLimitExpr = ""_Stz;

  OpSketch *pOpSketch = nullptr;
  OpDef* pHashOpDef = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the stage name of stage being synthesised
  m_szStageNm = pStageDef->szStageNm();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // look for hashOP and limitOp
  for (i8 i_Op = 0; i_Op < pStageDef->nItems(); i_Op++) {

    // get the opDef
    OpDef *pOpDef = pStageDef->pGet(i_Op);

    if (pOpDef->szOpNm()->bSame("hash"_Stz)) {

      // generate pot
      bGeneratePot = true;

      // get hash opDef
      pHashOpDef = pOpDef;
    }

    if (pOpDef->szOpNm()->bSame("limit"_Stz)) {

      // get limit expr from OpDef
      sLimitExpr = pOpDef->pArgDef_expr()->szArgVal();
    }
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // for each operator in the stageDef
  for (i8 i_Op = 0; i_Op < pStageDef->nItems(); i_Op++) {

    // get the opDef
    OpDef *pOpDef = pStageDef->pGet(i_Op);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflate CRW
    if (pOpDef->szOpNm()->bSame("scan"_Stz)) {

      m_eHandlerKind = HK_CRWBUFFER;
      m_sLabelLoop = "L_AdvanceNextCrwT"_Stz;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchInflateCrw(pStageDef, pOpDef);
      vAppend(pOpSketch);

      bInflateGenerated = true;
      m_ePreviousStencil = SR_IT;
      continue;
    }

    // generate code for indexScan OP
    if (pOpDef->szOpNm()->bSame("indexScan"_Stz)) {

      m_eHandlerKind = HK_INDEXBUFFER;
      m_sLabelLoop = "L_AdvanceNextIndexT"_Stz;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchIndexScan(pStageDef, pOpDef);
      vAppend(pOpSketch);

      bInflateGenerated = true;
      m_ePreviousStencil = SR_IT;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for inflateRow before handling the current operator
    if (!bInflateGenerated) {

      m_eHandlerKind = HK_ROWBUFFER;
      m_sLabelLoop = "L_AdvanceNextRowT"_Stz;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchInflateRow(pStageDef, pOpDef);
      vAppend(pOpSketch);

      bInflateGenerated = true;
      m_ePreviousStencil = SR_IT;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOP
    if (pOpDef->szOpNm()->bSame("aggreg"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchAggreg(pStageDef, pOpDef);
      vAppend(pOpSketch);

      m_ePreviousStencil = SR_AT;
      m_eHandlerKind = HK_EOS;

      OpSketch *pAggOpSketch = pOpSketch;

      pOpSketch = pSketchEos(pStageDef);
      vAppend(pOpSketch);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchPotTub(pAggOpSketch, bGeneratePot, pHashOpDef, sLimitExpr);
      vAppend(pOpSketch);

      bGeneratePotTub = false;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the filterOP
    if (pOpDef->szOpNm()->bSame("filter"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchFilter(pStageDef, pOpDef);
      vAppend(pOpSketch);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the aggregOp
    if (pOpDef->szOpNm()->bSame("gbyAgg"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchGbyAgg(pStageDef, pOpDef);
      vAppend(pOpSketch);

      m_ePreviousStencil = SR_PL;
      m_eHandlerKind = HK_EOS;

      OpSketch *pGbyAggOpSketch = pOpSketch;

      pOpSketch = pSketchEos(pStageDef);
      vAppend(pOpSketch);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchPotTub(pGbyAggOpSketch, bGeneratePot, pHashOpDef, sLimitExpr);
      vAppend(pOpSketch);

      bGeneratePotTub = false;
      bPayload = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the hashOp
    if (pOpDef->szOpNm()->bSame("hash"_Stz)) {

      // handled in pot
      continue;
    }

    // generate code for indexJoin OP
    if (pOpDef->szOpNm()->bSame("indexJoin"_Stz) ||
        pOpDef->szOpNm()->bSame("crossJoin"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchIndexJoin(pStageDef, pOpDef);
      vAppend(pOpSketch);

      m_sLabelLoop = "L_AdvanceNextRowJ"_Stz;

      m_ePreviousStencil = SR_TJ;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the limitOP
    if (pOpDef->szOpNm()->bSame("limit"_Stz)) {

      // limit after a payload will move to EOSHandler
      if (bPayload) {
        continue;
      }

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchLimit(pStageDef, pOpDef);
      vAppend(pOpSketch);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the ordAgg
    if (pOpDef->szOpNm()->bSame("ordAgg"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchOrdAgg(pStageDef, pOpDef);
      vAppend(pOpSketch);

      m_ePreviousStencil = SR_PL;
      m_eHandlerKind = HK_EOS;

      OpSketch *pOrdAggOpSketch = pOpSketch;

      pOpSketch = pSketchEos(pStageDef);
      vAppend(pOpSketch);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchPotTub(pOrdAggOpSketch, bGeneratePot, pHashOpDef, sLimitExpr);
      vAppend(pOpSketch);

      bGeneratePotTub = false;
      bPayload = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the projectOP
    if (pOpDef->szOpNm()->bSame("project"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchProject(pStageDef, pOpDef);
      vAppend(pOpSketch);

      m_ePreviousStencil = SR_PT;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the semiJoinOP
    if (pOpDef->szOpNm()->bSame("keyExists"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchSemiJoin(pStageDef, pOpDef);
      vAppend(pOpSketch);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the antiJoinOP
    if (pOpDef->szOpNm()->bSame("keyNotExists"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchAntiJoin(pStageDef, pOpDef);
      vAppend(pOpSketch);

      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the sort OP
    if (pOpDef->szOpNm()->bSame("sort"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchSort(pStageDef, pOpDef);
      vAppend(pOpSketch);

      m_ePreviousStencil = SR_PL;
      m_eHandlerKind = HK_EOS;

      OpSketch *pSortOpSketch = pOpSketch;

      pOpSketch = pSketchEos(pStageDef);
      vAppend(pOpSketch);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchPotTub(pSortOpSketch, bGeneratePot, pHashOpDef, sLimitExpr);
      vAppend(pOpSketch);

      bGeneratePotTub = false;
      bPayload = true;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // generate the code for the top OP
    if (pOpDef->szOpNm()->bSame("top"_Stz)) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchTop(pStageDef, pOpDef);
      vAppend(pOpSketch);

      m_ePreviousStencil = SR_PL;
      m_eHandlerKind = HK_EOS;

      OpSketch *pTopOpSketch = pOpSketch;

      pOpSketch = pSketchEos(pStageDef);
      vAppend(pOpSketch);

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      pOpSketch = pSketchPotTub(pTopOpSketch, bGeneratePot, pHashOpDef, sLimitExpr);
      vAppend(pOpSketch);

      bGeneratePotTub = false;
      bPayload = true;
      continue;
    }

  }

  OpSketch* pLastOpSketch = pOpSketch;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the POT/TUB if not already generated
  if ( bGeneratePotTub ) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // sketch pot/tub
    pOpSketch = pSketchPotTub(pLastOpSketch, bGeneratePot, pHashOpDef, sLimitExpr);
    vAppend(pOpSketch);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // sketch EOS handler function
    m_eHandlerKind = HK_EOS;
    pOpSketch = pSketchEos(pStageDef);
    vAppend(pOpSketch);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sketch StreamDef function
  m_eHandlerKind = HK_STREAMDEF;
  pOpSketch = pSketchStreamDef(pLastOpSketch);
  vAppend(pOpSketch);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //vTrace("OpSketch's generated for stage %s to be synthesised: \n"_Stz, pStageDef->szStageNm());

  // for (i8 i_OpSketch = 0; i_OpSketch < nItems(); i_OpSketch++) {
  //   OpSketch* pOpSketch = pGet(i_OpSketch);
  //   vTrace("  %s\n"_Stz, aSK[pOpSketch->m_eSketchKind]);
  // }

  return EC_NONE;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* synthesizecode for all operators for given stage                           */
/*----------------------------------------------------------------------------*/
void Stencil::vSynthesise(bool bFirstStage, bool bLastStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // synthesize the code for the combined operators
  m_pOpSource->vSynthesizeCode(this, bFirstStage, bLastStage);

}

/*----------------------------------------------------------------------------*/
/* return op source code                                                      */
/*----------------------------------------------------------------------------*/
Stz* Stencil::szOpSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
  
  char* azCode = m_pOpSource->azCode();

//  if (! StrLib::bIsTypical(azCode)) {
//    vTrace("~~~~\n"_Stz);
//    vTrace("%s\n"_Stz, azCode);
//    vTrace("~~~~\n"_Stz);
//    vDebugPoint();
//  }

  return (Stz*)azCode;
}

/*----------------------------------------------------------------------------*/
/* return op stage name                                                       */
/*----------------------------------------------------------------------------*/
Stz* Stencil::szStageNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  return (Stz*)m_szStageNm;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
void Stencil::vGetNextLabel(String* psLabelNext, StageDef* pStageDef, i8 i_Op, Stz* sDefault) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  i8 iNextOp = i_Op + 1;

  if (iNextOp >= pStageDef->nItems()) {
    psLabelNext->vCopy(sDefault);
  }
  else {

    OpDef *pOpNext = pStageDef->pGet(iNextOp);

    if (pOpNext->szOpNm()->bSame("hash"_Stz))
      psLabelNext->vCopy(sDefault);
    else {
      // form next label using next op
      psLabelNext->vSprint("L_%s_%d"_Stz, pOpNext->szOpNm(), iNextOp);
    }

  }
}

/*----------------------------------------------------------------------------*/
/* sketch code for indexJoin                                                  */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchIndexJoin(StageDef* pStageDef, OpDef* pOpDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

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
  pOpSketch = new OpSketch(pStageDef, i_Op, sJoinExpr, m_eHandlerKind, SK_INDEXJOIN, m_ePreviousStencil, SR_TJ, II_JJ);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // HACK-OPTIMISER
  // set the streamDefRht
  Table* pTable = pIndex->pTable();
  pStreamDefRht = pTable->pStreamDef();

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, pStreamDefRht, pStreamDefOut);

  // set the labels in the opSketch
  sLabelOuterLoop = m_sLabelLoop;
  pOpSketch->vSetLabels(sLabel, sLabelNext, "L_AdvanceNextRowJ"_Stz, sLabelOuterLoop);

  // set the indexDef
  pOpSketch->vTable(pTable);
  pOpSketch->vIndex(pIndex);
  pOpSketch->vIndexDef(pIndex->pIndexDef());
  pOpSketch->vIndexPositionLower(IP_GE);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sJoinExpr);

  // get join constraint from OP
  if (pOpDef->pArgDef_constraint()) {
    Stz* sConstraintExpr = pOpDef->pArgDef_constraint()->szArgVal();
    pOpSketch->vGenerateExprStacks(sConstraintExpr, true /* constraint */);
  }

  // is it a cross join ?
  if ( ! pOpSketch->m_pExprStack->nValues() ) {
    pOpSketch->m_bCrossJoin = true;
  }

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* get the next label for Op                                                  */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchIndexScan(StageDef* pStageDef, OpDef* pOpDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  OpSketch* pOpSketch;

  s256 sLabel;
  s256 sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get indexScan expr from OpDef
  Stz* sIndexScanExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefs

	StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;
#ifdef Z_REJIG_STREAMDEFS
	StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
#else
	StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_emitted;
#endif

	// get the colReq
	ColsReq* pColsReq = pOpDef->m_pColsReq;

  // get the index
  Index* pIndex = pOpDef->m_pIndex_indexScan;

  // create a new opSketch for this OP
  pOpSketch = new OpSketch(pStageDef, i_Op,sIndexScanExpr, m_eHandlerKind, SK_INDEXSCAN, SR_NONE, SR_IT, II_ST);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(),i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

	// copy the colsReq
	pOpSketch->vCopyColsReq(pColsReq);

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
OpSketch* Stencil::pSketchInflateCrw(StageDef* pStageDef, OpDef* pOpDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  OpSketch* pOpSketch;

  s256 sLabel;
  s256 sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get the streamDefs
	StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;
#ifdef Z_REJIG_STREAMDEFS
	StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
#else
	StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_emitted;
#endif

  // get the colReq
  ColsReq* pColsReq = pOpDef->m_pColsReq;

  // create a new opSketch for this OP
  pOpSketch = new OpSketch(pStageDef, i_Op, ""_Stz, m_eHandlerKind, SK_INFLATE_CRW, SR_NONE, SR_IT, II_NONE);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  // make the labels
  sLabel.vSprint("L_InflateCrwT"_Stz);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  // copy the colsReq
  pOpSketch->vCopyColsReq(pColsReq);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch inflate of row from rowBuffer                                       */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchInflateRow(StageDef* pStageDef, OpDef* pOpDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  OpSketch *pOpSketch;

  s256 sLabel;
  s256 sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get the streamDefOut
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;

  // create a new opSketch for this OP
  pOpSketch = new OpSketch(pStageDef, i_Op, ""_Stz, m_eHandlerKind, SK_INFLATE_ROW, SR_NONE, SR_IT, II_NONE);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefLft);

  // make the labels
  sLabel.vSprint("L_InflateRowT"_Stz);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch eos                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchEos(StageDef* pStageDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  // create a new opSketch
  OpSketch* pOpSketch = new OpSketch(pStageDef, C_UNKNOWN, ""_Stz, m_eHandlerKind, SK_EOS, m_ePreviousStencil, SR_NONE, II_NONE);

  sLabel = ""_Stz;
  sLabelNext = ""_Stz;

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch filter                                                              */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchFilter(StageDef* pStageDef, OpDef* pOpDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get filter expr from OpDef
  Stz* sFilterExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefOut
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_arriving;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(pStageDef, i_Op, sFilterExpr, m_eHandlerKind, SK_FILTER, m_ePreviousStencil, SR_NONE, II_NONE);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sFilterExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch semiJoin                                                            */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchSemiJoin(StageDef* pStageDef, OpDef* pOpDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

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
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_arriving;

  //HACK-OPTIMISER
  StreamDef *pStreamDefRht = nullptr; //pOpDef->m_pStreamDef_joinTable;

  // get the index
  Index *pIndex = pOpDef->m_pIndex_indexSemi;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the semi-join
  pOpSketch = new OpSketch(pStageDef, i_Op, sJoinExpr, m_eHandlerKind, SK_SEMIJOIN, m_ePreviousStencil, SR_IJ, II_JJ);

  //HACK-OPTIMISER
  // set the streamDefRht
  // pOpSketch->vStreamDefRht(pStreamDefRht);
  Table* pTable = pIndex->pTable();
  pStreamDefRht = pTable->pStreamDef();

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, pStreamDefRht, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  //sLabelOuterLoop.vSprint("L_%s_END_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

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
OpSketch* Stencil::pSketchLimit(StageDef* pStageDef, OpDef* pOpDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

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
  OpSketch *pOpSketch = new OpSketch(pStageDef, i_Op, sLimitExpr, m_eHandlerKind, SK_LIMIT, m_ePreviousStencil, SR_NONE, II_NONE);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch ordered aggregatewith group by                                      */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchOrdAgg(StageDef* pStageDef, OpDef* pOpDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s4K sLabelPrev;

  i8 i_Op = pOpDef->i_Op();

  // get the grouby aggregate expr from OpDef
  Stz* sGbyAggExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(pStageDef, i_Op, sGbyAggExpr, m_eHandlerKind, SK_ORDAGG, m_ePreviousStencil, SR_AT, II_PL);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  // set the labels in the opSketch
  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  // set the indexDef
  IndexDef *pIndexDef = nullptr; // pOpDef->m_pIndexDef_ordAgg;
  pOpSketch->vIndexDef(pIndexDef);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* sketch project expressions                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchAggreg(StageDef* pStageDef, OpDef* pOpDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

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
  OpSketch *pOpSketch = new OpSketch(pStageDef, i_Op, sAggregExpr, m_eHandlerKind, SK_AGGREG, m_ePreviousStencil, SR_AT, II_NONE);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sAggregExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch antiJoin                                                            */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchAntiJoin(StageDef* pStageDef, OpDef* pOpDef) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  OpSketch *pOpSketch;

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get join expr from OP
  Stz* sJoinExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get join constraint from OP
  Stz* sConstraintExpr = pOpDef->pArgDef_constraint()->szArgVal();

  // get the streamDefLft
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_arriving;

  //HACK-OPTIMISER
  StreamDef *pStreamDefRht = nullptr; //pOpDef->m_pStreamDef_joinTable;

  // get the index
  Index *pIndex = pOpDef->m_pIndex_indexSemi;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code for the semi-join
  pOpSketch = new OpSketch(pStageDef, i_Op, sJoinExpr, m_eHandlerKind, SK_ANTIJOIN, m_ePreviousStencil, SR_IJ, II_JJ);

  //HACK-OPTIMISER
  // set the streamDefRht
  // pOpSketch->vStreamDefRht(pStreamDefRht);
  Table* pTable = pIndex->pTable();
  pStreamDefRht = pTable->pStreamDef();

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, pStreamDefRht, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, "L_AdvanceNextRowJ"_Stz, m_sLabelLoop);

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
OpSketch* Stencil::pSketchProject(StageDef* pStageDef, OpDef* pOpDef)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get project expr from OpDef
  Stz* sProjectExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(pStageDef, i_Op, sProjectExpr, m_eHandlerKind, SK_PROJECT, m_ePreviousStencil, SR_PT, II_NONE);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);

  // set the labels in the opSketch
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  // generate the expressißon stacks
  pOpSketch->vGenerateExprStacks(sProjectExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch project group by expressions                                        */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchGbyAgg(StageDef* pStageDef, OpDef* pOpDef)
{

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  s256 szStageNm;

  i8 i_Op = pOpDef->i_Op();

  // get the grouby aggregate expr from OpDef
  Stz* sGbyAggExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the columns required
  ColMap* pColsMap = pOpDef->m_pColMap_gbyAgg;

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(pStageDef, i_Op, sGbyAggExpr, m_eHandlerKind, SK_GBYAGG, m_ePreviousStencil, SR_NONE, II_PL);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  // set the labels in the opSketch
  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  // set the indexDef
  IndexDef *pIndexDef = pOpDef->m_pIndexDef_gbyAgg;
  pOpSketch->vIndexDef(pIndexDef);

  // set the payLoad bytes
  i8 nPayloadBytes = (pStreamDefOut->nColumns() - pIndexDef->nColumns()) * 8;
  pOpSketch->vPayloadBytes(nPayloadBytes);

  // set the colsMap (payload information)
  pOpSketch->vCopyColMap(pColsMap);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sGbyAggExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch sort                                                                */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchSort(StageDef* pStageDef, OpDef* pOpDef)
{

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get sort expr from OpDef
  Stz* sSortExpr = pOpDef->pArgDef_expr()->szArgVal();

  // get the columns required
  ColMap* pColsMap = pOpDef->m_pColMap_sort;

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(pStageDef, i_Op, sSortExpr, m_eHandlerKind, SK_SORT, m_ePreviousStencil, SR_NONE,  II_PL);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  // set the labels in the opSketch
  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);
  pOpSketch->vSetLabels(sLabel, ""_Stz, m_sLabelLoop);

  // set the indexDef
  IndexDef *pIndexDef = _pIndexDef(pOpDef->m_pIndexDef_sort);
  pOpSketch->vIndexDef(pIndexDef);

  // set the payLoad bytes
  i8 nPayloadBytes = (pStreamDefOut->nColumns() - pIndexDef->nColumns()) * 8;
  pOpSketch->vPayloadBytes(nPayloadBytes);

  // set the colsMap (payload information)
  pOpSketch->vCopyColMap(pColsMap);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sSortExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch streamDef                                                           */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchStreamDef(OpSketch* pLastOp) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  // get the streamDefOut
  StreamDef *pStreamDefOut = pLastOp->m_pStreamDefOut;
  RASSERT(pStreamDefOut);

  // create a new opSketch
  OpSketch* pOpSketch = new OpSketch(nullptr, C_UNKNOWN, ""_Stz, m_eHandlerKind, SK_STREAMDEF, SR_NONE,  SR_NONE, II_NONE);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefOut, nullptr, pStreamDefOut);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch top                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchTop(StageDef* pStageDef, OpDef* pOpDef)
{

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;

  i8 i_Op = pOpDef->i_Op();

  // get top expr from OpDef
  Stz* sTopExpr = pOpDef->pArgDef_expr()->szArgVal();

  Stz* szTopCount = pOpDef->pArgDef_count()->szArgVal();

  // get the streamDefs
  StreamDef *pStreamDefLft = pOpDef->m_pStreamDef_arriving;
  StreamDef *pStreamDefOut = pOpDef->m_pStreamDef_emitted;

  // get the columns required
  ColMap* pColsMap = pOpDef->m_pColMap_top;

  // create a new opSketch for this OP
  OpSketch *pOpSketch = new OpSketch(pStageDef, i_Op, sTopExpr, m_eHandlerKind, SK_TOP, m_ePreviousStencil, SR_NONE,  II_PL);

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefLft, nullptr, pStreamDefOut);

  // set the labels in the opSketch
  sLabel.vSprint("L_%s_%d"_Stz, pOpDef->szOpNm(), i_Op);
  vGetNextLabel(sLabelNext, pStageDef, i_Op, "L_AddRowToTubPot"_Stz);
  pOpSketch->vSetLabels(sLabel, ""_Stz, m_sLabelLoop);

  // get the IndexDef from the TopOp
  IndexDef* pIndexDef = _pIndexDef(pOpDef->m_pIndexDef_top);
  pOpSketch->vIndexDef(pIndexDef);

  // set the payLoad bytes
  i8 nPayloadBytes = (pStreamDefOut->nColumns() - pIndexDef->nColumns()) * 8;
  pOpSketch->vPayloadBytes(nPayloadBytes);

  // set the colsMap (payload information)
  pOpSketch->vCopyColMap(pColsMap);

  // set the top count
  pOpSketch->vLimitExpr(szTopCount);

  // generate the expression stacks
  pOpSketch->vGenerateExprStacks(sTopExpr);

  return pOpSketch;
}

/*----------------------------------------------------------------------------*/
/* sketch tub                                                                 */
/*----------------------------------------------------------------------------*/
OpSketch* Stencil::pSketchPotTub(OpSketch* pLastOp, bool bGeneratePot, OpDef* pHashOpDef, Stz* sLimitExpr) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_STENCIL, _FOOTER_);

  s4K sLabel;
  s4K sLabelNext;
  OpSketch* pOpSketch;

  // get the streamDefOut
  StreamDef *pStreamDefOut = pLastOp->m_pStreamDefOut;
  RASSERT(pStreamDefOut);

  // create a new opSketch
  if (bGeneratePot) {
    pOpSketch = new OpSketch(nullptr, C_UNKNOWN, ""_Stz, m_eHandlerKind, SK_POT, m_ePreviousStencil, SR_PO,  II_PL);
    pOpSketch->vLimitExpr(sLimitExpr);
  }
  else {
    pOpSketch = new OpSketch(nullptr, C_UNKNOWN, ""_Stz, m_eHandlerKind, SK_TUB, m_ePreviousStencil, SR_TO,  II_PL);
    pOpSketch->vLimitExpr(sLimitExpr);
  }

  // set the streamDef's
  pOpSketch->vStreamDefs(pStreamDefOut, nullptr, pStreamDefOut);

  if (bGeneratePot) {
    // generate the expression stack
    //pOpSketch->vGenerateExprStacks("type INTEGER $2"_Stz);

		// copy the colHsh
		RASSERT(pHashOpDef);
		pOpSketch->vCopyColHsh(pHashOpDef->m_pColHsh);
  }

  if (pLastOp->eSketchKind() == SK_GBYAGG ||
      pLastOp->eSketchKind() == SK_TOP    ||
      pLastOp->eSketchKind() == SK_SORT) {

    // set the indexDef
    pOpSketch->vIndexDef(pLastOp->m_pIndexDef);

    // copy the colMap
    pOpSketch->vCopyColMap(pLastOp->m_pColMap);
  }

  // set the labels in the opSketch
  sLabel = "L_AddRowToTubPot"_Stz;
  sLabelNext = ""_Stz;
  pOpSketch->vSetLabels(sLabel, sLabelNext, m_sLabelLoop);

  if (pLastOp->m_pInitStack)
    pOpSketch->vCopyInitStack(pLastOp->m_pInitStack);

  return pOpSketch;
}

//*============================================================================*/
/* end                                                                         */
//*============================================================================*/
