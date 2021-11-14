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

  i8 nSubPipelines = pSubPipelines->nItems();
  for (i8 i_SubPipeline = 0; i_SubPipeline < nSubPipelines; i_SubPipeline++) {
    m_pPipelineSpec = pSubPipelines->pGet(i_SubPipeline);
    
    vDissectPipeline();
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
  
  m_pBoard = new Board("Board"_Stz);
  m_pBench->vAppend(m_pBoard);
  
  m_pBoard->vQueryNm(szPipelineNm);
  
  //vTrace("PipelineSpec %s\n"_Stz,szPipelineNm);
  

  
  //TagSet* pJobTags = m_pPipelineSpec->pJobTags();
  //pJobTags->vDumpSet(true);

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
  
//  vTrace("StageSpec %s %s\n"_Stz,szStageNm,aYG[eStageTopology]);
  m_bSeenAgg = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  s32 sRevisedStageNm;
  sRevisedStageNm.vSprint("stage%c"_Stz,'A' + i_StageSpec);
  Card_Stage* pCard_Stage = new Card_Stage(sRevisedStageNm.szStz(),eStageTopology);
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
  
//  vTrace("OpSpec %s\n"_Stz,szOpNm);

       if (szOpNm->bSame("aggreg"_Stz))     vDissectAggreg   (m_pOpSpec);
  else if (szOpNm->bSame("consumeOp"_Stz))  vDissectConsume  (m_pOpSpec);
  else if (szOpNm->bSame("filter"_Stz))     vDissectFilter   (m_pOpSpec);
  else if (szOpNm->bSame("indexJoin"_Stz))  vDissectIndexJoin(m_pOpSpec);
  else if (szOpNm->bSame("indexScan"_Stz))  vDissectIndexScan(m_pOpSpec);
  else if (szOpNm->bSame("limit"_Stz))      vDissectLimit    (m_pOpSpec);
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
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szExpr = nullptr;
  
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in aggreg argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  Card_Expr* pCard_Expr = pAtomiseExpr(szExpr);
  Card_AggregList* pCard_AggregList = pAnalyseAggreg(pCard_Expr);
  m_pBoard->vAppend(pCard_AggregList);
}

/*----------------------------------------------------------------------------*/
/* dissect consume op spec : consumeOp <Sends>                                */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectConsume(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
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
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
  TagSet* pArgTags = pOpSpec->pArgTags();
  //pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szExpr = nullptr;
  
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in filter argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
    
  Card_Expr* pCard_Expr = pAtomiseExpr(szExpr);
  Card_Filter* pCard_Filter = pAnalyseFilter(pCard_Expr);
  
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
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szTableNm = nullptr;
  Stz* szIndexNm = nullptr;
  Stz* szExpr    = nullptr;

  i8 nTags = pArgTags->nItems();
  if (4 == nTags) {
    szTableNm = pArgTags->szValue(1);
    szIndexNm = pArgTags->szValue(2);
    szExpr    = pArgTags->szValue(3);
  }
  else {
    vTrace("Error: only %lld tags in indexJoin argTags\n"_Stz,nTags);
    NEVER_REACH;
  }

  Table* pTable = pFindTable(szTableNm);
  Index* pIndex = pFindIndex(pTable,szIndexNm);
  
  Card_IndexJoin* pCard_IndexJoin = new Card_IndexJoin(pIndex);
  m_pBoard->vAppend(pCard_IndexJoin);
  
  Card_Index* pCard_Index = new Card_Index(pIndex);
  pCard_IndexJoin->vAppend(pCard_Index);

  Card_Expr* pCard_Expr = pAtomiseExpr(szExpr);
  Card_IndexPred* pCard_IndexPred = new Card_IndexPred;
  pCard_IndexPred->vTransferAll(pCard_Expr);
  pCard_IndexJoin->vAppend(pCard_IndexPred);
  delete pCard_Expr;
}

