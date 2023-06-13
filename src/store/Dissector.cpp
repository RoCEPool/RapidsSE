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
  pCard_Stage->vSymbolSprint("S%02lld"_Stz, i_StageSpec);

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
  Stz* szOpNm = m_pOpSpec->szOpNm();
  TagSet* pArgTags = m_pOpSpec->pArgTags();

  //vTrace("Op %s\n"_Stz,szOpNm);
  //pArgTags->vDumpSet(true);

       if (szOpNm->bSame("abstractJoin"_Stz)) pCard = pOp_AbsJoin     (pArgTags);
  else if (szOpNm->bSame("aggreg"_Stz))       pCard = pOp_Aggreg      (pArgTags);
  else if (szOpNm->bSame("call"_Stz))         pCard = pOp_Call        (pArgTags);
  else if (szOpNm->bSame("combined"_Stz))     pCard = pOp_Combined    (pArgTags);
  else if (szOpNm->bSame("consumeOp"_Stz))    pCard = pOp_Consume     (pArgTags);
  else if (szOpNm->bSame("debug"_Stz))        pCard = pOp_Debug       (pArgTags);
  else if (szOpNm->bSame("filter"_Stz))       pCard = pOp_Filter      (pArgTags);
  else if (szOpNm->bSame("gbyAgg"_Stz))       pCard = pOp_GbyAgg      (pArgTags);
  else if (szOpNm->bSame("hash"_Stz))         pCard = pOp_Hash        (pArgTags);
  else if (szOpNm->bSame("indexJoin"_Stz))    pCard = pOp_IndexJoin   (pArgTags);
  else if (szOpNm->bSame("indexScan"_Stz))    pCard = pOp_IndexScan   (pArgTags);
  else if (szOpNm->bSame("keyExists"_Stz))    pCard = pOp_KeyExists   (pArgTags);
  else if (szOpNm->bSame("keyNotExists"_Stz)) pCard = pOp_KeyNotExists(pArgTags);
  else if (szOpNm->bSame("limit"_Stz))        pCard = pOp_Limit       (pArgTags);
  else if (szOpNm->bSame("ordagg"_Stz))       pCard = pOp_OrdAgg      (pArgTags);
  else if (szOpNm->bSame("orderBy"_Stz))      pCard = pOp_Sort        (pArgTags);
  else if (szOpNm->bSame("output"_Stz))       pCard = pOp_Output      (pArgTags);
  else if (szOpNm->bSame("pulse"_Stz))        pCard = pOp_Pulse       (pArgTags);
  else if (szOpNm->bSame("project"_Stz))      pCard = pOp_Project     (pArgTags);
  else if (szOpNm->bSame("scan"_Stz))         pCard = pOp_Scan        (pArgTags);
  else if (szOpNm->bSame("sendSE"_Stz))       pCard = pOp_Send        (pArgTags);
  else if (szOpNm->bSame("sink"_Stz))         pCard = pOp_Sink        (pArgTags);
  else if (szOpNm->bSame("sort"_Stz))         pCard = pOp_Sort        (pArgTags);
  else if (szOpNm->bSame("start"_Stz))        pCard = pOp_Start       (pArgTags);
  else {
    vTrace("Skipping OpSpec %s\n"_Stz,szOpNm);
    pCard = nullptr;
  }
  
  if (pCard)
    m_pBoard->pDeck()->vAppend(pCard);
}

/*----------------------------------------------------------------------------*/
/* dissect : abstractJoin <szRpnExpr>                                         */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_AbsJoin(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags, 0);
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
Card* Dissector::pOp_Aggreg(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  m_bSawAggOp = true;
  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags, 0);
  Card_Aggreg* pCard_Aggreg = new Card_Aggreg;
  
  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Aggreg->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gbyElist : optional groupBy expression list

