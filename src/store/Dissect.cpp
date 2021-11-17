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
        
  m_pBench = pBench;
    
  m_pBoard        = nullptr;
  m_pPipelineSpec = nullptr;
  m_pStageSpec    = nullptr;
  m_pOpSpec       = nullptr;
  
  m_bSeenAgg = false;
  
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
    
  m_pStageSpec = nullptr;
  m_pOpSpec    = nullptr;
  m_bSeenAgg   = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
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
  m_bSeenAgg   = false;
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
 
//  Stz* szStageNm = m_pStageSpec->szStageNm();
  eYG eStageTopology = m_pStageSpec->eStageTopology();
  
  m_bSeenAgg = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sRevisedStageNm;
  sRevisedStageNm.vSprint("stage%c"_Stz,'A' + i_StageSpec);
  Card_Stage* pCard_Stage = new Card_Stage(sRevisedStageNm.szStz(),
                                           eStageTopology);
  m_pBoard->vAppend(pCard_Stage);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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
  else if (szOpNm->bSame("filter"_Stz))     vDissectFilter   (m_pOpSpec);
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
/* dissect aggreg op spec : aggreg <compiled> <expr>                          */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectAggreg(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szAtomSeq = nullptr;
  
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szAtomSeq = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in aggreg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  Card_AtomList* pCard_AtomList = pAtomiseSeq(szAtomSeq);
  Card_AggregList* pCard_AggregList = pAnalyseAggreg(pCard_AtomList);
  delete pCard_AtomList;
  m_pBoard->vAppend(pCard_AggregList);
}

/*----------------------------------------------------------------------------*/
/* dissect combine op spec : combined                                         */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectCombined(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  Card_Combine* pCard_Combine = new Card_Combine;

  m_pBoard->vAppend(pCard_Combine);
}

/*----------------------------------------------------------------------------*/
/* dissect consume op spec : consumeOp <Sends>                                */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectConsume(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  //TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect filter op spec : filter <compiled> <expr>                          */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectFilter(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szAtomSeq = nullptr;
  
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szAtomSeq = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in filter argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
    
  Card_AtomList* pCard_AtomList = pAtomiseSeq(szAtomSeq);
  Card_Filter* pCard_Filter = pAnalyseFilter(pCard_AtomList);
  delete pCard_AtomList;

  if (! m_bSeenAgg) {
    m_pBoard->vAppend(pCard_Filter);
    return;
  }
  
  Card_Having* pCard_Having = new Card_Having;
  pCard_Having->vTransferAll(pCard_Filter);
  m_pBoard->vAppend(pCard_Having);
  delete pCard_Filter;
}

/*----------------------------------------------------------------------------*/
/* dissect indexJoin op spec : indexJoin <compiled> <tableNm> <indexNm> <expr>*/
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexJoin(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szAtomSeq = nullptr;

  i8 nTags = pArgTags->nItems();
  if (4 == nTags) {
    szTableNm = pArgTags->szValue(1);
    szIndexNm = pArgTags->szValue(2);
    szAtomSeq = pArgTags->szValue(3);
  }
  else {
    vTrace("Error: only %lld tags in indexJoin argTags\n"_Stz,nTags);
    NEVER_REACH;
  }


  Table* pTable = pFindTable(szTableNm);
  Index* pIndex = pFindIndex(pTable,szIndexNm);
  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin(pIndex);
  m_pBoard->vAppend(pCard_IndexJoin);

  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexJoin->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexJoin->vAppend(pCard_Index);

  Card_AtomList* pCard_AtomList = pAtomiseSeq(szAtomSeq);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_AtomList);
  pCard_IndexJoin->vAppend(pCard_IndexPred);
  delete pCard_AtomList;
}

