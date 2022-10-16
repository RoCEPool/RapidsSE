/*============================================================================*/
/* Dissector.cpp -- Dissector                                                     */
/*============================================================================*/

#include "Dissector.hpp"

#include "Optimiser.hpp"

#include "Job.hpp"

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
  m_pStageSpec    = nullptr;
  m_pOpSpec       = nullptr;
  
  m_bSawAggOp     = false;
  
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
    vPipeline();
  }

  m_pPipeSpec = pPipeSpec;
  vPipeline();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pStageSpec = nullptr;
  m_pOpSpec    = nullptr;
  m_bSawAggOp  = false;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dissect pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
void Dissector::vPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  Stz* szPipelineNm;

  Job* pJob = MY::pJob();
  TagSet* pJobTags = pJob->pJobTags();
  if (pJobTags->bHasName("<pipelineName>"_Stz))
    szPipelineNm = pJobTags->szValue("<pipelineName>"_Stz);
  else
    szPipelineNm = m_pPipeSpec->szVertexNm();


  //TagSet* pJobTags = m_pPipeSpec->pJobTags();
  //pJobTags->vDumpSet(true);

  m_pBoard = new Board(BK_QUERY, BT_ORIGINAL,m_pBench);
  m_pOptimiser->vQueryNm(szPipelineNm);

  
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
 
  //Stz* szStageNm = m_pStageSpec->szStageNm();
  eYG eStageTopo = m_pStageSpec->eStageTopology();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    

  Card_Stage* pCard_Stage = new Card_Stage;
  pCard_Stage->vValueSprint("S%02lld"_Stz,i_StageSpec);

  Card_StageTopo* pCard_StageTopo = new Card_StageTopo(eStageTopo);
  pCard_Stage->vAppend(pCard_StageTopo);

  m_pBoard->pDeck()->vAppend(pCard_Stage);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bSawAggOp = false;
  
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

       if (szOpNm->bSame("abstractJoin"_Stz)) pCard = pCard_AbsJoin  (pArgTags);
  else if (szOpNm->bSame("aggreg"_Stz))       pCard = pCard_Aggreg   (pArgTags);
  else if (szOpNm->bSame("combined"_Stz))     pCard = pCard_Combined (pArgTags);
  else if (szOpNm->bSame("consumeOp"_Stz))    pCard = pCard_Consume  (pArgTags);
  else if (szOpNm->bSame("debug"_Stz))        pCard = pCard_Debug    (pArgTags);
  else if (szOpNm->bSame("filter"_Stz))       pCard = pCard_Filter   (pArgTags);
  else if (szOpNm->bSame("hash"_Stz))         pCard = pCard_Hash     (pArgTags);
  else if (szOpNm->bSame("indexJoin"_Stz))    pCard = pCard_IndexJoin(pArgTags);
  else if (szOpNm->bSame("indexScan"_Stz))    pCard = pCard_IndexScan(pArgTags);
  else if (szOpNm->bSame("limit"_Stz))        pCard = pCard_Limit    (pArgTags);
  else if (szOpNm->bSame("ordagg"_Stz))       pCard = pCard_OrdAgg   (pArgTags);
  else if (szOpNm->bSame("orderBy"_Stz))      pCard = pCard_Sort     (pArgTags);
  else if (szOpNm->bSame("output"_Stz))       pCard = pCard_Output   (pArgTags);
  else if (szOpNm->bSame("pulse"_Stz))        pCard = pCard_Pulse    (pArgTags);
  else if (szOpNm->bSame("project"_Stz))      pCard = pCard_Project  (pArgTags);
  else if (szOpNm->bSame("scan"_Stz))         pCard = pCard_Scan     (pArgTags);
  else if (szOpNm->bSame("sendSE"_Stz))       pCard = pCard_Send     (pArgTags);
  else if (szOpNm->bSame("sink"_Stz))         pCard = pCard_Sink     (pArgTags);
  else if (szOpNm->bSame("sort"_Stz))         pCard = pCard_Sort     (pArgTags);
  else if (szOpNm->bSame("start"_Stz))        pCard = pCard_Start    (pArgTags);
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
Card* Dissector::pCard_AbsJoin(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0);
  Card_AbsJoin* pCard_AbsJoin = new Card_AbsJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // expr : exression list

  Card_Calc* pCard_ExprCalc = pExprCalc(pCard_RpnTerms);
  pCard_AbsJoin->vAppend(pCard_ExprCalc);

  delete pCard_RpnTerms;

  return pCard_AbsJoin;
}

