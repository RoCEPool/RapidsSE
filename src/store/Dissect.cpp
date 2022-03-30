/*============================================================================*/
/* Dissect.cpp -- Dissect                                                     */
/*============================================================================*/

#include "Dissect.hpp"
#include "Optimiser.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Dissect,CN_DISSECT)
__aNewFunction(Dissect,CN_DISSECT)
__vDeleteFunction(Dissect,CN_DISSECT)
__aFooterFunction(Dissect)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Dissect::Dissect(Optimiser* pOptimiser)
: Object(CN_DISSECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
        
  m_pOptimiser    = pOptimiser;
  m_pBench        = nullptr;
  m_pBoard        = nullptr;
  
  m_pPipeSpec = nullptr;
  m_pStageSpec    = nullptr;
  m_pOpSpec       = nullptr;
  
  m_bSawAggOp     = false;
  
  _checkInitialised_(CN_DISSECT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Dissect::~Dissect(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* dissect pipeline spec                                                      */
/*----------------------------------------------------------------------------*/

Board* Dissect::pDissectSpec(PipeSpec* pPipeSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
        
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
  
  return m_pBoard;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dissect pipeline spec                                                      */
/*----------------------------------------------------------------------------*/
void Dissect::vPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szPipelineNm = m_pPipeSpec->szPipeSpecNm();
  //TagSet* pJobTags = m_pPipeSpec->pJobTags();
  //pJobTags->vDumpSet(true);

  m_pBoard = new Board("Board"_Stz,m_pOptimiser);
  m_pBoard->vQueryNm(szPipelineNm);

  //m_pBench->vAppend(m_pBoard);
  
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
void Dissect::vStage(i8 i_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
 
  //Stz* szStageNm = m_pStageSpec->szStageNm();
  eYG eStageTopo = m_pStageSpec->eStageTopology();
  Card_Stage* pCard_Stage = new Card_Stage;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  s32 sRevisedStageNm;
  sRevisedStageNm.vSprint("stage%c"_Stz,'A' + i_StageSpec);
  Card_Name* pCard_Name = new Card_Name(sRevisedStageNm.szStz());
  pCard_Stage->vAppend(pCard_Name);

  Card_StageTopo* pCard_StageTopo = new Card_StageTopo(eStageTopo);
  pCard_Stage->vAppend(pCard_StageTopo);
  
  m_pBoard->vAppend(pCard_Stage);

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
void Dissect::vOp(i8 i_OpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card* pCard;
  Stz* szOpNm = m_pOpSpec->szOpNm();
  TagSet* pArgTags = m_pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);

       if (szOpNm->bSame("aggreg"_Stz))    pCard = pCard_Aggreg   (pArgTags);
  else if (szOpNm->bSame("combined"_Stz))  pCard = pCard_Combined (pArgTags);
  else if (szOpNm->bSame("consumeOp"_Stz)) pCard = pCard_Consume  (pArgTags);
  else if (szOpNm->bSame("debug"_Stz))     pCard = pCard_Debug    (pArgTags);
  else if (szOpNm->bSame("filter"_Stz))    pCard = pCard_Filter   (pArgTags);
  else if (szOpNm->bSame("hash"_Stz))      pCard = pCard_Hash     (pArgTags);
  else if (szOpNm->bSame("indexJoin"_Stz)) pCard = pCard_IndexJoin(pArgTags);
  else if (szOpNm->bSame("indexScan"_Stz)) pCard = pCard_IndexScan(pArgTags);
  else if (szOpNm->bSame("limit"_Stz))     pCard = pCard_Limit    (pArgTags);
  else if (szOpNm->bSame("ordagg"_Stz))    pCard = pCard_OrdAgg   (pArgTags);
  else if (szOpNm->bSame("orderBy"_Stz))   pCard = pCard_Sort     (pArgTags);
  else if (szOpNm->bSame("output"_Stz))    pCard = pCard_Output   (pArgTags);
  else if (szOpNm->bSame("pulse"_Stz))     pCard = pCard_Pulse    (pArgTags);
  else if (szOpNm->bSame("project"_Stz))   pCard = pCard_Project  (pArgTags);
  else if (szOpNm->bSame("scan"_Stz))      pCard = pCard_Scan     (pArgTags);
  else if (szOpNm->bSame("sendSE"_Stz))    pCard = pCard_Send     (pArgTags);
  else if (szOpNm->bSame("sink"_Stz))      pCard = pCard_Sink     (pArgTags);
  else if (szOpNm->bSame("sort"_Stz))      pCard = pCard_Sort     (pArgTags);
  else if (szOpNm->bSame("start"_Stz))     pCard = pCard_Start    (pArgTags);
  else {
    vTrace("Skipping OpSpec %s\n"_Stz,szOpNm);
    pCard = nullptr;
  }
  
  if (pCard)
    m_pBoard->vAppend(pCard);
}

/*----------------------------------------------------------------------------*/
/* dissect : aggreg <iCompiled> <szRpnExpr : 0) aggElist ; 1) gbyElist>       */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Aggreg(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
     
  m_bSawAggOp = true;
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,1);
  Card_Aggreg* pCard_Aggreg = new Card_Aggreg;
  
  eAL eAggLevel = m_pStageSpec->eAggegLevel();
  Card_AggLevel* pCard_AggLevel = new Card_AggLevel(eAggLevel);
  pCard_Aggreg->vAppend(pCard_AggLevel);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggElist : aggregate expression list
  
  Card_AggElist* pCard_AggElist = pAggElist(pCard_RpnTerms);
  pCard_Aggreg->vAppend(pCard_AggElist);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gbyElist : optional groupBy expression list
  
  if (pCard_RpnTerms->nItems()) {
    Card_GbyElist* pCard_ByList = pGbyElist(pCard_RpnTerms);
    pCard_Aggreg->vAppend(pCard_ByList);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;
    
  return pCard_Aggreg;
}

/*----------------------------------------------------------------------------*/
/* dissect : combined <iGenerate>                                             */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Combined(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  Card_Combine* pCard_Combine = new Card_Combine;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  return pCard_Combine;
}

/*----------------------------------------------------------------------------*/
/* dissect : consumeOp <nSends>                                               */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Consume(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : debug <iOption>                                                  */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Debug(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  i8 iOption = iArgCheckLast(pArgTags,0);
  Card_Debug* pCard_Debug = new Card_Debug;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // iOption
  
  Card_Option* pCard_Option = new Card_Option(iOption);
  pCard_Debug->vAppend(pCard_Option);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Debug;
}

/*----------------------------------------------------------------------------*/
/* dissect : filter <iCompiled> <szRpnExpr: 0) filtElist>                     */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Filter(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,1);
  Card_Filter* pCard_Filter = new Card_Filter;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // filtElist : filter expression list

  Card_Expr* pCard_Expr = pExpr_b(pCard_RpnTerms);
  pCard_Filter->vAppend(pCard_Expr);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is it filter or having?
  
  if (! m_bSawAggOp) {
    delete pCard_RpnTerms;
    return pCard_Filter;
  }
  else {
    Card_Having* pCard_Having = new Card_Having;
    pCard_Having->vTransferAll(pCard_Filter);
    delete pCard_Filter;
  
    delete pCard_RpnTerms;
    return pCard_Having;
  }
}

/*----------------------------------------------------------------------------*/
/* dissect : hash <iCompiled> <szRpnExpr : 0) calcExpr>                       */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Hash(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,1);
  Card_Hash* pCard_Hash = new Card_Hash;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // calcExpr : calculation expression list

  pCard_Hash->vTransferAll(pCard_RpnTerms);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;

  return pCard_Hash;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexJoin <iCompiled> <szTblNm> <szIdxNm> <szRpnExpr: 0) idxPred>*/
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_IndexJoin(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;

  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,1,szTableNm,szIndexNm);
  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table
  
  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexJoin->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Index* pIndex = pTable->pFindIndex(szIndexNm);
  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexJoin->vAppend(pCard_Index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index predicate????

  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_RpnTerms);
  pCard_IndexJoin->vAppend(pCard_IndexPred);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  delete pCard_RpnTerms;
  
  return pCard_IndexJoin;
}

/*----------------------------------------------------------------------------*/
/* dissect : indexScan <iCompiled> <szTblNm> <szIdxNm> <szRpnExpr: 0) idxPred>*/
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_IndexScan(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;

  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,1,szTableNm,szIndexNm);
  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table

  Table* pTable = AM::pTableFind(szTableNm);
  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexScan->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index

  Index* pIndex = pTable->pFindIndex(szIndexNm);
  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // index predicate????

  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_RpnTerms);
  pCard_IndexScan->vAppend(pCard_IndexPred);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;
  
  return pCard_IndexScan;
}

