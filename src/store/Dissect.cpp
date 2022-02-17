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
/* dissect AGGREG op spec : aggreg <compiled> <expr>                          */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectAggreg(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szExpr = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in aggreg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bSawAggOp = true;
  
  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_Aggreg* pCard_Aggreg = pAnalyseAggregExpr(pCard_TermList);
  delete pCard_TermList;
  m_pBoard->vAppend(pCard_Aggreg);
}

/*----------------------------------------------------------------------------*/
/* dissect COMBINED op spec : combined                                        */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectCombined(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  Card_Combine* pCard_Combine = new Card_Combine;

  m_pBoard->vAppend(pCard_Combine);
}

/*----------------------------------------------------------------------------*/
/* dissect CONSUME op spec : consumeOp <sends>                                */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectConsume(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect DEBUG op spec : debug <option>                                     */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectDebug(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Stz* szOption = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
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
/* dissect FILTER op spec : filter <compiled> <expr>                          */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectFilter(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szExpr = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in filter argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_Filter* pCard_Filter = pAnalyseFilterExpr(pCard_TermList);
  delete pCard_TermList;

  if (! m_bSawAggOp) {
    m_pBoard->vAppend(pCard_Filter);
    return;
  }
  
  Card_Having* pCard_Having = new Card_Having;
  pCard_Having->vTransferAll(pCard_Filter);
  m_pBoard->vAppend(pCard_Having);
  delete pCard_Filter;
}

/*----------------------------------------------------------------------------*/
/* dissect HASH op spec : hash <expr>                                         */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectHash(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Stz* szExpr = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in hash argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_Hash* pCard_Hash = pAnalyseHashExpr(pCard_TermList);
  delete pCard_TermList;

  m_pBoard->vAppend(pCard_Hash);
}

/*----------------------------------------------------------------------------*/
/* dissect INDEXJOIN op spec : indexJoin <compiled> <tableNm> <indexNm> <expr>*/
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexJoin(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szExpr = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
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
  m_pBoard->vAppend(pCard_IndexJoin);

  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexJoin->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexJoin->vAppend(pCard_Index);

  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_TermList);
  pCard_IndexJoin->vAppend(pCard_IndexPred);
  delete pCard_TermList;
}

/*----------------------------------------------------------------------------*/
/* dissect INDEXSCAN op spec : indexScan <compiled> <tableNm> <indexNm> <expr>*/
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szExpr = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();

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
  m_pBoard->vAppend(pCard_IndexScan);

  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexScan->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_TermList);
  pCard_IndexScan->vAppend(pCard_IndexPred);
  delete pCard_TermList;
}

/*----------------------------------------------------------------------------*/
/* dissect LIMIT op spec : limit <compiled> <rows>                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectLimit(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szRows = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szRows = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in limit argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pBoard->vAppend(new Card_Limit(szRows));
}

/*----------------------------------------------------------------------------*/
/* dissect ORDAGG op spec : aggreg <compiled> <expr>                          */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOrdAgg(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szExpr = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in ordagg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_bSawAggOp = true;

  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_OrdAgg* pCard_OrdAgg = pAnalyseOrdAggExpr(pCard_TermList);
  delete pCard_TermList;
  m_pBoard->vAppend(pCard_OrdAgg);
}

/*----------------------------------------------------------------------------*/
/* dissect OUTPUT op spec : output                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOutput(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect PULSE op spec : : pulse once                                       */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectPulse(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect PROJECT op spec : project <compiled> <expr>                        */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectProject(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szExpr = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in project argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_Project* pCard_Project = pAnalyseProjectExpr(pCard_TermList);
  delete pCard_TermList;
  m_pBoard->vAppend(pCard_Project);
}

/*----------------------------------------------------------------------------*/
/* dissect SCAN op spec : scan [ <catalogNm> <schemaNm> ] <tableNm>           */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szTableNm = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
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

  //vAddScan(pTable);
  
  Card_Scan* pCard_Scan = new Card_Scan;
  pCard_Scan->vTable(pTable);
//  Card_Table* pCard_Table = new Card_Table(pTable);
//  pCard_Scan->vAppend(pCard_Table);
  m_pBoard->vAppend(pCard_Scan);
}