//  if (pCard_RpnTerms->nItems()) {
//    Card_Key* pCard_Key = pPopKey(pCard_RpnTerms,true);
//    pCard_Aggreg->vAppend(pCard_Key);
//    pCard_Aggreg->vCard(CK_GBYAGG);
//  }

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
Card* Dissector::pOp_Call(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  pArgTags->vDumpSet(true);

  Stz* szSubPipeNm = szGetStz(pArgTags,0);

  StrLib::vStrip(szSubPipeNm->azChar(),' ');

  Card_Call* pCard_Call = new Card_Call;
  pCard_Call->vSymbol(szSubPipeNm);

  return pCard_Call;
}

/*----------------------------------------------------------------------------*/
/* dissect : combined <iGenerate>                                             */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Combined(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  Card_Combined* pCard_Combine = new Card_Combined;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return pCard_Combine;
}

/*----------------------------------------------------------------------------*/
/* dissect : consumeOp <nSends>                                               */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Consume(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : debug <iOption>                                                  */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Debug(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  Stz* szOption = szGetStz(pArgTags, 0);
  Card_Debug* pCard_Debug = new Card_Debug;

  pCard_Debug->pSymbol()->Y_vSet(P_DebugOption, szOption);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Debug;
}

/*----------------------------------------------------------------------------*/
/* dissect : filter <szRpnExpr: 0) filtElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Filter(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags, 0);
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
    return pCard_Filter;
  }
}

/*----------------------------------------------------------------------------*/
/* dissect : gbyAgg <szRpnExpr : 0) aggElist ; 1) gbyElist>                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_GbyAgg(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  m_bSawAggOp = true;
  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags, 0);
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

  return pCard_Aggreg;
}

/*----------------------------------------------------------------------------*/
/* dissect : hash <szRpnExpr : 0) calcExpr>                                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Hash(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags, 0);
  Card_Hash* pCard_Hash = new Card_Hash;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // calcExpr : calculation expression list

  //pCard_Hash->vTransferChildren(pCard_RpnTerms);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_HshCol* p_hshCol = new Card_HshCol;

  Card_Term* p_term = _pCard_Term(pCard_RpnTerms->pGet(1));

  Card_Term* p_term2 = _pCard_Term(pCard_RpnTerms->pGet(2));

  p_hshCol->vSymbol(p_term2->pSymbol());
  eCT eColumnType = Symbol::eTo_eCT_str(p_term->szSymbol());
  p_hshCol->pSymbol()->Y_vSet(P_Type,szCT_char[eColumnType]);

  pCard_Hash->vAppend(p_hshCol);

  delete pCard_RpnTerms;

  return pCard_Hash;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexJoin <szTblNm> <szIdxNm> <szRpnExpr: 0) idxPred>            */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_IndexJoin(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 3);

  vTrace("pCard_IndexJoin\n"_Stz);
  pArgTags->vDumpSet(true);

  Stz* szTableNm = szGetStz(pArgTags,0);
  Stz* szIndexNm = szGetStz(pArgTags,1);
  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags,2);

  pCard_RpnTerms->vDump();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_IndexJoin->vAppend(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table
  
  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table;
  pCard_Table->pSymbol()->Y_vSet(P_Pointer,pTable);
  pCard_IndexJoin->vAppend(pCard_Table);

  Card_TableNm* pCard_TableNm = new Card_TableNm;
  pCard_TableNm->vSymbol(szTableNm);
  pCard_Table->vAppend(pCard_TableNm);

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
  i8 nTerms = pCard_Bound->nChildCards();
  Card_Term* p_term_last = pCard_Bound->pGetTerm(nTerms-1);
  pCard_Bound->pSymbol()->vSet(p_term_last->pSymbol());
  pCard_Bound->vDeleteChild(p_term_last);
  Card_Term* p_term_first = pCard_Bound->pGetTerm(0);
  pCard_Bound->vDeleteChild(p_term_first);

  pCard_Bounds->vAppend(pCard_Bound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;

  pCard_IndexJoin->vDump();

  return pCard_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexScan <szTblNm> <szIdxNm> <bounds>                           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_IndexScan(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 3);

  Stz* szTableNm = szGetStz(pArgTags,0);
  Stz* szIndexNm = szGetStz(pArgTags,1);
  Card_RpnTerms* p_rpnTerms_bounds = pGetRpnTerms(pArgTags, 2);

  p_rpnTerms_bounds->vDump();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_IndexScan->vAppend(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table;
  pCard_Table->pSymbol()->Y_vSet(P_Pointer,pTable);
  pCard_IndexScan->vAppend(pCard_Table);

  Card_TableNm* pCard_TableNm = new Card_TableNm;
  pCard_TableNm->vSymbol(szTableNm);
  pCard_Table->vAppend(pCard_TableNm);

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

  Card_Bounds* pCard_Bounds = new Card_Bounds;
  p_index->vAppend(pCard_Bounds);

  Card_Bound* pCard_Bound = new Card_Bound;


  pCard_Bound->vTransferChildren(p_rpnTerms_bounds);
  i8 nTerms = pCard_Bound->nChildCards();
  Card_Term* p_term_last = pCard_Bound->pGetTerm(nTerms-1);
  pCard_Bound->pSymbol()->vSet(p_term_last->pSymbol());
  pCard_Bound->vDeleteChild(p_term_last);
  Card_Term* p_term_first = pCard_Bound->pGetTerm(0);
  pCard_Bound->vDeleteChild(p_term_first);

  pCard_Bounds->vAppend(pCard_Bound);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnTerms_bounds;


//  CardIter xCI(pCard_IndexScan);
//
//  pCard_IndexScan->vDump();
//  pCard_IndexScan->vUpdate_projection(&xCI);
//  pCard_IndexScan->vDump();
//
//  pCard_IndexScan->vValidate(&xCI,0,nullptr);

  pCard_IndexScan->vDump();

  return pCard_IndexScan;
}

/*----------------------------------------------------------------------------*/
/* dissect : keyExists <tableNm> <indexNm> <bounds> <constraint>              */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_KeyExists(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DISSECTOR, _FOOTER_);

  vCheckArgs(pArgTags, 4);

  Stz* szTableNm = szGetStz(pArgTags,0);
  Stz* szIndexNm = szGetStz(pArgTags,1);
  Card_RpnTerms* p_rpnTerms_bounds = pGetRpnTerms(pArgTags, 2);

  p_rpnTerms_bounds->vDump();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_KeyExists* pCard_KeyExists = new Card_KeyExists;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_KeyExists->vAppend(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table;
  pCard_Table->pSymbol()->Y_vSet(P_Pointer,pTable);
  pCard_KeyExists->vAppend(pCard_Table);

  Card_TableNm* pCard_TableNm = new Card_TableNm;
  pCard_TableNm->vSymbol(szTableNm);
  pCard_Table->vAppend(pCard_TableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Card_IndexList* p_indexList = new Card_IndexList;
  pCard_KeyExists->vAppend(p_indexList);

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


  pCard_Bound->vTransferChildren(p_rpnTerms_bounds);
  i8 nTerms = pCard_Bound->nChildCards();
  Card_Term* p_term_last = pCard_Bound->pGetTerm(nTerms-1);
  pCard_Bound->pSymbol()->vSet(p_term_last->pSymbol());
  pCard_Bound->vDeleteChild(p_term_last);
  Card_Term* p_term_first = pCard_Bound->pGetTerm(0);
  pCard_Bound->vDeleteChild(p_term_first);

  pCard_Bounds->vAppend(pCard_Bound);



  //Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags,0);
  Card_Filter* pCard_Filter = new Card_Filter;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // filtElist : filter expression list

  i8 i_Expr = 1;
  Card_Expr* pCard_Expr = pPopExpr(p_rpnTerms_bounds, false, i_Expr);

  Card_RpnCalc* pCard_RpnCalc = pCard_Expr->pSub_rpnCalc();
  pCard_Expr->vRemoveSub(pCard_RpnCalc);
  DELETE_CARD(pCard_Expr);

  pCard_Filter->vAppend(pCard_RpnCalc);

  delete p_rpnTerms_bounds;


  return pCard_KeyExists;
}

/*----------------------------------------------------------------------------*/
/* dissect : keyNotExists <tableNm> <indexNm> <bounds> <constraint>           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_KeyNotExists(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_DISSECTOR, _FOOTER_);

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : limit <nRows>                                                    */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Limit(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  //pArgTags->vDumpSet(true);

  vCheckArgs(pArgTags, 1);

  i8 nRows = iGetInt(pArgTags, 0);

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
Card* Dissector::pOp_OrdAgg(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  m_bSawAggOp = true;
  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags, 0);
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
Card* Dissector::pOp_Output(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : pulse <szMode>                                                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Pulse(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : project <szRpnExpr : 0) projElist>                               */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Project(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  m_bSawProjectOp = true;
  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags, 0);
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

//  Card_RpnCalc* p_rpnCalc = pCard_Project->pSub_rpnCalc();
//  if (p_rpnCalc && p_rpnCalc->pSymbol()->Y_bHas(P_AggOp))
//    pCard_Project->vCard(CK_AGGREG);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;

  return pCard_Project;
}

/*----------------------------------------------------------------------------*/
/* dissect : scan [ <szCatalogNm> <szSchemaNm> ] <szTableNm>                  */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Scan(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  i8 nTags = pArgTags->nItems();
  
  Stz* szTableNm = nullptr;
  if (3 == nTags) {
    szTableNm = pArgTags->szValue(2);
  }
  else if (1 == nTags) {
    szTableNm = pArgTags->szValue((i8)0);
  }
  else {
    vTrace("Error: only %lld tags in scan argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  Card_Scan* pOp_Scan = new Card_Scan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table
  
  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table;
  Card_TableNm* pCard_TableNm = new Card_TableNm(szTableNm);
  pCard_Table->vAppend(pCard_TableNm);
  pOp_Scan->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pOp_Scan;
}

/*----------------------------------------------------------------------------*/
/* dissect : send <eSendKind>                                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Send(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  eXK eSendKind = (eXK) iGetInt(pArgTags, 0);
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
Card* Dissector::pOp_Sink(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : orderBy <szRpnExpr: 0) sbyElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pOp_Sort(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vCheckArgs(pArgTags, 1);

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(pArgTags, 0);
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
Card* Dissector::pOp_Start(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  return nullptr;
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
  RASSERT(pCard_Term->bIsSymbol("AS"_Stz));
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
/* collect <sortDirn> terms :                                                 */
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
/* eval typed calc expr : TYPE <type> [ AS <as> ] <rpnCalc> [ ASC | DESC ]    */
/*----------------------------------------------------------------------------*/
Card_Expr* Dissector::pPopExpr(Card_RpnTerms* pCard_RpnTerms, bool bIsKeyExpr, i8& ri_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Expr* pCard_Expr = new Card_Expr;

  if (bIsKeyExpr) {
    Symbol xSymbol_stmColOdn;
    xSymbol_stmColOdn.vSetKeyOrdinal(ri_Expr);
    pCard_Expr->vSymbol(xSymbol_stmColOdn);
  }
  else {
    Symbol xSymbol_stmColOdn;
    xSymbol_stmColOdn.vSetStreamOrdinal(ri_Expr);
    pCard_Expr->vSymbol(xSymbol_stmColOdn);
  }

  Stz* szP_Type = this->szPopType(pCard_RpnTerms);


  // [ AS <as> ]
  Card* pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && pCard_Term_head->bIsSymbol("AS"_Stz)) {

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
      && pCard_Term_head->bIsSymbol("AS"_Stz)) {

    Card_As* pCard_As = pPopAs(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_As);
  }


  // [ ASC | DESC ]
  pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && (pCard_Term_head->bIsSymbol("ASC"_Stz) ||
          pCard_Term_head->bIsSymbol("DESC"_Stz))) {

    Card_Direction* pCard_SortDirn = pPopDirection(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_SortDirn);
  }


  if (! pCard_Expr->pSub_as()) {
    Card_As* pCard_As = new Card_As(ri_Expr+1);
    pCard_Expr->vInsert(pCard_As);
  }

  ri_Expr++;
  return pCard_Expr;
}

/*----------------------------------------------------------------------------*/
/* collect <groupby> terms :  GROUPBY { <type> [<as>] <by> }                  */
/*----------------------------------------------------------------------------*/
Card_Key* Dissector::pPopKey(Card_RpnTerms* pCard_RpnTerms, bool bAsc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Key* pCard_Key = new Card_Key;

  // GROUPBY
  Card_Term* pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPeek(0));
  if (pCard_Term_head->bIsSymbol("GROUPBY"_Stz)) {
    pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPop());
    delete pCard_Term_head;
    pCard_Term_head = nullptr;
  }

  i8 i_CardNo = 0;
  i8 i_Expr = 1;
  while ( (pCard_Term_head = _pCard_Term(pCard_RpnTerms->pHead())) ) {

    // AGGREGATES
    if (pCard_Term_head->bIsSymbol("AGGREGATES"_Stz)) {
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

//    Card_As* p_as = pCard_Expr->pSub_as();
//    if (p_as) {
//      pCard_Expr->vRemoveChild(p_as);
//      DELETE_CARD(p_as);
//    }

    if (bAsc) {
      Card_Direction *p_direction = new Card_Direction("ASC"_Stz);
      pCard_Expr->vAppend(p_direction);
    }

    pCard_Key->vAppend(pCard_Expr);
  }

  return pCard_Key;
}

/*----------------------------------------------------------------------------*/
/* collect <agg> terms :                                                      */
/*----------------------------------------------------------------------------*/
Card_Projection* Dissector::pPopProjection(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Projection* pCard_Projection = new Card_Projection;

  i8 iCardNo = 0;

  Card_Term* pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPeek(0));
  if (pCard_Term_head->bIsSymbol("AGGREGATES"_Stz)) {
    pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPop());
    delete pCard_Term_head;

  }


  i8 i_Expr = 1;

  while ( (pCard_Term_head = _pCard_Term(pCard_RpnTerms->pHead())) ) {

    // GROUPBY
    if (pCard_Term_head->bIsSymbol("GROUPBY"_Stz)) {
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
/* collect <predicate>                                                        */
/*----------------------------------------------------------------------------*/
Card_RpnCalc* Dissector::pPopRpnCalc(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_RpnCalc* pCard_RpnCalc = new Card_RpnCalc;

  Card_Term* pCard_Term_popped = nullptr;
  while ( (pCard_Term_popped = _pCard_Term(pCard_RpnTerms->pPop())) ) {

    if (
        pCard_Term_popped->bIsSymbol("AGGREGATES"_Stz)
        || pCard_Term_popped->bIsSymbol("AS"_Stz)
        || pCard_Term_popped->bIsSymbol("ASC"_Stz)
        || pCard_Term_popped->bIsSymbol("DESC"_Stz)
        || pCard_Term_popped->bIsSymbol("GROUPBY"_Stz)
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
/* collect <type> terms : TYPE <columnType>                                   */
/*----------------------------------------------------------------------------*/
Stz* Dissector::szPopType(Card_RpnTerms* pCard_RpnTerms)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Dissector::iGetInt(TagSet* pArgTags, i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  i8 iArg = pArgTags->iValue(i_Arg);

  return iArg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissector::pGetRpnTerms(TagSet* pArgTags, i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Stz* szRpnExpr = pArgTags->szValue(i_Arg);
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);

  return pCard_RpnTerms;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Dissector::szGetStz(TagSet* pArgTags, i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Stz* szRpnExpr = pArgTags->szValue(i_Arg);

  return szRpnExpr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Dissector::vCheckArgs(TagSet* pArgTags, i8 nArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  i8 nTags = pArgTags->nItems();
    
  if (nTags < nArgs ) {
    Stz* szOpNm = m_pOpSpec->szOpNm();
    vTrace("Error: only %lld tags in %s argTags\n"_Stz,nTags,szOpNm);
    NEVER_REACH;
  }
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
