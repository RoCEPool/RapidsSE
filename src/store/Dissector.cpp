/*============================================================================*/
/* Dissector.cpp -- Dissector                                                 */
/*============================================================================*/

#include "Dissector.hpp"

#include "Optimiser.hpp"

#include "Job.hpp"
#include "Index.hpp"
#include "Table.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Dissector, CN_DISSECTOR)
__aNewFunction(Dissector, CN_DISSECTOR)
__vDeleteFunction(Dissector, CN_DISSECTOR)
__aFooterFunction(Dissector)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dissector::Dissector(Bench* pBench)
: OptimiserTool(CN_DISSECTOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  m_pBench        = pBench;
  m_pOptimiser    = m_pBench->pOptimiser();
  m_pBoard        = nullptr;
  
  m_pPipeSpec     = nullptr;
  m_pDeckSpec     = nullptr;
  m_pStageSpec    = nullptr;
  m_pOpSpec       = nullptr;

  m_pOpArgs       = nullptr;
  m_szOpNm        = nullptr;
  m_bSawAggOp     = false;
  m_bSawProjectOp = false;

  _checkInitialised_(CN_DISSECTOR);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dissector::~Dissector(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* dissect pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
void Dissector::vDissectSpec(VertexSpec* pPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
        
  i8 nSubPipelines = pPipeSpec->nItems();
  for (i8 i_SubPipeline = 0; i_SubPipeline < nSubPipelines; i_SubPipeline++) {
    m_pPipeSpec = pPipeSpec->pGet(i_SubPipeline);
    m_pDeckSpec = m_pPipeSpec->pDeckSpec();
    vSubPipe(i_SubPipeline);
  }

  m_pPipeSpec = pPipeSpec;
  m_pDeckSpec = m_pPipeSpec->pDeckSpec();
  vSubPipe(-1);

  for (i8 i_Board = 0; i_Board < m_pBench->nBoards(); i_Board++) {
    m_pBoard = m_pBench->pGetBoard(i_Board);
    m_pBoard->vBoardNo(i_Board);

    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pDeckSpec  = nullptr;
  m_pStageSpec = nullptr;
  m_pOpSpec    = nullptr;
  m_bSawAggOp  = false;
  m_bSawProjectOp = false;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dissect pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
void Dissector::vSubPipe(i8 i_SubPipe)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  Stz* szSqyNm;
  if (C_UNKNOWN == i_SubPipe) {
    if (MY::pJob()->bHasTag("<pipelineName>"_Stz))
      szSqyNm = MY::pJob()->pJobTags()->szValue("<pipelineName>"_Stz);
    else
      szSqyNm = m_pPipeSpec->szVertexNm();

    m_pOptimiser->vQueryNm(szSqyNm);
    m_pBench->vPipelineNm(szSqyNm);
  }
  else {
    szSqyNm = m_pPipeSpec->szVertexNm();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pBoard = new Board(i_SubPipe + 1,
                       BK_SUBQUERY,
                       szSqyNm,
                       m_pBench);

  Card_SubQuery* p_subQuery = new Card_SubQuery;
  m_pBoard->pDeck()->vAppendChild(p_subQuery);

  p_subQuery->pSymbol()->Y_vSet(P_Pointer,m_pBoard->pSubQuery());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  i8 nStageSpecs = m_pPipeSpec->nStageSpecs();
  for (i8 i_StageSpec = 1; i_StageSpec < nStageSpecs - 1; i_StageSpec++) {
    m_pStageSpec = m_pPipeSpec->pStageSpec(i_StageSpec);
    
    vStage(i_StageSpec);
  }
}

/*----------------------------------------------------------------------------*/
/* dissect stage spec                                                         */
/*----------------------------------------------------------------------------*/
void Dissector::vStage(i8 i_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
 
  eYG eStageTopo = m_pStageSpec->eStageTopology();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    

  Card_Stage* p_stage = new Card_Stage;
  p_stage->SYM(SF_STG_NO,i_StageSpec);

  Card_Topology* p_stageTopo = new Card_Topology(eStageTopo);
  p_stage->vAppendChild(p_stageTopo);

  m_pBoard->pDeck()->vAppendChild(p_stage);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // params
  if (1 == i_StageSpec) {
    ParamsSpec* pParamsSpec = m_pDeckSpec->pParamsSpec();
    if (pParamsSpec) {
      Card_Correlation* p_paramList = new Card_Correlation;
      m_pBoard->pDeck()->vAppendChild(p_paramList);
      Card_OuterColNm* p_param = new Card_OuterColNm;
      StrLib::vStrip((char*)pParamsSpec->pGet(0)->szParamNm(), pParamsSpec->pGet(0)->iParamNmBufSize(), ' ');
      p_param->SYM(SF_NMD_COL, pParamsSpec->pGet(0)->szParamNm());
      p_param->pSymbol()->Y_vSet(P_ParamOdn, "%1"_Stz);
      p_paramList->vAppendChild(p_param);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bSawAggOp = false;
  m_bSawProjectOp = false;

  i8 nOpSpecs = m_pStageSpec->nItems();
  for (i8 i_OpSpec = 0; i_OpSpec < nOpSpecs; i_OpSpec++) {
    m_pOpSpec = m_pStageSpec->pGet(i_OpSpec);
    
    vOp(i_OpSpec);
  }
}

/*----------------------------------------------------------------------------*/
/* dissect op spec                                                            */
/*----------------------------------------------------------------------------*/
void Dissector::vOp(i8 i_OpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  UNUSED(i_OpSpec);

  Card* pCard;

  m_szOpNm = m_pOpSpec->szOpNm();

  m_pOpArgs = m_pOpSpec->pArgTags();

  //vTrace("Op %s\n"_Stz,m_szOpNm);
  //m_pOpArgs->vDumpSet(true);

       if (m_szOpNm->bSame("abstractJoin"_Stz)) pCard = pOp_absJoin     ();
  else if (m_szOpNm->bSame("aggreg"_Stz))       pCard = pOp_aggreg      ();
  else if (m_szOpNm->bSame("call"_Stz))         pCard = pOp_call        ();
  else if (m_szOpNm->bSame("combined"_Stz))     pCard = pOp_combined    ();
  else if (m_szOpNm->bSame("consumeOp"_Stz))    pCard = pOp_consume     ();
  else if (m_szOpNm->bSame("debug"_Stz))        pCard = pOp_debug       ();
  else if (m_szOpNm->bSame("filter"_Stz))       pCard = pOp_filter      ();
  else if (m_szOpNm->bSame("gbyAgg"_Stz))       pCard = pOp_gbyAgg      ();
  else if (m_szOpNm->bSame("hash"_Stz))         pCard = pOp_hash        ();
  else if (m_szOpNm->bSame("indexJoin"_Stz))    pCard = pOp_indexJoin   ();
  else if (m_szOpNm->bSame("indexScan"_Stz))    pCard = pOp_indexScan   ();
  else if (m_szOpNm->bSame("keyExists"_Stz))    pCard = pOp_keyExists   ();
  else if (m_szOpNm->bSame("keyNotExists"_Stz)) pCard = pOp_keyNotExists();
  else if (m_szOpNm->bSame("limit"_Stz))        pCard = pOp_limit       ();
  else if (m_szOpNm->bSame("ordagg"_Stz))       pCard = pOp_ordAgg      ();
  else if (m_szOpNm->bSame("orderBy"_Stz))      pCard = pOp_sort        ();
  else if (m_szOpNm->bSame("output"_Stz))       pCard = pOp_output      ();
  else if (m_szOpNm->bSame("pulse"_Stz))        pCard = pOp_pulse       ();
  else if (m_szOpNm->bSame("project"_Stz))      pCard = pOp_project     ();
  else if (m_szOpNm->bSame("scan"_Stz))         pCard = pOp_scan        ();
  else if (m_szOpNm->bSame("sendSE"_Stz))       pCard = pOp_send        ();
  else if (m_szOpNm->bSame("sink"_Stz))         pCard = pOp_sink        ();
  else if (m_szOpNm->bSame("sort"_Stz))         pCard = pOp_sort        ();
  else if (m_szOpNm->bSame("start"_Stz))        pCard = pOp_start       ();
  else if (m_szOpNm->bSame("top"_Stz))          pCard = pOp_top         ();
  else {
    if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
      vTrace("Skipping OpSpec %s\n"_Stz, m_szOpNm);
    pCard = nullptr;
  }
  
  if (pCard)
    m_pBoard->pDeck()->vAppendChild(pCard);
}

/*----------------------------------------------------------------------------*/
/* dissect : abstractJoin <ON:szRpnExpr>                                      */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);
  Card_AbsJoin* p_absJoin = new Card_AbsJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // expr : expression list

  Card_RpnCalc* p_rpnCalc = pPop_rpnCalc(p_rpnExpr);
  Cast xCast;
  xCast.m_xTypeId = TypeId::xIdOf(CT_BOOLEAN);
  p_rpnCalc->pSymbol()->vCast(xCast);

  Card_On* p_on = new Card_On;
  p_on->vAppendChild(p_rpnCalc);

  p_absJoin->vAppendChild(p_on);

  delete p_rpnExpr;

  return p_absJoin;
}

/*----------------------------------------------------------------------------*/
/* dissect : aggreg <szRpnExpr : 0) aggElist ; 1) gbyElist>                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_aggreg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  m_bSawAggOp = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Aggreg* p_aggreg = new Card_Aggreg;
  
  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_aggLevel = new Card_Level(eAggLevel);
  p_aggreg->vAppendChild(p_aggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Stream* p_stream = new Card_Stream;
  p_aggreg->vAppendChild(p_stream);

  Card_Emit* p_emit = pPop_emit(p_rpnExpr);
  p_emit->pSymbol()->Y_vSet(P_BreakPt);

  p_stream->vAppendChild(p_emit);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnExpr;
    
  return p_aggreg;
}

/*----------------------------------------------------------------------------*/
/* dissect : call <szSubPipe>                                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_call(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Stz* szSubPipeNm = szArgValue(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeline

  StrLib::vStrip(szSubPipeNm->azChar(), szSubPipeNm->nSize(), ' ');

  Card_Call* p_call = new Card_Call;
  p_call->SYM(SF_SQY_NM, szSubPipeNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return p_call;
}

/*----------------------------------------------------------------------------*/
/* dissect : combined <iGenerate>                                             */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  Card_Combined* p_combine = new Card_Combined;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return p_combine;
}

/*----------------------------------------------------------------------------*/
/* dissect : consumeOp <nSends>                                               */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_consume(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : debug <iOption>                                                  */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_debug(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  Stz* szOption = szArgValue(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Debug* p_debug = new Card_Debug;

  p_debug->pSymbol()->Y_vSet(P_DebugOption, szOption);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return p_debug;
}

/*----------------------------------------------------------------------------*/
/* dissect : filter <szRpnExpr: 0) filtElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);
  Card_Filter* p_filter = new Card_Filter;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // filter expression

  i8 i_Expr = 1;
  Card_Expr* p_expr = pPop_expr(p_rpnExpr, false, i_Expr, false);

  Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();
  p_expr->vRemoveSub(p_rpnCalc);
  DELETE_CARD(p_expr);
  p_filter->vAppendChild(p_rpnCalc);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is it filter or having?
  
  if (m_bSawAggOp || m_bSawProjectOp) {
    Card_Having* p_having = new Card_Having;

    eAL eAggLevel = m_pStageSpec->eAggegLevel();
    Card_Level* p_aggLevel = new Card_Level(eAggLevel);
    p_having->vAppendChild(p_aggLevel);

    p_having->vAppendChild(p_filter);

    delete p_rpnExpr;
    return p_having;
  }
  else {
    Card_Where* p_where = new Card_Where;

    eAL eAggLevel = m_pStageSpec->eAggegLevel();
    Card_Level* p_aggLevel = new Card_Level(eAggLevel);
    p_where->vAppendChild(p_aggLevel);

    p_where->vAppendChild(p_filter);

    delete p_rpnExpr;
    return p_where;
  }
}
/*----------------------------------------------------------------------------*/
//Card* Dissector::pOp_filter(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // op arguments
//
//  vCheckArgs(1);
//
//  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);
//  Card_Filter* p_filter = new Card_Filter;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // filtElist : filter expression list
//
//  i8 i_Expr = 1;
//  Card_Expr* p_expr = pPop_expr(p_rpnExpr, false, i_Expr, false);
//
//  Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();
//  p_expr->vRemoveSub(p_rpnCalc);
//  DELETE_CARD(p_expr);
//
//  p_filter->vAppendChild(p_rpnCalc);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // is it filter or having?
//
//  if (m_bSawAggOp || m_bSawProjectOp) {
//    Card_Having* p_having = new Card_Having;
//    p_having->vTransferChildren(p_filter);
//    delete p_filter;
//    delete p_rpnExpr;
//
//    eAL eAggLevel = m_pStageSpec->eAggegLevel();
//    Card_Level* p_aggLevel = new Card_Level(eAggLevel);
//    p_having->vPush(p_aggLevel);
//
//    return p_having;
//  }
//  else {
//    delete p_rpnExpr;
//
//    p_filter->vCard(CK_o_WHERE);
//    Card_Where* p_where = _pCard_Where(p_filter);
//
//    eAL eAggLevel = m_pStageSpec->eAggegLevel();
//    Card_Level* p_aggLevel = new Card_Level(eAggLevel);
//    p_where->vPush(p_aggLevel);
//
//    return p_where;
//  }
//}

/*----------------------------------------------------------------------------*/
/* dissect : gbyAgg <szRpnExpr : 0) aggElist ; 1) gbyElist>                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_gbyAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  m_bSawAggOp = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  Card_Aggreg* p_aggreg = new Card_Aggreg;

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_aggLevel = new Card_Level(eAggLevel);
  p_aggreg->vAppendChild(p_aggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gbyElist : optional groupBy expression list

  if (p_rpnExpr->nItems()) {
    Card_Key* p_key = pPop_key(p_rpnExpr, true);
    p_aggreg->vAppendChild(p_key);
    p_aggreg->vCard(CK_o_GBYAGG);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Stream* p_stream = new Card_Stream;
  p_aggreg->vAppendChild(p_stream);

  Card_Emit* p_emit = pPop_emit(p_rpnExpr);
  p_stream->vAppendChild(p_emit);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnExpr;

  return p_aggreg;
}

/*----------------------------------------------------------------------------*/
/* dissect : hash <szRpnExpr : 0) calcExpr>                                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_hash(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Hash* p_hash = new Card_Hash;

  eAL eLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_level = new Card_Level(eLevel);
  p_hash->vAppendChild(p_level);

  Card_HshColList* p_hshColList = pPop_hshColList(p_rpnExpr);
  p_hash->vAppendChild(p_hshColList);

  delete p_rpnExpr;

  return p_hash;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexJoin <szTblNm> <szIdxNm> <szRpnExpr: 0) idxPred>            */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_indexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(3);

  Stz* szTableNm = szArgValue(0);
  Stz* szIndexNm = szArgValue(1);
  Card_RpnExpr* p_rpnExpr_bound = pArg_rpnExpr(2);
  Card_RpnExpr* p_rpnExpr_constraint =
      m_pOpArgs->nItems() >= 4 ? pArg_rpnExpr(3) : nullptr;

  //p_rpnExpr_bound->vDump();

  //Table* pTable = AM::pTableFind(szTableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_IndexJoin* p_indexJoin = new Card_IndexJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  p_indexJoin->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table
  
  Card_Table* p_table = pForm_table(szTableNm);
  p_indexJoin->vAppendChild(p_table);

   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Card_IndexList* p_indexList = new Card_IndexList;
  p_table->vAppendChild(p_indexList);

 // Index* pIndex = pTable->pFindIndex(szIndexNm);

  Card_Index* p_index = new Card_Index;
  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_index->vAppendChild(p_indexNm);
  p_indexNm->SYM(SF_IDX_NM,szIndexNm);

  p_indexList->vAppendChild(p_index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* p_bounds = new Card_Bounds;
  p_index->vAppendChild(p_bounds);

  Card_Bound* p_bound = new Card_Bound;


  p_bound->vTransferChildren(p_rpnExpr_bound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // possible LEFT_OUTER

  Card_Term* p_term_first = p_bound->pGetTerm(0);
  if (p_term_first->pSymbol()->bHasFacet("LEFT_OUTER"_Stz)) {
    Card_OuterJoin* p_outerJoin = new Card_OuterJoin;
    p_outerJoin->pSymbol()->vCastFacets(p_term_first->pSymbol());
    p_indexJoin->vAppendChild(p_outerJoin);
  }
  p_bound->vDeleteChild(p_term_first);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // possible BOUND

  p_term_first = p_bound->pGetTerm(0);
  if (p_term_first->pSymbol()->bHasFacet("BOUND"_Stz))
    p_bound->vDeleteChild(p_term_first);

  i8 nTerms = p_bound->nChildCards();

  Card_Term* p_term_last = p_bound->pGetTerm(nTerms-1);
  p_bound->pSymbol()->vCastFacets(p_term_last->pSymbol());
  p_bound->vDeleteChild(p_term_last);

  p_bounds->vAppendChild(p_bound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // constraint

  if (p_rpnExpr_constraint) {
    Card_Constraint* p_constraint = pPop_constraint(p_rpnExpr_constraint);
    //p_constraint->vDump(__func__);
    p_indexJoin->vAppendChild(p_constraint);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete p_rpnExpr_bound;

  if (p_rpnExpr_constraint)
    delete p_rpnExpr_constraint;

  //p_indexJoin->vDump();

  return p_indexJoin;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexScan <szTblNm> <szIdxNm> <bounds>                           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(3);

  Stz* szTableNm = szArgValue(0);
  Stz* szIndexNm = szArgValue(1);
  Card_RpnExpr* p_rpnExpr_bounds = pArg_rpnExpr(2);

  //Table* pTable = AM::pTableFind(szTableNm);

  //p_rpnExpr_bounds->vDump();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_IndexScan* p_indexScan = new Card_IndexScan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  p_indexScan->vAppendChild(p_level);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Card_Table* p_table = pForm_table(szTableNm);

  p_indexScan->vAppendChild(p_table);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Card_IndexList* p_indexList = new Card_IndexList;
  p_table->vAppendChild(p_indexList);

//  Index* pIndex = pTable->pFindIndex(szIndexNm);
//  UNUSED(pIndex);

  Card_Index* p_index = new Card_Index;
  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_index->vAppendChild(p_indexNm);
  p_indexNm->SYM(SF_IDX_NM,szIndexNm);

  p_indexList->vAppendChild(p_index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* p_bounds = pPop_bounds(p_rpnExpr_bounds);
  if (p_bounds)
    p_index->vAppendChild(p_bounds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnExpr_bounds;

  //p_indexScan->vDump();

  return p_indexScan;
}

/*----------------------------------------------------------------------------*/
/* dissect : keyExists <tableNm> <indexNm> <bounds> <constraint>              */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_keyExists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DISSECTOR, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(4);
  Stz* szTableNm = szArgValue(0);
  Stz* szIndexNm = szArgValue(1);
  Card_RpnExpr* p_rpnExpr_bounds = pArg_rpnExpr(2);
  Card_RpnExpr* p_rpnExpr_constraint = pArg_rpnExpr(3);

  //Table* pTable = AM::pTableFind(szTableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // keyExists

  Card_KeyExists* p_keyExists = new Card_KeyExists;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  p_keyExists->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Card_Table* p_table = pForm_table(szTableNm);
  p_keyExists->vAppendChild(p_table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index list

  Card_IndexList* p_indexList = pForm_indexList(szIndexNm);
  p_table->vAppendChild(p_indexList);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* p_bounds = pPop_bounds(p_rpnExpr_bounds);
  //p_bounds->vDump(__func__);
  p_indexList->pSub_index()->vAppendChild(p_bounds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // constraint

  Card_Constraint* p_constraint = pPop_constraint(p_rpnExpr_constraint);
  //p_constraint->vDump(__func__);
  p_keyExists->vAppendChild(p_constraint);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnExpr_bounds;
  delete p_rpnExpr_constraint;

  //p_keyExists->vDump(__func__);

  return p_keyExists;
}

/*----------------------------------------------------------------------------*/
/* dissect : keyNotExists <tableNm> <indexNm> <bounds> <constraint>           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_keyNotExists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DISSECTOR, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(4);
  Stz* szTableNm = szArgValue(0);
  Stz* szIndexNm = szArgValue(1);
  Card_RpnExpr* p_rpnExpr_bounds = pArg_rpnExpr(2);
  Card_RpnExpr* p_rpnExpr_constraint = pArg_rpnExpr(3);

  //Table* pTable = AM::pTableFind(szTableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // keyExists

  Card_KeyNotExists* p_keyNotExists = new Card_KeyNotExists;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  p_keyNotExists->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Card_Table* p_table = pForm_table(szTableNm);
  p_keyNotExists->vAppendChild(p_table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index list

  Card_IndexList* p_indexList = pForm_indexList(szIndexNm);
  p_table->vAppendChild(p_indexList);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* p_bounds = pPop_bounds(p_rpnExpr_bounds);
  //p_bounds->vDump(__func__);
  p_indexList->pSub_index()->vAppendChild(p_bounds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // constraint

  Card_Constraint* p_constraint = pPop_constraint(p_rpnExpr_constraint);
  //p_constraint->vDump(__func__);
  p_keyNotExists->vAppendChild(p_constraint);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnExpr_bounds;
  delete p_rpnExpr_constraint;

  //p_keyNotExists->vDump(__func__);

  return p_keyNotExists;
}

/*----------------------------------------------------------------------------*/
/* dissect : limit <nRows>                                                    */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_limit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  //m_pOpArgs->vDumpSet(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  i8 nRows = iArgValue(0);


  Card_Limit* p_limit = _pCard_Limit((new Card_Limit) ->SYM(SF_INT_LIT,nRows));

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_aggLevel = new Card_Level(eAggLevel);
  p_limit->vAppendChild(p_aggLevel);

  return p_limit;
}

/*----------------------------------------------------------------------------*/
/* dissect : ordAgg <szRpnExpr : 0) aggElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_ordAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  m_bSawAggOp = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);
  Card_OrdAgg* p_ordAgg = new Card_OrdAgg;
  
  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_aggLevel = new Card_Level(eAggLevel);
  p_ordAgg->vAppendChild(p_aggLevel);

  if (p_rpnExpr->nItems()) {
    Card_Key* p_key = pPop_key(p_rpnExpr, true);
    p_ordAgg->vAppendChild(p_key);
    //p_ordAgg->vCard(CK_o_GBYAGG);
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Stream* p_stream = new Card_Stream;
  p_ordAgg->vAppendChild(p_stream);

  Card_Emit* p_emit = pPop_emit(p_rpnExpr);
  p_stream->vAppendChild(p_emit);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete p_rpnExpr;

  return p_ordAgg;
}

/*----------------------------------------------------------------------------*/
/* dissect : output                                                           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_output(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : pulse <szMode>                                                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_pulse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : project <szRpnExpr : 0) projElist>                               */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_project(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  m_bSawProjectOp = true;
  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);

  Card_Project* p_project = new Card_Project(CK_o_PROJECT);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_aggLevel = new Card_Level(eAggLevel);
  p_project->vAppendChild(p_aggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Stream* p_stream = new Card_Stream;
  p_project->vAppendChild(p_stream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // projElist : project expression list

  Card_Emit* p_emit = pPop_emit(p_rpnExpr);
  p_stream->vAppendChild(p_emit);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnExpr;

  return p_project;
}

/*----------------------------------------------------------------------------*/
/* dissect : scan [ <szCatalogNm> <szSchemaNm> ] <szTableNm>                  */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get tableNm from op args

  i8 nTags = m_pOpArgs->nItems();
  
  Stz* szTableNm = nullptr;
  if (3 == nTags) {
    szTableNm = m_pOpArgs->szValue(2);
  }
  else if (1 == nTags) {
    szTableNm = m_pOpArgs->szValue((i8)0);
  }
  else {
    vTrace("Error: only %lld tags in scan argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  //Table* pTable = AM::pTableFind(szTableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scan

  Card_Scan* pOp_scan = new Card_Scan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pOp_scan->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form table

  Card_Table* p_table = pForm_table(szTableNm);
  pOp_scan->vAppendChild(p_table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pOp_scan;
}

/*----------------------------------------------------------------------------*/
/* dissect : send <eSendKind>                                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments
  vCheckArgs(1);

  eXK eSendKind = (eXK) iArgValue(0);
  Card_Send* p_send = new Card_Send;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // eSendKind

  p_send->pSymbol()->Y_vSet(P_SendKind, (i8)eSendKind);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return p_send;
}

/*----------------------------------------------------------------------------*/
/* dissect : sink                                                             */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_sink(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : orderBy <szRpnExpr: 0) sbyElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_sort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);
  Card_Sort* p_sort = new Card_Sort;

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_aggLevel = new Card_Level(eAggLevel);
  p_sort->vAppendChild(p_aggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sbyElist : sortBy expression list

  Card_Key* p_key = pPop_key(p_rpnExpr, true);
  p_sort->vAppendChild(p_key);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete p_rpnExpr;

  return p_sort;
}

/*----------------------------------------------------------------------------*/
/* dissect : start                                                            */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_start(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : top                                                              */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_top(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(2);

  i8 nRows = iArgValue(1);

  Card_Top* p_top = _pCard_Top((new Card_Top) ->SYM(SF_INT_LIT, nRows));


  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_aggLevel = new Card_Level(eAggLevel);
  p_top->vAppendChild(p_aggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sbyElist : sortBy expression list

  Card_RpnExpr* p_rpnExpr = pArg_rpnExpr(0);
  Card_Key* p_key = pPop_key(p_rpnExpr, true);
  p_top->vAppendChild(p_key);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Count* p_count = new Card_Count;
  p_count->SYM(SF_INT_LIT, nRows);
  p_top->vAppendChild(p_count);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnExpr;

  return p_top;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* form table                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexList* Dissector::pForm_indexList(Stz* szIndexNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_IndexList* p_indexList = new Card_IndexList;

  Card_Index* p_index = new Card_Index;

  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_indexNm->SYM(SF_IDX_NM,szIndexNm);

  p_index->vAppendChild(p_indexNm);
  p_indexList->vAppendChild(p_index);

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/* form table                                                                 */
/*----------------------------------------------------------------------------*/
Card_Table* Dissector::pForm_table(Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Table* p_table = new Card_Table;

  Card_TableNm* p_tableNm = (new Card_TableNm) ->SYM(SF_TBL_NM,szTableNm);
  p_table->vAppendChild(p_tableNm);

  return p_table;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect <as> terms : AS <name>                                             */
/*----------------------------------------------------------------------------*/
Card_As* Dissector::pPop_as(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // test for: AS
  if (! p_rpnExpr->bHeadSame("AS"_Stz))
    return nullptr;

  // AS
  Card_Term* p_term = _pCard_Term(p_rpnExpr->pPopChild());
  RASSERT(p_term->pSymbol()->bHasFacet("AS"_Stz));
  delete p_term;
  p_term = nullptr;

  // <name>
  p_term = _pCard_Term(p_rpnExpr->pPopChild());
  Stz* szAsNm = p_term->szFacet(SF_NONE);

  Card_As* p_as = (new Card_As) ->SYM(SF_NMD_COL,szAsNm);
  delete p_term;
  p_term = nullptr;

  return p_as;
}

/*----------------------------------------------------------------------------*/
/* pop bounds                                                                 */
/*----------------------------------------------------------------------------*/
Card_Bounds* Dissector::pPop_bounds(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Bounds* p_bounds = new Card_Bounds;

  while (Card_Bound* p_bound = pPop_bound(p_rpnExpr))
    p_bounds->vAppendChild(p_bound);

  return p_bounds;
}

/*----------------------------------------------------------------------------*/
/* pop bound                                                                  */
/*----------------------------------------------------------------------------*/
Card_Bound* Dissector::pPop_bound(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  //p_rpnExpr->vDump();

  // is there a bound term sequence at the top of rpnTerms?
  Card* p_rpnItem = p_rpnExpr->pHead();
  if (! p_rpnItem || !p_rpnItem->pSymbol()->bHasFacet(SF_BOUND))
    return nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pop one (l/u) bound term sequence as a card bound

  Card_Bound* p_bound = new Card_Bound;

  // the first term in sequence will be a LBOUND or UBOUND
  //   which is not transfered but should match last reln op in sequence
  p_rpnItem = p_rpnExpr->pPopChild();
  RASSERT(p_rpnItem->pSymbol()->bHasFacet(SF_BOUND));

  // transfer terms for this bound sequence
  Card* p_term_xfr;
  for (i8 i_Loop = 0; ; i_Loop++) {
    UNUSED(i_Loop);

    Card_Term* p_term_top = _pCard_Term(p_rpnExpr->pHead());
    if (! p_term_top || p_term_top->pSymbol()->bHasFacet(SF_BOUND))
      break;
    p_term_xfr = p_rpnExpr->pPopChild();
    p_bound->vAppendChild(p_term_xfr);
  }


  // use last term to set bound symbol
  Card_Term* p_term_last = p_bound->pGetTerm_last();
  p_bound->pSymbol()->vCastFacets(p_term_last->pSymbol());

  // cross-check validity of bound symbol
  RASSERT(Card_Bound::bValidBoundForRelnOp(p_rpnItem->pSymbol(),
                                           p_term_last->pSymbol()));

  delete p_rpnItem;
  p_bound->vDeleteChild(p_term_last);

  //p_bound->vDump();
  //p_rpnExpr->vDump();

  return p_bound;
}

/*----------------------------------------------------------------------------*/
/* pop detailed typeId etc                                                    */
/*----------------------------------------------------------------------------*/
Cast Dissector::xPop_cast(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // TYPE

  if (p_rpnExpr->bHeadSame("TYPE"_Stz)) {
    Card_Term *p_term = _pCard_Term(p_rpnExpr->pPopChild());
    Stz *szTerm = p_term->szFacet(SF_NONE);
    RASSERT(szTerm->bSame("TYPE"_Stz));
    delete p_term;
    p_term = nullptr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column type (e.g. BOOLEAN)

  Card_Term* p_term_type = _pCard_Term(p_rpnExpr->pPopChild());
  TypeId xTypeId = Symbol::xTo_TypeId(p_term_type->szFacet(SF_NONE));
  delete p_term_type;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form cast

  Cast xCast;
  xCast.m_xTypeId = xTypeId;
  xCast.m_nTypeLength   = 0;
  xCast.m_nTypeScale    = 0;
  xCast.m_bTypeNullable = 1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // [ ( <length> [ , <scale> ] ) ]

  if (p_rpnExpr->bHeadSame("("_Stz)) {
    p_rpnExpr->vDiscardHead();

    Card_Term* p_term = _pCard_Term(p_rpnExpr->pPopChild());
    xCast.m_nTypeLength = p_term->szFacet(SF_INT_LIT)->iToI8();
    delete p_term;

    if (! p_rpnExpr->bHeadSame(")"_Stz)) {

      p_term = _pCard_Term(p_rpnExpr->pPopChild());
      xCast.m_nTypeScale = p_term->szFacet(SF_INT_LIT)->iToI8();
      delete p_term;
    }

    RASSERT(p_rpnExpr->bHeadSame(")"_Stz));
    p_rpnExpr->vDiscardHead();
  }

  return xCast;
}

/*----------------------------------------------------------------------------*/
/* pop constraint                                                             */
/*----------------------------------------------------------------------------*/
Card_Constraint* Dissector::pPop_constraint(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Cast xCast = xPop_cast(p_rpnExpr);

  Card_Constraint* p_constraint = new Card_Constraint;

  Card_RpnCalc* p_rpnCalc = pPop_rpnCalc(p_rpnExpr);

  p_rpnCalc->vCast(xCast);

  p_constraint->vAppendChild(p_rpnCalc);

  return p_constraint;
}

/*----------------------------------------------------------------------------*/
/* pop direction                                                              */
/*----------------------------------------------------------------------------*/
Card_Direction* Dissector::pPop_direction(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // AS
  Card_Term* p_term = _pCard_Term(p_rpnExpr->pPopChild());
  Stz* szDirn = p_term->szFacet(SF_NONE);
  RASSERT(szDirn->bSame("ASC"_Stz) || szDirn->bSame("DESC"_Stz));

  Card_Direction* p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, szDirn);

  delete p_term;
  p_term = nullptr;

  return p_direction;
}

/*----------------------------------------------------------------------------*/
/* pop expr                                                                   */
/*----------------------------------------------------------------------------*/
Card_Expr* Dissector::pPop_expr(Card_RpnExpr* p_rpnExpr,
                                bool bIsKeyExpr, i8& ri_Expr,
                                bool bEnsureDirection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Expr* p_expr = new Card_Expr;

  Symbol oSymbol_nmdCol;
  if (bIsKeyExpr) {
    Symbol xSymbol_expCol(SF_KEY_COL, ri_Expr);
    p_expr->pSymbol()->vCastFacets(&xSymbol_expCol);
  }
  else {
    Symbol xSymbol_expCol(SF_EXP_COL, ri_Expr);
    p_expr->pSymbol()->vCastFacets(&xSymbol_expCol);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // <type>

  Cast xCast = this->xPop_cast(p_rpnExpr);

  if (xCast.m_xTypeId == TypeId::xIdOf(CT_DECIMAL)) {
    xCast.m_nTypeLength = 19;
    xCast.m_nTypeScale = 2;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // [ AS <as> ]

  if (Card_As* p_as = pPop_as(p_rpnExpr)) {
    oSymbol_nmdCol.vCastFacets(p_as->pSymbol());
    p_expr->pSymbol()->vAppendFacets(&oSymbol_nmdCol);
    delete p_as;
    p_as = nullptr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // <rpnCalc>

  Card_RpnCalc* p_rpnCalc = pPop_rpnCalc(p_rpnExpr);
  p_expr->vAppendChild(p_rpnCalc);

  p_rpnCalc->vCast(xCast);
  p_expr->vCast(xCast);

  if (bIsKeyExpr && p_expr->pSub_rpnCalc()->nChildCards() == 1) {
    Card_Term* p_term_0 = p_expr->pGet_term0();
    p_term_0->vCast(xCast);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // [ AS <as> ]

  if (Card_As* p_as = pPop_as(p_rpnExpr)) {
    oSymbol_nmdCol.vCastFacets(p_as->pSymbol());
    p_expr->pSymbol()->vAppendFacets(&oSymbol_nmdCol);
    delete p_as;
    p_as = nullptr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // [ ASC | DESC ]

  Card* p_term_head = p_rpnExpr->pHead();
  if (p_term_head
      && (p_term_head->pSymbol()->bHasFacet("ASC"_Stz) ||
          p_term_head->pSymbol()->bHasFacet("DESC"_Stz))) {

    Card_Direction* p_direction = pPop_direction(p_rpnExpr);
    p_expr->vAppendChild(p_direction);
  }
  else
  if (bEnsureDirection) {
    Card_Direction* p_direction = (new Card_Direction) ->SYM(SF_DIRECTION, "ASC"_Stz);
    p_expr->vAppendChild(p_direction);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  ri_Expr++;
  return p_expr;
}

/*----------------------------------------------------------------------------*/
/* pop hshCol                                                                 */
/*----------------------------------------------------------------------------*/
Card_HshCol* Dissector::pPop_hshCol(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  if (! p_rpnExpr->pHead())
    return nullptr;

  if (p_rpnExpr->bHeadSame(","_Stz)) {
    Card*p_term_head = p_rpnExpr->pPopChild();
    delete p_term_head;
  }

  Card_HshCol* p_hshCol = new Card_HshCol;

  p_rpnExpr->vDiscardHead();

  Cast xCast = xPop_cast(p_rpnExpr);

  Card_Term* p_term_hsh = _pCard_Term(p_rpnExpr->pPopChild());
  p_term_hsh->vCast(xCast);
  p_hshCol->vAppendChild(p_term_hsh);

  return p_hshCol;
}

/*----------------------------------------------------------------------------*/
/* pop hshColList                                                             */
/*----------------------------------------------------------------------------*/
Card_HshColList* Dissector::pPop_hshColList(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_HshColList* p_hshColList = new Card_HshColList;

  while (Card_HshCol* p_hshCol = pPop_hshCol(p_rpnExpr)) {
    p_hshColList->vAppendChild(p_hshCol);
  }

  return p_hshColList;
}

/*----------------------------------------------------------------------------*/
/* pop key                                                                    */
/*----------------------------------------------------------------------------*/
Card_Key* Dissector::pPop_key(Card_RpnExpr* p_rpnExpr, bool bEnsureDirection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Key* p_key = new Card_Key;

  // GROUPBY
  Card_Term* p_term_head = _pCard_Term(p_rpnExpr->pHead());
  if (p_term_head->pSymbol()->bHasFacet("GROUPBY"_Stz) ||
      p_term_head->pSymbol()->bHasFacet("ORDGROUPBY"_Stz) ) {
    p_term_head = _pCard_Term(p_rpnExpr->pPopChild());
    delete p_term_head;
    p_term_head = nullptr;
  }

  i8 i_CardNo = 0;
  i8 i_Expr = 1;
  while ( (p_term_head = _pCard_Term(p_rpnExpr->pHead())) ) {

    // AGGREGATES
    if (p_term_head->pSymbol()->bHasFacet("AGGREGATES"_Stz)) {
      break;
    }

    // SEPARATOR
    if (p_term_head->pSymbol()->bIs_Comma()) {
      delete p_rpnExpr->pPopChild();
      continue;
    }

    // TYPE <type> <calcExpr> [ AS <as> ]
    Card_Expr* p_expr = pPop_expr(p_rpnExpr, true, i_Expr, bEnsureDirection);
    p_expr->v_Child(++i_CardNo);

//    if (bEnsureDirection) {
//      Card_Direction *p_direction = new Card_Direction("ASC"_Stz);
//      p_expr->vAppendChild(p_direction);
//    }

    if (p_expr->xTypeId() == TypeId::xIdOf(CT_DECIMAL)) {
      Cast xCast;
      xCast.m_xTypeId = TypeId::xIdOf(CT_DECIMAL);
      xCast.m_nTypeLength = 19;
      xCast.m_nTypeScale = 2;
      p_expr->vCast(xCast);
      p_expr->pSub_rpnCalc()->vCast(xCast);
    }

    p_key->vAppendChild(p_expr);

    if (1 == p_key->nChildCards())
      p_key->vCast(p_expr->xCast());
  }

  return p_key;
}

/*----------------------------------------------------------------------------*/
/* pop projection                                                             */
/*----------------------------------------------------------------------------*/
Card_Emit* Dissector::pPop_emit(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Emit* p_emit = new Card_Emit;

  i8 iCardNo = 0;

  if (p_rpnExpr->bHeadSame("AGGREGATES"_Stz))
    p_rpnExpr->vDiscardHead();

  i8 i_Expr = 1;

  while (Card_Term* p_term_head = _pCard_Term(p_rpnExpr->pHead())) {

    // GROUPBY or ORDGROUPBY
    if (p_term_head->pSymbol()->bHasFacet("GROUPBY"_Stz) ||
				p_term_head->pSymbol()->bHasFacet("ORDGROUPBY"_Stz)) {
      break;
    }

    // SEPARATOR
    if (p_term_head->pSymbol()->bIs_Comma()) {
      delete p_rpnExpr->pPopChild();
      continue;
    }

    // TYPE <type> [ AS <as> ] <rpnCalc>
    Card_Expr* p_expr = pPop_expr(p_rpnExpr, false, i_Expr, false);
    p_expr->v_Child(++iCardNo);
    //vAsEnsure(p_expr);

    if (p_expr->xTypeId() == TypeId::xIdOf(CT_DECIMAL)) {
      Cast xCast;
      xCast.m_xTypeId = TypeId::xIdOf(CT_DECIMAL);
      xCast.m_nTypeLength = 19;
      xCast.m_nTypeScale = 2;
      p_expr->vCast(xCast);
      p_expr->pSub_rpnCalc()->vCast(xCast);
    }


    p_emit->vAppendChild(p_expr);
  }

  return p_emit;
}

/*----------------------------------------------------------------------------*/
/* pop rpnCalc                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnCalc* Dissector::pPop_rpnCalc(Card_RpnExpr* p_rpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;

  Card_Term* p_term_popped = nullptr;
  while ( (p_term_popped = _pCard_Term(p_rpnExpr->pPopChild())) ) {

    if (
        p_term_popped->pSymbol()->bHasFacet("AGGREGATES"_Stz)
        || p_term_popped->pSymbol()->bHasFacet("AS"_Stz)
        || p_term_popped->pSymbol()->bHasFacet("ASC"_Stz)
        || p_term_popped->pSymbol()->bHasFacet("DESC"_Stz)
        || p_term_popped->pSymbol()->bHasFacet("GROUPBY"_Stz)
        || p_term_popped->pSymbol()->bIs_Comma()
        ) {
      p_rpnExpr->vPushChild(p_term_popped);
      break;
    }

    if (p_term_popped->pSymbol()->bHasFacet(SF_AGG_FN))
      p_rpnCalc->pSymbol()->Y_vSet(P_AggOp);

    p_rpnCalc->vAppendChild(p_term_popped);
  }

  return p_rpnCalc;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check number of args                                                       */
/*----------------------------------------------------------------------------*/
void Dissector::vCheckArgs(i8 nArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  i8 nTags = m_pOpArgs->nItems();

  if (nTags < nArgs ) {
    Stz* szOpNm = m_pOpSpec->szOpNm();
    vTrace("Error: only %lld tags in %s argTags\n"_Stz,nTags,szOpNm);
    NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* get rpnExpr arg                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnExpr* Dissector::pArg_rpnExpr(i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Stz* szRpnExpr = m_pOpArgs->szValue(i_Arg);

  Card_RpnExpr* p_rpnExpr = pTo_rpnExpr(szRpnExpr);

  return p_rpnExpr;
}

/*----------------------------------------------------------------------------*/
/* get stz arg                                                                */
/*----------------------------------------------------------------------------*/
Stz* Dissector::szArgValue(i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Stz* szRpnExpr = m_pOpArgs->szValue(i_Arg);

  return szRpnExpr;
}

/*----------------------------------------------------------------------------*/
/* get int arg                                                                */
/*----------------------------------------------------------------------------*/
i8 Dissector::iArgValue(i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  i8 iArg = m_pOpArgs->iValue(i_Arg);

  return iArg;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert rpn expr string into rpn terms                                     */
/*----------------------------------------------------------------------------*/
Card_RpnExpr* Dissector::pTo_rpnExpr(Stz* szRpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  s4K sString;

  Card_RpnExpr* p_rpnExpr = new Card_RpnExpr;
    
  TokenVec* pTokenVec = AM::TOKENISE::pTokeniseSen(szRpnExpr);
    
  i8 nTokens = pTokenVec->nItems();
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    Token* pToken = pTokenVec->pGet(i_Tkn);

    s256 sDissectedVal;
    if (pToken->eToken() == TN_STRING) {
      sDissectedVal.vSprint("'%s'"_Stz, pToken->szChars());
    }
    else {
      sDissectedVal = pToken->szChars();
    }

    eSF eSymbol = Symbol::eEvalSymbol(sDissectedVal);
    if (SF_NONE == eSymbol)
      eSymbol = SF_NMD_COL;
    Card* p_rpnItem = (new Card_Term) ->SYM(eSymbol, sDissectedVal);

    p_rpnExpr->vAppendChild(p_rpnItem);
  }
  
  delete pTokenVec;

  return p_rpnExpr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
