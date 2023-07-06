/*============================================================================*/
/* JoinOrdering.cpp -- JoinOrdering                                           */
/*============================================================================*/

#include "JoinOrdering.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Optimiser.hpp"

#include "XF_Fix_table_Pos.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, JoinOrdering, CN_JOINORDERING)
__aNewFunction(JoinOrdering, CN_JOINORDERING)
__vDeleteFunction(JoinOrdering, CN_JOINORDERING)
__aFooterFunction(JoinOrdering)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
JoinOrdering::JoinOrdering(CardIter* pCI)
: Object(CN_JOINORDERING)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_JOINORDERING);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
JoinOrdering::~JoinOrdering(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vJoinOrdering(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_JOINORDERING, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Predicate Pull Up : move suitable where predicates to table filter

  // @ mark where predicates wrt. table
  m_pCI->vTransform(__func__, XF_NONE, "put a table @c against where pred"_Stz);
  Card_Where::vMarkWherePredWrtTable(m_pCI);
  m_pCI->vEndTransform();

  // move @ marked where predicates to table filter
  m_pCI->vTransform(__func__, XF_NONE, "move @ marked where predicates to table filter"_Stz);
  Card_Where::vMoveWherePredsToTableFilter(m_pCI);
  m_pCI->vEndTransform();

  // cull @ marked where predicates
  m_pCI->vTransform(__func__, XF_NONE, "cull @ marked where predicates"_Stz);
  Card_Where::vCullMarkedCmpPred(m_pCI);
  m_pCI->vEndTransform();

  // delete empty where
  m_pCI->vTransform(__func__, XF_NONE, "delete empty where"_Stz);
  Card_Where::vDeleteEmpyWhere(m_pCI);
  m_pCI->vEndTransform();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Index Bounds : move table filter predicates to table index bounds

  // derive index bounds from table filter
  m_pCI->vTransform(__func__, XF_NONE, "derive index bounds from table filter"_Stz);
  Card_Table::vDeriveBoundsFromFilter(m_pCI);
  m_pCI->vEndTransform();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fix position of table which uses subquery result as pos 0
  XF_Fix_table_Pos::vXF_Fix_table_Pos(m_pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Join Ordering Proper

  if (AM::JOB::bHasTag(C_JO_DISABLE)) {
    m_pCI->G.pDeck->vProjectionUpdate(m_pCI);
    return;
  }

  if (Card_Table::nTables_reverse_foldBreak(m_pCI) < 2)
    return;

  // reorder the tables
  vReorderTables();

  vPredicateReposition();

  // re-derive bounds from filter after join ordering
  m_pCI->vTransform(__func__, XF_NONE, "re-derive bounds from filter after join ordering"_Stz);
  Card_Table::vDeriveBoundsFromFilter(m_pCI);
  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vReorderTables(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "join ordering"_Stz);

  File* pFile = nullptr;

  if (AM::JOB::bHasTag(C_JOWRITE)) {
    pFile = new File(this);
    RASSERT(pFile->bOpenAppend("/tmp/optimiser.txt"_Stz));
  }


  Optimiser* pOptimiser = m_pCI->m_pBench->pOptimiser();

  if (pFile) {
    s256 sBar;
    StrLib::vPad(sBar.azChar(),120,'-');
    pFile->vPrint("%s\n"_Stz,sBar.azChar());
    pFile->vPrint("Query %s"_Stz,pOptimiser->szQueryNm());
    pFile->vPrint(" (Ec %.2f Fc %.2f Jc %.2f Jl%.2f)"_Stz,
                  C_EXPANSION_COEFF,C_FILTER_COEFF,C_JOIN_COEFF,C_JOIN_LONGER);
    if (AM::JOB::bHasTag(C_FILTERPRED))
      pFile->vPrint(" /filterPred"_Stz);
    pFile->vPrint("\n"_Stz,pOptimiser->szQueryNm());
    pFile->vPrint("%s\n"_Stz,sBar.azChar());
    pFile->vPrint("\n"_Stz);
  }

  Card_Deck* pPipeline = m_pCI->G.pDeck;

  Board* pBoard = new Board(aBK[BK_JOINORDERING],
                            BK_JOINORDERING,
                            C_UNKNOWN,
                            m_pCI->m_pBench);
  m_pCI->G_vPushBoard(pBoard);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) Extract tableList from pipeline

  m_pCI->G.p_tableList = Card_TableList::pExtractList(pPipeline);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_tableList);
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  // 2) Extract joinPredList from pipeline

  m_pCI->G.p_joinPredList = Card_JoinPredList::pExtractList(pPipeline);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_joinPredList);
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  if (! m_pCI->G.p_joinPredList->nChildCards()) {
    s4K sTableList;
    Card_JoinPredList::vTablesWithNoIndexes(pPipeline,sTableList);

    Error xError(EC_MISSING_JOIN_INDEX,sTableList.szStz());
    throw xError;
  }

  // 3) Extract filtPredList from pipeline

  m_pCI->G.p_filtPredList = Card_FiltPredList::pExtractList(pPipeline);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_filtPredList);
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 4) Compute all possible join permutations

  Card_StreamSet* p_streamPrmAll =
      Card_StreamSet::pComputeAllPermutations(m_pCI->G.p_tableList, m_pCI->G.p_joinPredList);

  m_pCI->G.pDeck->vAppend(new Card_Section("All Permutations"_Stz));
  CardIter xCI(p_streamPrmAll);
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_streamPer->pDupCardRecursive());
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  // 5) Remove invalid permutations

  p_streamPrmAll->vRemoveInvalid();
  m_pCI->G.pDeck->vAppend(new Card_Section("Valid Permutations"_Stz));

  xCI.vRestart();
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_streamPer->pDupCardRecursive());
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  if (! p_streamPrmAll->nChildCards()) {
    s4K sTableList;
    Card_JoinPredList::vTablesWithNoIndexes(pPipeline,sTableList);

    Error xError(EC_MISSING_JOIN_INDEX,sTableList.szStz());
    throw xError;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 5) Calculate join load

  p_streamPrmAll->vEstCostsForAllPermutations(m_pCI->G.p_tableList, m_pCI->G.p_filtPredList);
  m_pCI->G.pDeck->vAppend(new Card_Section("Join Cost Est."_Stz));
  xCI.vRestart();
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_streamPer->pDupCardRecursive());
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  // get the join permutations strings
  xCI.vRestart();
  i8 iJoinPermutations = 0;
  while (xCI.G_bEachMain(CK_STREAMPER)) {
    Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pDerive(xCI.G.p_streamPer);
    //p_joinOrdering->vDump();
    s4K sJoinOrder;
    if (pFile) {
      p_joinOrdering->vJoinOrderString(sJoinOrder, m_pCI->G.p_tableList, true);
      //vTrace("JoinOrder %lld :%s\n"_Stz, iJoinPermutations, sJoinOrder.szStz());
      pFile->vPrint("%s\n"_Stz, sJoinOrder.szStz());
    }
    p_joinOrdering->vJoinOrderString(sJoinOrder,m_pCI->G.p_tableList);
    //vTrace("JoinOrder %lld :%s\n"_Stz, iJoinPermutations, sJoinOrder.szStz());
    pOptimiser->vJoinPermutation(iJoinPermutations, sJoinOrder);
    iJoinPermutations++;
    DELETE_CARD(p_joinOrdering);
  }
  pOptimiser->vJoinPermutations(iJoinPermutations);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 6) Pick final join ordering

  Card_JoinOrdering* p_joinOrdering = nullptr;
  Stz* sJoinOrder = ""_Stz;

  // Specific join order /jo ?
  if (AM::JOB::bHasTag(C_JO_PICK)) {
    sJoinOrder= AM::JOB::szTagValue(C_JO_PICK);
    i8 iJoinOrder = StrLib::iToI8((char*)sJoinOrder);
    p_joinOrdering =
        Card_JoinOrdering::pPick(p_streamPrmAll, iJoinOrder);

    if ( ! p_joinOrdering ) {
      vTrace("\n\nWARNING: INVALID JOIN ORDER IN /JO, PICKING THE BEST!!!\n\n"_Stz);
    }

  }

  if (! p_joinOrdering) {
    p_joinOrdering =
        Card_JoinOrdering::pPick(p_streamPrmAll,true);
  }

  if (! p_joinOrdering)  {

    Error xError(EC_MISSING_JOIN_INDEX,"Missing INDEX on TABLE"_Stz);

    throw xError;
  }

    m_pCI->G.pDeck->vAppend(new Card_Section("Picked Ordering"_Stz));
  m_pCI->G.pDeck->vAppend(p_joinOrdering->pDupCardRecursive());
  m_pCI->vRenumberCards();
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 7) get the final join ordering string

  s4K sJoinOrdering;
  if (pFile) {
    p_joinOrdering->vJoinOrderString(sJoinOrdering, m_pCI->G.p_tableList, true);
    //vTrace("JoinOrder: %s\n"_Stz,sJoinOrder.szStz());
    pFile->vPrint("\nPicked:\n%s\n\n"_Stz, sJoinOrdering.szStz());
  }
  p_joinOrdering->vJoinOrderString(sJoinOrdering,m_pCI->G.p_tableList);
  //vTrace("JoinOrder: %s\n"_Stz,sJoinOrdering.szStz());
  pOptimiser->vJoinOrder(sJoinOrdering);

  if (pFile) {
    pFile->vClose();
    delete pFile;
    pFile = nullptr;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  DELETE_CARD(p_streamPrmAll);

  pBoard = m_pCI->G.pBoard;
  m_pCI->G_vPopIter();
  m_pCI->m_pBench->vRemove(pBoard);
  delete pBoard;

  m_pCI->vPrintBoard(__LINE__);
  p_joinOrdering->vReorder(m_pCI->G.pDeck);
  m_pCI->vPrintBoard(__LINE__);

  DELETE_CARD(p_joinOrdering);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->G.pDeck->vProjectionUpdate(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Predicate repositioning post join ordering                                 */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vPredicateReposition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, XF_NONE, "predicate repositioning post join ordering"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

  m_pCI->vPrintBoard(__LINE__);

  while (m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vMarkWrt_table(m_pCI);
  }

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCopyMarkedPredsToTable(m_pCI->G.pDeck);
  }

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard(__LINE__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete Empty Where

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_TABLE, CK_FILTER)) {
    m_pCI->G.p_table->vDeleteFilterIfEmpty(m_pCI->G.p_filter);
  }

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
