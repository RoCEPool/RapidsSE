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

  //vGroup_PredicatePullup();
  vTransform_MarkCmpPredWrtTable();
  vTransform_CopyMarkedCmpPred();
  vTransform_CullMarkedCmpPred();
  vTransform_DeleteEmptyWhere();

  //vGroup_IndexBounds();
  vTransform_DeriveBoundsFromFilter();

  //vGroup_JoinOrdering();
  if (Card_Table::nTables_reverse_foldBreak(m_pCI) < 2)
    return;
//  if (Card_Table::nTables_foldBreak(m_pCI) < 2)
//    return;

  vTransform_JoinOrdering();
  vTransform_PredicateReposition();
  vTransform_DeriveBoundsFromFilter();

}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vTransform_MarkCmpPredWrtTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

  m_pCI->vPrintBoard(__LINE__);
  Card_Where::vMarkWherePredWrtTable(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vTransform_CopyMarkedCmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  m_pCI->G_vRestart();
  Card_Where::vMoveWherePredToTableFilter(m_pCI);

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vTransform_CullMarkedCmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  m_pCI->G_vRestart();
  Card_Where::vCullMarkedCmpPred(m_pCI);

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vTransform_DeleteEmptyWhere(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  m_pCI->G_vRestart();
  Card_Where::vDeleteEmpyWhere(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* copy <table>/<filter> up into <table>/<bounds>                             */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vTransform_DeriveBoundsFromFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, "Add <bounds> under each filtered <table>"_Stz);

  Card_Table::vDeriveBoundsFromFilter(m_pCI);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vTransform_JoinOrdering(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, "((replace with an explanation))"_Stz);

  Optimiser* pOptimiser = m_pCI->m_pBench->pOptimiser();

  Card_Deck* pPipeline = m_pCI->G.pDeck;

  Board* pBoard = new Board(aBK[BK_JOINORDERING],
                            BK_JOINORDERING,
                            C_UNKNOWN,
                            m_pCI->m_pBench);
  m_pCI->G_vPushBoard(pBoard);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) Extract tableList from pipeline

  m_pCI->G.p_tableList = Card_TableList::pExtract(pPipeline);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_tableList);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) Extract joinPredList from pipeline

  m_pCI->G.p_joinPredList = Card_JoinPredList::pExtract(pPipeline);
  m_pCI->G.pDeck->vAppend(m_pCI->G.p_joinPredList);
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3) Compute all possible join permutations

  Card_JoinPermAll* p_joinPermAll =
      Card_JoinPermAll::pAllPermutedJoins(m_pCI->G.p_tableList, m_pCI->G.p_joinPredList);

  m_pCI->G.pDeck->vAppend(new Card_Section("All Permutations"_Stz));
  CardIter xCI(p_joinPermAll);
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_joinPermSeq->pDupCardAndSubCards());
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 4) Remove invalid permutations

  p_joinPermAll->vRemoveInvalid();
  m_pCI->G.pDeck->vAppend(new Card_Section("Valid Permutations"_Stz));

  xCI.vRestart();
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_joinPermSeq->pDupCardAndSubCards());
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 5) Calculate join load

  p_joinPermAll->vCalculateLoad(m_pCI->G.p_tableList);
  m_pCI->G.pDeck->vAppend(new Card_Section("Join Loads"_Stz));
  xCI.vRestart();
  while (xCI.G_bEachMain())
    m_pCI->G.pDeck->vAppend(xCI.G.p_joinPermSeq->pDupCardAndSubCards());
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // get the join permutations strings
  xCI.vRestart();
  i8 iJoinPermutations = 0;
  while (xCI.G_bEachMain(CK_JOINPERMSEQ)) {
    Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pDerive(xCI.G.p_joinPermSeq);
    s256 sJoinOrder;
    p_joinOrdering->vJoinOrderString(sJoinOrder,m_pCI->G.p_tableList);
    //vTrace("JoinOrder %lld :%s\n"_Stz, iJoinPermutations, sJoinOrder.szStz());
    pOptimiser->vJoinPermutation(iJoinPermutations, sJoinOrder);
    iJoinPermutations++;
    DELETE_CARD(p_joinOrdering);
  }
  pOptimiser->vJoinPermutations(iJoinPermutations);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 6) Pick final join ordering

  Card_JoinOrdering* p_joinOrdering =
      Card_JoinOrdering::pPick(p_joinPermAll);
  m_pCI->G.pDeck->vAppend(new Card_Section("Picked Ordering"_Stz));
  m_pCI->G.pDeck->vAppend(p_joinOrdering->pDupCardAndSubCards());
  m_pCI->G.pDeck->vRenumberRecursive();
  m_pCI->vPrintBoard(__LINE__);

  // get the final join ordering string
  s256 sJoinOrder;
  p_joinOrdering->vJoinOrderString(sJoinOrder,m_pCI->G.p_tableList);
  vTrace("JoinOrder: %s\n"_Stz,sJoinOrder.szStz());
  pOptimiser->vJoinOrder(sJoinOrder);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  DELETE_CARD(p_joinPermAll);

  pBoard = m_pCI->G.pBoard;
  m_pCI->G_vPopIter();
  m_pCI->m_pBench->vRemove(pBoard);
  delete pBoard;

  m_pCI->vPrintBoard(__LINE__);
  p_joinOrdering->vReorder(m_pCI->G.pDeck);
  m_pCI->vPrintBoard(__LINE__);

  DELETE_CARD(p_joinOrdering);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Projection::vProjection_Analysis_v2(m_pCI);

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Predicate Pullup                                                           */
/*----------------------------------------------------------------------------*/
void JoinOrdering::vTransform_PredicateReposition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOINORDERING,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

  m_pCI->vPrintBoard(__LINE__);

  while (m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vMarkWrt_table(m_pCI->G.pDeck);
  }

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCopyMarkedPredToTable(m_pCI->G.pDeck);
  }

  m_pCI->vPrintBoard();

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete Empty Where

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub_reverse_foldBreak(CK_TABLE, CK_FILTER)) {
    m_pCI->G.p_table->vRemove_filter_IfEmpty(m_pCI->G.p_filter);
  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