/*----------------------------------------------------------------------------*/
/* dissect indexScan op spec : indexScan <compiled> <tableNm> <indexNm> <expr>*/
/*----------------------------------------------------------------------------*/
void Dissect::vDissectIndexScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // to be completed ...
}

/*----------------------------------------------------------------------------*/
/* dissect limit op spec : limit <compiled> <rows>                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectLimit(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  Stz* szOpNm = pOpSpec->szOpNm();
////  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
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
/* dissect output op spec : output                                            */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectOutput(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
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
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
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
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Stz* szExpr = nullptr;
  
  i8 nTags = pArgTags->nItems();
  if (2 == nTags) {
    szExpr = pArgTags->szValue(1);
  }
  else {
    vTrace("Error: only %lld tags in project argTags\n"_Stz,nTags);
    NEVER_REACH;
  }
  
  //vAnalyseProjectExpr(szExpr);
  
  Card_Expr* pCard_Expr = pAtomiseExpr(szExpr);
  Card_ProjectList* pCard_ProjectList = pAnalyseProject(pCard_Expr);
  m_pBoard->vAppend(pCard_ProjectList);
}

/*----------------------------------------------------------------------------*/
/* dissect scan op spec : scan [ <catalogNm> <schemaNm> ] <tableNm>           */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectScan(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
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
  m_pBoard->vAppend(pCard_Scan);

  Card_Table* pCard_Table = new Card_Table(pTable);
  pCard_Scan->vAppend(pCard_Table);

}

