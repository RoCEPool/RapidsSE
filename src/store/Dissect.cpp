/*============================================================================*/
/* Dissect.cpp -- Dissect                                                     */
/*============================================================================*/

#include "Dissect.hpp"
#include "Bench.hpp"
#include "SubPipelines.hpp"

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
  
  Card_AtomList* pCard_AtomList = pToAtomList(szExpr);
  Card_AggregList* pCard_AggregList = pAnalyseAggregExpr(pCard_AtomList);
  delete pCard_AtomList;
  m_pBoard->vAppend(pCard_AggregList);
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

  Card_AtomList* pCard_AtomList = pToAtomList(szExpr);
  Card_Filter* pCard_Filter = pAnalyseFilterExpr(pCard_AtomList);
  delete pCard_AtomList;

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

  Card_AtomList* pCard_AtomList = pToAtomList(szExpr);
  Card_Hash* pCard_Hash = pAnalyseHashExpr(pCard_AtomList);
  delete pCard_AtomList;

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

  Card_AtomList* pCard_AtomList = pToAtomList(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_AtomList);
  pCard_IndexJoin->vAppend(pCard_IndexPred);
  delete pCard_AtomList;
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

  Card_AtomList* pCard_AtomList = pToAtomList(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_AtomList);
  pCard_IndexScan->vAppend(pCard_IndexPred);
  delete pCard_AtomList;
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

  Card_AtomList* pCard_AtomList = pToAtomList(szExpr);
  Card_OrdAggList* pCard_OrdAggList = pAnalyseOrdAggExpr(pCard_AtomList);
  delete pCard_AtomList;
  m_pBoard->vAppend(pCard_OrdAggList);
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

  Card_AtomList* pCard_AtomList = pToAtomList(szExpr);
  Card_ProjectList* pCard_ProjectList = pAnalyseProjectExpr(pCard_AtomList);
  delete pCard_AtomList;
  m_pBoard->vAppend(pCard_ProjectList);
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
  Card_Table* pCard_Table = new Card_Table(pTable);
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
/* analyse the atoms of an AGGREG <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_AggregList* Dissect::pAnalyseAggregExpr(Card_AtomList* pCard_AtomList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_AggregList* pCard_AggregList = new Card_AggregList;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // topology

  eAL eAggLvl = eAggegLevel();
  pCard_AggregList->vAggLevel(eAggLvl);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate sequences
  
  Card_Project* pCard_Project = nullptr;
  Card_Atom* pCard_Atom_check = nullptr;
  while ( (pCard_Atom_check = pCard_AtomList_expr->pPopAtom()) ) {
    
    // GROUPBY
    
    if (pCard_Atom_check->bIsSame("GROUPBY"_Stz))
      break;
    
    // SEPARATOR
    
    if (pCard_Atom_check->bIsSeparator())
      continue;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    // TYPE
    
    if (pCard_Atom_check->bIsSame("TYPE"_Stz)) {
      if (! pCard_Project)
        pCard_Project = new Card_Project;
      
      // column type (e.g. BOOLEAN)
      Card_Type* pCard_Type = pCollectType(pCard_AtomList_expr);
      pCard_Project->vAppend(pCard_Type);
      continue;
    }
    
    // AS
    
    if (pCard_Atom_check->bIsSame("AS"_Stz)) {
      if (! pCard_Project)
        pCard_Project = new Card_Project;

      Card_As* pCard_As = pCollectAs(pCard_AtomList_expr);
      pCard_Project->vAppend(pCard_As);
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    pCard_AtomList_expr->vPush(pCard_Atom_check);
    
    // PROJECT/AGGREG

    if (! pCard_Project)
      pCard_Project = new Card_Project;
    
    // PREDICATE
    
    bool bIsAggregation = false;
    Card_ProjectPred* pCard_ProjectPred
      = pCollectProject(pCard_AtomList_expr,bIsAggregation);
      
    // is project an aggregation?
    if (bIsAggregation) {
      // convert project to aggreg
      Card_Aggreg* pCard_Aggreg = new Card_Aggreg;
      pCard_Aggreg->vTransferAll(pCard_Project);
      delete pCard_Project;
      pCard_Project = nullptr;
      
      Card_AggregPred* pCard_AggregPred = new Card_AggregPred;
      pCard_AggregPred->vTransferAll(pCard_ProjectPred);
      pCard_Aggreg->vAppend(pCard_AggregPred);
      delete pCard_ProjectPred;

      pCard_AggregList->vAppend(pCard_Aggreg);
      pCard_Project = nullptr;
    }
    else {
      // it's a project, leave as is
      pCard_Project->vAppend(pCard_ProjectPred);
      
      pCard_AggregList->vAppend(pCard_Project);
      pCard_Project = nullptr;
    }
    
  }
  
  // dangling aggreg
  ASSERT(! pCard_Project);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // collect any group by
  
  if (pCard_Atom_check) {
    RASSERT(pCard_Atom_check->bIsSame("GROUPBY"_Stz));

    Card_ByList* pCard_ByList = pCollectBy(pCard_AtomList_expr);
    pCard_AggregList->vAppend(pCard_ByList);
  }

  return pCard_AggregList;
}

/*----------------------------------------------------------------------------*/
/* analyse the atoms of a FILTER <expr>                                       */
/*----------------------------------------------------------------------------*/
Card_Filter* Dissect::pAnalyseFilterExpr(Card_AtomList* pCard_AtomList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Filter* pCard_Filter = new Card_Filter;

  pCard_Filter->vTransferAll(pCard_AtomList_expr);

  vEvaluateFilter(pCard_Filter);

  return pCard_Filter;
}