/*----------------------------------------------------------------------------*/
/* dissect : aggreg <szRpnExpr : 0) aggElist ; 1) gbyElist>                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Aggreg(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
     
  m_bSawAggOp = true;
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0);
  Card_Aggreg* pCard_Aggreg = new Card_Aggreg;
  
  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_AggLevel* pCard_AggLevel = new Card_AggLevel(eAggLevel);
  pCard_Aggreg->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gbyElist : optional groupBy expression list

  if (pCard_RpnTerms->nItems()) {
    Card_GbyElist* pCard_ByList = pGbyElist(pCard_RpnTerms);
    pCard_Aggreg->vAppend(pCard_ByList);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list
  
  Card_AggElist* pCard_AggElist = pAggElist(pCard_RpnTerms);
  pCard_Aggreg->vAppend(pCard_AggElist);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;
    
  return pCard_Aggreg;
}

/*----------------------------------------------------------------------------*/
/* dissect : combined <iGenerate>                                             */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Combined(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  Card_Combined* pCard_Combine = new Card_Combined;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return pCard_Combine;
}

/*----------------------------------------------------------------------------*/
/* dissect : consumeOp <nSends>                                               */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Consume(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : debug <iOption>                                                  */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Debug(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  Stz* szOption = szArgCheckLast(pArgTags,0);
  Card_Debug* pCard_Debug = new Card_Debug;

  pCard_Debug->Y_vSet(P_DebugOption, szOption);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Debug;
}

/*----------------------------------------------------------------------------*/
/* dissect : filter <szRpnExpr: 0) filtElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Filter(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0);
  Card_Filter* pCard_Filter = new Card_Filter;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // filtElist : filter expression list

  Card_Expr* pCard_Expr = pExpr_c(pCard_RpnTerms);
  pCard_Filter->vAppend(pCard_Expr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is it filter or having?
  
  if (! m_bSawAggOp) {
    delete pCard_RpnTerms;
    pCard_Filter->vCard(CK_WHERE);
    return pCard_Filter;
  }
  else {
    Card_Having* pCard_Having = new Card_Having;
    pCard_Having->vTransferChildren(pCard_Filter);
    delete pCard_Filter;
  
    delete pCard_RpnTerms;
    return pCard_Having;
  }
}

/*----------------------------------------------------------------------------*/
/* dissect : hash <szRpnExpr : 0) calcExpr>                                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Hash(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0);
  Card_Hash* pCard_Hash = new Card_Hash;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // calcExpr : calculation expression list

  pCard_Hash->vTransferChildren(pCard_RpnTerms);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;

  return pCard_Hash;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexJoin <szTblNm> <szIdxNm> <szRpnExpr: 0) idxPred>            */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_IndexJoin(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vTrace("pCard_IndexJoin\n"_Stz);
  pArgTags->vDumpSet(true);

  Stz* szTableNm = szArgsStz(pArgTags,0);
  Stz* szIndexNm = szArgsStz(pArgTags,1);
  Card_RpnTerms* pCard_RpnTerms = pArgsRpnTerms(pArgTags,2);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0,szTableNm,szIndexNm);
  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table
  
  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table;
  Card_TablePtr* pCard_TablePtr = new Card_TablePtr(pTable);
  pCard_Table->vAppend(pCard_TablePtr);
  pCard_IndexJoin->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Index* pIndex = pTable->pFindIndex(szIndexNm);
  Card_IndexPtr* pCard_Index = new Card_IndexPtr(pIndex);
  pCard_IndexJoin->vAppend(pCard_Index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index predicate????

  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferChildren(pCard_RpnTerms);
  pCard_IndexJoin->vAppend(pCard_IndexPred);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;
  
  return pCard_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexScan <szTblNm> <szIdxNm> <szRpnExpr: 0) idxPred>            */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_IndexScan(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;

  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0,szTableNm,szIndexNm);
  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table;
  Card_TablePtr* pCard_TablePtr = new Card_TablePtr(pTable);
  pCard_Table->vAppend(pCard_TablePtr);
  pCard_IndexScan->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Index* pIndex = pTable->pFindIndex(szIndexNm);
  Card_IndexPtr* pCard_Index = new Card_IndexPtr(pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index predicate????

  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferChildren(pCard_RpnTerms);
  pCard_IndexScan->vAppend(pCard_IndexPred);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;
  
  return pCard_IndexScan;
}

/*----------------------------------------------------------------------------*/
/* dissect : limit <nRows>                                                    */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Limit(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  pArgTags->vDumpSet(true);

  i8 nRows = iArgCheckLast(pArgTags,0);

  s32 sRows;
  sRows.vSprint("%lld"_Stz,nRows);

  Card_Limit* pCard_Limit = new Card_Limit(sRows);

  return pCard_Limit;
}

/*----------------------------------------------------------------------------*/
/* dissect : ordAgg <szRpnExpr : 0) aggElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_OrdAgg(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  m_bSawAggOp = true;
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0);
  Card_OrdAgg* pCard_OrdAgg = new Card_OrdAgg;
  
  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_AggLevel* pCard_AggLevel = new Card_AggLevel(eAggLevel);
  pCard_OrdAgg->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list

  Card_AggElist* pCard_AggElist = pAggElist(pCard_RpnTerms);
  pCard_OrdAgg->vAppend(pCard_AggElist);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;

  return pCard_OrdAgg;
}

/*----------------------------------------------------------------------------*/
/* dissect : output                                                           */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Output(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : pulse <szMode>                                                   */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Pulse(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : project <szRpnExpr : 0) projElist>                               */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Project(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0);
  Card_Project* pCard_Project = new Card_Project(CK_PROJECT);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // projElist : project expression list

  Card_ProjElist* pCard_ProjElist = pProjElist(pCard_RpnTerms);
  pCard_Project->vAppend(pCard_ProjElist);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;

  return pCard_Project;
}

/*----------------------------------------------------------------------------*/
/* dissect : scan [ <szCatalogNm> <szSchemaNm> ] <szTableNm>                  */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Scan(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
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

  Card_Scan* pCard_Scan = new Card_Scan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table
  
  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table(szTableNm);
  Card_TablePtr* pCard_TablePtr = new Card_TablePtr(pTable);
  pCard_Table->vAppend(pCard_TablePtr);
  pCard_Scan->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Scan;
}

/*----------------------------------------------------------------------------*/
/* dissect : send <eSendKind>                                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Send(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  eXK eSendKind = (eXK)iArgCheckLast(pArgTags,0);
  Card_Send* pCard_Send = new Card_Send;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // eSendKind

  pCard_Send->Y_vSet(P_SendKind, (i8)eSendKind);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Send;
}

/*----------------------------------------------------------------------------*/
/* dissect : sink                                                             */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Sink(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : orderBy <szRpnExpr: 0) sbyElist>                                 */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Sort(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
    
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,0);
  Card_Sort* pCard_Sort = new Card_Sort;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // sbyElist : sortBy expression list

  Card_SbyElist* pCard_ObyElist = pSbyElist(pCard_RpnTerms);
  pCard_Sort->vAppend(pCard_ObyElist);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;

  return pCard_Sort;
}

