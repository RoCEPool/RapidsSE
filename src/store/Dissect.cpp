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
/*-- Private ------------------------------------------------------------------*/
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
/* dissect opspec : aggreg <compiled> <expr>                                  */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectAggreg(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szExpr = nullptr;
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
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
  
  Card_TermSeq* pCard_TermSeq = pToTermSeq(szExpr);
  Card_AggElist* pCard_AggElist = pCollectAggElist(pCard_TermSeq);

  pCard_Aggreg->vAppend(pCard_AggElist);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect optional group by
  
  if (pCard_TermSeq->nItems()) {

    Card_GbyElist* pCard_ByList = pCollectGbyElist(pCard_TermSeq);
    pCard_Aggreg->vAppend(pCard_ByList);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  delete pCard_TermSeq;
  
  m_pBoard->vAppend(pCard_Aggreg);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : combined <generate>                                       */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectCombined(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
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
/* dissect opspec : filter <compiled> <expr>                                  */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectFilter(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szExpr = nullptr;
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in filter argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  
  Card_Filter* pCard_Filter = new Card_Filter;
  
  Card_TermSeq* pCard_TermSeq = pToTermSeq(szExpr);
  Card_TypedExpr* pCard_TypedExpr = pCollectTypedBool(pCard_TermSeq);
  pCard_Filter->vAppend(pCard_TypedExpr);

  
  delete pCard_TermSeq;

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
/* dissect opspec : hash <compiled> <expr>                                    */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectHash(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szExpr = nullptr;
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in hash argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_TermSeq* pCard_TermSeq = pToTermSeq(szExpr);
  
  Card_Hash* pCard_Hash = new Card_Hash;
  pCard_Hash->vTransferAll(pCard_TermSeq);

  delete pCard_TermSeq;

  m_pBoard->vAppend(pCard_Hash);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : indexJoin <compiled> <tableNm> <indexNm> <expr>           */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexJoin(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szExpr = nullptr;
  if (4 == nTags) {
    szTableNm = pArgTags->szValue(1);
    szIndexNm = pArgTags->szValue(2);
    szExpr = pArgTags->szValue(3);
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

  Card_TermSeq* pCard_TermSeq = pToTermSeq(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_TermSeq);
  pCard_IndexJoin->vAppend(pCard_IndexPred);
  
  delete pCard_TermSeq;
  
  m_pBoard->vAppend(pCard_IndexJoin);}

/*----------------------------------------------------------------------------*/
/* dissect opspec : indexScan <compiled> <tableNm> <indexNm> <expr>           */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();

  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szExpr    = nullptr;
  if (4 == nTags) {
    szTableNm = pArgTags->szValue(1);
    szIndexNm = pArgTags->szValue(2);
    szExpr = pArgTags->szValue(3);
  }
  else {
    vTrace("Error: only %lld tags in indexScan argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Table* pTable = AM::pTableFind(szTableNm);
  Index* pIndex = pTable->pFindIndex(szIndexNm);
  Card_IndexScan* pCard_IndexScan = new Card_IndexScan(pIndex);

  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexScan->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  Card_TermSeq* pCard_TermSeq = pToTermSeq(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_TermSeq);
  pCard_IndexScan->vAppend(pCard_IndexPred);
  delete pCard_TermSeq;
  
  m_pBoard->vAppend(pCard_IndexScan);

}

/*----------------------------------------------------------------------------*/
/* dissect opspec : limit <compiled> <nRows>                                  */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectLimit(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
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
/* dissect opspec : ordAgg <compiled> <expr>                                  */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOrdAgg(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szExpr = nullptr;
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in ordagg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bSawAggOp = true;

  
  Card_OrdAgg* pCard_OrdAgg = new Card_OrdAgg("partition"_Stz);
  
  Card_TermSeq* pCard_TermSeq = pToTermSeq(szExpr);
  Card_AggElist* pCard_AggElist = pCollectAggElist(pCard_TermSeq);

  pCard_OrdAgg->vAppend(pCard_AggElist);

  delete pCard_TermSeq;
  m_pBoard->vAppend(pCard_OrdAgg);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : output                                                    */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOutput(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : pulse once                                                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectPulse(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : project <compiled> <expr>                                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectProject(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szExpr = nullptr;
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in project argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Project* pCard_Project = new Card_Project(CK_PROJECT);
  
  Card_TermSeq* pCard_TermSeq = pToTermSeq(szExpr);
  Card_ProjElist* pCard_ProjElist = pCollectProjElist(pCard_TermSeq);
  pCard_Project->vAppend(pCard_ProjElist);

  delete pCard_TermSeq;
  m_pBoard->vAppend(pCard_Project);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : scan [ <catalogNm> <schemaNm> ] <tableNm>                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
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

  Card_Scan* pCard_Scan = new Card_Scan;
  pCard_Scan->vTable(pTable);

  m_pBoard->vAppend(pCard_Scan);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : send <sendKind>                                           */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSend(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (1 == nTags) {
    //szSendKind = pArgTags->szValue((i8)0);
  }
  else {
    vTrace("Error: only %lld tags in send argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

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
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : orderBy <compiled> <expr>                                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSort(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  Stz* szExpr = nullptr;
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in ordagg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Card_Sort* pCard_Sort = new Card_Sort;
  
  Card_TermSeq* pCard_TermSeq = pToTermSeq(szExpr);
  Card_SbyElist* pCard_ObyElist = pCollectSbyElist(pCard_TermSeq);

  pCard_Sort->vAppend(pCard_ObyElist);

  delete pCard_TermSeq;
  m_pBoard->vAppend(pCard_Sort);
}

/*----------------------------------------------------------------------------*/
/* dissect opspec : start                                                     */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectStart(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // ignored
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect <agg> terms :                                                      */
/*----------------------------------------------------------------------------*/
Card_AggElist* Dissect::pCollectAggElist(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_AggElist* pCard_AggElist = new Card_AggElist;
  
  Card_Term* pCard_Term_head = nullptr;
  while ( (pCard_Term_head = pCard_TermSeq->pHeadTerm()) ) {
    
    // GROUPBY
    if (pCard_Term_head->bIsSame("GROUPBY"_Stz)) {
      break;
    }
    
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_TermSeq->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
    Card_TypedExpr* pCard_TypedExpr = pCollectTypedCalc(pCard_TermSeq);
    pCard_AggElist->vAppend(pCard_TypedExpr);
  }
  
  return pCard_AggElist;
}

/*----------------------------------------------------------------------------*/
/* collect <as> terms : AS <name>                                             */
/*----------------------------------------------------------------------------*/
Card_As* Dissect::pCollectAs(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  RASSERT(pCard_Term->bIsSame("AS"_Stz));

  // <name>
  pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  Stz* szAsNm = pCard_Term->szTerm();

  Card_As* pCard_As = new Card_As(szAsNm);

  return pCard_As;
}

/*----------------------------------------------------------------------------*/
/* evaluate boolean predicate termlist                                        */
/*----------------------------------------------------------------------------*/
Card_BoolExpr* Dissect::pCollectBoolExpr(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_BoolExpr* pCard_BoolExpr = new Card_BoolExpr;
  
  // all the action is here
    
  while (pCard_TermSeq->nItems()) {
    eTE eTopTerm = pCard_TermSeq->eTopTermEval();
    
    switch (eTopTerm) {
      
      case TE_AND:
        vEvalAndPred(pCard_BoolExpr,pCard_TermSeq);
        break;
        
      case TE_OR:
        vEvalOrPred(pCard_BoolExpr,pCard_TermSeq);
        break;
        
      case TE_OTHER:
        vEvalCmpPred(pCard_BoolExpr,pCard_TermSeq);
        break;
        
      default:
        vTrace("pEvalBoolPred: unexpected eTopTerm %lld %s\n"_Stz
               ,(i8)eTopTerm
               ,aTE[eTopTerm]);
        RASSERT(0);
        break;
    }

    
  }
  return pCard_BoolExpr;
}

/*----------------------------------------------------------------------------*/
/* collect <predicate>                                                        */
/*----------------------------------------------------------------------------*/
Card_CalcExpr* Dissect::pCollectCalcExpr(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_CalcExpr* pCard_CalcExpr = new Card_CalcExpr;

  Card_Term* pCard_Term_popped = nullptr;
  while ( (pCard_Term_popped = _pCard_Term(pCard_TermSeq->pPop())) ) {

    if (pCard_Term_popped->bIsSame(")"_Stz))
      continue;
    
    if (   pCard_Term_popped->bIsSame("ASC"_Stz)
        || pCard_Term_popped->bIsSame("DESC"_Stz)
        || pCard_Term_popped->bIsSame("GROUPBY"_Stz)
        || pCard_Term_popped->bIsSeparator()
        ) {
      pCard_TermSeq->vPush(pCard_Term_popped);
      break;
    }
    
    if (pCard_Term_popped->bIsAggOp())
      pCard_CalcExpr->vAggOp();

    pCard_CalcExpr->vAppend(pCard_Term_popped);
  }

  return pCard_CalcExpr;
}

/*----------------------------------------------------------------------------*/
/* collect <groupby> terms :  GROUPBY { <type> [<as>] <by> }                  */
/*----------------------------------------------------------------------------*/
Card_GbyElist* Dissect::pCollectGbyElist(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_GbyElist* pCard_GbyElist = new Card_GbyElist;
  
  // GROUPBY
  Card_Term* pCard_Term_head = _pCard_Term(pCard_TermSeq->pPop());
  RASSERT(pCard_Term_head->bIsSame("GROUPBY"_Stz));

  while ( (pCard_Term_head = pCard_TermSeq->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_TermSeq->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
    Card_TypedExpr* pCard_TypedExpr = pCollectTypedCalc(pCard_TermSeq);
    
    pCard_GbyElist->vAppend(pCard_TypedExpr);
  }

  return pCard_GbyElist;
}

/*----------------------------------------------------------------------------*/
/* collect <project> terms :                                                  */
/*----------------------------------------------------------------------------*/
Card_ProjElist* Dissect::pCollectProjElist(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_ProjElist* pCard_ProjElist = new Card_ProjElist;

  Card_Term* pCard_Term_head = nullptr;
  while ( (pCard_Term_head = pCard_TermSeq->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_TermSeq->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
    Card_TypedExpr* pCard_TypedExpr = pCollectTypedCalc(pCard_TermSeq);
    pCard_ProjElist->vAppend(pCard_TypedExpr);
  }
  
  return pCard_ProjElist;
}

/*----------------------------------------------------------------------------*/
/* collect <orderBy> terms :                                                  */
/*----------------------------------------------------------------------------*/
Card_SbyElist* Dissect::pCollectSbyElist(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_SbyElist* pCard_SbyElist = new Card_SbyElist;

  Card_Term* pCard_Term_head = nullptr;
  while ( (pCard_Term_head = pCard_TermSeq->pHeadTerm()) ) {
        
    // SEPARATOR
    if (pCard_Term_head->bIsSeparator()) {
      pCard_TermSeq->pPop();
      continue;
    }
        
    // TYPE <type> [ AS <as> ] <calcExpr>
    Card_TypedExpr* pCard_TypedExpr = pCollectTypedCalc(pCard_TermSeq);
    
    pCard_SbyElist->vAppend(pCard_TypedExpr);
  }

  return pCard_SbyElist;
}

/*----------------------------------------------------------------------------*/
/* collect <sortDirn> terms :                                                 */
/*----------------------------------------------------------------------------*/
Card_SortDirn* Dissect::pCollectSortDirn(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  // AS
  Card_Term* pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  Stz* szDirn = pCard_Term->szTerm();
  RASSERT(szDirn->bSame("ASC"_Stz) || szDirn->bSame("DESC"_Stz));

  Card_SortDirn* pCard_SortDirn = new Card_SortDirn(szDirn);

  return pCard_SortDirn;
}

/*----------------------------------------------------------------------------*/
/* collect <type> terms : TYPE <columnType>                                   */
/*----------------------------------------------------------------------------*/
Card_Type* Dissect::pCollectType(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // TYPE
  Card_Term* pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  eTE eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_TYPE == eTermEval);
  
  // column type (e.g. BOOLEAN)
  Card_Term* pCard_Term_type = pCard_TermSeq->pPopTerm();
  //eCT eColumn = Column::eColumn(pCard_Term_type->szTerm());

  Card_Type* pCard_Type = new Card_Type(pCard_Term_type->szTerm());

  return pCard_Type;
}

/*----------------------------------------------------------------------------*/
/* evaluate typed boolean expr : TYPE <type> <boolExpr>                       */
/*----------------------------------------------------------------------------*/
Card_TypedExpr* Dissect::pCollectTypedBool(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
   
  //pCard_TermSeq->vDump();

  Card_TypedExpr* pCard_TypedExpr = new Card_TypedExpr;

  
  // <type>
  Card_Type* pCard_Type = pCollectType(pCard_TermSeq);
  pCard_TypedExpr->vAppend(pCard_Type);

  // <boolExpr>
  Card_BoolExpr* pCard_BoolExpr = pCollectBoolExpr(pCard_TermSeq);
  pCard_TypedExpr->vAppend(pCard_BoolExpr);
  
  return pCard_TypedExpr;
}

/*----------------------------------------------------------------------------*/
/* eval typed calc expr : TYPE <type> [ AS <as> ] <calcExpr> [ ASC | DESC ]   */
/*----------------------------------------------------------------------------*/
Card_TypedExpr* Dissect::pCollectTypedCalc(Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
   
  //pCard_TermSeq->vDump();

  Card_TypedExpr* pCard_TypedExpr = new Card_TypedExpr;

  // TYPE <type>
  Card_Term* pCard_Term_popped = pCard_TermSeq->pPopTerm();
  eTE eTermEval = pCard_Term_popped->eTermEval();
  RASSERT(TE_TYPE == eTermEval);
  
  pCard_Term_popped = pCard_TermSeq->pPopTerm();
  Card_Type* pCard_Type = new Card_Type(pCard_Term_popped->szTerm());
  pCard_TypedExpr->vAppend(pCard_Type);

  // [ AS <as> ]
  Card* pCard_Term_head = pCard_TermSeq->pHead();
  if (pCard_Term_head->bIsSame("AS"_Stz)) {
        
    Card_As* pCard_As = pCollectAs(pCard_TermSeq);
    pCard_TypedExpr->vAppend(pCard_As);
  }
  
  // <calcExpr>
  Card_CalcExpr* pCard_CalcExpr = pCollectCalcExpr(pCard_TermSeq);
  pCard_TypedExpr->vAppend(pCard_CalcExpr);
  
  // [ ASC | DESC ]
  pCard_Term_head = pCard_TermSeq->pHead();
  if (pCard_Term_head
      && (pCard_Term_head->bIsSame("ASC"_Stz) ||
          pCard_Term_head->bIsSame("DESC"_Stz))) {

    Card_SortDirn* pCard_SortDirn = pCollectSortDirn(pCard_TermSeq);
    pCard_TypedExpr->vAppend(pCard_SortDirn);
  }
  
  return pCard_TypedExpr;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* evaluate or predicate termlist                                             */
/*----------------------------------------------------------------------------*/
void Dissect::vEvalOrPred(Card_BoolExpr* pCard_BoolExpr,
                          Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
 
  Card_Term* pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  UNUSED(pCard_Term);
  
  Card_PRED* pCard_PRED_a = (Card_PRED*)(pCard_BoolExpr->pPop());
  Card_PRED* pCard_PRED_b = (Card_PRED*)(pCard_BoolExpr->pPop());

  Card_OrPred* pCard_OrPred = new Card_OrPred;
  pCard_OrPred->vAppend(pCard_PRED_b);
  pCard_OrPred->vAppend(pCard_PRED_a);
  pCard_BoolExpr->vAppend(pCard_OrPred);
}

/*----------------------------------------------------------------------------*/
/* evaluate and predicate termlist                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vEvalAndPred(Card_BoolExpr* pCard_BoolExpr,
                           Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_Term* pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  UNUSED(pCard_Term);
  
  Card_PRED* pCard_PRED_a = (Card_PRED*)(pCard_BoolExpr->pPop());
  Card_PRED* pCard_PRED_b = (Card_PRED*)(pCard_BoolExpr->pPop());

  Card_AndPred* pCard_AndPred = new Card_AndPred;
  pCard_AndPred->vAppend(pCard_PRED_b);
  pCard_AndPred->vAppend(pCard_PRED_a);
  pCard_BoolExpr->vAppend(pCard_AndPred);
}

/*----------------------------------------------------------------------------*/
/* evaluate compare predicate termlist                                        */
/*----------------------------------------------------------------------------*/
void Dissect::vEvalCmpPred(Card_BoolExpr* pCard_BoolExpr,
                               Card_TermSeq* pCard_TermSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_CmpPred* pCard_CmpPred = new Card_CmpPred;

  // <col> | <literal>
  Card_Term* pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  //Stz* szTerm = pCard_Term->szTerm();
  eTE eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_OTHER == eTermEval);
  pCard_CmpPred->vAppend(pCard_Term);

  // <col> | <literal>
  pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  //szTerm = pCard_Term->szTerm();
  eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_OTHER == eTermEval);
  pCard_CmpPred->vAppend(pCard_Term);

  // < | <= | == | != | >= | >
  pCard_Term = _pCard_Term(pCard_TermSeq->pPop());
  //szTerm = pCard_Term->szTerm();
  eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_COMPARE == eTermEval);
  pCard_CmpPred->vAppend(pCard_Term);
  
  pCard_BoolExpr->vAppend(pCard_CmpPred);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert expr string into term list                                         */
/*----------------------------------------------------------------------------*/
Card_TermSeq* Dissect::pToTermSeq(Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_TermSeq* pCard_TermSeq = new Card_TermSeq;
    
  TokenVec* pTokenVec = TokenVec::pTokeniseSen(szExpr);
    
  i8 nTokens = pTokenVec->nItems();
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    Token* pToken = pTokenVec->pGet(i_Tkn);
    
    //Card_Term* pCard_Term = new Card_Term(pToken->szChars());
    Card_Term* pCard_Term = new Card_Term(pToken);

    pCard_TermSeq->vAppend(pCard_Term);
  }
  
  delete pTokenVec;

  return pCard_TermSeq;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
