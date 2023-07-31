/*============================================================================*/
/* Dissector.cpp -- Dissector                                                     */
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
void Dissector::vSubPipe(i8 i_SubPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  Stz* sBoardNm;
  if (C_UNKNOWN == i_SubPipeSpec) {
    sBoardNm = "mainSQ"_Stz;
  }
  else {
    sBoardNm = m_pPipeSpec->szVertexNm();
  }
  m_pBoard = new Board(sBoardNm,
                       BK_SUBQUERY, i_SubPipeSpec + 1,
                       m_pBench);

  Stz* szQueryNm;
  if (MY::pJob()->bHasTag("<pipelineName>"_Stz))
    szQueryNm = MY::pJob()->pJobTags()->szValue("<pipelineName>"_Stz);
  else
    szQueryNm = "_unnamed_"_Stz;

  m_pOptimiser->vQueryNm(szQueryNm);
  
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
    

  Card_Stage* pCard_Stage = new Card_Stage;
  pCard_Stage->pSymbol()->vSet_stageNo(i_StageSpec);

  Card_Topology* pCard_StageTopo = new Card_Topology(eStageTopo);
  pCard_Stage->vAppend(pCard_StageTopo);

  m_pBoard->pDeck()->vAppend(pCard_Stage);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // params
  if (1 == i_StageSpec) {
    ParamsSpec* pParamsSpec = m_pDeckSpec->pParamsSpec();
    if (pParamsSpec) {
      Card_Correlation* p_paramList = new Card_Correlation;
      m_pBoard->pDeck()->vAppend(p_paramList);
      Card_OuterColNm* p_param = new Card_OuterColNm;
      StrLib::vStrip((char*)pParamsSpec->pGet(0)->szParamNm(),' ');
      p_param->vSymbol(pParamsSpec->pGet(0)->szParamNm());
      p_param->pSymbol()->Y_vSet(P_ParamOdn, "%1"_Stz);
      p_paramList->vAppend(p_param);
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
  
  Card* pCard;

  m_szOpNm = m_pOpSpec->szOpNm();

  m_pOpArgs = m_pOpSpec->pArgTags();

  //vTrace("Op %s\n"_Stz,m_szOpNm);
  //m_pOpArgs->vDumpSet(true);

       if (m_szOpNm->bSame("abstractJoin"_Stz)) pCard = pOp_AbsJoin     ();
  else if (m_szOpNm->bSame("aggreg"_Stz))       pCard = pOp_Aggreg      ();
  else if (m_szOpNm->bSame("call"_Stz))         pCard = pOp_Call        ();
  else if (m_szOpNm->bSame("combined"_Stz))     pCard = pOp_Combined    ();
  else if (m_szOpNm->bSame("consumeOp"_Stz))    pCard = pOp_Consume     ();
  else if (m_szOpNm->bSame("debug"_Stz))        pCard = pOp_Debug       ();
  else if (m_szOpNm->bSame("filter"_Stz))       pCard = pOp_Filter      ();
  else if (m_szOpNm->bSame("gbyAgg"_Stz))       pCard = pOp_GbyAgg      ();
  else if (m_szOpNm->bSame("hash"_Stz))         pCard = pOp_Hash        ();
  else if (m_szOpNm->bSame("indexJoin"_Stz))    pCard = pOp_IndexJoin   ();
  else if (m_szOpNm->bSame("indexScan"_Stz))    pCard = pOp_IndexScan   ();
  else if (m_szOpNm->bSame("keyExists"_Stz))    pCard = pOp_KeyExists   ();
  else if (m_szOpNm->bSame("keyNotExists"_Stz)) pCard = pOp_KeyNotExists();
  else if (m_szOpNm->bSame("limit"_Stz))        pCard = pOp_Limit       ();
  else if (m_szOpNm->bSame("ordagg"_Stz))       pCard = pOp_OrdAgg      ();
  else if (m_szOpNm->bSame("orderBy"_Stz))      pCard = pOp_Sort        ();
  else if (m_szOpNm->bSame("output"_Stz))       pCard = pOp_Output      ();
  else if (m_szOpNm->bSame("pulse"_Stz))        pCard = pOp_Pulse       ();
  else if (m_szOpNm->bSame("project"_Stz))      pCard = pOp_Project     ();
  else if (m_szOpNm->bSame("scan"_Stz))         pCard = pOp_Scan        ();
  else if (m_szOpNm->bSame("sendSE"_Stz))       pCard = pOp_Send        ();
  else if (m_szOpNm->bSame("sink"_Stz))         pCard = pOp_Sink        ();
  else if (m_szOpNm->bSame("sort"_Stz))         pCard = pOp_Sort        ();
  else if (m_szOpNm->bSame("start"_Stz))        pCard = pOp_Start       ();
  else if (m_szOpNm->bSame("top"_Stz))          pCard = pOp_Top         ();
  else {
    if (AM::JOB::bHasTag(C_GLEAN_INTERACTIVE))
      vTrace("Skipping OpSpec %s\n"_Stz, m_szOpNm);
    pCard = nullptr;
  }
  
  if (pCard)
    m_pBoard->pDeck()->vAppend(pCard);
}

/*----------------------------------------------------------------------------*/
/* dissect : abstractJoin <ON:szRpnExpr>                                      */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_AbsJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
  Card_AbsJoin* pCard_AbsJoin = new Card_AbsJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // expr : exression list

  Card_RpnCalc* pCard_RpnCalc = pPopRpnCalc(pCard_RpnTerms);
  Card_On* pCard_On = new Card_On;
  pCard_On->vAppend(pCard_RpnCalc);

  pCard_AbsJoin->vAppend(pCard_On);

  delete pCard_RpnTerms;

  return pCard_AbsJoin;
}

/*----------------------------------------------------------------------------*/
/* dissect : aggreg <szRpnExpr : 0) aggElist ; 1) gbyElist>                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Aggreg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  m_bSawAggOp = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Aggreg* pCard_Aggreg = new Card_Aggreg;
  
  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Aggreg->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Projection* pCard_Projection = pPopProjection(pCard_RpnTerms);
  pCard_Aggreg->vAppend(pCard_Projection);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;
    
  return pCard_Aggreg;
}

/*----------------------------------------------------------------------------*/
/* dissect : call <szSubPipe>                                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Call(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Stz* szSubPipeNm = szGetStz(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipeline

  StrLib::vStrip(szSubPipeNm->azChar(),' ');

  Card_Call* pCard_Call = new Card_Call;
  pCard_Call->vSymbol(szSubPipeNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Call;
}

/*----------------------------------------------------------------------------*/
/* dissect : combined <iGenerate>                                             */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  Card_Combined* pCard_Combine = new Card_Combined;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return pCard_Combine;
}

