/*============================================================================*/
/* Dissect.cpp -- Dissect                                                     */
/*============================================================================*/

#include "Dissect.hpp"
#include "Bench.hpp"
#include "SubPipelines.hpp"

#include "eAE.h"
#include "Eval.hpp"

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
Dissect::Dissect(Bench* pBench)
: Object(CN_DISSECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
        
  m_pBench        = pBench;
    
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
void Dissect::vDissectSpec(PipelineSpec* pPipelineSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
      
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
  Card_Stage* pCard_Stage = new Card_Stage(CK_STAGE,
                                           sRevisedStageNm.szStz(),
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
      
  Card_Combine* pCard_Combine = new Card_Combine(CK_COMBINE);

  m_pBoard->vAppend(pCard_Combine);
}

/*----------------------------------------------------------------------------*/
/* dissect CONSUME op spec : consumeOp <Sends>                                */
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

  Card_Debug* pCard_Debug = new Card_Debug(CK_DEBUG,szOption);
  
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
  
  Card_Having* pCard_Having = new Card_Having(CK_HAVING);
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
  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin(CK_INDEX,pIndex);
  m_pBoard->vAppend(pCard_IndexJoin);

  Card_Table* pCard_Table = new Card_Table(CK_TABLE,pTable);
  pCard_IndexJoin->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(CK_INDEX,pIndex);
  pCard_IndexJoin->vAppend(pCard_Index);

  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred(CK_INDEXPRED);
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
  Card_IndexScan* pCard_IndexScan = new Card_IndexScan(CK_INDEXSCAN,pIndex);
  m_pBoard->vAppend(pCard_IndexScan);

  Card_Table* pCard_Table = new Card_Table(CK_TABLE,pTable);
  pCard_IndexScan->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(CK_INDEX,pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  Card_TermList* pCard_TermList = pToTermList(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred(CK_INDEXPRED);
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
  
  m_pBoard->vAppend(new Card_Limit(CK_LIMIT,szRows));
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
  
  Card_Scan* pCard_Scan = new Card_Scan(CK_SCAN);
  Card_Table* pCard_Table = new Card_Table(CK_TABLE,pTable);
  pCard_Scan->vAppend(pCard_Table);
  m_pBoard->vAppend(pCard_Scan);
}

/*----------------------------------------------------------------------------*/
/* dissect SEND op spec : send <sendKind>                                     */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSend(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Stz* szSendKind = nullptr;
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  i8 nTags = pArgTags->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (1 == nTags) {
    szSendKind = pArgTags->szValue((i8)0);
  }
  else {
    vTrace("Error: only %lld tags in send argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  eXK eSendKind = (eXK)szSendKind->iToI8();
  
  Card_Send* pCard_Send = new Card_Send(CK_SEND,eSendKind);

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
/* analyse the atoms of an AGGREG <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_Aggreg* Dissect::pAnalyseAggregExpr(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_Aggreg* pCard_Aggreg = new Card_Aggreg(CK_AGGREG);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // topology

  eAL eAggLvl = eAggegLevel();
  pCard_Aggreg->vAggLevel(eAggLvl);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate sequences
  
  Card_Projection* pCard_Project = nullptr;
  Card_Term* pCard_Term_check = nullptr;
  while ( (pCard_Term_check = pCard_TermList_expr->pPopTerm()) ) {
    
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
        pCard_Project = new Card_Projection(CK_PROJECTION);
      
      // column type (e.g. BOOLEAN)
      Card_Type* pCard_Type = pCollectType(pCard_TermList_expr);
      pCard_Project->vAppend(pCard_Type);
      continue;
    }
    
    // AS
    
    if (pCard_Term_check->bIsSame("AS"_Stz)) {
      if (! pCard_Project)
        pCard_Project = new Card_Projection(CK_PROJECTION);

      Card_As* pCard_As = pCollectAs(pCard_TermList_expr);
      pCard_Project->vAppend(pCard_As);
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    pCard_TermList_expr->vPush(pCard_Term_check);
    
    // PROJECT/AGGREG

    if (! pCard_Project)
      pCard_Project = new Card_Projection(CK_PROJECTION);
    
    // PREDICATE
    
    Card_Predicate* pCard_Predicate = pCollectPredicate(pCard_TermList_expr);
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

    Card_ByList* pCard_ByList = pCollectBy(pCard_TermList_expr);
    pCard_Aggreg->vAppend(pCard_ByList);
  }

  return pCard_Aggreg;
}

/*----------------------------------------------------------------------------*/
/* analyse the atoms of a FILTER <expr>                                       */
/*----------------------------------------------------------------------------*/
Card_Filter* Dissect::pAnalyseFilterExpr(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Filter* pCard_Filter = new Card_Filter(CK_FILTER);

  pCard_Filter->vTransferAll(pCard_TermList_expr);

  vEvaluateFilter(pCard_Filter);

  return pCard_Filter;
}

/*----------------------------------------------------------------------------*/
/* analyse the atoms of a HASH <expr>                                         */
/*----------------------------------------------------------------------------*/
Card_Hash* Dissect::pAnalyseHashExpr(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Hash* pCard_Hash = new Card_Hash(CK_HASH);

  pCard_Hash->vTransferAll(pCard_TermList_expr);

  return pCard_Hash;
}

/*----------------------------------------------------------------------------*/
/* analyse the atoms of an ORDAGG <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_OrdAgg* Dissect::pAnalyseOrdAggExpr(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_Aggreg* pCard_Aggreg
    = Dissect::pAnalyseAggregExpr(pCard_TermList_expr);
  
  Card_OrdAgg* pCard_OrdAgg = new Card_OrdAgg(CK_ORDAGG);
  
  pCard_OrdAgg->vTransferAll(pCard_Aggreg);
  
  delete pCard_Aggreg;
  
  return pCard_OrdAgg;
}

/*----------------------------------------------------------------------------*/
/* analyse the atoms of a PROJECT <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_Project* Dissect::pAnalyseProjectExpr(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_Project* pCard_Project = new Card_Project(CK_PROJECT);
  

  Card_Projection* pCard_Projection = nullptr;
  Card_Term* pCard_Term_check = nullptr;
  while ( (pCard_Term_check = pCard_TermList_expr->pPopTerm()) ) {
    
    // SEPARATOR
    
    if (pCard_Term_check->bIsSeparator())
      continue;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    // TYPE
    
    if (pCard_Term_check->bIsSame("TYPE"_Stz)) {
      if (! pCard_Projection)
        pCard_Projection = new Card_Projection(CK_PROJECTION);
      
      // column type (e.g. BOOLEAN)
      Card_Type* pCard_Type = pCollectType(pCard_TermList_expr);
      pCard_Projection->vAppend(pCard_Type);
      continue;
    }
    
    // AS
    
    if (pCard_Term_check->bIsSame("AS"_Stz)) {
      if (! pCard_Projection)
        pCard_Projection = new Card_Projection(CK_PROJECTION);

      Card_As* pCard_As = pCollectAs(pCard_TermList_expr);
      pCard_Projection->vAppend(pCard_As);
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    pCard_TermList_expr->vPush(pCard_Term_check);
    
    // PROJECT/AGGREG

    if (! pCard_Projection)
      pCard_Projection = new Card_Projection(CK_PROJECTION);
    
    // PREDICATE
    Card_Predicate* pCard_Predicate
      = pCollectPredicate(pCard_TermList_expr);
    
    pCard_Projection->vAppend(pCard_Predicate);
    
    pCard_Project->vAppend(pCard_Projection);
    pCard_Projection = nullptr;
  }
  
  return pCard_Project;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect <as> atoms : AS <name>                                             */
/*----------------------------------------------------------------------------*/
Card_As* Dissect::pCollectAs(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // <name>
  Card_Term* pCard_Term = _pCard_Term(pCard_TermList_expr->pPop());
  Stz* szAsNm = pCard_Term->szTerm();

  Card_As* pCard_As = new Card_As(CK_AS,szAsNm);

  return pCard_As;
}

/*----------------------------------------------------------------------------*/
/* collect <groupby> atoms :  GROUPBY { <type> [<as>] <by> }                  */
/*----------------------------------------------------------------------------*/
Card_ByList* Dissect::pCollectBy(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_ByList* pCard_ByList = new Card_ByList(CK_BYLIST);

  while (Card_Term* pCard_Term_typeLit = pCard_TermList_expr->pPopTerm()) {

    Card_By* pCard_By = new Card_By(CK_BY);
    
    // type boolean
    Card_Term* pCard_Term_type = pCard_TermList_expr->pPopTerm();
    //eCT eColumn = Column::eColumn(pCard_Term_type->szTerm());
    Card_Type* pCard_Type = new Card_Type(CK_TYPE,pCard_Term_type->szTerm());
    pCard_By->vAppend(pCard_Type);

    Card_Term* pCard_Term_test = pCard_TermList_expr->pPopTerm();
    
    if (pCard_Term_test->bIsSame("as"_Stz)) {
      // as name
      Card_Term* pCard_Term_as = pCard_Term_test;
      RASSERT(pCard_Term_as->bIsSame("as"_Stz));
      Card_Term* pCard_Term_asNm = pCard_TermList_expr->pPopTerm();
      Stz* szAsNm = pCard_Term_asNm->szTerm();
      Card_As* pCard_As = new Card_As(CK_AS,szAsNm);
      pCard_By->vAppend(pCard_As);
    }
    else {
      pCard_TermList_expr->vPush(pCard_Term_test);
    }
    
    // by predicate
    Card_Predicate* pCard_Predicate = pCollectPredicate(pCard_TermList_expr);

    pCard_By->vAppend(pCard_Predicate);
    
    pCard_ByList->vAppend(pCard_By);
    
    Card_Term* pCard_Term = pCard_TermList_expr->pPopTerm();
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
Card_Predicate* Dissect::pCollectPredicate(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Predicate* pCard_Predicate = new Card_Predicate(CK_PREDICATE);

  Card_Term* pCard_Term_pred = nullptr;
  while ( (pCard_Term_pred = _pCard_Term(pCard_TermList_expr->pPop())) ) {

    if (pCard_Term_pred->bIsSame(")"_Stz))
      continue;
    
    if (pCard_Term_pred->bIsSame("GROUPBY"_Stz) ||
        pCard_Term_pred->bIsSeparator()) {
      pCard_TermList_expr->vPush(pCard_Term_pred);
      break;
    }
    
    if (pCard_Term_pred->bIsAggOp())
      pCard_Predicate->vAddProperty("AggOp"_Stz);

    pCard_Predicate->vAppend(pCard_Term_pred);
  }

  return pCard_Predicate;
}
/*----------------------------------------------------------------------------*/
/* collect <type> atoms : TYPE <columnType>                                   */
/*----------------------------------------------------------------------------*/
Card_Type* Dissect::pCollectType(Card_TermList* pCard_TermList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // column type (e.g. BOOLEAN)
  Card_Term* pCard_Term_type = pCard_TermList_expr->pPopTerm();
  //eCT eColumn = Column::eColumn(pCard_Term_type->szTerm());

  Card_Type* pCard_Type = new Card_Type(CK_TYPE,pCard_Term_type->szTerm());

  return pCard_Type;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode stage topology into aggreg level                                    */
/*----------------------------------------------------------------------------*/
eAL Dissect::eAggegLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  eYG eStageTopology = m_pStageSpec->eStageTopology();
  if (YG_ORIGIN == eStageTopology)
    return AL_ORIGIN;
  else
  if (YG_ALL_NODES == eStageTopology)
    return AL_NODE;
  else
  if (YG_ALL_PARTITIONS == eStageTopology)
    return AL_PARTITION;
  else
    NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* convert expr string into atom list                                         */
/*----------------------------------------------------------------------------*/
Card_TermList* Dissect::pToTermList(Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_TermList* pCard_TermList_expr = new Card_TermList(CK_TERMLIST);
    
  TokenVec* pTokenVec = TokenVec::pTokenise(szExpr);
    
  i8 nTokens = pTokenVec->nItems();
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    Token* pToken = pTokenVec->pGet(i_Tkn);
    
    Card_Term* pCard_Term = new Card_Term(CK_TERM,pToken->szChars());

    pCard_TermList_expr->vAppend(pCard_Term);
  }
  
  delete pTokenVec;

  return pCard_TermList_expr;
}




/*----------------------------------------------------------------------------*/
/* evaluate filter                                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vEvaluateFilter(Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
 
  Card* pCard_PredA = nullptr;
  Card* pCard_PredB = nullptr;

  Card_OrPred*      pCard_OrPred      = nullptr;
  Card_AndPred*     pCard_AndPred     = nullptr;
  Card_ComparePred* pCard_ComparePred = nullptr;
  
  Card_Filter* pCard_Filter_nested = new Card_Filter(CK_FILTER);
  Card_Filter* pCard_Filter_others = new Card_Filter(CK_FILTER);

  Card_Expr* pCard_Expr = new Card_Expr(CK_EXPR);
  
  while (Card* pPoppedCard = pCard_Filter->pPop()) {
    
    Card_Term* pCard_Term = _pCard_Term(pPoppedCard);
    Stz* szTerm = pCard_Term->szTerm();
    eAE eAtomEval = Eval::eAtomEval(szTerm);
    
    switch (eAtomEval) {
        
      case AE_OR:
        if (pCard_ComparePred) {
          pCard_Filter_nested->vAppend(pCard_ComparePred);
          pCard_ComparePred = nullptr;
        }
        
        pCard_OrPred = new Card_OrPred(CK_ORPRED);

        pCard_PredA = pCard_Filter_nested->pPop();
        pCard_PredB = pCard_Filter_nested->pPop();

        pCard_OrPred->vAppend(pPoppedCard);
        pCard_OrPred->vAppend(pCard_PredA);
        pCard_OrPred->vAppend(pCard_PredB);
        
        pCard_Filter_nested->vAppend(pCard_OrPred);
        pCard_OrPred = nullptr;
        break;

      case AE_AND:
        if (pCard_ComparePred) {
          pCard_Filter_nested->vAppend(pCard_ComparePred);
          pCard_ComparePred = nullptr;
        }
        
        pCard_AndPred = new Card_AndPred(CK_ANDPRED);

        pCard_PredA = pCard_Filter_nested->pPop();
        pCard_PredB = pCard_Filter_nested->pPop();
        
        pCard_AndPred->vAppend(pPoppedCard);
        pCard_AndPred->vAppend(pCard_PredA);
        pCard_AndPred->vAppend(pCard_PredB);
        
        pCard_Filter_nested->vAppend(pCard_AndPred);
        pCard_AndPred = nullptr;
        break;
        
      case AE_COMPARE:
        pCard_ComparePred->vAppend(pPoppedCard);
        pCard_Filter_nested->vAppend(pCard_ComparePred);
        pCard_ComparePred = nullptr;
        break;

      case AE_OTHER:
        if (! pCard_ComparePred)
          pCard_ComparePred = new Card_ComparePred(CK_COMPAREPRED);
        pCard_ComparePred->vAppend(pPoppedCard);
        break;

      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        
      case AE_BOOLEAN:
        pCard_Filter_others->vAppend(pPoppedCard);
        break;
        
      case AE_TYPE: {
        //pCard_Filter_others->vAppend(pPoppedCard);
        //pCard_Filter_others->vAppend(pPoppedCard);
        
        pPoppedCard = pCard_Filter->pPop();
        Card_Term* pCard_Term_column = _pCard_Term(pPoppedCard);
        //eCT eColumn = Column::eColumn(pCard_Term_column->szTerm());
        Card_Type* pCard_Type = new Card_Type(CK_TYPE,
                                              pCard_Term_column->szTerm());
        pCard_Expr->vAppend(pCard_Type);
        break;
      }
        
      default:
        pCard_Filter_others->vAppend(pPoppedCard);
        
        break;
    }

  }
  
  // dangling compare
  if (pCard_ComparePred) {
    pCard_Filter_nested->vAppend(pCard_ComparePred);
    pCard_ComparePred = nullptr;
  }
  
  pCard_Filter->vTransferAll(pCard_Expr);
  
  while (Card* pPoppedCard = pCard_Filter_others->pPop()) {
    pCard_Expr->vAppend(pPoppedCard);
  }
  while (Card* pPoppedCard = pCard_Filter_nested->pPop()) {
    pCard_Expr->vAppend(pPoppedCard);
  }

  pCard_Filter->vTransferAll(pCard_Expr);
  delete pCard_Expr;

  delete pCard_Filter_nested;
  delete pCard_Filter_others;

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
