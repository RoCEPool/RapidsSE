/*============================================================================*/
/* Dissect.cpp -- Dissect                                                     */
/*============================================================================*/

#include "Dissect.hpp"
#include "SubPipelines.hpp"
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
Dissect::Dissect(void)
: Object(CN_DISSECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
        
  m_pBench        = nullptr;
  m_pBoard        = nullptr;
  m_pPipelineSpec = nullptr;
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
void Dissect::vDissectSpec(Bench* pBench, PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  m_pBench = pBench;
  
  SubPipelines* pSubPipelines = pPipelineSpec->pSubPipelines();
  if (pSubPipelines) {
    i8 nSubPipelines = pSubPipelines->nItems();
    for (i8 i_SubPipeline = 0; i_SubPipeline < nSubPipelines; i_SubPipeline++) {
      m_pPipelineSpec = pSubPipelines->pGet(i_SubPipeline);
      
      vDissectPipeline();
    }
  }
  
  m_pPipelineSpec = pPipelineSpec;
  vDissectPipeline();
  
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
void Dissect::vDissectPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szPipelineNm = m_pPipelineSpec->szPipelineNm();
  //TagSet* pJobTags = m_pPipelineSpec->pJobTags();
  //pJobTags->vDumpSet(true);

  m_pBoard = new Board("Board"_Stz);
  m_pBoard->vQueryNm(szPipelineNm);

  m_pBench->vAppend(m_pBoard);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  i8 nStageSpecs = m_pPipelineSpec->nItems();
  for (i8 i_StageSpec = 1; i_StageSpec < nStageSpecs - 1; i_StageSpec++) {
    m_pStageSpec = m_pPipelineSpec->pGet(i_StageSpec);
    
    vDissectStage(i_StageSpec);
  }
}

/*----------------------------------------------------------------------------*/
/* dissect stage spec                                                         */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectStage(i8 i_StageSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
 
  //Stz* szStageNm = m_pStageSpec->szStageNm();
  eYG eStageTopology = m_pStageSpec->eStageTopology();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sRevisedStageNm;
  sRevisedStageNm.vSprint("stage%c"_Stz,'A' + i_StageSpec);
  Card_Stage* pCard_Stage = new Card_Stage(sRevisedStageNm.szStz(),
                                           eStageTopology);
  m_pBoard->vAppend(pCard_Stage);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bSawAggOp = false;
  
  i8 nOpSpecs = m_pStageSpec->nItems();
  for (i8 i_OpSpec = 0; i_OpSpec < nOpSpecs; i_OpSpec++) {
    m_pOpSpec = m_pStageSpec->pGet(i_OpSpec);
    
    vDissectOp(i_OpSpec);
  }
}

/*----------------------------------------------------------------------------*/
/* dissect op spec                                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOp(i8 i_OpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Stz* szOpNm = m_pOpSpec->szOpNm();
  
       if (szOpNm->bSame("aggreg"_Stz))     vDissectAggreg   (m_pOpSpec);
  else if (szOpNm->bSame("combined"_Stz))   vDissectCombined (m_pOpSpec);
  else if (szOpNm->bSame("consumeOp"_Stz))  vDissectConsume  (m_pOpSpec);
  else if (szOpNm->bSame("debug"_Stz))      vDissectDebug    (m_pOpSpec);
  else if (szOpNm->bSame("filter"_Stz))     vDissectFilter   (m_pOpSpec);
  else if (szOpNm->bSame("hash"_Stz))       vDissectHash     (m_pOpSpec);
  else if (szOpNm->bSame("indexJoin"_Stz))  vDissectIndexJoin(m_pOpSpec);
  else if (szOpNm->bSame("indexScan"_Stz))  vDissectIndexScan(m_pOpSpec);
  else if (szOpNm->bSame("limit"_Stz))      vDissectLimit    (m_pOpSpec);
  else if (szOpNm->bSame("ordagg"_Stz))     vDissectOrdAgg   (m_pOpSpec);
  else if (szOpNm->bSame("orderBy"_Stz))    vDissectSort     (m_pOpSpec);
  else if (szOpNm->bSame("output"_Stz))     vDissectOutput   (m_pOpSpec);
  else if (szOpNm->bSame("pulse"_Stz))      vDissectPulse    (m_pOpSpec);
  else if (szOpNm->bSame("project"_Stz))    vDissectProject  (m_pOpSpec);
  else if (szOpNm->bSame("scan"_Stz))       vDissectScan     (m_pOpSpec);
  else if (szOpNm->bSame("send"_Stz))       vDissectSend     (m_pOpSpec);
  else if (szOpNm->bSame("sink"_Stz))       vDissectSink     (m_pOpSpec);
  else if (szOpNm->bSame("sort"_Stz))       vDissectSort     (m_pOpSpec);
  else if (szOpNm->bSame("start"_Stz))      vDissectStart    (m_pOpSpec);
  else {
    vTrace("Skipping OpSpec %s\n"_Stz,szOpNm);
  }
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : aggreg <compiled> <rpnExpr>                               */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectAggreg(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // 0) aggElist ; 1) gbyList
 
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szRpnExpr = nullptr;
  if (2 == nTags) {
    szRpnExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in aggreg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bSawAggOp = true;
  
  eAL eAggLvl = m_pStageSpec->eAggegLevel();
  Card_Aggreg* pCard_Aggreg = new Card_Aggreg(eAggLvl);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate expression list
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);
  Card_AggElist* pCard_AggElist = pAggElist(pCard_RpnTerms);

  pCard_Aggreg->vAppend(pCard_AggElist);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect optional group by
  
  if (pCard_RpnTerms->nItems()) {

    Card_GbyElist* pCard_ByList = pGbyElist(pCard_RpnTerms);
    pCard_Aggreg->vAppend(pCard_ByList);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_RpnTerms;
  
  m_pBoard->vAppend(pCard_Aggreg);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : combined <generate>                                       */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectCombined(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  // 0) combined :

  Card_Combine* pCard_Combine = new Card_Combine;

  m_pBoard->vAppend(pCard_Combine);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : consumeOp <sends>                                         */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectConsume(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : debug <option>                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectDebug(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szOption = nullptr;
  if (2 == nTags) {
    szOption = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in debug argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Debug* pCard_Debug = new Card_Debug(szOption);
  
  m_pBoard->vAppend(pCard_Debug);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : filter <compiled> <rpnExpr>                               */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectFilter(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // filter : 0) filtElist

  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szRpnExpr = nullptr;
  if (2 == nTags) {
    szRpnExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in filter argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Card_Filter* pCard_Filter = new Card_Filter;
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);
  
  
  
  
  Card_Expr* pCard_Expr = pExpr_b(pCard_RpnTerms);

  
  
  pCard_Filter->vAppend(pCard_Expr);

  delete pCard_RpnTerms;

  if (! m_bSawAggOp) {
    m_pBoard->vAppend(pCard_Filter);
    return;
  }
  
  Card_Having* pCard_Having = new Card_Having;
  pCard_Having->vTransferAll(pCard_Filter);
  
  delete pCard_Filter;
  
  m_pBoard->vAppend(pCard_Having);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : hash <compiled> <rpnExpr>                                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectHash(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // hash : 0) calcExpr??

  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szRpnExpr = nullptr;
  if (2 == nTags) {
    szRpnExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in hash argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);
  
  Card_Hash* pCard_Hash = new Card_Hash;
  pCard_Hash->vTransferAll(pCard_RpnTerms);

  delete pCard_RpnTerms;

  m_pBoard->vAppend(pCard_Hash);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : indexJoin <compiled> <tableNm> <indexNm> <rpnExpr>        */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexJoin(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // indexJoin : 0) {table} 1) {index} 2) indexPred
  
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szRpnExpr = nullptr;
  if (4 == nTags) {
    szTableNm = pArgTags->szValue(1);
    szIndexNm = pArgTags->szValue(2);
    szRpnExpr = pArgTags->szValue(3);
  }
  else {
    vTrace("Error: only %lld tags in indexJoin argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Table* pTable = AM::pTableFind(szTableNm);
  Index* pIndex = pTable->pFindIndex(szIndexNm);
  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin(pIndex);

  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexJoin->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexJoin->vAppend(pCard_Index);

  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_RpnTerms);
  pCard_IndexJoin->vAppend(pCard_IndexPred);
  
  delete pCard_RpnTerms;
  
  m_pBoard->vAppend(pCard_IndexJoin);}

/*----------------------------------------------------------------------------*/
/* dissect opspec : indexScan <compiled> <tableNm> <indexNm> <rpnExpr>        */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  // indexScan : 0) {table} 1) {index} 2) indexPred

  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();

  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szRpnExpr    = nullptr;
  if (4 == nTags) {
    szTableNm = pArgTags->szValue(1);
    szIndexNm = pArgTags->szValue(2);
    szRpnExpr = pArgTags->szValue(3);
  }
  else {
    vTrace("Error: only %lld tags in indexScan argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Table* pTable = AM::pTableFind(szTableNm);
  Index* pIndex = pTable->pFindIndex(szIndexNm);
  Card_IndexScan* pCard_IndexScan = new Card_IndexScan;

  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexScan->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_RpnTerms);
  pCard_IndexScan->vAppend(pCard_IndexPred);
  delete pCard_RpnTerms;
  
  m_pBoard->vAppend(pCard_IndexScan);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : limit <compiled> <nRows>                                  */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectLimit(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // limit : 0) rows

  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();

  Stz* szRows = nullptr;
  i8 nRows = -1;
  if (2 == nTags) {
    szRows = pArgTags->szValue(1);
    nRows = szRows->iToI8();
  }
  else {
    vTrace("Error: only %lld tags in limit argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Card_Limit* pCard_Limit = new Card_Limit(nRows);
  
  m_pBoard->vAppend(pCard_Limit);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : ordAgg <compiled> <rpnExpr>                               */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOrdAgg(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // ordAgg : 0) aggElist
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szRpnExpr = nullptr;
  if (2 == nTags) {
    szRpnExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in ordagg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bSawAggOp = true;

  Card_OrdAgg* pCard_OrdAgg = new Card_OrdAgg("partition"_Stz);
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);
  Card_AggElist* pCard_AggElist = pAggElist(pCard_RpnTerms);

  pCard_OrdAgg->vAppend(pCard_AggElist);

  delete pCard_RpnTerms;
  m_pBoard->vAppend(pCard_OrdAgg);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : output                                                    */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOutput(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  // outPut :
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : pulse once                                                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectPulse(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // pulse :
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : project <compiled> <rpnExpr>                              */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectProject(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // project : 0) projElist
  
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szRpnExpr = nullptr;
  if (2 == nTags) {
    szRpnExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in project argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Project* pCard_Project = new Card_Project(CK_PROJECT);
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);
  Card_ProjElist* pCard_ProjElist = pProjElist(pCard_RpnTerms);
  pCard_Project->vAppend(pCard_ProjElist);

  delete pCard_RpnTerms;
  m_pBoard->vAppend(pCard_Project);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : scan [ <catalogNm> <schemaNm> ] <tableNm>                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // scan : 0) {table}

  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Table* pTable = AM::pTableFind(szTableNm);

  Card_Table* pCard_Table = new Card_Table(pTable);
  
  Card_Scan* pCard_Scan = new Card_Scan;
  pCard_Scan->vAppend(pCard_Table);

  m_pBoard->vAppend(pCard_Scan);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : send <sendKind>                                           */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSend(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // send :

  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  if (1 == nTags) {
    //szSendKind = pArgTags->szValue((i8)0);
  }
  else {
    vTrace("Error: only %lld tags in send argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  eXK eSendKind = (eXK)pArgTags->iValue((i8)0);

  Card_Send* pCard_Send = new Card_Send(eSendKind);

  m_pBoard->vAppend(pCard_Send);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : sink                                                      */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSink(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // sink :
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : orderBy <compiled> <rpnExpr>                              */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSort(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // sort : 0) ???

  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szRpnExpr = nullptr;
  if (2 == nTags) {
    szRpnExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in ordagg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Card_Sort* pCard_Sort = new Card_Sort;
  
  Card_RpnTerms* pCard_RpnTerms = pToRpnTerms(szRpnExpr);
  Card_SbyElist* pCard_ObyElist = pSbyElist(pCard_RpnTerms);

  pCard_Sort->vAppend(pCard_ObyElist);

  delete pCard_RpnTerms;
  m_pBoard->vAppend(pCard_Sort);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : start                                                     */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectStart(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // start :
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

  while ( (pCard_Term_head = pCard_RpnTerms->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_RpnTerms->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
    Card_Expr* pCard_Expr = pExpr_c(pCard_RpnTerms);
    
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
  Card_Type* pCard_Type = pCollectType(pCard_RpnTerms);
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
        
    Card_As* pCard_As = pCollectAs(pCard_RpnTerms);
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

    Card_SortDirn* pCard_SortDirn = pCollectSortDirn(pCard_RpnTerms);
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
Card_As* Dissect::pCollectAs(Card_RpnTerms* pCard_RpnTerms)
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
Card_SortDirn* Dissect::pCollectSortDirn(Card_RpnTerms* pCard_RpnTerms)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_RpnTerms->pPop());
  Stz* szDirn = pCard_Term->szTerm();
  RASSERT(szDirn->bSame("ASC"_Stz) || szDirn->bSame("DESC"_Stz));

  Card_SortDirn* pCard_SortDirn = new Card_SortDirn(szDirn);

  return pCard_SortDirn;
}

/*----------------------------------------------------------------------------*/
/* collect <type> terms : TYPE <columnType>                                   */
/*----------------------------------------------------------------------------*/
Card_Type* Dissect::pCollectType(Card_RpnTerms* pCard_RpnTerms)
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