/*----------------------------------------------------------------------------*/
/* dissect : consumeOp <nSends>                                               */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Consume(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : debug <iOption>                                                  */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Debug(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  Stz* szOption = szGetStz(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Debug* pCard_Debug = new Card_Debug;

  pCard_Debug->pSymbol()->Y_vSet(P_DebugOption, szOption);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Debug;
}

/*----------------------------------------------------------------------------*/
/* dissect : filter <szRpnExpr: 0) filtElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
  Card_Filter* pCard_Filter = new Card_Filter;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // filtElist : filter expression list

  i8 i_Expr = 1;
  Card_Expr* pCard_Expr = pPopExpr(pCard_RpnTerms, false, i_Expr);

  Card_RpnCalc* pCard_RpnCalc = pCard_Expr->pSub_rpnCalc();
  pCard_Expr->vRemoveSub(pCard_RpnCalc);
  DELETE_CARD(pCard_Expr);

  pCard_Filter->vAppend(pCard_RpnCalc);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is it filter or having?
  
  if (m_bSawAggOp || m_bSawProjectOp) {
    Card_Having* pCard_Having = new Card_Having;
    pCard_Having->vTransferChildren(pCard_Filter);
    delete pCard_Filter;
    delete pCard_RpnTerms;

    eAL eAggLevel = m_pStageSpec->eAggegLevel();
    Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
    pCard_Having->vPush(pCard_AggLevel);

    return pCard_Having;
  }
  else {
    delete pCard_RpnTerms;

    pCard_Filter->vCard(CK_WHERE);
    Card_Where* p_where = _pCard_Where(pCard_Filter);

    eAL eAggLevel = m_pStageSpec->eAggegLevel();
    Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
    p_where->vPush(pCard_AggLevel);

    return p_where;
  }
}

/*----------------------------------------------------------------------------*/
/* dissect : gbyAgg <szRpnExpr : 0) aggElist ; 1) gbyElist>                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_GbyAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  m_bSawAggOp = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  Card_Aggreg* pCard_Aggreg = new Card_Aggreg;

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Aggreg->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gbyElist : optional groupBy expression list

  if (pCard_RpnTerms->nItems()) {
    Card_Key* pCard_Key = pPopKey(pCard_RpnTerms, true);
    pCard_Aggreg->vAppend(pCard_Key);
    pCard_Aggreg->vCard(CK_GBYAGG);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Projection* pCard_Projection = pPopProjection(pCard_RpnTerms);
  pCard_Aggreg->vAppend(pCard_Projection);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;

  //pCard_Aggreg->vDump(__func__ );
  return pCard_Aggreg;
}

/*----------------------------------------------------------------------------*/
/* dissect : hash <szRpnExpr : 0) calcExpr>                                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Hash(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Hash* pCard_Hash = new Card_Hash;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // calcExpr : calculation expression list

  Card_HshCol* p_hshCol = new Card_HshCol;

  Card_Term* p_term = _pCard_Term(pCard_RpnTerms->pGet(1));

  Card_Term* p_term2 = _pCard_Term(pCard_RpnTerms->pGet(2));

  p_hshCol->pSymbol()->vSet(p_term2->pSymbol());
  eCT eColumnType = Symbol::eTo_eCT_str(p_term->szSymbol());
  p_hshCol->pSymbol()->Y_vSet(P_Type,szCT_char[eColumnType]);

  Card_Term* p_term_hsh = p_term2->pClone();
  p_term_hsh->pSymbol()->Y_vSet(P_Type,szCT_char[eColumnType]);
  p_hshCol->vAppend(p_term_hsh);
  p_hshCol->pSymbol()->Y_vSet(P_BreakPt);

  pCard_Hash->vAppend(p_hshCol);

  delete pCard_RpnTerms;

  return pCard_Hash;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexJoin <szTblNm> <szIdxNm> <szRpnExpr: 0) idxPred>            */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_IndexJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(3);

  Stz* szTableNm = szGetStz(0);
  Stz* szIndexNm = szGetStz(1);
  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(2);

  //pCard_RpnTerms->vDump();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_IndexJoin->vAppend(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table
  
  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);
  pCard_IndexJoin->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Card_IndexList* p_indexList = new Card_IndexList;
  pCard_IndexJoin->vAppend(p_indexList);

  Index* pIndex = pTable->pFindIndex(szIndexNm);

  Card_Index* p_index = new Card_Index;
  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_index->vAppend(p_indexNm);
  p_indexNm->vSymbol(pIndex->szIndexNm());

  p_indexList->vAppend(p_index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* pCard_Bounds = new Card_Bounds;
  p_index->vAppend(pCard_Bounds);

  Card_Bound* pCard_Bound = new Card_Bound;


  pCard_Bound->vTransferChildren(pCard_RpnTerms);

  Card_Term* p_term_first = pCard_Bound->pGetTerm(0);
  if (p_term_first->bSameSymbol("LEFT_OUTER"_Stz)) {
    Card_OuterJoin* pCard_OuterJoin = new Card_OuterJoin;
    pCard_OuterJoin->pSymbol()->vSet(p_term_first->pSymbol());
    pCard_IndexJoin->vAppend(pCard_OuterJoin);
  }
  pCard_Bound->vDeleteChild(p_term_first);

  p_term_first = pCard_Bound->pGetTerm(0);
  if (p_term_first->bSameSymbol("BOUND"_Stz))
    pCard_Bound->vDeleteChild(p_term_first);

  i8 nTerms = pCard_Bound->nChildCards();

  Card_Term* p_term_last = pCard_Bound->pGetTerm(nTerms-1);
  pCard_Bound->pSymbol()->vSet(p_term_last->pSymbol());
  pCard_Bound->vDeleteChild(p_term_last);

  pCard_Bounds->vAppend(pCard_Bound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;

  //pCard_IndexJoin->vDump();

  return pCard_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexScan <szTblNm> <szIdxNm> <bounds>                           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_IndexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(3);

  Stz* szTableNm = szGetStz(0);
  Stz* szIndexNm = szGetStz(1);
  Card_RpnTerms* p_rpnTerms_bounds = pGetRpnTerms(2);

  //p_rpnTerms_bounds->vDump();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_IndexScan->vAppend(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);

  pCard_IndexScan->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Card_IndexList* p_indexList = new Card_IndexList;
  pCard_IndexScan->vAppend(p_indexList);

  Index* pIndex = pTable->pFindIndex(szIndexNm);

  Card_Index* p_index = new Card_Index;
  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_index->vAppend(p_indexNm);
  p_indexNm->vSymbol(pIndex->szIndexNm());

  p_indexList->vAppend(p_index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* pCard_Bounds = pPopBounds(p_rpnTerms_bounds);
  if (pCard_Bounds)
    p_index->vAppend(pCard_Bounds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnTerms_bounds;

  //pCard_IndexScan->vDump();

  return pCard_IndexScan;
}

/*----------------------------------------------------------------------------*/
/* dissect : keyExists <tableNm> <indexNm> <bounds> <constraint>              */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_KeyExists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DISSECTOR, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(4);
  Stz* szTableNm = szGetStz(0);
  Stz* szIndexNm = szGetStz(1);
  Card_RpnTerms* p_rpnTerms_bounds = pGetRpnTerms(2);
  Card_RpnTerms* p_rpnTerms_constraint = pGetRpnTerms(3);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // keyExists

  Card_KeyExists* pCard_KeyExists = new Card_KeyExists;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_KeyExists->vAppend(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);
  pCard_KeyExists->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index list

  Card_IndexList* p_indexList = pFormIndex(szIndexNm,pTable);
  pCard_KeyExists->vAppend(p_indexList);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* pCard_Bounds = pPopBounds(p_rpnTerms_bounds);
  pCard_Bounds->vDump(__func__);
  p_indexList->pSub_index()->vAppend(pCard_Bounds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // constraint

  Card_Constraint* pCard_Constraint = pPopConstraint(p_rpnTerms_constraint);
  pCard_Constraint->vDump(__func__);
  pCard_KeyExists->vAppend(pCard_Constraint);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnTerms_bounds;
  delete p_rpnTerms_constraint;

  pCard_KeyExists->vDump(__func__);

  return pCard_KeyExists;
}

/*----------------------------------------------------------------------------*/
/* dissect : keyNotExists <tableNm> <indexNm> <bounds> <constraint>           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_KeyNotExists(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DISSECTOR, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(4);
  Stz* szTableNm = szGetStz(0);
  Stz* szIndexNm = szGetStz(1);
  Card_RpnTerms* p_rpnTerms_bounds = pGetRpnTerms(2);
  Card_RpnTerms* p_rpnTerms_constraint = pGetRpnTerms(3);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // keyExists

  Card_KeyNotExists* pCard_KeyNotExists = new Card_KeyNotExists;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_KeyNotExists->vAppend(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);
  pCard_KeyNotExists->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index list

  Card_IndexList* p_indexList = pFormIndex(szIndexNm,pTable);
  pCard_KeyNotExists->vAppend(p_indexList);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* pCard_Bounds = pPopBounds(p_rpnTerms_bounds);
  pCard_Bounds->vDump(__func__);
  p_indexList->pSub_index()->vAppend(pCard_Bounds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // constraint

  Card_Constraint* pCard_Constraint = pPopConstraint(p_rpnTerms_constraint);
  pCard_Constraint->vDump(__func__);
  pCard_KeyNotExists->vAppend(pCard_Constraint);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnTerms_bounds;
  delete p_rpnTerms_constraint;

  pCard_KeyNotExists->vDump(__func__);

  return pCard_KeyNotExists;
}

/*----------------------------------------------------------------------------*/
/* dissect : limit <nRows>                                                    */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Limit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  //m_pOpArgs->vDumpSet(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  i8 nRows = iGetInt(0);

  s32 sRows;
  sRows.vSprint("%lld"_Stz,nRows);

  Card_Limit* pCard_Limit = new Card_Limit(sRows);

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Limit->vAppend(pCard_AggLevel);

  return pCard_Limit;
}

/*----------------------------------------------------------------------------*/
/* dissect : ordAgg <szRpnExpr : 0) aggElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_OrdAgg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  m_bSawAggOp = true;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
  Card_OrdAgg* pCard_OrdAgg = new Card_OrdAgg;
  
  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_OrdAgg->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Projection* pCard_Projection = pPopProjection(pCard_RpnTerms);
  pCard_OrdAgg->vAppend(pCard_Projection);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;

  return pCard_OrdAgg;
}

/*----------------------------------------------------------------------------*/
/* dissect : output                                                           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Output(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : pulse <szMode>                                                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Pulse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : project <szRpnExpr : 0) projElist>                               */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Project(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  m_bSawProjectOp = true;
  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
  Card_Project* pCard_Project = new Card_Project(CK_PROJECT);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // projElist : project expression list

  Card_Projection* pCard_Projection = pPopProjection(pCard_RpnTerms);
  pCard_Project->vAppend(pCard_Projection);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Project->vPush(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;

  return pCard_Project;
}

/*----------------------------------------------------------------------------*/
/* dissect : scan [ <szCatalogNm> <szSchemaNm> ] <szTableNm>                  */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Scan(void)
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scan

  Card_Scan* pOp_Scan = new Card_Scan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pOp_Scan->vAppend(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form table
  Table* pTable = AM::pTableFind(szTableNm);

  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);
  pOp_Scan->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pOp_Scan;
}

/*----------------------------------------------------------------------------*/
/* dissect : send <eSendKind>                                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments
  vCheckArgs(1);

  eXK eSendKind = (eXK) iGetInt(0);
  Card_Send* pCard_Send = new Card_Send;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // eSendKind

  pCard_Send->pSymbol()->Y_vSet(P_SendKind, (i8)eSendKind);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Send;
}

/*----------------------------------------------------------------------------*/
/* dissect : sink                                                             */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Sink(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : orderBy <szRpnExpr: 0) sbyElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Sort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
  Card_Sort* pCard_Sort = new Card_Sort;

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Sort->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sbyElist : sortBy expression list

  Card_Key* pCard_Key = pPopKey(pCard_RpnTerms, false);
  pCard_Sort->vAppend(pCard_Key);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;

  return pCard_Sort;
}

/*----------------------------------------------------------------------------*/
/* dissect : start                                                            */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Start(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : top                                                              */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Top(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // op arguments

  vCheckArgs(2);

  i8 iCount = iGetInt(1);

  s32 sCount;
  sCount = iCount;
  Card_Top* pCard_Top = new Card_Top(sCount);


  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Top->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sbyElist : sortBy expression list

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
  Card_Key* pCard_Key = pPopKey(pCard_RpnTerms, false);
  pCard_Top->vAppend(pCard_Key);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Count* pCard_Count = new Card_Count;
  pCard_Count->pSymbol()->vSet_number(iCount);
  pCard_Top->vAppend(pCard_Count);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;

  return pCard_Top;
}
/*----------------------------------------------------------------------------*/
//Card* Dissector::pOp_Top(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // op arguments
//
//  vCheckArgs(2);
//
//
//  i8 i_Expr = 1;
//  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
//  Card_Expr* pCard_Expr = pPopExpr(pCard_RpnTerms, false, i_Expr);
//
//  i8 iCount = iGetInt(1);
//
//  s32 sCount;
//  sCount = iCount;
//  Card_Top* pCard_Top = new Card_Top(sCount);
//
//  eAL eAggLevel = m_pStageSpec->eAggegLevel();
//  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
//  pCard_Top->vPush(pCard_AggLevel);
//
//  //pCard_Top->vAppend(pCard_Expr);
//
//  Card_Key* p_key = new Card_Key;
//
//  p_key->vAppend(pCard_Expr);
//  pCard_Top->vAppend(p_key);
//
//  Symbol xSymbol;
//  xSymbol = iCount;
//  Card_Count* pCard_Count = new Card_Count;
//  pCard_Count->pSymbol()->vSet(&xSymbol);
//  pCard_Top->vAppend(pCard_Count);
//
//  //pCard_Top->vDump(__func__);
//
//
//  return pCard_Top;
//}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* form table                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexList* Dissector::pFormIndex(Stz* szIndexNm, Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Index* pIndex = pTable->pFindIndex(szIndexNm);

  Card_IndexList* p_indexList = new Card_IndexList;

  Card_Index* p_index = new Card_Index;

  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_indexNm->vSymbol(pIndex->szIndexNm());

  p_index->vAppend(p_indexNm);
  p_indexList->vAppend(p_index);

  return p_indexList;
}

/*----------------------------------------------------------------------------*/
/* form table                                                                 */
/*----------------------------------------------------------------------------*/
Card_Table* Dissector::pFormTable(Stz* szTableNm, Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Table* pCard_Table = new Card_Table;
  pCard_Table->pSymbol()->Y_vSet(P_Pointer,pTable);

  Card_TableNm* pCard_TableNm = new Card_TableNm(szTableNm);
  pCard_Table->vAppend(pCard_TableNm);

  return pCard_Table;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect <as> terms : AS <name>                                             */
/*----------------------------------------------------------------------------*/
Card_As* Dissector::pPopAs(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  RASSERT(pCard_Term->bSameSymbol("AS"_Stz));
  delete pCard_Term;
  pCard_Term = nullptr;

  // <name>
  pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szAsNm = pCard_Term->szSymbol();

  Card_As* pCard_As = new Card_As(szAsNm);
  delete pCard_Term;
  pCard_Term = nullptr;

  return pCard_As;
}

/*----------------------------------------------------------------------------*/
/* pop bounds                                                                 */
/*----------------------------------------------------------------------------*/
Card_Bounds* Dissector::pPopBounds(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Bounds* pCard_Bounds = new Card_Bounds;

  while (Card_Bound* pCard_Bound = pPopBound(pCard_RpnTerms))
    pCard_Bounds->vAppend(pCard_Bound);

  return pCard_Bounds;
}

/*----------------------------------------------------------------------------*/
/* pop bound                                                                  */
/*----------------------------------------------------------------------------*/
Card_Bound* Dissector::pPopBound(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  //pCard_RpnTerms->vDump();

  // is there a bound term sequence at the top of rpnTerms?
  Card_Term* p_term_first = _pCard_Term(pCard_RpnTerms->pPeek(0));
  if (! p_term_first || ! p_term_first->pSymbol()->bIsBound())
    return nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pop one (l/u) bound term sequence as a card bound

  Card_Bound* pCard_Bound = new Card_Bound;

  // the first term in sequence will be a LBOUND or UBOUND
  //   which is not transfered but should match last reln op in sequence
  Card_Term* p_term_bound = pCard_RpnTerms->pPop_term();
  RASSERT(p_term_bound->pSymbol()->bIsBound());

  // transfer terms for this bound sequence
  Card_Term* p_term_xfr;
  for (i8 i_Loop = 0; ; i_Loop++) {
    Card_Term* p_term_top = _pCard_Term(pCard_RpnTerms->pPeek(0));
    if (! p_term_top || p_term_top->pSymbol()->bIsBound())
      break;
    p_term_xfr = pCard_RpnTerms->pPop_term();
    pCard_Bound->vAppend(p_term_xfr);
  }


  // use last term to set bound symbol
  Card_Term* p_term_last = pCard_Bound->pGetTerm_last();
  pCard_Bound->pSymbol()->vSet(p_term_last->pSymbol());

  // cross-check validity of bound symbol
  RASSERT(Card_Bound::bValidBoundForRelnOp(p_term_bound->pSymbol(),
                                           p_term_last->pSymbol()));

  delete p_term_bound;
  pCard_Bound->vDeleteChild(p_term_last);

  //pCard_Bound->vDump();
  //pCard_RpnTerms->vDump();

  return pCard_Bound;
}

/*----------------------------------------------------------------------------*/
/* pop constraint                                                             */
/*----------------------------------------------------------------------------*/
Card_Constraint* Dissector::pPopConstraint(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Constraint* pCard_Constraint = new Card_Constraint;

  Stz* szType = szPopType_char(pCard_RpnTerms);

  Card_RpnCalc* pCard_RpnCalc = pPopRpnCalc(pCard_RpnTerms);

  pCard_RpnCalc->pSymbol()->vColType(szType);

  pCard_Constraint->vAppend(pCard_RpnCalc);

  return pCard_Constraint;
}

/*----------------------------------------------------------------------------*/
/* pop direction                                                              */
/*----------------------------------------------------------------------------*/
Card_Direction* Dissector::pPopDirection(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szDirn = pCard_Term->szSymbol();
  RASSERT(szDirn->bSame("ASC"_Stz) || szDirn->bSame("DESC"_Stz));

  Card_Direction* pCard_Direction = new Card_Direction(szDirn);

  delete pCard_Term;
  pCard_Term = nullptr;

  return pCard_Direction;
}

/*----------------------------------------------------------------------------*/
/* pop expr                                                                   */
/*----------------------------------------------------------------------------*/
Card_Expr* Dissector::pPopExpr(Card_RpnTerms* pCard_RpnTerms, bool bIsKeyExpr, i8& ri_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Expr* pCard_Expr = new Card_Expr;

  if (bIsKeyExpr) {
    Symbol xSymbol_stmColOdn;
    xSymbol_stmColOdn.vSetKeyOrdinal(ri_Expr);
    pCard_Expr->pSymbol()->vSet(&xSymbol_stmColOdn);
  }
  else {
    Symbol xSymbol_stmColOdn;
    xSymbol_stmColOdn.vSetStreamOrdinal(ri_Expr);
    pCard_Expr->pSymbol()->vSet(&xSymbol_stmColOdn);
  }

  Stz* szP_Type = this->szPopType_char(pCard_RpnTerms);


  // [ AS <as> ]
  Card* pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && pCard_Term_head->bSameSymbol("AS"_Stz)) {

    Card_As* pCard_As = pPopAs(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_As);
  }


  // <rpnCalc>
  Card_RpnCalc* pCard_RpnCalc = pPopRpnCalc(pCard_RpnTerms);
  pCard_Expr->vAppend(pCard_RpnCalc);

  pCard_RpnCalc->pSymbol()->Y_vSet(P_Type, szP_Type);
  pCard_Expr->pSymbol()->Y_vSet(P_Type, szP_Type);


  // [ AS <as> ]
  pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && pCard_Term_head->bSameSymbol("AS"_Stz)) {

    Card_As* pCard_As = pPopAs(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_As);
  }


  // [ ASC | DESC ]
  pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && (pCard_Term_head->bSameSymbol("ASC"_Stz) ||
          pCard_Term_head->bSameSymbol("DESC"_Stz))) {

    Card_Direction* pCard_Direction = pPopDirection(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_Direction);
  }


  if (! pCard_Expr->pSub_as()) {
    Card_As* pCard_As = new Card_As(ri_Expr);
    pCard_Expr->vInsert(pCard_As);
  }

  ri_Expr++;
  return pCard_Expr;
}