/*----------------------------------------------------------------------------*/
/* dissect SEND op spec : send <sendKind>                                     */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSend(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  //Stz* szSendKind = nullptr;
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

  //eXK eSendKind = (eXK)szSendKind->iToI8();
  eXK eSendKind = (eXK)pArgTags->iValue((i8)0);

  Card_Send* pCard_Send = new Card_Send(eSendKind);

  m_pBoard->vAppend(pCard_Send);
}

/*----------------------------------------------------------------------------*/
/* dissect SINK op spec : sink                                                */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSink(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect SORT op spec : NOT YET IMPLEMENTED                                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSort(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // to be completed ...
}

/*----------------------------------------------------------------------------*/
/* dissect START op spec : : start                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectStart(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  // ignored
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse the terms of an AGGREG <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_Aggreg* Dissect::pAnalyseAggregExpr(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  eAL eAggLvl = m_pStageSpec->eAggegLevel();
  Card_Aggreg* pCard_Aggreg = new Card_Aggreg(eAggLvl);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate sequences
  
  Card_TypedExpr* pCard_Project = nullptr;
  Card_Term* pCard_Term_check = nullptr;
  while ( (pCard_Term_check = pCard_TermList->pPopTerm()) ) {
    
    // GROUPBY
    
    if (pCard_Term_check->bIsSame("GROUPBY"_Stz))
      break;
    
    // SEPARATOR
    
    if (pCard_Term_check->bIsSeparator())
      continue;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    // TYPE
    
    if (pCard_Term_check->bIsSame("TYPE"_Stz)) {
      if (! pCard_Project)
        pCard_Project = new Card_TypedExpr;
      
      // column type (e.g. BOOLEAN)
      Card_Type* pCard_Type = pCollectType(pCard_TermList);
      pCard_Project->vAppend(pCard_Type);
      continue;
    }
    
    // AS
    
    if (pCard_Term_check->bIsSame("AS"_Stz)) {
      if (! pCard_Project)
        pCard_Project = new Card_TypedExpr;

      Card_As* pCard_As = pCollectAs(pCard_TermList);
      pCard_Project->vAppend(pCard_As);
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    pCard_TermList->vPush(pCard_Term_check);
    
    // PROJECT/AGGREG

    if (! pCard_Project)
      pCard_Project = new Card_TypedExpr;
    
    // PREDICATE
    
    Card_Predicate* pCard_Predicate = pCollectPredicate(pCard_TermList);
    pCard_Project->vAppend(pCard_Predicate);
    pCard_Aggreg->vAppend(pCard_Project);
    pCard_Project = nullptr;

  }
  
  // dangling aggreg
  ASSERT(! pCard_Project);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // collect any group by
  
  if (pCard_Term_check) {
    RASSERT(pCard_Term_check->bIsSame("GROUPBY"_Stz));

    Card_ByList* pCard_ByList = pCollectGroupBy(pCard_TermList);
    pCard_Aggreg->vAppend(pCard_ByList);
  }

  return pCard_Aggreg;
}

/*----------------------------------------------------------------------------*/
/* analyse the terms of a FILTER <expr>                                       */
/*----------------------------------------------------------------------------*/
Card_Filter* Dissect::pAnalyseFilterExpr(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Filter* pCard_Filter = pEvalFilter(pCard_TermList);

  return pCard_Filter;
}

/*----------------------------------------------------------------------------*/
/* analyse the terms of a HASH <expr>                                         */
/*----------------------------------------------------------------------------*/
Card_Hash* Dissect::pAnalyseHashExpr(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Hash* pCard_Hash = new Card_Hash;

  pCard_Hash->vTransferAll(pCard_TermList);

  return pCard_Hash;
}

/*----------------------------------------------------------------------------*/
/* analyse the terms of an ORDAGG <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_OrdAgg* Dissect::pAnalyseOrdAggExpr(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_Aggreg* pCard_Aggreg
    = Dissect::pAnalyseAggregExpr(pCard_TermList);
  
  Card_OrdAgg* pCard_OrdAgg = new Card_OrdAgg("partition"_Stz);
  
  pCard_OrdAgg->vTransferAll(pCard_Aggreg);
  
  delete pCard_Aggreg;
  
  return pCard_OrdAgg;
}

/*----------------------------------------------------------------------------*/
/* analyse the terms of a PROJECT <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_Project* Dissect::pAnalyseProjectExpr(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_Project* pCard_Project = new Card_Project(CK_PROJECT);
  

  Card_TypedExpr* pCard_TypedExpr = nullptr;
  Card_Term* pCard_Term_check = nullptr;
  while ( (pCard_Term_check = pCard_TermList->pPopTerm()) ) {
    
    pCard_Term_check->vDump();
    
    // SEPARATOR
    
    if (pCard_Term_check->bIsSeparator())
      continue;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    // TYPE
    
    if (pCard_Term_check->bIsSame("TYPE"_Stz)) {
      if (! pCard_TypedExpr)
        pCard_TypedExpr = new Card_TypedExpr;
      
      // column type (e.g. BOOLEAN)
      Card_Type* pCard_Type = pCollectType(pCard_TermList);
      pCard_TypedExpr->vAppend(pCard_Type);
      continue;
    }
    
    // AS
    
    if (pCard_Term_check->bIsSame("AS"_Stz)) {
      if (! pCard_TypedExpr)
        pCard_TypedExpr = new Card_TypedExpr;

      Card_As* pCard_As = pCollectAs(pCard_TermList);
      pCard_TypedExpr->vAppend(pCard_As);
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    pCard_TermList->vPush(pCard_Term_check);
    
    // PROJECT/AGGREG

    if (! pCard_TypedExpr)
      pCard_TypedExpr = new Card_TypedExpr;
    
    // PREDICATE
    Card_CalcPred* pCard_ProjPred
      = pCollectProjectPred(pCard_TermList);
    
    pCard_TypedExpr->vAppend(pCard_ProjPred);
    
    pCard_Project->vAppend(pCard_TypedExpr);
    pCard_TypedExpr = nullptr;
  }
  
  return pCard_Project;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect <as> terms : AS <name>                                             */
/*----------------------------------------------------------------------------*/
Card_As* Dissect::pCollectAs(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // <name>
  Card_Term* pCard_Term = _pCard_Term(pCard_TermList->pPop());
  Stz* szAsNm = pCard_Term->szTerm();

  Card_As* pCard_As = new Card_As(szAsNm);

  return pCard_As;
}

/*----------------------------------------------------------------------------*/
/* collect <groupby> terms :  GROUPBY { <type> [<as>] <by> }                  */
/*----------------------------------------------------------------------------*/
Card_ByList* Dissect::pCollectGroupBy(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_ByList* pCard_ByList = new Card_ByList;

  while (Card_Term* pCard_Term_typeLit = pCard_TermList->pPopTerm()) {

    Card_By* pCard_By = new Card_By;
    
    // type boolean
    Card_Term* pCard_Term_type = pCard_TermList->pPopTerm();
    //eCT eColumn = Column::eColumn(pCard_Term_type->szTerm());
    Card_Type* pCard_Type = new Card_Type(pCard_Term_type->szTerm());
    pCard_By->vAppend(pCard_Type);

    Card_Term* pCard_Term_test = pCard_TermList->pPopTerm();
    
    if (pCard_Term_test->bIsSame("as"_Stz)) {
      // as name
      Card_Term* pCard_Term_as = pCard_Term_test;
      RASSERT(pCard_Term_as->bIsSame("as"_Stz));
      Card_Term* pCard_Term_asNm = pCard_TermList->pPopTerm();
      Stz* szAsNm = pCard_Term_asNm->szTerm();
      Card_As* pCard_As = new Card_As(szAsNm);
      pCard_By->vAppend(pCard_As);
    }
    else {
      pCard_TermList->vPush(pCard_Term_test);
    }
    
    // by predicate
    Card_Predicate* pCard_Predicate = pCollectPredicate(pCard_TermList);

    pCard_By->vAppend(pCard_Predicate);
    
    pCard_ByList->vAppend(pCard_By);
    
    Card_Term* pCard_Term = pCard_TermList->pPopTerm();
    if (! pCard_Term)
      break;
    if (pCard_Term->bIsSeparator())
      break;
  }

  return pCard_ByList;
}

/*----------------------------------------------------------------------------*/
/* collect <predicate>                                                        */
/*----------------------------------------------------------------------------*/
Card_Predicate* Dissect::pCollectPredicate(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Predicate* pCard_Predicate = new Card_Predicate;

  Card_Term* pCard_Term_pred = nullptr;
  while ( (pCard_Term_pred = _pCard_Term(pCard_TermList->pPop())) ) {

    if (pCard_Term_pred->bIsSame(")"_Stz))
      continue;
    
    if (pCard_Term_pred->bIsSame("GROUPBY"_Stz) ||
        pCard_Term_pred->bIsSeparator()) {
      pCard_TermList->vPush(pCard_Term_pred);
      break;
    }
    
    if (pCard_Term_pred->bIsAggOp())
      pCard_Predicate->vAddProperty("AggOp"_Stz);

    pCard_Predicate->vAppend(pCard_Term_pred);
  }

  return pCard_Predicate;
}

/*----------------------------------------------------------------------------*/
/* collect <predicate>                                                        */
/*----------------------------------------------------------------------------*/
Card_CalcPred* Dissect::pCollectProjectPred(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_CalcPred* pCard_CalcPred = new Card_CalcPred;

  Card_Term* pCard_Term_pred = nullptr;
  while ( (pCard_Term_pred = _pCard_Term(pCard_TermList->pPop())) ) {

    if (pCard_Term_pred->bIsSame(","_Stz)) {
      pCard_TermList->vPush(pCard_Term_pred);
      break;
    }
    
    pCard_CalcPred->vAppend(pCard_Term_pred);
  }

  return pCard_CalcPred;
}

/*----------------------------------------------------------------------------*/
/* collect <type> terms : TYPE <columnType>                                   */
/*----------------------------------------------------------------------------*/
Card_Type* Dissect::pCollectType(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // column type (e.g. BOOLEAN)
  Card_Term* pCard_Term_type = pCard_TermList->pPopTerm();
  //eCT eColumn = Column::eColumn(pCard_Term_type->szTerm());

  Card_Type* pCard_Type = new Card_Type(pCard_Term_type->szTerm());

  return pCard_Type;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* evaluate filter                                                            */
/*----------------------------------------------------------------------------*/
Card_Filter* Dissect::pEvalFilter(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
   
  pCard_TermList->vDump();

  Card_Filter* pCard_Filter = new Card_Filter;

  Card_Term* pCard_Term = _pCard_Term(pCard_TermList->pPop());
  //Stz* szTerm = pCard_Term->szTerm();
  eTE eTermEval = pCard_Term->eTermEval();

  RASSERT(TE_TYPE == eTermEval);
        
  // TYPE <type>
  pCard_Term = _pCard_Term(pCard_TermList->pPop());
  Card_Type* pCard_Type = new Card_Type(pCard_Term->szTerm());
  pCard_Filter->vAppend(pCard_Type);

  // <filterPredicate>
  Card_BoolPred* pCard_BoolPred = pEvalFilterPred(pCard_TermList);
  pCard_Filter->vAppend(pCard_BoolPred);

  return pCard_Filter;
}

/*----------------------------------------------------------------------------*/
/* evaluate filter predicate                                                  */
/*----------------------------------------------------------------------------*/
Card_BoolPred* Dissect::pEvalFilterPred(Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_BoolPred* pCard_BoolPred = new Card_BoolPred;
  
  // all the action is here
    
  while (pCard_TermList->nItems()) {
    eTE eTopTerm = pCard_TermList->eTopTermEval();
    
    switch (eTopTerm) {
      
      case TE_AND:
        vEvalAndPred(pCard_BoolPred,pCard_TermList);
        break;
        
      case TE_OR:
        vEvalOrPred(pCard_BoolPred,pCard_TermList);
        break;
        
      case TE_OTHER:
        vEvalComparePred(pCard_BoolPred,pCard_TermList);
        break;
        
      default:
        vTrace("pEvalFilterPred: unexpected eTopTerm %lld %s\n"_Stz
               ,(i8)eTopTerm
               ,aTE[eTopTerm]);
        RASSERT(0);
        break;
    }

    
  }
  return pCard_BoolPred;
}

/*----------------------------------------------------------------------------*/
/* evaluate or predicate                                                      */
/*----------------------------------------------------------------------------*/
void Dissect::vEvalOrPred(Card_BoolPred* pCard_BoolPred,
                          Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
 
  Card_Term* pCard_Term = _pCard_Term(pCard_TermList->pPop());
  UNUSED(pCard_Term);
  
  Card_PRED* pCard_PRED_a = (Card_PRED*)(pCard_BoolPred->pPop());
  Card_PRED* pCard_PRED_b = (Card_PRED*)(pCard_BoolPred->pPop());

  Card_OrPred* pCard_OrPred = new Card_OrPred;
  pCard_OrPred->vAppend(pCard_PRED_b);
  pCard_OrPred->vAppend(pCard_PRED_a);
  pCard_BoolPred->vAppend(pCard_OrPred);
}

/*----------------------------------------------------------------------------*/
/* evaluate and predicate                                                     */
/*----------------------------------------------------------------------------*/
void Dissect::vEvalAndPred(Card_BoolPred* pCard_BoolPred,
                           Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_Term* pCard_Term = _pCard_Term(pCard_TermList->pPop());
  UNUSED(pCard_Term);
  
  Card_PRED* pCard_PRED_a = (Card_PRED*)(pCard_BoolPred->pPop());
  Card_PRED* pCard_PRED_b = (Card_PRED*)(pCard_BoolPred->pPop());

  Card_AndPred* pCard_AndPred = new Card_AndPred;
  pCard_AndPred->vAppend(pCard_PRED_b);
  pCard_AndPred->vAppend(pCard_PRED_a);
  pCard_BoolPred->vAppend(pCard_AndPred);
}

/*----------------------------------------------------------------------------*/
/* evaluate compare predicate                                                 */
/*----------------------------------------------------------------------------*/
void Dissect::vEvalComparePred(Card_BoolPred* pCard_BoolPred,
                               Card_TermList* pCard_TermList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_ComparePred* pCard_ComparePred = new Card_ComparePred;

  // <col> | <literal>
  Card_Term* pCard_Term = _pCard_Term(pCard_TermList->pPop());
  //Stz* szTerm = pCard_Term->szTerm();
  eTE eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_OTHER == eTermEval);
  pCard_ComparePred->vAppend(pCard_Term);

  // <col> | <literal>
  pCard_Term = _pCard_Term(pCard_TermList->pPop());
  //szTerm = pCard_Term->szTerm();
  eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_OTHER == eTermEval);
  pCard_ComparePred->vAppend(pCard_Term);

  // < | <= | == | != | >= | >
  pCard_Term = _pCard_Term(pCard_TermList->pPop());
  //szTerm = pCard_Term->szTerm();
  eTermEval = pCard_Term->eTermEval();
  RASSERT(TE_COMPARE == eTermEval);
  pCard_ComparePred->vAppend(pCard_Term);
  
  pCard_BoolPred->vAppend(pCard_ComparePred);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert expr string into term list                                         */
/*----------------------------------------------------------------------------*/
Card_TermList* Dissect::pToTermList(Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_TermList* pCard_TermList = new Card_TermList;
    
  TokenVec* pTokenVec = TokenVec::pTokeniseSen(szExpr);
    
  i8 nTokens = pTokenVec->nItems();
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    Token* pToken = pTokenVec->pGet(i_Tkn);
    
    //Card_Term* pCard_Term = new Card_Term(pToken->szChars());
    Card_Term* pCard_Term = new Card_Term(pToken);

    pCard_TermList->vAppend(pCard_Term);
  }
  
  delete pTokenVec;

  return pCard_TermList;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
