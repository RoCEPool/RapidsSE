/*============================================================================*/
/* Card_IndexJoin.cpp -- Index Join                                           */
/*============================================================================*/

#include "Card_IndexJoin.hpp"
#include "CardIter.hpp"
#include "Board.hpp"
#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexJoin,CK_INDEXJOIN)
__aNewFunction(Card_IndexJoin,CN_CARD)
__vDeleteFunction(Card_IndexJoin,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::Card_IndexJoin(void)
: Card_OP(CK_INDEXJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::~Card_IndexJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* derive projection key                                                      */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // extends the projection key

  RASSERT(bPeg_Chosen_index(pCI));

  // snap onto <indexJoin>/<projection>, delete existing <indexJoin>/<projection>/<key>
  pCI->G.p_projection = pSub_projection();
  pCI->G.p_projection->vDelete_key(pCI);

  if (pCI->StreamOrder.m_pKey_running) {
    pCI->G.p_key = _pCard_Key(pCI->StreamOrder.m_pKey_running->pDupCardAndSubCards());
    pCI->G.p_projection->vAppend(pCI->G.p_key);
  }
  else {
    pCI->G.p_key = new Card_Key;
    pCI->G.p_projection->vAppend(pCI->G.p_key);
  }
  pCI->G.p_index->vAdd_key_exprs(pCI);

  // set running projection for use by next op
  pCI->StreamOrder.m_pProjection_running = pCI->G.p_projection;
  pCI->StreamOrder.m_pKey_running = pCI->G.p_key;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vEarly_project_recurse_add(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Projection* p_projection_needed_save = pCI->EarlyProject.m_pProjection_needed->pClone();
  Card_Needed* p_needed_save = pCI->EarlyProject.m_pNeeded->pClone();

  // check for temporary/additional transients
  vEarly_project_check_transients(pCI);

  vAppend(pCI->EarlyProject.m_pProjection_needed->pClone());
  vAppend(pCI->EarlyProject.m_pNeeded->pClone());

  pCI->vPrintBoard();

  Card_OP::vEarly_project_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  pCI->vPrintBoard();

  vUpdate_projection_Off_running(pCI);

  pCI->vPrintBoard();

  //pCI->G.p_filter->vFlipTerms(pCI->EarlyProject.m_pProjection_needed);

  //pCI->G.p_indexList->vFlipTerms(pCI->EarlyProject.m_pProjection_needed);

  pCI->G.p_projection = pSub_projection();
  //pCI->G.p_projection->vFlipTerms(pCI->EarlyProject.m_pProjection_needed);

  pCI->vPrintBoard();


  Card_Project* p_project_early = Card_Project::pNew_project_early(pCI);
  pCI->G_vInsertAfter(p_project_early, this);

  delete pCI->EarlyProject.m_pProjection_needed;
  pCI->EarlyProject.m_pProjection_needed = p_projection_needed_save;
  delete pCI->EarlyProject.m_pNeeded;
  pCI->EarlyProject.m_pNeeded = p_needed_save;

  pCI->ProjectionAnalysis.m_pProjection_running = p_project_early->pSub_projection();

  pCI->vPrintBoard();

  vDebugPoint();
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vEarly_project_check_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_filter = pSub_filter();

  pCI->G.p_indexList = pSub_indexList();
  pCI->G.p_index     = pCI->G.p_indexList->pFindChosenIndex();
  pCI->G.p_bounds    = pCI->G.p_index->pSub_bounds();
  pCI->G.p_bound     = pCI->G.p_bounds->pSub_bound();
  pCI->G.p_bound->vDump();
  pCI->G.p_term      = pCI->G.p_bound->pGet_tblColOdn();

  RASSERT(pCI->G.p_term);
  pCI->G.p_term->vDump();

  /*
   * todo:
   *
   * i) is the bound $t.c in the needed projection?
   *    n: find defn expr for $t.c
   *       add expr (defn expr for $t.c) into needed projection
   */

  if (! pCI->EarlyProject.m_pProjection_needed->pFind_expr_With_projectedCol(pCI->G.p_term)) {

    pCI->EarlyProject.m_pProjection_needed->vDump();

    if (Card_Expr* p_expr_defn = pCI->pResolvePriorExpr_With_ProjectedCol(pCI->G.p_term)) {
      p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone(true);
      pCI->EarlyProject.m_pProjection_needed->vAppend(p_expr_clone);
      pCI->EarlyProject.m_pProjection_needed->vRenumberAllExpr();
    }

    pCI->EarlyProject.m_pProjection_needed->vDump();

    vDebugPoint();
  }

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* individuate filter                                                         */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vIndividuateFilter(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    Board::vIndividuate_indexJoin_filter_b(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* table morphs to indexJoin                                                  */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vTableMorphsToIndexJoin(STORE::CardIter *pCI)
{
  _static_

  if (! pCI->G.p_bounds) {
    s256 sErrorMsg;
    sErrorMsg.vSprint("No index for table(%c) %s"_Stz,
                      pCI->G.p_table->cValue(),
                      pCI->G.p_table->pSub_tableNm()->szValue());
    Error xError(EC_MISSING_JOIN_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

  pCI->G.p_indexList->vChoose_index_For_indexJoin();

  pCI->G_vNewMember(CK_INDEXJOIN);

  pCI->G.p_indexJoin->vTransferChildren(pCI->G.p_table);

  pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table, pCI->G.p_indexJoin);
  pCI->G.p_indexJoin->vInsert(pCI->G.p_table);

  //pCI->G.p_indexJoin->vRemoveSub(pCI->G.p_index);
  //pCI->G.p_indexJoin->vAppend(pCI->G.p_index);

  pCI->G.p_MainCard = pCI->G.p_indexJoin;

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