/*----------------------------------------------------------------------------*/
/* dissect : start                                                            */
/*----------------------------------------------------------------------------*/
Card* Dissector::pCard_Start(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
      
  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* collect <agg> terms :                                                      */
/*----------------------------------------------------------------------------*/
Card_AggElist* Dissector::pAggElist(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_AggElist* pCard_AggElist = new Card_AggElist;
  
  i8 iCardNo = 0;

  Card_Term* pCard_Term_head = pCard_RpnTerms->pPopTerm();
  RASSERT(pCard_Term_head->bIsValue("AGGREGATES"_Stz));
  delete pCard_Term_head;



  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {
    
    // GROUPBY
    if (pCard_Term_head->bIsValue("GROUPBY"_Stz)) {
      break;
    }
    
    // SEPARATOR
    if (pCard_Term_head->bIsCommaSeparator()) {
      delete pCard_RpnTerms->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calc>
    Card_Expr* pCard_Expr = pExpr_c(pCard_RpnTerms);
    pCard_Expr->vCardNo(++iCardNo);
    pCard_Expr->vAsEnsure();

    pCard_AggElist->vAppend(pCard_Expr);
  }
  
  return pCard_AggElist;
}

/*----------------------------------------------------------------------------*/
/* collect <groupby> terms :  GROUPBY { <type> [<as>] <by> }                  */
/*----------------------------------------------------------------------------*/
Card_GbyElist* Dissector::pGbyElist(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_GbyElist* pCard_GbyElist = new Card_GbyElist;
  
  // GROUPBY
  Card_Term* pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPop());
  RASSERT(pCard_Term_head->bIsValue("GROUPBY"_Stz));

  delete pCard_Term_head;
  pCard_Term_head = nullptr;

  i8 iCardNo = 0;
  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {

    // GROUPBY
    if (pCard_Term_head->bIsValue("AGGREGATES"_Stz)) {
      break;
    }

    // SEPARATOR
    if (pCard_Term_head->bIsCommaSeparator()) {
      delete pCard_RpnTerms->pPop();
      continue;
    }
        
    // TYPE <type> <calcExpr> [ AS <as> ]
    Card_Expr* pCard_Expr = pExpr_c(pCard_RpnTerms);
    pCard_Expr->vCardNo(++iCardNo);
    pCard_Expr->vAsEnsure();
    
    Card_Direction* pCard_Direction = new Card_Direction(aSD_short[SD_ASC]);
    pCard_Expr->vAppend(pCard_Direction);
    
    pCard_GbyElist->vAppend(pCard_Expr);
  }

  return pCard_GbyElist;
}

/*----------------------------------------------------------------------------*/
/* collect <project> terms :                                                  */
/*----------------------------------------------------------------------------*/
Card_ProjElist* Dissector::pProjElist(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_ProjElist* pCard_ProjElist = new Card_ProjElist;

  Card_Term* pCard_Term_head = nullptr;
  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsCommaSeparator()) {
      delete pCard_RpnTerms->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
    Card_Expr* pCard_Expr = pExpr_c(pCard_RpnTerms);
    pCard_ProjElist->vAppend(pCard_Expr);
  }
  
  return pCard_ProjElist;
}