/*----------------------------------------------------------------------------*/
/* analyse the atoms of a HASH <expr>                                         */
/*----------------------------------------------------------------------------*/
Card_Hash* Dissect::pAnalyseHashExpr(Card_AtomList* pCard_AtomList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Hash* pCard_Hash = new Card_Hash;

  pCard_Hash->vTransferAll(pCard_AtomList_expr);

  return pCard_Hash;
}

/*----------------------------------------------------------------------------*/
/* analyse the atoms of an ORDAGG <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_OrdAggList* Dissect::pAnalyseOrdAggExpr(Card_AtomList* pCard_AtomList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_AggregList* pCard_AggregList
    = Dissect::pAnalyseAggregExpr(pCard_AtomList_expr);
  
  Card_OrdAggList* pCard_OrdAggList = new Card_OrdAggList;
  
  pCard_OrdAggList->vTransferAll(pCard_AggregList);
  
  delete pCard_AggregList;
  
  return pCard_OrdAggList;
}

/*----------------------------------------------------------------------------*/
/* analyse the atoms of a PROJECT <expr>                                      */
/*----------------------------------------------------------------------------*/
Card_ProjectList* Dissect::pAnalyseProjectExpr(Card_AtomList* pCard_AtomList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_ProjectList* pCard_ProjectList = new Card_ProjectList;
  

  Card_Project* pCard_Project = nullptr;
  Card_Atom* pCard_Atom_check = nullptr;
  while ( (pCard_Atom_check = pCard_AtomList_expr->pPopAtom()) ) {
    
    // SEPARATOR
    
    if (pCard_Atom_check->bIsSeparator())
      continue;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    // TYPE
    
    if (pCard_Atom_check->bIsSame("TYPE"_Stz)) {
      if (! pCard_Project)
        pCard_Project = new Card_Project;
      
      // column type (e.g. BOOLEAN)
      Card_Type* pCard_Type = pCollectType(pCard_AtomList_expr);
      pCard_Project->vAppend(pCard_Type);
      continue;
    }
    
    // AS
    
    if (pCard_Atom_check->bIsSame("AS"_Stz)) {
      if (! pCard_Project)
        pCard_Project = new Card_Project;

      Card_As* pCard_As = pCollectAs(pCard_AtomList_expr);
      pCard_Project->vAppend(pCard_As);
      continue;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    pCard_AtomList_expr->vPush(pCard_Atom_check);
    
    // PROJECT/AGGREG

    if (! pCard_Project)
      pCard_Project = new Card_Project;
    
    // PREDICATE
    bool bIsAggregation = false;
    Card_ProjectPred* pCard_ProjectPred
      = pCollectProject(pCard_AtomList_expr,bIsAggregation);
    ASSERT(! bIsAggregation);
    
    pCard_Project->vAppend(pCard_ProjectPred);
    
    pCard_ProjectList->vAppend(pCard_Project);
    pCard_Project = nullptr;
  }
  
  return pCard_ProjectList;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect <as> atoms : AS <name>                                             */
/*----------------------------------------------------------------------------*/
Card_As* Dissect::pCollectAs(Card_AtomList* pCard_AtomList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // <name>
  Card_Atom* pCard_Atom = _pCard_Atom(pCard_AtomList_expr->pPop());
  Stz* szAsNm = pCard_Atom->szAtom();

  Card_As* pCard_As = new Card_As(szAsNm);

  return pCard_As;
}

/*----------------------------------------------------------------------------*/
/* collect <groupby> atoms :  GROUPBY { <type> [<as>] <by> }                  */
/*----------------------------------------------------------------------------*/
Card_ByList* Dissect::pCollectBy(Card_AtomList* pCard_AtomList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_ByList* pCard_ByList = new Card_ByList;

  while (Card_Atom* pCard_Atom_typeLit = pCard_AtomList_expr->pPopAtom()) {

    Card_By* pCard_By = new Card_By;
    
    // type boolean
    Card_Atom* pCard_Atom_type = pCard_AtomList_expr->pPopAtom();
    eCT eColumn = Column::eColumn(pCard_Atom_type->szAtom());
    Card_Type* pCard_Type = new Card_Type(eColumn);
    pCard_By->vAppend(pCard_Type);

    Card_Atom* pCard_Atom_test = pCard_AtomList_expr->pPopAtom();
    
    if (pCard_Atom_test->bIsSame("as"_Stz)) {
      // as name
      Card_Atom* pCard_Atom_as = pCard_Atom_test;
      RASSERT(pCard_Atom_as->bIsSame("as"_Stz));
      Card_Atom* pCard_Atom_asNm = pCard_AtomList_expr->pPopAtom();
      Stz* szAsNm = pCard_Atom_asNm->szAtom();
      Card_As* pCard_As = new Card_As(szAsNm);
      pCard_By->vAppend(pCard_As);
    }
    else {
      pCard_AtomList_expr->vPush(pCard_Atom_test);
    }
    
    // by predicate
    Card_ByPred* pCard_ByPred = new Card_ByPred;
    for (;;) {
      Card_Atom* pCard_Atom = pCard_AtomList_expr->pPopAtom();
      if (! pCard_Atom)
        break;
      if (pCard_Atom->bIsSeparator())
        break;
      pCard_ByPred->vAppend(pCard_Atom);
    }
    pCard_By->vAppend(pCard_ByPred);
    
    pCard_ByList->vAppend(pCard_By);
  }

  return pCard_ByList;
}

/*----------------------------------------------------------------------------*/
/* collect <project> atoms : { <atom> } GROUPBY                               */
/*----------------------------------------------------------------------------*/
Card_ProjectPred* Dissect::pCollectProject(Card_AtomList* pCard_AtomList_expr,
                                            bool& rbIsAggregation)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  rbIsAggregation = false;

  Card_ProjectPred* pCard_ProjectPred = new Card_ProjectPred;

  Card_Atom* pCard_Atom_pred = nullptr;
  while ( (pCard_Atom_pred = _pCard_Atom(pCard_AtomList_expr->pPop())) ) {

    if (pCard_Atom_pred->bIsSame(")"_Stz))
      continue;
    
    if (pCard_Atom_pred->bIsSame("GROUPBY"_Stz) ||
        pCard_Atom_pred->bIsSeparator()) {
      pCard_AtomList_expr->vPush(pCard_Atom_pred);
      break;
    }
    
    if (pCard_Atom_pred->bIsAggOp())
      rbIsAggregation = true;

    pCard_ProjectPred->vAppend(pCard_Atom_pred);
  }

  return pCard_ProjectPred;
}

/*----------------------------------------------------------------------------*/
/* collect <type> atoms : TYPE <columnType>                                   */
/*----------------------------------------------------------------------------*/
Card_Type* Dissect::pCollectType(Card_AtomList* pCard_AtomList_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  // column type (e.g. BOOLEAN)
  Card_Atom* pCard_Atom_type = pCard_AtomList_expr->pPopAtom();
  eCT eColumn = Column::eColumn(pCard_Atom_type->szAtom());

  Card_Type* pCard_Type = new Card_Type(eColumn);

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
/* evaluate filter                                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vEvaluateFilter(Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
 
  Eval_Filter* pEval_Filter = new Eval_Filter;
  
  pEval_Filter->vEvaluateFilter(pCard_Filter);
  
  delete pEval_Filter;
}

/*----------------------------------------------------------------------------*/
/* convert expr string into atom list                                         */
/*----------------------------------------------------------------------------*/
Card_AtomList* Dissect::pToAtomList(Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_AtomList* pCard_AtomList_expr = new Card_AtomList;
    
  TokenVec* pTokenVec = TokenVec::pTokenise(szExpr);
    
  i8 nTokens = pTokenVec->nItems();
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    Token* pToken = pTokenVec->pGet(i_Tkn);
    
    Card_Atom* pCard_Atom = new Card_Atom(pToken->szChars());

    pCard_AtomList_expr->vAppend(pCard_Atom);
  }
  
  delete pTokenVec;

  return pCard_AtomList_expr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