/*----------------------------------------------------------------------------*/
/* dissect indexScan op spec : indexScan <compiled> <tableNm> <indexNm> <expr>*/
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szAtomSeq = nullptr;

  i8 nTags = pArgTags->nItems();
  if (4 == nTags) {
    szTableNm = pArgTags->szValue(1);
    szIndexNm = pArgTags->szValue(2);
    szAtomSeq = pArgTags->szValue(3);
  }
  else {
    vTrace("Error: only %lld tags in indexScan argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  Table* pTable = pFindTable(szTableNm);
  Index* pIndex = pFindIndex(pTable,szIndexNm);
  Card_IndexScan* pCard_IndexScan = new Card_IndexScan(pIndex);
  m_pBoard->vAppend(pCard_IndexScan);

  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_IndexScan->vAppend(pCard_Table);

  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexScan->vAppend(pCard_Index);

  Card_AtomList* pCard_AtomList = pAtomiseSeq(szAtomSeq);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_AtomList);
  pCard_IndexScan->vAppend(pCard_IndexPred);
  delete pCard_AtomList;
}

/*----------------------------------------------------------------------------*/
/* dissect limit op spec : limit <compiled> <rows>                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectLimit(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz* szRows = nullptr;
  
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szRows = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in limit argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  i8 nRows = szRows->iToI8();
  
  m_pBoard->vAppend(new Card_Limit(nRows));
}

/*----------------------------------------------------------------------------*/
/* dissect ordagg op spec : aggreg <compiled> <expr>                          */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOrdAgg(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szAtomSeq = nullptr;
  
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szAtomSeq = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in ordagg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  Card_AtomList* pCard_AtomList = pAtomiseSeq(szAtomSeq);
  Card_OrdAggList* pCard_OrdAggList = pAnalyseOrdAgg(pCard_AtomList);
  delete pCard_AtomList;
  m_pBoard->vAppend(pCard_OrdAggList);
}

/*----------------------------------------------------------------------------*/
/* dissect output op spec : output                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOutput(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect pulse op spec : : pulse once                                       */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectPulse(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect project op spec : project <compiled> <expr>                        */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectProject(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szAtomSeq = nullptr;
  
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szAtomSeq = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in project argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  Card_AtomList* pCard_AtomList = pAtomiseSeq(szAtomSeq);
  Card_ProjectList* pCard_ProjectList = pAnalyseProject(pCard_AtomList);
  delete pCard_AtomList;
  m_pBoard->vAppend(pCard_ProjectList);
}

/*----------------------------------------------------------------------------*/
/* dissect scan op spec : scan [ <catalogNm> <schemaNm> ] <tableNm>           */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szTableNm = nullptr;
  
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
    
  Table* pTable = pFindTable(szTableNm);

  //vAddScan(pTable);
  
  Card_Scan* pCard_Scan = new Card_Scan;
  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_Scan->vAppend(pCard_Table);
  m_pBoard->vAppend(pCard_Scan);
}