/*----------------------------------------------------------------------------*/
/* collect <orderBy> terms :                                                  */
/*----------------------------------------------------------------------------*/
Card_SbyElist* Dissector::pSbyElist(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_SbyElist* pCard_SbyElist = new Card_SbyElist;

  Card_Term* pCard_Term_head = nullptr;
  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsCommaSeparator()) {
      delete pCard_RpnTerms->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
    Card_Expr* pCard_Expr = pExpr_c(pCard_RpnTerms);
    
    pCard_SbyElist->vAppend(pCard_Expr);
  }

  return pCard_SbyElist;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* eval typed calc expr : TYPE <type> [ AS <as> ] <calcExpr> [ ASC | DESC ]   */
/*----------------------------------------------------------------------------*/
Card_Expr* Dissector::pExpr_c(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
   
  //pCard_RpnTerms->vDump();

  Card_Expr* pCard_Expr = new Card_Expr;

  L_TYPE:

  // TYPE <type>
  Card_Term* pCard_Term_popped = pCard_RpnTerms->pPopTerm();
  eTE eTermEval = pCard_Term_popped->eTermEval();
  Stz* szTerm = pCard_Term_popped->sz();

  RASSERT(szTerm->bSame("TYPE"_Stz));
  delete pCard_Term_popped;
  pCard_Term_popped = nullptr;

  pCard_Term_popped = pCard_RpnTerms->pPopTerm();
  Card_Type* pCard_Type = new Card_Type(pCard_Term_popped->sz());
  pCard_Expr->vAppend(pCard_Type);

  delete pCard_Term_popped;
  pCard_Term_popped = nullptr;

  // [ AS <as> ]
  Card* pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && pCard_Term_head->bIsValue("AS"_Stz)) {

    Card_As* pCard_As = pAs(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_As);
  }


  // <calcExpr>
  Card_Calc* pCard_ExprCalc = pExprCalc(pCard_RpnTerms);
  pCard_Expr->vAppend(pCard_ExprCalc);

  // [ AS <as> ]
  pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && pCard_Term_head->bIsValue("AS"_Stz)) {

    Card_As* pCard_As = pAs(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_As);
  }

  // [ ASC | DESC ]
  pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && (pCard_Term_head->bIsValue("ASC"_Stz) ||
          pCard_Term_head->bIsValue("DESC"_Stz))) {

    Card_Direction* pCard_SortDirn = pDirection(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_SortDirn);
  }