/*----------------------------------------------------------------------------*/
/* pop key                                                                    */
/*----------------------------------------------------------------------------*/
Card_Key* Dissector::pPopKey(Card_RpnTerms* pCard_RpnTerms, bool bAsc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Key* pCard_Key = new Card_Key;

  // GROUPBY
  Card_Term* pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPeek(0));
  if (pCard_Term_head->bSameSymbol("GROUPBY"_Stz)) {
    pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPop());
    delete pCard_Term_head;
    pCard_Term_head = nullptr;
  }

  i8 i_CardNo = 0;
  i8 i_Expr = 1;
  while ( (pCard_Term_head = _pCard_Term(pCard_RpnTerms->pHead())) ) {

    // AGGREGATES
    if (pCard_Term_head->bSameSymbol("AGGREGATES"_Stz)) {
      break;
    }

    // SEPARATOR
    if (pCard_Term_head->pSymbol()->bIsComma()) {
      delete pCard_RpnTerms->pPop();
      continue;
    }

    // TYPE <type> <calcExpr> [ AS <as> ]
    Card_Expr* pCard_Expr = pPopExpr(pCard_RpnTerms, true, i_Expr);
    pCard_Expr->v_Child(++i_CardNo);

    if (bAsc) {
      Card_Direction *p_direction = new Card_Direction("ASC"_Stz);
      pCard_Expr->vAppend(p_direction);
    }

    pCard_Key->vAppend(pCard_Expr);
  }

  return pCard_Key;
}

