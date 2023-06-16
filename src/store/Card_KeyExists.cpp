/*============================================================================*/
/* Card_KeyExists.cpp -- Key Exists                                           */
/*============================================================================*/

#include "Card_KeyExists.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_KeyExists,CK_KEYEXISTS)
__aNewFunction(Card_KeyExists,CN_CARD)
__vDeleteFunction(Card_KeyExists,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_KeyExists::Card_KeyExists(void)
: Card_OP(CK_KEYEXISTS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_KeyExists::~Card_KeyExists(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* table morphs to keyExists                                                  */
/*----------------------------------------------------------------------------*/
void Card_KeyExists::vMorph_table_To_keyExists(STORE::CardIter *pCI)
{
  _static_

  pCI->G.p_keyExists = pCI->G.p_table->pSub_keyExists();

  pCI->G.p_table->vRelegate_table(pCI, pCI->G.p_keyExists);

  // todo: this should be in index list
  pCI->G.p_index = pCI->G.p_keyExists->pSub_index();
  pCI->G.p_index->vSet_Choose_index();
  pCI->G.p_index->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);

  pCI->G.p_filter = pCI->G.p_keyExists->pSub_filter();
  pCI->G.p_filter->vCard(CK_CONSTRAINT);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_KeyExists::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_KeyExists::vEarlyProject_recurse_add(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pCI->pPrevChild()->bIsCard(CK_PROJECT)) {

    pCI->Early.m_pOP_leaf = this;
    pCI->Analysis.m_pProjection_prior = pSub_projection();

    pCI->vPrintBoard();
    return;

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  // check for additional/temporary transients
  vEarlyProject_add_transients(pCI);

  // insert new empty early project (has only needed) before this indexJoin
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->G_vInsertBefore(p_project_early, this);
  pCI->G_vMainResynch();

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard();

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...


  pCI->vPrintBoard();

  vUpdate_projection(pCI);

  pCI->vPrintBoard();

  vCorrectStmOdn(pCI->Analysis.m_pProjection_prior);

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;

  pCI->Analysis.m_pProjection_prior = pSub_projection();

  pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_KeyExists::vEarlyProject_add_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  pCI->G.p_indexList = pSub_indexList();
  pCI->G.p_index     = pCI->G.p_indexList->pFindChosenIndex();
  pCI->G.p_bounds    = pCI->G.p_index->pSub_bounds();
  pCI->G.p_bound     = pCI->G.p_bounds->pFindChosenBound();
  //pCI->G.p_bound->vDump();
  pCI->G.p_term      = pCI->G.p_bound->pGet_tblColOdn();

  RASSERT(pCI->G.p_term);
  //pCI->G.p_term->vDump();

  /*
   * todo:
   *
   * i) is the bound $t.c in the needed projection?
   *    n: find defn expr for $t.c
   *       add expr (defn expr for $t.c) into needed projection
   */

  if (!pCI->Early.m_pNeeded_later->pFindExpr(pCI->G.p_term->pSymbol())) {

    //pCI->Early.m_pNeeded_later->vDump();

    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(pCI->G.p_term)) {
      //p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      pCI->Early.m_pNeeded_later->vAppend(p_expr_clone);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }

    //pCI->Early.m_pNeeded_later->vDump();

    vDebugPoint();
  }

  vDebugPoint();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  auto bLambda = [pCI] (Card_Term* p_term) {
//
//    if (! p_term->bIsValOdn())
//      return C_CONTINUE_RECURSION;
//
//    if (pCI->Early.m_pNeeded_later->pFindExpr(p_term))
//      return C_CONTINUE_RECURSION;
//
//    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(p_term)) {
//      p_expr_defn->vDump();
//      Card_Expr* p_expr_clone = p_expr_defn->pClone(true);
//      pCI->Early.m_pNeeded_later->vAppend(p_expr_clone);
//      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
//    }
//
//    return C_CONTINUE_RECURSION;
//  };
//
//  pCI->G.p_filter = pSub_filter();
//
//  CardIter xCI_filter(pCI->G.p_filter);
//  while (xCI_filter.G_bEachMain(CK_BOOLCALC)) {
//
//    xCI_filter.G.p_boolCalc->bApplyRecursive_term(bLambda);
//
//  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_KeyExists::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ( (pCI->G.p_projection = pSub_projection()) ) {
    vDeleteChild(pCI->G.p_projection);
  }

  if (pCI->Analysis.m_pProjection_prior) {
    pCI->G.p_projection = pCI->Analysis.m_pProjection_prior->pClone();
    vAppend(pCI->G.p_projection);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