//  pCard_Term_head = pCard_RpnTerms->pHead();
//  if (pCard_Term_head
//      && pCard_Term_head->bIsValue("AGGREGATES"_Stz)) {
//
//    Card_Aggregates* pCard_Aggregates = new Card_Aggregates;
//    pCard_Expr->vAppend(pCard_Aggregates);
//
//    pCard_Term_popped = pCard_RpnTerms->pPopTerm();
//    delete pCard_Term_popped;
//
//    //HACK
//    goto L_TYPE;
//  }

  return pCard_Expr;
}

/*----------------------------------------------------------------------------*/
/* collect <predicate>                                                        */
/*----------------------------------------------------------------------------*/
Card_Calc* Dissector::pExprCalc(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_Calc* pCard_ExprCalc = new Card_Calc;

  Card_Term* pCard_Term_popped = nullptr;
  while ( (pCard_Term_popped = _pCard_Term(pCard_RpnTerms->pPop())) ) {

    //if (pCard_Term_popped->bIsValue(")"_Stz))
    //  continue;
    
    if (
        pCard_Term_popped->bIsValue("AGGREGATES"_Stz)
        || pCard_Term_popped->bIsValue("AS"_Stz)
        || pCard_Term_popped->bIsValue("ASC"_Stz)
        || pCard_Term_popped->bIsValue("DESC"_Stz)
        || pCard_Term_popped->bIsValue("GROUPBY"_Stz)
        || pCard_Term_popped->bIsCommaSeparator()
        ) {
      pCard_RpnTerms->vPush(pCard_Term_popped);
      break;
    }

    if (pCard_Term_popped->bIsAggOp())
      pCard_ExprCalc->Y_vSet(P_AggOp);

    pCard_ExprCalc->vAppend(pCard_Term_popped);
  }

  return pCard_ExprCalc;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* append or predicates from term list onto Card_ExprBool                     */
/*----------------------------------------------------------------------------*/
void Dissector::vPredOr(Card_ExprBool* pCard_ExprBool,
                        Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
 
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  UNUSED(pCard_Term);
  
  Card_PRED* pCard_PRED_a = (Card_PRED*)(pCard_ExprBool->pPop());
  Card_PRED* pCard_PRED_b = (Card_PRED*)(pCard_ExprBool->pPop());

  Card_OrPred* pCard_PredOr = new Card_OrPred;
  pCard_PredOr->vAppend(pCard_PRED_b);
  pCard_PredOr->vAppend(pCard_PRED_a);
  pCard_ExprBool->vAppend(pCard_PredOr);
}

/*----------------------------------------------------------------------------*/
/* append and predicates from term list onto Card_ExprBool                    */
/*----------------------------------------------------------------------------*/
void Dissector::vPredAnd(Card_ExprBool* pCard_ExprBool,
                         Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  UNUSED(pCard_Term);
  
  Card_PRED* pCard_PRED_a = (Card_PRED*)(pCard_ExprBool->pPop());
  Card_PRED* pCard_PRED_b = (Card_PRED*)(pCard_ExprBool->pPop());

  Card_AndPred* pCard_PredAnd = new Card_AndPred;
  pCard_PredAnd->vAppend(pCard_PRED_b);
  pCard_PredAnd->vAppend(pCard_PRED_a);
  pCard_ExprBool->vAppend(pCard_PredAnd);
}

/*----------------------------------------------------------------------------*/
/* append cmp predicates from term list onto Card_ExprBool                    */
/*----------------------------------------------------------------------------*/
void Dissector::vPredCmp(Card_ExprBool* pCard_ExprBool,
                         Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Card_CmpPred* pCard_PredCmp = new Card_CmpPred;

  // <col> | <literal>
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  //Stz* szTerm = pCard_Term->sz();
  eTE eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_OTHER == eTermEval);
  pCard_PredCmp->vAppend(pCard_Term);

  // <col> | <literal>
  pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  //szTerm = pCard_Term->sz();
  eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_OTHER == eTermEval);
  pCard_PredCmp->vAppend(pCard_Term);

  // < | <= | == | != | >= | >
  pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  //szTerm = pCard_Term->sz();
  eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_COMPARE == eTermEval);
  pCard_PredCmp->vAppend(pCard_Term);
  
  pCard_ExprBool->vAppend(pCard_PredCmp);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect <as> terms : AS <name>                                             */
