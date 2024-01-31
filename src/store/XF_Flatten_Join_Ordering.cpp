/*============================================================================*/
/* XF_Flatten_Join_Ordering.cpp -- XF_Flatten_Join_Ordering                                           */
/*============================================================================*/

#include "XF_Flatten_Join_Ordering.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "XF_Fold.hpp"
#include "XF_Flatten_Smoosh.hpp"
#include "XF_Flatten_Smooth.hpp"
#include "Optimiser.hpp"

#include "XF_Flatten_Anchor_table.hpp"
#include "XF_Flatten_Derive_bounds.hpp"
#include "XF_Flatten_Purge_where.hpp"
#include "XF_Flatten_Pullup_rlnConj.hpp"
#include "XF_Flatten_Predicate_Reposition.hpp"
#include "XF_Prolog_Analyse_emit.hpp"
#include "eOT.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Flatten_Join_Ordering, CN_XF_FLATTEN_JOIN_ORDERING)
__aNewFunction(XF_Flatten_Join_Ordering, CN_XF_FLATTEN_JOIN_ORDERING)
__vDeleteFunction(XF_Flatten_Join_Ordering, CN_XF_FLATTEN_JOIN_ORDERING)
__aFooterFunction(XF_Flatten_Join_Ordering)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Flatten_Join_Ordering::XF_Flatten_Join_Ordering(CardIter* pCI)
: Object(CN_XF_FLATTEN_JOIN_ORDERING)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_JOIN_ORDERING, _FOOTER_);

  m_pCI = pCI;

  _checkInitialised_(CN_XF_FLATTEN_JOIN_ORDERING);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_JOIN_ORDERING, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Flatten_Join_Ordering::~XF_Flatten_Join_Ordering(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_JOIN_ORDERING, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Join_Ordering::vXF_Join_Ordering(CardIter* pCI)
{
  _static_

  XF_Flatten_Join_Ordering* pXF_Join_Ordering = new XF_Flatten_Join_Ordering(pCI);

  try {
    pXF_Join_Ordering->vJoinOrdering();
    delete pXF_Join_Ordering;
  }
  catch (Error xError) {
    delete pXF_Join_Ordering;
    throw xError;
  }

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Join_Ordering::vJoinOrdering(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_JOIN_ORDERING, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Predicate Pull Up : move suitable where predicates to table filter

  m_pCI->vValidateBench();

  XF_Flatten_Pullup_rlnConj::vXF_Pullup_rlnConj(m_pCI);
  m_pCI->vValidateBench();

  // delete empty where
  XF_Flatten_Purge_where::vXF_Delete_Empty_where(m_pCI);
  m_pCI->vValidateBench();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Index Bounds : move table filter predicates to table index bounds

  // derive index bounds from table filter
  XF_Flatten_Derive_bounds::vXF_Derive_bounds(m_pCI);
  m_pCI->vValidateBench();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fix position of table which uses subquery result as pos 0
  XF_Flatten_Anchor_table::vXF_Fix_table_Pos(m_pCI);
  m_pCI->vValidateBench();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Join Ordering Proper

  if (AM::JOB::bHasTag(C_JO_DISABLE)) {
    XF_Prolog_Analyse_emit::vAnalyse(m_pCI);
    return;
  }

  if (Card_Table::nTables_reverse_foldBreak(m_pCI) < 2)
    return;

  // reorder the tables
  vReorderTables();

  XF_Prolog_Analyse_emit::vAnalyse(m_pCI);

  XF_Flatten_Predicate_Reposition::vXF_Predicate_Reposition(m_pCI);

  m_pCI->vValidateBench();

  XF_Flatten_Predicate_Reposition::vXF_Predicate_Reposition(m_pCI);
  m_pCI->vValidateBench();

  XF_Flatten_Pullup_rlnConj::vXF_Pullup_rlnConj(m_pCI);
  m_pCI->vValidateBench();

  // re-derive bounds from filter after join ordering
  XF_Flatten_Derive_bounds::vXF_Derive_bounds(m_pCI);
  m_pCI->vValidateBench();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Flatten_Join_Ordering::vReorderTables(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_FLATTEN_JOIN_ORDERING, _FOOTER_);

  m_pCI->vTransform(__func__, XF_FLATTEN_JOIN_ORDERING, "join ordering"_Stz);

  File* pFile = nullptr;

  if (AM::JOB::bHasTag(C_JOWRITE)) {
    pFile = new File(this);
    RASSERT(pFile->bOpenAppend("/tmp/optimiser.txt"_Stz));
  }


  Optimiser* pOptimiser = m_pCI->m_pBench->pOptimiser();

  if (pFile) {
    s256 sBar;
    StrLib::vPad(sBar.azChar(), sBar.nAreaBytes(), 120,'-');
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

  Board* pBoard = new Board(C_UNKNOWN,
                            BK_JOINORDERING,
                            aBK[BK_JOINORDERING],
                            m_pCI->m_pBench);
  m_pCI->G_vPushBoard(pBoard);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) Extract tableList from pipeline

  m_pCI->G.p_tableList = Card_TableList::pExtractList(pPipeline);
  m_pCI->G.pDeck->vAppendChild(m_pCI->G.p_tableList);

  // 2) Extract joinPredList from pipeline

  m_pCI->G.p_joinPredList = Card_JoinPredList::pExtractList(pPipeline);
  m_pCI->G.pDeck->vAppendChild(m_pCI->G.p_joinPredList);

  if (! m_pCI->G.p_joinPredList->nChildCards()) {
    s4K sTableList;
    Card_JoinPredList::vTablesWithNoIndexes(pPipeline,sTableList);

    Error xError(EC_MISSING_JOIN_INDEX,sTableList.szStz());
    throw xError;
  }

  // 3) Extract filtPredList from pipeline

  m_pCI->G.p_filtPredList = Card_FiltPredList::pExtractList(pPipeline);
  m_pCI->G.pDeck->vAppendChild(m_pCI->G.p_filtPredList);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 4) Compute all possible join permutations

  Card_StreamSet* p_streamPrmAll =
      Card_StreamSet::pComputeAllPermutations(m_pCI->G.p_tableList, m_pCI->G.p_joinPredList);

  m_pCI->G.pDeck->vAppendChild((new Card_Section) ->SYM(SF_NOTE,"All Permutations"_Stz));
  Kid xKid(p_streamPrmAll);
  while (xKid.bEach())
    m_pCI->G.pDeck->vAppendChild(xKid.p_streamPer()->pDupCardRecursive());

  // 5) Remove invalid permutations

  p_streamPrmAll->vRemoveInvalid();
  m_pCI->G.pDeck->vAppendChild((new Card_Section) ->SYM(SF_NOTE,"Valid Permutations"_Stz));

  xKid.vEnded();
  xKid.vBegin();
  while (xKid.bEach())
    m_pCI->G.pDeck->vAppendChild(xKid.p_streamPer()->pDupCardRecursive());

  if (! p_streamPrmAll->nChildCards()) {
    s4K sTableList;
    Card_JoinPredList::vTablesWithNoIndexes(pPipeline,sTableList);

    Error xError(EC_MISSING_JOIN_INDEX,sTableList.szStz());
    throw xError;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 5) Calculate join load

  p_streamPrmAll->vEstCostsForAllPermutations(m_pCI->G.p_tableList, m_pCI->G.p_filtPredList);
  m_pCI->G.pDeck->vAppendChild((new Card_Section) ->SYM(SF_NOTE,"Join Cost Est."_Stz));

  xKid.vEnded();
  xKid.vBegin();
  while (xKid.bEach())
    m_pCI->G.pDeck->vAppendChild(xKid.p_streamPer()->pDupCardRecursive());

  // get the join permutations strings

  xKid.vEnded();
  xKid.vBegin();
  i8 iJoinPermutations = 0;
  while (xKid.bEach(CK_STREAMPER)) {
    Card_JoinOrdering* p_joinOrdering = Card_JoinOrdering::pDerive(xKid.p_streamPer());
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

    m_pCI->G.pDeck->vAppendChild((new Card_Section) ->SYM(SF_NOTE,"Picked Ordering"_Stz));
  m_pCI->G.pDeck->vAppendChild(p_joinOrdering->pDupCardRecursive());

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
  m_pCI->G_vPopBoard();
  m_pCI->m_pBench->vRemove(pBoard);
  delete pBoard;

  m_pCI->vValidateBench();

 // m_pCI->vPrintBoard(__func__,__LINE__);
  p_joinOrdering->vReorder(m_pCI->G.pDeck);
 // m_pCI->vPrintBoard(__func__,__LINE__);


  DELETE_CARD(p_joinOrdering);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->vValidateBench();

  m_pCI->vPrintBoard(__func__,__LINE__);

  //m_pCI->G.pDeck->vProjectionUpdate(m_pCI);

  m_pCI->pBoard()->pState()->vInsert(aOT[OT_JOIN_ORDERING]);

 // m_pCI->vPrintBoard(__func__,__LINE__);

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