/*----------------------------------------------------------------------------*/
/* dissect : limit <iCompiled> <nRows>                                        */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Limit(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  i8 nRows = iArgCheckLast(pArgTags,1);
  Card_Limit* pCard_Limit = new Card_Limit;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // nRows
  
  Card_Rows* pCard_Rows = new Card_Rows(nRows);
  pCard_Limit->vAppend(pCard_Rows);

  return pCard_Limit;
}

/*----------------------------------------------------------------------------*/
/* dissect : ordAgg <iCompiled> <szRpnExpr : 0) aggElist>                     */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_OrdAgg(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  m_bSawAggOp = true;
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,1);
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
Card* Dissect::pCard_Output(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : pulse <szMode>                                                   */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Pulse(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // ignored
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : project <iCompiled> <szRpnExpr : 0) projElist>                   */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Project(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,1);
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
Card* Dissect::pCard_Scan(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
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
  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_Scan->vAppend(pCard_Table);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Scan;
}

/*----------------------------------------------------------------------------*/
/* dissect : send <eSendKind>                                                 */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Send(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  eXK eSendKind = (eXK)iArgCheckLast(pArgTags,0);
  Card_Send* pCard_Send = new Card_Send;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // eSendKind
  
  Card_SendKind* pCard_SendKind = new Card_SendKind(eSendKind);
  pCard_Send->vAppend(pCard_SendKind);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  return pCard_Send;
}

/*----------------------------------------------------------------------------*/
/* dissect : sink                                                             */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Sink(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* dissect : orderBy <iCompiled> <szRpnExpr: 0) sbyElist>                     */
/*----------------------------------------------------------------------------*/
Card* Dissect::pCard_Sort(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,1);
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
Card* Dissect::pCard_Start(TagSet* pArgTags)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* collect <agg> terms :                                                      */
/*----------------------------------------------------------------------------*/
Card_AggElist* Dissect::pAggElist(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_AggElist* pCard_AggElist = new Card_AggElist;
  
  i8 iCardNo = 0;
  Card_Term* pCard_Term_head = nullptr;
  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {
    
    // GROUPBY
    if (pCard_Term_head->bIsSame("GROUPBY"_Stz)) {
      break;
    }
    
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_RpnTerms->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
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
Card_GbyElist* Dissect::pGbyElist(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_GbyElist* pCard_GbyElist = new Card_GbyElist;
  
  // GROUPBY
  Card_Term* pCard_Term_head = _pCard_Term(pCard_RpnTerms->pPop());
  RASSERT(pCard_Term_head->bIsSame("GROUPBY"_Stz));

  i8 iCardNo = 0;
  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_RpnTerms->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
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
Card_ProjElist* Dissect::pProjElist(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_ProjElist* pCard_ProjElist = new Card_ProjElist;

  Card_Term* pCard_Term_head = nullptr;
  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_RpnTerms->pPop();
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
Card_SbyElist* Dissect::pSbyElist(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_SbyElist* pCard_SbyElist = new Card_SbyElist;

  Card_Term* pCard_Term_head = nullptr;
  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_RpnTerms->pPop();
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
Card_Expr* Dissect::pExpr_b(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
   
  //pCard_RpnTerms->vDump();

  Card_Expr* pCard_Expr = new Card_Expr;

  // <type>
  Card_Type* pCard_Type = pType(pCard_RpnTerms);
  pCard_Expr->vAppend(pCard_Type);

  // <boolExpr>
  Card_ExprBool* pCard_ExprBool = pExprBool(pCard_RpnTerms);
  pCard_Expr->vAppend(pCard_ExprBool);


  return pCard_Expr;
}

/*----------------------------------------------------------------------------*/
/* eval typed calc expr : TYPE <type> [ AS <as> ] <calcExpr> [ ASC | DESC ]   */
/*----------------------------------------------------------------------------*/
Card_Expr* Dissect::pExpr_c(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
   
  //pCard_RpnTerms->vDump();

  Card_Expr* pCard_Expr = new Card_Expr;

  // TYPE <type>
  Card_Term* pCard_Term_popped = pCard_RpnTerms->pPopTerm();
  eTE eTermEval = pCard_Term_popped->eTermEval();
  RASSERT(TE_TYPE == eTermEval);
  
  pCard_Term_popped = pCard_RpnTerms->pPopTerm();
  Card_Type* pCard_Type = new Card_Type(pCard_Term_popped->szTerm());
  pCard_Expr->vAppend(pCard_Type);

  // [ AS <as> ]
  Card* pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head->bIsSame("AS"_Stz)) {
        
    Card_As* pCard_As = pAs(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_As);
  }
  
  
  // <calcExpr>
  Card_ExprCalc* pCard_ExprCalc = pExprCalc(pCard_RpnTerms);
  pCard_Expr->vAppend(pCard_ExprCalc);
  
  // [ ASC | DESC ]
  pCard_Term_head = pCard_RpnTerms->pHead();
  if (pCard_Term_head
      && (pCard_Term_head->bIsSame("ASC"_Stz) ||
          pCard_Term_head->bIsSame("DESC"_Stz))) {

    Card_Direction* pCard_SortDirn = pDirection(pCard_RpnTerms);
    pCard_Expr->vAppend(pCard_SortDirn);
  }
    
  return pCard_Expr;
}

/*----------------------------------------------------------------------------*/
/* evaluate boolean predicate termlist                                        */
/*----------------------------------------------------------------------------*/
Card_ExprBool* Dissect::pExprBool(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_ExprBool* pCard_ExprBool = new Card_ExprBool;
      
  while (pCard_RpnTerms->nItems()) {
    eTE eTopTerm = pCard_RpnTerms->eTopTermEval();
    
    switch (eTopTerm) {
      
      case TE_AND:
        vPredAnd(pCard_ExprBool,pCard_RpnTerms);
        break;
        
      case TE_OR:
        vPredOr(pCard_ExprBool,pCard_RpnTerms);
        break;
        
      case TE_OTHER:
        vPredCmp(pCard_ExprBool,pCard_RpnTerms);
        break;
        
      default:
        vTrace("pEvalBoolPred: unexpected eTopTerm %lld %s\n"_Stz
               ,(i8)eTopTerm
               ,aTE[eTopTerm]);
        RASSERT(0);
        break;
    }
  }
  
  return pCard_ExprBool;
}

/*----------------------------------------------------------------------------*/
/* collect <predicate>                                                        */
/*----------------------------------------------------------------------------*/
Card_ExprCalc* Dissect::pExprCalc(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_ExprCalc* pCard_ExprCalc = new Card_ExprCalc;

  Card_Term* pCard_Term_popped = nullptr;
  while ( (pCard_Term_popped = _pCard_Term(pCard_RpnTerms->pPop())) ) {

    if (pCard_Term_popped->bIsSame(")"_Stz))
      continue;
    
    if (   pCard_Term_popped->bIsSame("ASC"_Stz)
        || pCard_Term_popped->bIsSame("DESC"_Stz)
        || pCard_Term_popped->bIsSame("GROUPBY"_Stz)
        || pCard_Term_popped->bIsSeparator()
        ) {
      pCard_RpnTerms->vPush(pCard_Term_popped);
      break;
    }
    
    if (pCard_Term_popped->bIsAggOp())
      pCard_ExprCalc->vAggOp();

    pCard_ExprCalc->vAppend(pCard_Term_popped);
  }

  return pCard_ExprCalc;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* append or predicates from term list onto Card_ExprBool                     */
/*----------------------------------------------------------------------------*/
void Dissect::vPredOr(Card_ExprBool* pCard_ExprBool,
                      Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
 
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  UNUSED(pCard_Term);
  
  Card_PRED* pCard_PRED_a = (Card_PRED*)(pCard_ExprBool->pPop());
  Card_PRED* pCard_PRED_b = (Card_PRED*)(pCard_ExprBool->pPop());

  Card_PredOr* pCard_PredOr = new Card_PredOr;
  pCard_PredOr->vAppend(pCard_PRED_b);
  pCard_PredOr->vAppend(pCard_PRED_a);
  pCard_ExprBool->vAppend(pCard_PredOr);
}

/*----------------------------------------------------------------------------*/
/* append and predicates from term list onto Card_ExprBool                    */
/*----------------------------------------------------------------------------*/
void Dissect::vPredAnd(Card_ExprBool* pCard_ExprBool,
                       Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  UNUSED(pCard_Term);
  
  Card_PRED* pCard_PRED_a = (Card_PRED*)(pCard_ExprBool->pPop());
  Card_PRED* pCard_PRED_b = (Card_PRED*)(pCard_ExprBool->pPop());

  Card_PredAnd* pCard_PredAnd = new Card_PredAnd;
  pCard_PredAnd->vAppend(pCard_PRED_b);
  pCard_PredAnd->vAppend(pCard_PRED_a);
  pCard_ExprBool->vAppend(pCard_PredAnd);
}

/*----------------------------------------------------------------------------*/
/* append cmp predicates from term list onto Card_ExprBool                    */
/*----------------------------------------------------------------------------*/
void Dissect::vPredCmp(Card_ExprBool* pCard_ExprBool,
                       Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_PredCmp* pCard_PredCmp = new Card_PredCmp;

  // <col> | <literal>
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  //Stz* szTerm = pCard_Term->szTerm();
  eTE eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_OTHER == eTermEval);
  pCard_PredCmp->vAppend(pCard_Term);

  // <col> | <literal>
  pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  //szTerm = pCard_Term->szTerm();
  eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_OTHER == eTermEval);
  pCard_PredCmp->vAppend(pCard_Term);

  // < | <= | == | != | >= | >
  pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  //szTerm = pCard_Term->szTerm();
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
Card_As* Dissect::pAs(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  RASSERT(pCard_Term->bIsSame("AS"_Stz));

  // <name>
  pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szAsNm = pCard_Term->szTerm();

  Card_As* pCard_As = new Card_As(szAsNm);

  return pCard_As;
}

/*----------------------------------------------------------------------------*/
/* collect <sortDirn> terms :                                                 */
/*----------------------------------------------------------------------------*/
Card_Direction* Dissect::pDirection(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szDirn = pCard_Term->szTerm();
  RASSERT(szDirn->bSame("ASC"_Stz) || szDirn->bSame("DESC"_Stz));

  Card_Direction* pCard_SortDirn = new Card_Direction(szDirn);

  return pCard_SortDirn;
}

/*----------------------------------------------------------------------------*/
/* collect <type> terms : TYPE <columnType>                                   */
/*----------------------------------------------------------------------------*/
Card_Type* Dissect::pType(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // TYPE
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  eTE eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_TYPE == eTermEval);
  
  // column type (e.g. BOOLEAN)
  Card_Term* pCard_Term_type = pCard_RpnTerms->pPopTerm();
  //eCT eColumn = Column::eColumn(pCard_Term_type->szTerm());

  Card_Type* pCard_Type = new Card_Type(pCard_Term_type->szTerm());

  return pCard_Type;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Dissect::iArgCheckLast(TagSet* pArgTags, i8 i_Last)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  vArgCheck(pArgTags,i_Last + 1);
  
  i8 iArg = pArgTags->iValue(i_Last);

  return iArg;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissect::pArgCheckLast(TagSet* pArgTags, i8 i_Last)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  vArgCheck(pArgTags,i_Last + 1);
  
  Stz* szRpnExpr = pArgTags->szValue(i_Last);
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);

  return pCard_RpnTerms;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnTerms* Dissect::pArgCheckLast(TagSet* pArgTags, i8 i_Last,
                                      Stz*& szTableNm, Stz*& szIndexNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_RpnTerms* pCard_RpnTerms = pArgCheckLast(pArgTags,i_Last);
  
  szTableNm = pArgTags->szValue(1);
  szIndexNm = pArgTags->szValue(2);

  return pCard_RpnTerms;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Dissect::szArgCheckLast(TagSet* pArgTags, i8 i_Last)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  vArgCheck(pArgTags,i_Last + 1);
  
  Stz* szRpnExpr = pArgTags->szValue(i_Last);

  return szRpnExpr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vArgCheck(TagSet* pArgTags, i8 nArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  i8 nTags = pArgTags->nItems();
    
  if (nArgs != nTags) {
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
Card_RpnTerms* Dissect::pToRpnTerms(Stz* szRpnExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_RpnTerms* pCard_RpnTerms = new Card_RpnTerms;
    
  TokenVec* pTokenVec = TokenVec::pTokeniseSen(szRpnExpr);
    
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