/*----------------------------------------------------------------------------*/
/* dissect send op spec : send <sendKind>                                     */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSend(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
 
  Stz* szSendKind = nullptr;

  i8 nTags = pArgTags->nItems();
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
/* dissect sink op spec : sink                                                */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSink(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect sort op spec : NOT YET IMPLEMENTED                                 */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSort(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // to be completed ...
}

/*----------------------------------------------------------------------------*/
/* dissect start op spec : : start                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectStart(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse atoms of aggreg expr                                               */
/*----------------------------------------------------------------------------*/
Card_AggregList* Dissect::pAnalyseAggreg(Card_AtomList* pCard_AtomList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  m_bSeenAgg = true;
  
  Card_AggregList* pCard_AggregList = new Card_AggregList;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // topology

  eYG eStageTopology = m_pStageSpec->eStageTopology();
  if (YG_ORIGIN == eStageTopology)
    pCard_AggregList->vAggLevel(AL_ORIGIN);
  else
  if (YG_ALL_NODES == eStageTopology)
    pCard_AggregList->vAggLevel(AL_NODE);
  else
  if (YG_ALL_PARTITIONS == eStageTopology)
    pCard_AggregList->vAggLevel(AL_PARTITION);
  else
    NEVER_REACH;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // aggregate sequence
  
  Card_Aggreg* pCard_Aggreg = nullptr;
  Card_Atom* pCard_Atom_check = nullptr;
  while ( (pCard_Atom_check = pCard_AtomList->pPopAtom()) ) {
    
    // GROUPBY
    
    if (pCard_Atom_check->bIsSame("GROUPBY"_Stz)) {
      if (pCard_Aggreg) {
        pCard_AggregList->vAppend(pCard_Aggreg);
        pCard_Aggreg = nullptr;
      }
      break;
    }
    
    // separator
    
    if (pCard_Atom_check->bIsSeparator()) {
      if (pCard_Aggreg) {
        pCard_AggregList->vAppend(pCard_Aggreg);
        pCard_Aggreg = nullptr;
      }
      continue;
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    // TYPE
    
    if (pCard_Atom_check->bIsSame("TYPE"_Stz)) {
      if (! pCard_Aggreg)
        pCard_Aggreg = new Card_Aggreg;
      
      // column type (e.g. BOOLEAN)
      Card_Atom* pCard_Atom_type = pCard_AtomList->pPopAtom();
      eCT eColumn = Column::eColumn(pCard_Atom_type->szAtom());

      Card_Type* pCard_Type = new Card_Type(eColumn);
      pCard_Aggreg->vAppend(pCard_Type);
      continue;
    }
    
    // AS
    
    if (pCard_Atom_check->bIsSame("AS"_Stz)) {
      if (! pCard_Aggreg)
        pCard_Aggreg = new Card_Aggreg;

      // name
      Card_Atom* pCard_Atom = _pCard_Atom(pCard_AtomList->pPop());
      Stz* szAsNm = pCard_Atom->szAtom();

      Card_As* pCard_As = new Card_As(szAsNm);
      pCard_Aggreg->vAppend(pCard_As);
      continue;
    }
    
    // predicate
    
    if (! pCard_Aggreg)
      pCard_Aggreg = new Card_Aggreg;

    Card_ProjectPred* pCard_ProjectPred = new Card_ProjectPred;

    bool bSawAggOp = false;
    Card_Atom* pCard_Atom_pred = nullptr;
    while ( (pCard_Atom_pred = _pCard_Atom(pCard_AtomList->pPop())) ) {

      if (pCard_Atom_pred->bIsSame("groupBy"_Stz) ||
          pCard_Atom_pred->bIsSeparator()) {
        pCard_AtomList->vPush(pCard_Atom_pred);
        break;
      }
      
      if (pCard_Atom_pred->bIsAggOp())
        bSawAggOp = true;

      pCard_ProjectPred->vAppend(pCard_Atom_pred);
    }
      
    if (bSawAggOp) {
      
      Card_AggregPred* pCard_AggregPred = new Card_AggregPred;
      pCard_AggregPred->vTransferAll(pCard_ProjectPred);
      delete pCard_ProjectPred;
      pCard_Aggreg->vAppend(pCard_AggregPred);
    }
    else {
      pCard_Aggreg->vAppend(pCard_ProjectPred);
    }
  }
  
  if (pCard_Aggreg) {
    pCard_AggregList->vAppend(pCard_Aggreg);
    pCard_Aggreg = nullptr;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // group by sequence
  
  if (! pCard_Atom_check) {
    return pCard_AggregList;
  }
  
  RASSERT(pCard_Atom_check->bIsSame("GROUPBY"_Stz));

  Card_ByList* pCard_ByList = pAnalyseBy(pCard_AtomList);
  pCard_AggregList->vAppend(pCard_ByList);

  Card_By* pCard_By = nullptr;

  while ( (pCard_Atom_check = pCard_AtomList->pPopAtom()) ) {
    
    // separator
    
    if (pCard_Atom_check->bIsSeparator()) {
      if (pCard_By) {
        pCard_ByList->vAppend(pCard_By);
        pCard_By = nullptr;
      }
      continue;
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    
    // TYPE
    
    if (pCard_Atom_check->bIsSame("TYPE"_Stz)) {
      if (! pCard_By)
        pCard_By = new Card_By;
      
      // column type (e.g. BOOLEAN)
      Card_Atom* pCard_Atom_type = pCard_AtomList->pPopAtom();
      eCT eColumn = Column::eColumn(pCard_Atom_type->szAtom());

      Card_Type* pCard_Type = new Card_Type(eColumn);
      pCard_By->vAppend(pCard_Type);
      continue;
    }
    
    // AS
    
    if (pCard_Atom_check->bIsSame("AS"_Stz)) {
      if (! pCard_By)
        pCard_By = new Card_By;

      // name
      Card_Atom* pCard_Atom = _pCard_Atom(pCard_AtomList->pPop());
      Stz* szAsNm = pCard_Atom->szAtom();

      Card_As* pCard_As = new Card_As(szAsNm);
      pCard_By->vAppend(pCard_As);
      continue;
    }
    
    // predicate
    
    if (! pCard_By)
      pCard_By = new Card_By;

    Card_ByPred* pCard_ByPred = new Card_ByPred;

    Card_Atom* pCard_Atom_pred = nullptr;
    while ( (pCard_Atom_pred = _pCard_Atom(pCard_AtomList->pPop())) ) {

      pCard_ByPred->vAppend(pCard_Atom_pred);
    }
  }
  
  if (pCard_By) {
    pCard_ByList->vAppend(pCard_By);
    pCard_ByList = nullptr;
  }

  return pCard_AggregList;
}
/*----------------------------------------------------------------------------*/
//Card_AggregList* Dissect::pAnalyseAggreg(Card_AtomList* pCard_AtomList)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
//
//  m_bSeenAgg = true;
//
//  Card_AggregList* pCard_AggregList = new Card_AggregList;
//
//  eYG eStageTopology = m_pStageSpec->eStageTopology();
//  if (YG_ORIGIN == eStageTopology)
//    pCard_AggregList->vAggLevel(AL_ORIGIN);
//  else
//  if (YG_ALL_NODES == eStageTopology)
//    pCard_AggregList->vAggLevel(AL_NODE);
//  else
//  if (YG_ALL_PARTITIONS == eStageTopology)
//    pCard_AggregList->vAggLevel(AL_PARTITION);
//  else
//    NEVER_REACH;
//
//
//  while (Card_Atom* pCard_Atom_typeLit = pCard_AtomList->pPopAtom()) {
//
//    Card_Aggreg* pCard_Aggreg = new Card_Aggreg;
//
//    // type
//    Card_Atom* pCard_Atom_type = pCard_AtomList->pPopAtom();
//    eCT eColumn = Column::eColumn(pCard_Atom_type->szAtom());
//    Card_Type* pCard_Type = new Card_Type(eColumn);
//    pCard_Aggreg->vAppend(pCard_Type);
//
//    Card_Atom* pCard_Atom_test = pCard_AtomList->pPopAtom();
//
//    if (pCard_Atom_test->bIsSame("as"_Stz)) {
//      // as name
//      Card_Atom* pCard_Atom = _pCard_Atom(pCard_AtomList->pPop());
//      Stz* szAsNm = pCard_Atom->szAtom();
//      Card_As* pCard_As = new Card_As(szAsNm);
//      pCard_Aggreg->vAppend(pCard_As);
//    }
//    else {
//      pCard_AtomList->vPush(pCard_Atom_test);
//    }
//
//    // expr
//    Card_AggregPred* pCard_AggregPred = new Card_AggregPred;
//    bool bSawAggOp = false;
//
//    Card_Atom* pCard_Atom_pred = nullptr;
//    while ( (pCard_Atom_pred = _pCard_Atom(pCard_AtomList->pPop())) ) {
//
//      if (pCard_Atom_pred->bIsSame("groupBy"_Stz) ||
//          pCard_Atom_pred->bIsSeparator())
//        break;
//      if (pCard_Atom_pred->bIsAggOp())
//        bSawAggOp = true;
//      pCard_AggregPred->vAppend(pCard_Atom_pred);
//    }
//
//    if (bSawAggOp) {
//      pCard_Aggreg->vAppend(pCard_AggregPred);
//      pCard_AggregList->vAppend(pCard_Aggreg);
//    }
//    else {
//      Card_ProjectPred* pCard_ProjectPred = new Card_ProjectPred;
//      pCard_ProjectPred->vTransferAll(pCard_AggregPred);
//      delete pCard_AggregPred;
//      pCard_Aggreg->vAppend(pCard_ProjectPred);
//      pCard_AggregList->vAppend(pCard_Aggreg);
//    }
//
//    // by
//    if (pCard_Atom_pred && pCard_Atom_pred->bIsSame("groupBy"_Stz)) {
//      Card_ByList* pCard_ByList = pAnalyseBy(pCard_AtomList);
//      pCard_AggregList->vAppend(pCard_ByList);
//    }
//  }
//
//  return pCard_AggregList;
//}


/*----------------------------------------------------------------------------*/
/* analyse atoms of a group by                                                */
/*----------------------------------------------------------------------------*/
Card_ByList* Dissect::pAnalyseBy(Card_AtomList* pCard_AtomList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_ByList* pCard_ByList = new Card_ByList;

  while (Card_Atom* pCard_Atom_typeLit = pCard_AtomList->pPopAtom()) {

    Card_By* pCard_By = new Card_By;
    
    // type boolean
    Card_Atom* pCard_Atom_type = pCard_AtomList->pPopAtom();
    eCT eColumn = Column::eColumn(pCard_Atom_type->szAtom());
    Card_Type* pCard_Type = new Card_Type(eColumn);
    pCard_By->vAppend(pCard_Type);

    Card_Atom* pCard_Atom_test = pCard_AtomList->pPopAtom();
    
    if (pCard_Atom_test->bIsSame("as"_Stz)) {
      // as name
      Card_Atom* pCard_Atom_as = pCard_Atom_test;
      RASSERT(pCard_Atom_as->bIsSame("as"_Stz));
      Card_Atom* pCard_Atom_asNm = pCard_AtomList->pPopAtom();
      Stz* szAsNm = pCard_Atom_asNm->szAtom();
      Card_As* pCard_As = new Card_As(szAsNm);
      pCard_By->vAppend(pCard_As);
    }
    else {
      pCard_AtomList->vPush(pCard_Atom_test);
    }
    
    // by predicate
    Card_ByPred* pCard_ByPred = new Card_ByPred;
    for (;;) {
      Card_Atom* pCard_Atom = pCard_AtomList->pPopAtom();
      if (! pCard_Atom)
        break;
      if (pCard_Atom->bIsSeparator())
        break;
      pCard_ByPred->vAppend(pCard_Atom);
    }
    //vEvaluateExpr(pCard_Expr);
    pCard_By->vAppend(pCard_ByPred);
    
    pCard_ByList->vAppend(pCard_By);
  }

  return pCard_ByList;
}

/*----------------------------------------------------------------------------*/
/* analyse atoms of filter expr                                               */
/*----------------------------------------------------------------------------*/
Card_Filter* Dissect::pAnalyseFilter(Card_AtomList* pCard_AtomList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Filter* pCard_Filter = new Card_Filter;

  //Card_Expr* pCard_Expr = new Card_Expr;
  //pCard_Filter->vAppend(pCard_Expr);
  
  //vSeparateAtoms(pCard_Filter,szAtomSeq);
  pCard_Filter->vTransferAll(pCard_AtomList);

  vEvaluateFilter(pCard_Filter);

  return pCard_Filter;
}

/*----------------------------------------------------------------------------*/
/* analyse atoms of aggreg expr                                               */
/*----------------------------------------------------------------------------*/
Card_OrdAggList* Dissect::pAnalyseOrdAgg(Card_AtomList* pCard_AtomList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  m_bSeenAgg = true;
  
  Card_OrdAggList* pCard_OrdAggList = new Card_OrdAggList;
  
  eYG eStageTopology = m_pStageSpec->eStageTopology();
  if (YG_ORIGIN == eStageTopology)
    pCard_OrdAggList->vAggLevel(AL_ORIGIN);
  else
  if (YG_ALL_NODES == eStageTopology)
    pCard_OrdAggList->vAggLevel(AL_NODE);
  else
  if (YG_ALL_PARTITIONS == eStageTopology)
    pCard_OrdAggList->vAggLevel(AL_PARTITION);
  else
    NEVER_REACH;

  
  while (Card_Atom* pCard_Atom_typeLit = pCard_AtomList->pPopAtom()) {

    Card_OrdAgg* pCard_OrdAgg = new Card_OrdAgg;
    
    // type
    Card_Atom* pCard_Atom_type = pCard_AtomList->pPopAtom();
    eCT eColumn = Column::eColumn(pCard_Atom_type->szAtom());
    Card_Type* pCard_Type = new Card_Type(eColumn);
    pCard_OrdAgg->vAppend(pCard_Type);

    Card_Atom* pCard_Atom_test = pCard_AtomList->pPopAtom();

    if (pCard_Atom_test->bIsSame("as"_Stz)) {
      // as name
      Card_Atom* pCard_Atom = _pCard_Atom(pCard_AtomList->pPop());
      Stz* szAsNm = pCard_Atom->szAtom();
      Card_As* pCard_As = new Card_As(szAsNm);
      pCard_OrdAgg->vAppend(pCard_As);
    }
    else {
      pCard_OrdAgg->vPush(pCard_Atom_test);
    }
    
    // expr
    Card_AggregPred* pCard_AggregPred = new Card_AggregPred;
    bool bSawAggOp = false;
    
    Card_Atom* pCard_Atom_pred = nullptr;
    while ( (pCard_Atom_pred = _pCard_Atom(pCard_AtomList->pPop())) ) {

      if (pCard_Atom_pred->bIsSame("groupBy"_Stz) ||
          pCard_Atom_pred->bIsSeparator())
        break;
      if (pCard_Atom_pred->bIsAggOp())
        bSawAggOp = true;
      pCard_AggregPred->vAppend(pCard_Atom_pred);
    }

    if (bSawAggOp) {
      pCard_OrdAgg->vAppend(pCard_AggregPred);
      pCard_OrdAggList->vAppend(pCard_OrdAgg);
    }
    else {
      Card_ProjectPred* pCard_ProjectPred = new Card_ProjectPred;
      pCard_ProjectPred->vTransferAll(pCard_AggregPred);
      delete pCard_AggregPred;
      pCard_OrdAgg->vAppend(pCard_ProjectPred);
      pCard_OrdAggList->vAppend(pCard_OrdAgg);
    }
    
    // by
    if (pCard_Atom_pred && pCard_Atom_pred->bIsSame("groupBy"_Stz)) {
      Card_ByList* pCard_ByList = pAnalyseBy(pCard_AtomList);
      pCard_OrdAggList->vAppend(pCard_ByList);
    }
  }

  return pCard_OrdAggList;
}

/*----------------------------------------------------------------------------*/
/* analyse atoms of project expr                                              */
/*----------------------------------------------------------------------------*/
Card_ProjectList* Dissect::pAnalyseProject(Card_AtomList* pCard_AtomList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_ProjectList* pCard_ProjectList = new Card_ProjectList;
  
  while (Card_Atom* pCard_Atom_typeLit = pCard_AtomList->pPopAtom()) {

    Card_Project* pCard_Project = new Card_Project;
    
    // type boolean
    Card_Atom* pCard_Atom_type = pCard_AtomList->pPopAtom();
    eCT eColumn = Column::eColumn(pCard_Atom_type->szAtom());
    Card_Type* pCard_Type = new Card_Type(eColumn);
    pCard_Project->vAppend(pCard_Type);

    Card_Atom* pCard_Atom_test = pCard_AtomList->pPopAtom();
    
    if (pCard_Atom_test->bIsSame("as"_Stz)) {
      // as name
      Card_Atom* pCard_Atom_as = pCard_Atom_test;
      RASSERT(pCard_Atom_as->bIsSame("as"_Stz));
      Card_Atom* pCard_Atom_asNm = pCard_AtomList->pPopAtom();
      Stz* szAsNm = pCard_Atom_asNm->szAtom();
      Card_As* pCard_As = new Card_As(szAsNm);
      pCard_Project->vAppend(pCard_As);
    }
    else {
      pCard_AtomList->vPush(pCard_Atom_test);
    }
    
    // expr
    Card_ProjectPred* pCard_ProjectPred = new Card_ProjectPred;
    for (;;) {
      Card_Atom* pCard_Atom = pCard_AtomList->pPopAtom();
      if (! pCard_Atom)
        break;
      if (pCard_Atom->bIsSeparator())
        break;
      pCard_ProjectPred->vAppend(pCard_Atom);
    }
    //vEvaluateExpr(pCard_Expr);
    pCard_Project->vAppend(pCard_ProjectPred);
    
    pCard_ProjectList->vAppend(pCard_Project);
  }
    
  return pCard_ProjectList;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert atom seq into atom list                                            */
/*----------------------------------------------------------------------------*/
Card_AtomList* Dissect::pAtomiseSeq(Stz* szAtomSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_AtomList* pCard_AtomList = new Card_AtomList;
    
  vSeparateAtoms(pCard_AtomList,szAtomSeq);

  return pCard_AtomList;
}

/*----------------------------------------------------------------------------*/
/* evaluate expr                                                              */
/*----------------------------------------------------------------------------*/
void Dissect::vEvaluateExpr(Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
 
  Eval_Filter* pEval_Filter = new Eval_Filter;
  
  pEval_Filter->vEvaluateExpr(pCard_Expr);
  
  delete pEval_Filter;
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
/* find index of table given name                                             */
/*----------------------------------------------------------------------------*/
Index* Dissect::pFindIndex(Table* pTable, Stz* szIndexNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Index* pIndex = nullptr;
  
  IdxVec* pIdxVec = pTable->pIdxVec();
  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Index = 0; i_Index < nIndexes; i_Index++) {
    Index* pIndex_test = pIdxVec->pGet(i_Index);
    Stz* szIndexNm_test = pIndex_test->szIndexNm();
    
    if (szIndexNm_test->bSame(szIndexNm)) {
      pIndex = pIndex_test;
      break;
    }
  }

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* find table given name                                                      */
/*----------------------------------------------------------------------------*/
Table* Dissect::pFindTable(Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Table* pTable = AM::pTableFind(szTableNm);

  return pTable;
}

/*----------------------------------------------------------------------------*/
/* tokenise expr and append its atoms to given card                           */
/*----------------------------------------------------------------------------*/
void Dissect::vSeparateAtoms(Card* pCard, Stz* szAtomSeq)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  TokenVec* pTokenVec = TokenVec::pTokenise(szAtomSeq);
    
  i8 nTokens = pTokenVec->nItems();
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    Token* pToken = pTokenVec->pGet(i_Tkn);
    
    Card_Atom* pCard_Atom = new Card_Atom(pToken->szChars());

    pCard->vAppend(pCard_Atom);
  }
  
  delete pTokenVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