/*----------------------------------------------------------------------------*/
Card_As* Dissector::pAs(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  RASSERT(pCard_Term->bIsValue("AS"_Stz));
  delete pCard_Term;
  pCard_Term = nullptr;

  // <name>
  pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szAsNm = pCard_Term->sz();

  Card_As* pCard_As = new Card_As(szAsNm);
  delete pCard_Term;
  pCard_Term = nullptr;

  return pCard_As;
}

/*----------------------------------------------------------------------------*/
/* collect <sortDirn> terms :                                                 */
/*----------------------------------------------------------------------------*/
Card_Direction* Dissector::pDirection(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szDirn = pCard_Term->sz();
  RASSERT(szDirn->bSame("ASC"_Stz) || szDirn->bSame("DESC"_Stz));

  Card_Direction* pCard_SortDirn = new Card_Direction(szDirn);

  delete pCard_Term;
  pCard_Term = nullptr;

  return pCard_SortDirn;
}

/*----------------------------------------------------------------------------*/
/* collect <type> terms : TYPE <columnType>                                   */
/*----------------------------------------------------------------------------*/
Card_Type* Dissector::pType(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  // TYPE
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szTerm = pCard_Term->sz();
  RASSERT(szTerm->bSame("TYPE"_Stz));

  delete pCard_Term;
  pCard_Term = nullptr;

  // column type (e.g. BOOLEAN)
  Card_Term* pCard_Term_type = pCard_RpnTerms->pPopTerm();
  //eCT eColumn = Column::eColumn(pCard_Term_type->szTerm());

  Card_Type* pCard_Type = new Card_Type(pCard_Term_type->sz());

  delete pCard_Term_type;
  pCard_Term_type = nullptr;

  return pCard_Type;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Dissector::iArgCheckLast(TagSet* pArgTags, i8 i_Last)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vArgCheck(pArgTags,i_Last + 1);

  i8 iArg = pArgTags->iValue(i_Last);

  return iArg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissector::pArgCheckLast(TagSet* pArgTags, i8 i_Last)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vArgCheck(pArgTags,i_Last + 1);
  
  Stz* szRpnExpr = pArgTags->szValue(i_Last);
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);

  return pCard_RpnTerms;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissector::pArgCheckLast(TagSet* pArgTags, i8 i_Last,
                                        Stz*& szTableNm, Stz*& szIndexNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,i_Last);
  
  szTableNm = pArgTags->szValue(1);
  szIndexNm = pArgTags->szValue(2);

  return pCard_RpnTerms;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Dissector::szArgCheckLast(TagSet* pArgTags, i8 i_Last)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  vArgCheck(pArgTags,i_Last + 1);
  
  Stz* szRpnExpr = pArgTags->szValue(i_Last);

  return szRpnExpr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Dissector::vArgCheck(TagSet* pArgTags, i8 nArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  i8 nTags = pArgTags->nItems();
    
  if (nArgs != nTags) {
    Stz* szOpNm = m_pOpSpec->szOpNm();
    vTrace("Error: only %lld tags in %s argTags\n"_Stz,nTags,szOpNm);
    NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Dissector::szArgsStz(TagSet* pArgTags, i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

 Stz* szValue = pArgTags->szValue(i_Arg);

  return szValue;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissector::pArgsRpnTerms(TagSet* pArgTags, i8 i_Arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);

  Stz* szRpnExpr = pArgTags->szValue(i_Arg);

  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);

  return pCard_RpnTerms;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert rpn expr string into rpn terms                                     */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissector::pToRpnTerms(Stz* szRpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECTOR,_FOOTER_);
  
  Card_RpnTerms* pCard_RpnTerms = new Card_RpnTerms;
    
  TokenVec* pTokenVec = AM::TOKENISE::pTokeniseSen(szRpnExpr);
    
  i8 nTokens = pTokenVec->nItems();
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    Token* pToken = pTokenVec->pGet(i_Tkn);
    
    //Card_Term* pCard_Term = new Card_Term(pToken->szChars());
    Card_Term* pCard_Term = new Card_Term(pToken);

    pCard_RpnTerms->vAppend(pCard_Term);
  }
  
  delete pTokenVec;

  return pCard_RpnTerms;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
