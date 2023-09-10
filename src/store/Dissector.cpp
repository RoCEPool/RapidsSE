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
  pCard_Stage->SYM(SF_STG_NO,i_StageSpec);

  Card_Topology* pCard_StageTopo = new Card_Topology(eStageTopo);
  pCard_Stage->vAppendChild(pCard_StageTopo);

  m_pBoard->pDeck()->vAppendChild(pCard_Stage);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // params
  if (1 == i_StageSpec) {
    ParamsSpec* pParamsSpec = m_pDeckSpec->pParamsSpec();
    if (pParamsSpec) {
      Card_Correlation* p_paramList = new Card_Correlation;
      m_pBoard->pDeck()->vAppendChild(p_paramList);
      Card_OuterColNm* p_param = new Card_OuterColNm;
      StrLib::vStrip((char*)pParamsSpec->pGet(0)->szParamNm(), pParamsSpec->pGet(0)->iParamNmBufSize(), ' ');
      p_param->SYM(SF_COL_NM,pParamsSpec->pGet(0)->szParamNm());
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
  pCard_On->vAppendChild(pCard_RpnCalc);

  pCard_AbsJoin->vAppendChild(pCard_On);

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
  pCard_Aggreg->vAppendChild(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Stream* p_stream = new Card_Stream;
  pCard_Aggreg->vAppendChild(p_stream);

  Card_Projection* pCard_Projection = pPopProjection(pCard_RpnTerms);
  p_stream->vAppendChild(pCard_Projection);

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

  StrLib::vStrip(szSubPipeNm->azChar(), szSubPipeNm->nSize(), ' ');

  Card_Call* pCard_Call = new Card_Call;
  pCard_Call->SYM(SF_PPE_NM,szSubPipeNm);

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
  Card_Expr* pCard_Expr = pPopExpr(pCard_RpnTerms, false, i_Expr, false);

  Card_RpnCalc* pCard_RpnCalc = pCard_Expr->pSub_rpnCalc();
  pCard_Expr->vRemoveSub(pCard_RpnCalc);
  DELETE_CARD(pCard_Expr);

  pCard_Filter->vAppendChild(pCard_RpnCalc);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is it filter or having?
  
  if (m_bSawAggOp || m_bSawProjectOp) {
    Card_Having* pCard_Having = new Card_Having;

    eAL eAggLevel = m_pStageSpec->eAggegLevel();
    Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
    pCard_Having->vAppendChild(pCard_AggLevel);

    pCard_Having->vAppendChild(pCard_Filter);

    delete pCard_RpnTerms;
    return pCard_Having;
  }
  else {
    Card_Where* pCard_Where = new Card_Where;

    eAL eAggLevel = m_pStageSpec->eAggegLevel();
    Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
    pCard_Where->vAppendChild(pCard_AggLevel);

    pCard_Where->vAppendChild(pCard_Filter);

    delete pCard_RpnTerms;
    return pCard_Where;
  }
}
/*----------------------------------------------------------------------------*/
//Card* Dissector::pOp_Filter(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // op arguments
//
//  vCheckArgs(1);
//
//  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
//  Card_Filter* pCard_Filter = new Card_Filter;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // filtElist : filter expression list
//
//  i8 i_Expr = 1;
//  Card_Expr* pCard_Expr = pPopExpr(pCard_RpnTerms, false, i_Expr, false);
//
//  Card_RpnCalc* pCard_RpnCalc = pCard_Expr->pSub_rpnCalc();
//  pCard_Expr->vRemoveSub(pCard_RpnCalc);
//  DELETE_CARD(pCard_Expr);
//
//  pCard_Filter->vAppendChild(pCard_RpnCalc);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // is it filter or having?
//
//  if (m_bSawAggOp || m_bSawProjectOp) {
//    Card_Having* pCard_Having = new Card_Having;
//    pCard_Having->vTransferChildren(pCard_Filter);
//    delete pCard_Filter;
//    delete pCard_RpnTerms;
//
//    eAL eAggLevel = m_pStageSpec->eAggegLevel();
//    Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
//    pCard_Having->vPush(pCard_AggLevel);
//
//    return pCard_Having;
//  }
//  else {
//    delete pCard_RpnTerms;
//
//    pCard_Filter->vCard(CK_WHERE);
//    Card_Where* p_where = _pCard_Where(pCard_Filter);
//
//    eAL eAggLevel = m_pStageSpec->eAggegLevel();
//    Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
//    p_where->vPush(pCard_AggLevel);
//
//    return p_where;
//  }
//}

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
  pCard_Aggreg->vAppendChild(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gbyElist : optional groupBy expression list

  if (pCard_RpnTerms->nItems()) {
    Card_Key* pCard_Key = pPopKey(pCard_RpnTerms, true);
    pCard_Aggreg->vAppendChild(pCard_Key);
    pCard_Aggreg->vCard(CK_GBYAGG);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Stream* p_stream = new Card_Stream;
  pCard_Aggreg->vAppendChild(p_stream);

  Card_Projection* pCard_Projection = pPopProjection(pCard_RpnTerms);
  p_stream->vAppendChild(pCard_Projection);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;

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

  eAL eLevel = m_pStageSpec->eAggegLevel();
  Card_Level* p_level = new Card_Level(eLevel);
  pCard_Hash->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // calcExpr : calculation expression list

//  Card_HshCol* p_hshCol = new Card_HshCol;
//
//  Card_Term* p_term = _pCard_Term(pCard_RpnTerms->pGet(1));
//
//  Card_Term* p_term2 = _pCard_Term(pCard_RpnTerms->pGet(2));
//
//  p_hshCol->pSymbol()->vSym(p_term2->pSymbol());
//  eCT eColumnType = Symbol::eTo_eCT_str(p_term->szSymbol(SF_NONE));
//  p_hshCol->pSymbol()->Y_vColType(szCT_char[eColumnType]);
//
//  Card_Term* p_term_hsh = p_term2->pClone();
//  p_term_hsh->pSymbol()->Y_vColType(szCT_char[eColumnType]);
//  p_hshCol->vAppendChild(p_term_hsh);
//  p_hshCol->pSymbol()->Y_vSet(P_BreakPt);

  Card_HshColList* p_hshColList = pPopHshColList(pCard_RpnTerms);
  pCard_Hash->vAppendChild(p_hshColList);

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
  Card_RpnTerms* p_rpnTerms_bound = pGetRpnTerms(2);
  Card_RpnTerms* p_rpnTerms_constraint =
      m_pOpArgs->nItems() >= 4 ? pGetRpnTerms(3) : nullptr;

  //p_rpnTerms_bound->vDump();

  Table* pTable = AM::pTableFind(szTableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_IndexJoin->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table
  
  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);
  pCard_IndexJoin->vAppendChild(pCard_Table);

   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Card_IndexList* p_indexList = new Card_IndexList;
  pCard_Table->vAppendChild(p_indexList);

  Index* pIndex = pTable->pFindIndex(szIndexNm);

  Card_Index* p_index = new Card_Index;
  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_index->vAppendChild(p_indexNm);
  p_indexNm->SYM(SF_IDX_NM,pIndex->szIndexNm());

  p_indexList->vAppendChild(p_index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* pCard_Bounds = new Card_Bounds;
  p_index->vAppendChild(pCard_Bounds);

  Card_Bound* pCard_Bound = new Card_Bound;


  pCard_Bound->vTransferChildren(p_rpnTerms_bound);

  Card_Term* p_term_first = pCard_Bound->pGetTerm(0);
  if (p_term_first->pSymbol()->bSame("LEFT_OUTER"_Stz)) {
    Card_OuterJoin* pCard_OuterJoin = new Card_OuterJoin;
    pCard_OuterJoin->pSymbol()->vSym(p_term_first->pSymbol());
    pCard_IndexJoin->vAppendChild(pCard_OuterJoin);
  }
  pCard_Bound->vDeleteChild(p_term_first);

  p_term_first = pCard_Bound->pGetTerm(0);
  if (p_term_first->pSymbol()->bSame("BOUND"_Stz))
    pCard_Bound->vDeleteChild(p_term_first);

  i8 nTerms = pCard_Bound->nChildCards();

  Card_Term* p_term_last = pCard_Bound->pGetTerm(nTerms-1);
  pCard_Bound->pSymbol()->vSym(p_term_last->pSymbol());
  pCard_Bound->vDeleteChild(p_term_last);

  pCard_Bounds->vAppendChild(pCard_Bound);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // constraint

  if (p_rpnTerms_constraint) {
    Card_Constraint* pCard_Constraint = pPopConstraint(p_rpnTerms_constraint);
    //pCard_Constraint->vDump(__func__);
    pCard_IndexJoin->vAppendChild(pCard_Constraint);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete p_rpnTerms_bound;

  if (p_rpnTerms_constraint)
    delete p_rpnTerms_constraint;

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

  Table* pTable = AM::pTableFind(szTableNm);

  //p_rpnTerms_bounds->vDump();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_IndexScan->vAppendChild(p_level);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);

  pCard_IndexScan->vAppendChild(pCard_Table);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Card_IndexList* p_indexList = new Card_IndexList;
  pCard_Table->vAppendChild(p_indexList);

  Index* pIndex = pTable->pFindIndex(szIndexNm);
  UNUSED(pIndex);

  Card_Index* p_index = new Card_Index;
  Card_IndexNm* p_indexNm = new Card_IndexNm;
  p_index->vAppendChild(p_indexNm);
  p_indexNm->SYM(SF_IDX_NM,szIndexNm);

  p_indexList->vAppendChild(p_index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* pCard_Bounds = pPopBounds(p_rpnTerms_bounds);
  if (pCard_Bounds)
    p_index->vAppendChild(pCard_Bounds);

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

  Table* pTable = AM::pTableFind(szTableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // keyExists

  Card_KeyExists* pCard_KeyExists = new Card_KeyExists;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_KeyExists->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);
  pCard_KeyExists->vAppendChild(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index list

  Card_IndexList* p_indexList = pFormIndex(szIndexNm,pTable);
  pCard_Table->vAppendChild(p_indexList);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* pCard_Bounds = pPopBounds(p_rpnTerms_bounds);
  //pCard_Bounds->vDump(__func__);
  p_indexList->pSub_index()->vAppendChild(pCard_Bounds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // constraint

  Card_Constraint* pCard_Constraint = pPopConstraint(p_rpnTerms_constraint);
  //pCard_Constraint->vDump(__func__);
  pCard_KeyExists->vAppendChild(pCard_Constraint);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnTerms_bounds;
  delete p_rpnTerms_constraint;

  //pCard_KeyExists->vDump(__func__);

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

  Table* pTable = AM::pTableFind(szTableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // keyExists

  Card_KeyNotExists* pCard_KeyNotExists = new Card_KeyNotExists;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pCard_KeyNotExists->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);
  pCard_KeyNotExists->vAppendChild(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index list

  Card_IndexList* p_indexList = pFormIndex(szIndexNm,pTable);
  pCard_Table->vAppendChild(p_indexList);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index bounds

  Card_Bounds* pCard_Bounds = pPopBounds(p_rpnTerms_bounds);
  //pCard_Bounds->vDump(__func__);
  p_indexList->pSub_index()->vAppendChild(pCard_Bounds);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // constraint

  Card_Constraint* pCard_Constraint = pPopConstraint(p_rpnTerms_constraint);
  //pCard_Constraint->vDump(__func__);
  pCard_KeyNotExists->vAppendChild(pCard_Constraint);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete p_rpnTerms_bounds;
  delete p_rpnTerms_constraint;

  //pCard_KeyNotExists->vDump(__func__);

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


  Card_Limit* pCard_Limit = _pCard_Limit((new Card_Limit) ->SYM(SF_INT_LIT,nRows));

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Limit->vAppendChild(pCard_AggLevel);

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
  pCard_OrdAgg->vAppendChild(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_Projection* pCard_Projection = pPopProjection(pCard_RpnTerms);
  pCard_OrdAgg->vAppendChild(pCard_Projection);

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

  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Project->vAppendChild(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Stream* pCard_Stream = new Card_Stream;
  pCard_Project->vAppendChild(pCard_Stream);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // projElist : project expression list

  Card_Projection* pCard_Projection = pPopProjection(pCard_RpnTerms);
  pCard_Stream->vAppendChild(pCard_Projection);

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

  Table* pTable = AM::pTableFind(szTableNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // scan

  Card_Scan* pOp_Scan = new Card_Scan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // level

  Card_Level* p_level = new Card_Level(AL_PARTITION);
  pOp_Scan->vAppendChild(p_level);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form table

  Card_Table* pCard_Table = pFormTable(szTableNm,pTable);
  pOp_Scan->vAppendChild(pCard_Table);

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
  pCard_Sort->vAppendChild(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sbyElist : sortBy expression list

  Card_Key* pCard_Key = pPopKey(pCard_RpnTerms, true);
  pCard_Sort->vAppendChild(pCard_Key);

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

  i8 nRows = iGetInt(1);

  Card_Top* pCard_Top = _pCard_Top((new Card_Top) ->SYM(SF_INT_LIT, nRows));


  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_Level* pCard_AggLevel = new Card_Level(eAggLevel);
  pCard_Top->vAppendChild(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sbyElist : sortBy expression list

  Card_RpnTerms* pCard_RpnTerms = pGetRpnTerms(0);
  Card_Key* pCard_Key = pPopKey(pCard_RpnTerms, true);
  pCard_Top->vAppendChild(pCard_Key);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Count* pCard_Count = new Card_Count;
  pCard_Count->SYM(SF_INT_LIT, nRows);
  pCard_Top->vAppendChild(pCard_Count);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;

  return pCard_Top;
}

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
  p_indexNm->SYM(SF_IDX_NM,pIndex->szIndexNm());

  p_index->vAppendChild(p_indexNm);
  p_indexList->vAppendChild(p_index);

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

  Card_TableNm* pCard_TableNm = (new Card_TableNm) ->SYM(SF_TBL_NM,szTableNm);
  pCard_Table->vAppendChild(pCard_TableNm);

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
  RASSERT(pCard_Term->pSymbol()->bSame("AS"_Stz));
  delete pCard_Term;
  pCard_Term = nullptr;

  // <name>
  pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szAsNm = pCard_Term->szSymbol(SF_NONE);

  Card_As* pCard_As = (new Card_As) ->SYM(SF_AS_NM,szAsNm);
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
    pCard_Bounds->vAppendChild(pCard_Bound);

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
  if (! p_term_first || !p_term_first->pSymbol()->bIs_Bound())
    return nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pop one (l/u) bound term sequence as a card bound

  Card_Bound* pCard_Bound = new Card_Bound;

  // the first term in sequence will be a LBOUND or UBOUND
  //   which is not transfered but should match last reln op in sequence
  Card_Term* p_term_bound = pCard_RpnTerms->pPop_term();
  RASSERT(p_term_bound->pSymbol()->bIs_Bound());

  // transfer terms for this bound sequence
  Card_Term* p_term_xfr;
  for (i8 i_Loop = 0; ; i_Loop++) {
    UNUSED(i_Loop);

    Card_Term* p_term_top = _pCard_Term(pCard_RpnTerms->pPeek(0));
    if (! p_term_top || p_term_top->pSymbol()->bIs_Bound())
      break;
    p_term_xfr = pCard_RpnTerms->pPop_term();
    pCard_Bound->vAppendChild(p_term_xfr);
  }


  // use last term to set bound symbol
  Card_Term* p_term_last = pCard_Bound->pGetTerm_last();
  pCard_Bound->pSymbol()->vSym(p_term_last->pSymbol());

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

  pCard_RpnCalc->pSymbol()->Y_vColType(szType);

  pCard_Constraint->vAppendChild(pCard_RpnCalc);

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
  Stz* szDirn = pCard_Term->szSymbol(SF_NONE);
  RASSERT(szDirn->bSame("ASC"_Stz) || szDirn->bSame("DESC"_Stz));

  Card_Direction* pCard_Direction = (new Card_Direction) ->SYM(SF_DIRECTION, szDirn);

  delete pCard_Term;
  pCard_Term = nullptr;

  return pCard_Direction;
}

/*----------------------------------------------------------------------------*/
/* pop expr                                                                   */
/*----------------------------------------------------------------------------*/
Card_Expr* Dissector::pPopExpr(Card_RpnTerms* pCard_RpnTerms,
                               bool bIsKeyExpr, i8& ri_Expr,
                               bool bEnsureDirection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Expr* pCard_Expr = new Card_Expr;

  if (bIsKeyExpr) {
    Symbol xSymbol_stmColOdn(SF_KEY_COL,ri_Expr);
    pCard_Expr->pSymbol()->vSym(&xSymbol_stmColOdn);
  }
  else {
    Symbol xSymbol_stmColOdn(SF_COL_NO,ri_Expr);
    pCard_Expr->pSymbol()->vSym(&xSymbol_stmColOdn);
  }

  Stz* szP_Type = this->szPopType_char(pCard_RpnTerms);


  // [ AS <as> ]
  Card* pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && pCard_Term_head->pSymbol()->bSame("AS"_Stz)) {

    Card_As* pCard_As = pPopAs(pCard_RpnTerms);
    pCard_Expr->vAppendChild(pCard_As);
  }


  // <rpnCalc>
  Card_RpnCalc* pCard_RpnCalc = pPopRpnCalc(pCard_RpnTerms);
  pCard_Expr->vAppendChild(pCard_RpnCalc);

  pCard_RpnCalc->pSymbol()->Y_vColType(szP_Type);
  pCard_Expr->pSymbol()->Y_vColType(szP_Type);


  // [ AS <as> ]
  pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && pCard_Term_head->pSymbol()->bSame("AS"_Stz)) {

    Card_As* pCard_As = pPopAs(pCard_RpnTerms);
    pCard_Expr->vAppendChild(pCard_As);
  }


  // [ ASC | DESC ]
  pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && (pCard_Term_head->pSymbol()->bSame("ASC"_Stz) ||
          pCard_Term_head->pSymbol()->bSame("DESC"_Stz))) {

    Card_Direction* pCard_Direction = pPopDirection(pCard_RpnTerms);
    pCard_Expr->vAppendChild(pCard_Direction);
  }
  else
  if (bEnsureDirection) {
    Card_Direction* pCard_Direction = (new Card_Direction) ->SYM(SF_DIRECTION, "ASC"_Stz);
    pCard_Expr->vAppendChild(pCard_Direction);
  }


  if (! pCard_Expr->pSub_as()) {
    Card_As *pCard_As;
    if (pCard_Expr->pSub_rpnCalc()->nChildCards() == 1) {
      Stz* AsNm = pCard_Expr->pGetTerm(0)->szSymbol(SF_NONE);
      if (Symbol::bIs_Col_Nm(AsNm)) {
        pCard_As = (new Card_As)->SYM(SF_AS_NM, AsNm);
      }
      else {
        pCard_As = (new Card_As)->SYM(SF_AS_NM_syn, ri_Expr);
      }
    }
    else {
      pCard_As = (new Card_As)->SYM(SF_AS_NM_syn, ri_Expr);
    }
    pCard_Expr->vPushChild(pCard_As);
  }

  ri_Expr++;
  return pCard_Expr;
}

/*----------------------------------------------------------------------------*/
/* pop hshCol                                                                 */
/*----------------------------------------------------------------------------*/
Card_HshCol* Dissector::pPopHshCol(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card* pCard_Term_head = pCard_RpnTerms->pHead();
  if (! pCard_Term_head)
    return nullptr;

  if (pCard_Term_head->pSymbol()->szSF_symbol(SF_NONE)->bSame(","_Stz)) {
    pCard_Term_head = pCard_RpnTerms->pPop();
    delete pCard_Term_head;
  }

  Card_HshCol* p_hshCol = new Card_HshCol;

  Card* p_card_type = pCard_RpnTerms->pPop();
  delete p_card_type;

  Card_Term* p_term1 = _pCard_Term(pCard_RpnTerms->pPop());
  eCT eColumnType = Symbol::eTo_eCT_str(p_term1->szSymbol(SF_NONE));
  delete p_term1;

  Card_Term* p_term_hsh = _pCard_Term(pCard_RpnTerms->pPop());
  p_term_hsh->pSymbol()->Y_vColType(szCT_char[eColumnType]);
  p_hshCol->vAppendChild(p_term_hsh);

  return p_hshCol;
}

/*----------------------------------------------------------------------------*/
/* pop hshColList                                                             */
/*----------------------------------------------------------------------------*/
Card_HshColList* Dissector::pPopHshColList(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_HshColList* pCard_HshColList = new Card_HshColList;

  while (Card_HshCol* pCard_HshCol = pPopHshCol(pCard_RpnTerms)) {
    pCard_HshColList->vAppendChild(pCard_HshCol);
  }

  return pCard_HshColList;
}

/*----------------------------------------------------------------------------*/
/* pop key                                                                    */
/*----------------------------------------------------------------------------*/
Card_Key* Dissector::pPopKey(Card_RpnTerms* pCard_RpnTerms, bool bEnsureDirection)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Key* pCard_Key = new Card_Key;

  // GROUPBY
  Card_Term* pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPeek(0));
  if (pCard_Term_head->pSymbol()->bSame("GROUPBY"_Stz)) {
    pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPop());
    delete pCard_Term_head;
    pCard_Term_head = nullptr;
  }

  i8 i_CardNo = 0;
  i8 i_Expr = 1;
  while ( (pCard_Term_head = _pCard_Term(pCard_RpnTerms->pHead())) ) {

    // AGGREGATES
    if (pCard_Term_head->pSymbol()->bSame("AGGREGATES"_Stz)) {
      break;
    }

    // SEPARATOR
    if (pCard_Term_head->pSymbol()->bIs_Comma()) {
      delete pCard_RpnTerms->pPop();
      continue;
    }

    // TYPE <type> <calcExpr> [ AS <as> ]
    Card_Expr* pCard_Expr = pPopExpr(pCard_RpnTerms, true, i_Expr, bEnsureDirection);
    pCard_Expr->v_Child(++i_CardNo);

//    if (bEnsureDirection) {
//      Card_Direction *p_direction = new Card_Direction("ASC"_Stz);
//      pCard_Expr->vAppendChild(p_direction);
//    }

    pCard_Key->vAppendChild(pCard_Expr);
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
  if (pCard_Term_head->pSymbol()->bSame("AGGREGATES"_Stz)) {
    pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPop());
    delete pCard_Term_head;

  }

  i8 i_Expr = 1;

  while ( (pCard_Term_head = _pCard_Term(pCard_RpnTerms->pHead())) ) {

    // GROUPBY
    if (pCard_Term_head->pSymbol()->bSame("GROUPBY"_Stz)) {
      break;
    }

    // SEPARATOR
    if (pCard_Term_head->pSymbol()->bIs_Comma()) {
      delete pCard_RpnTerms->pPop();
      continue;
    }

    // TYPE <type> [ AS <as> ] <rpnCalc>
    Card_Expr* pCard_Expr = pPopExpr(pCard_RpnTerms, false, i_Expr, false);
    pCard_Expr->v_Child(++iCardNo);
    //vAsEnsure(pCard_Expr);

    pCard_Projection->vAppendChild(pCard_Expr);
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
        pCard_Term_popped->pSymbol()->bSame("AGGREGATES"_Stz)
        || pCard_Term_popped->pSymbol()->bSame("AS"_Stz)
        || pCard_Term_popped->pSymbol()->bSame("ASC"_Stz)
        || pCard_Term_popped->pSymbol()->bSame("DESC"_Stz)
        || pCard_Term_popped->pSymbol()->bSame("GROUPBY"_Stz)
        || pCard_Term_popped->pSymbol()->bIs_Comma()
        ) {
      pCard_RpnTerms->vPushChild(pCard_Term_popped);
      break;
    }

    if (pCard_Term_popped->pSymbol()->bIs_Agg_Fn())
      pCard_RpnCalc->pSymbol()->Y_vSet(P_AggOp);

    pCard_RpnCalc->vAppendChild(pCard_Term_popped);
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
  Stz* szTerm = pCard_Term->szSymbol(SF_NONE);
  RASSERT(szTerm->bSame("TYPE"_Stz));

  delete pCard_Term;
  pCard_Term = nullptr;

  // column type (e.g. BOOLEAN)
  Card_Term* pCard_Term_type = _pCard_Term(pCard_RpnTerms->pPop());

  Stz* szType = szCT_char[Symbol::eTo_eCT_str(pCard_Term_type->szSymbol(SF_NONE))];

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

    eSF eSymbol = Symbol::eEvalSymbol(sDissectedVal);
    if (SF_NONE == eSymbol)
      eSymbol = SF_COL_NM;
    Card_Term* pCard_Term = (new Card_Term) ->SYM(eSymbol,sDissectedVal);

    pCard_RpnTerms->vAppendChild(pCard_Term);
  }
  
  delete pTokenVec;

  return pCard_RpnTerms;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