/*----------------------------------------------------------------------------*/
/* pop projection                                                             */
/*----------------------------------------------------------------------------*/
Card_Projection* Dissector::pPopProjection(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Projection* pCard_Projection = new Card_Projection(0);

  i8 iCardNo = 0;

  Card_Term* pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPeek(0));
  if (pCard_Term_head->bSameSymbol("AGGREGATES"_Stz)) {
    pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPop());
    delete pCard_Term_head;

  }

  i8 i_Expr = 1;

  while ( (pCard_Term_head = _pCard_Term(pCard_RpnTerms->pHead())) ) {

    // GROUPBY
    if (pCard_Term_head->bSameSymbol("GROUPBY"_Stz)) {
      break;
    }

    // SEPARATOR
    if (pCard_Term_head->pSymbol()->bIsComma()) {
      delete pCard_RpnTerms->pPop();
      continue;
    }

    // TYPE <type> [ AS <as> ] <rpnCalc>
    Card_Expr* pCard_Expr = pPopExpr(pCard_RpnTerms, false, i_Expr);
    pCard_Expr->v_Child(++iCardNo);
    //vAsEnsure(pCard_Expr);

    pCard_Projection->vAppend(pCard_Expr);
  }

  return pCard_Projection;
}

/*----------------------------------------------------------------------------*/
/* pop rpnCalc                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnCalc* Dissector::pPopRpnCalc(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_RpnCalc* pCard_RpnCalc = new Card_RpnCalc;

  Card_Term* pCard_Term_popped = nullptr;
  while ( (pCard_Term_popped = _pCard_Term(pCard_RpnTerms->pPop())) ) {

    if (
        pCard_Term_popped->bSameSymbol("AGGREGATES"_Stz)
        || pCard_Term_popped->bSameSymbol("AS"_Stz)
        || pCard_Term_popped->bSameSymbol("ASC"_Stz)
        || pCard_Term_popped->bSameSymbol("DESC"_Stz)
        || pCard_Term_popped->bSameSymbol("GROUPBY"_Stz)
        || pCard_Term_popped->pSymbol()->bIsComma()
        ) {
      pCard_RpnTerms->vPush(pCard_Term_popped);
      break;
    }

    if (pCard_Term_popped->pSymbol()->bIsAggOp())
      pCard_RpnCalc->pSymbol()->Y_vSet(P_AggOp);

    pCard_RpnCalc->vAppend(pCard_Term_popped);
  }

  return pCard_RpnCalc;
}

/*----------------------------------------------------------------------------*/
/* pop type (single char form)                                                */
/*----------------------------------------------------------------------------*/
Stz* Dissector::szPopType_char(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // TYPE
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szTerm = pCard_Term->szSymbol();
  RASSERT(szTerm->bSame("TYPE"_Stz));

  delete pCard_Term;
  pCard_Term = nullptr;

  // column type (e.g. BOOLEAN)
  Card_Term* pCard_Term_type = _pCard_Term(pCard_RpnTerms->pPop());

  Stz* szType = szCT_char[Symbol::eTo_eCT_str(pCard_Term_type->szSymbol())];

  delete pCard_Term_type;

  return szType;
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
/* get int arg                                                                */
/*----------------------------------------------------------------------------*/
i8 Dissector::iGetInt(i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  i8 iArg = m_pOpArgs->iValue(i_Arg);

  return iArg;
}

/*----------------------------------------------------------------------------*/
/* get rpnTerms arg                                                           */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissector::pGetRpnTerms(i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Stz* szRpnExpr = m_pOpArgs->szValue(i_Arg);
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);

  return pCard_RpnTerms;
}

/*----------------------------------------------------------------------------*/
/* get stz arg                                                                */
/*----------------------------------------------------------------------------*/
Stz* Dissector::szGetStz(i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Stz* szRpnExpr = m_pOpArgs->szValue(i_Arg);

  return szRpnExpr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert rpn expr string into rpn terms                                     */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissector::pToRpnTerms(Stz* szRpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  s4K sString;

  Card_RpnTerms* pCard_RpnTerms = new Card_RpnTerms;
    
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

    Card_Term* pCard_Term = new Card_Term(sDissectedVal);

    pCard_RpnTerms->vAppend(pCard_Term);
  }
  
  delete pTokenVec;

  return pCard_RpnTerms;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