/*----------------------------------------------------------------------------*/
/* dissect send op spec : send <sendKind>                                     */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSend(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/* dissect sink op spec : sink                                                */
/*----------------------------------------------------------------------------*/
void Dissect::vDissectSink(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
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
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
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
  
//  Stz* szOpNm = pOpSpec->szOpNm();
//  vTrace("Dissecting %s\n"_Stz,szOpNm);
  
//  TagSet* pArgTags = pOpSpec->pArgTags();
//  pArgTags->vDumpSet(true);
  
  // ignored
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse atoms of aggreg expr                                               */
/*----------------------------------------------------------------------------*/
Card_AggregList* Dissect::pAnalyseAggreg(Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  m_bSeenAgg = true;
  
  Card_AggregList* pCard_AggregList = new Card_AggregList;
  
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

  
  while (Card_Atom* pCard_Atom_type = pCard_Expr->pPopAtom()) {

    Card_Atom* pCard_Atom_column = pCard_Expr->pPopAtom();

    Card_Project* pCard_Project = new Card_Project;
    
    eCT eColumn = Column::eColumn(pCard_Atom_column->szAtom());
    
    Card_Type* pCard_Type = new Card_Type(eColumn);
    pCard_Project->vAppend(pCard_Type);

    IGNORE_RETURN(_pCard_Atom(pCard_Expr->pPop()));
    Card_Atom* pCard_Atom = _pCard_Atom(pCard_Expr->pPop());
    Stz* szAsNm = pCard_Atom->szAtom();
    Card_As* pCard_As = new Card_As(szAsNm);
    pCard_Project->vAppend(pCard_As);

    Card_Expr* pCard_ExprProject = new Card_Expr;
    Card_Atom* pCard_Atom_subexpr = nullptr;
    while ( (pCard_Atom_subexpr = _pCard_Atom(pCard_Expr->pPop())) ) {

      if (pCard_Atom_subexpr->bIsSame("groupBy"_Stz) ||
          pCard_Atom_subexpr->bIsSeparator())
        break;
      pCard_ExprProject->vAppend(pCard_Atom_subexpr);
    }
    pCard_Project->vAppend(pCard_ExprProject);
    
    if (pCard_Atom_subexpr && pCard_Atom_subexpr->bIsSame("groupBy"_Stz)) {
      Card_By* pCard_By = pAnalyseBy(pCard_Expr);
      pCard_Project->vAppend(pCard_By);
    }
    
    pCard_AggregList->vAppend(pCard_Project);
  }

  delete pCard_Expr;

  return pCard_AggregList;
}

/*----------------------------------------------------------------------------*/
/* analyse atoms of a group by                                                */
/*----------------------------------------------------------------------------*/
Card_By* Dissect::pAnalyseBy(Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_By* pCard_By = new Card_By;
  
  pCard_By->vTransferAll(pCard_Expr);

  return pCard_By;
}


/*----------------------------------------------------------------------------*/
/* analyse atoms of filter expr                                               */
/*----------------------------------------------------------------------------*/
Card_Filter* Dissect::pAnalyseFilter(Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  Card_Filter* pCard_Filter = new Card_Filter;

  //vSeparateAtoms(pCard_Filter,szExpr);
  pCard_Filter->vTransferAll(pCard_Expr);

  vEvaluateFilter(pCard_Filter);

  //if (! m_bSeenAgg) {
    //m_pBoard->vAppend(pCard_Filter);
    return pCard_Filter;
  //}

//  Card_Having* pCard_Having = new Card_Having;
//  pCard_Having->vTransferAll(pCard_Filter);
//  //m_pBoard->vAppend(pCard_Having);
//  delete pCard_Filter;
//
//  return pCard_Having;
//
}

/*----------------------------------------------------------------------------*/
/* analyse atoms of project expr                                              */
/*----------------------------------------------------------------------------*/
Card_ProjectList* Dissect::pAnalyseProject(Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
    
  Card_ProjectList* pCard_ProjectList = new Card_ProjectList;
  
  while (Card_Atom* pCard_Atom_type = pCard_Expr->pPopAtom()) {

    Card_Atom* pCard_Atom_column = pCard_Expr->pPopAtom();

    Card_Project* pCard_Project = new Card_Project;
    
    eCT eColumn = Column::eColumn(pCard_Atom_column->szAtom());
    
    Card_Type* pCard_Type = new Card_Type(eColumn);
    pCard_Project->vAppend(pCard_Type);

    IGNORE_RETURN(_pCard_Atom(pCard_Expr->pPop()));
    Card_Atom* pCard_Atom = _pCard_Atom(pCard_Expr->pPop());
    Stz* szAsNm = pCard_Atom->szAtom();
    Card_As* pCard_As = new Card_As(szAsNm);
    pCard_Project->vAppend(pCard_As);

    Card_Expr* pCard_ExprProject = new Card_Expr;
    for (;;) {
      Card_Atom* pCard_Atom = _pCard_Atom(pCard_Expr->pPop());
      if (! pCard_Atom)
        break;
      if (pCard_Atom->bIsSeparator())
        break;
      pCard_ExprProject->vAppend(pCard_Atom);
    }
    pCard_Project->vAppend(pCard_ExprProject);
    pCard_ProjectList->vAppend(pCard_Project);
  }
  
  //m_pBoard->vAppend(pCard_ProjectList);

  delete pCard_Expr;
  
  return pCard_ProjectList;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert expr into atoms                                                    */
/*----------------------------------------------------------------------------*/
Card_Expr* Dissect::pAtomiseExpr(Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);
  
  Card_Expr* pCard_Expr = new Card_Expr;
    
  vSeparateAtoms(pCard_Expr,szExpr);

  return pCard_Expr;
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
void Dissect::vSeparateAtoms(Card* pCard, Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DISSECT,_FOOTER_);

  TokenVec* pTokenVec = TokenVec::pTokenise(szExpr);
    
  i8 nTokens = pTokenVec->nItems();
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    Token* pToken = pTokenVec->pGet(i_Tkn);
    
    Card_Atom* pCard_Atom = new Card_Atom(pToken->szChars());

    pCard->vAppend(pCard_Atom);
  }
  
  delete pTokenVec;
}



/*≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣*/
/*≣≣                               SUPPORT                                  ≣≣*/
/*≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣≣*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
