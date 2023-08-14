/*============================================================================*/
/* Card_KeyNotExists.cpp -- Key Not Exists                                    */
/*============================================================================*/

#include "Card_KeyNotExists.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_KeyNotExists,CK_KEYNOTEXISTS)
__aNewFunction(Card_KeyNotExists,CN_CARD)
__vDeleteFunction(Card_KeyNotExists,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_KeyNotExists::Card_KeyNotExists(void)
: Card_COND(CK_KEYNOTEXISTS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_KeyNotExists::~Card_KeyNotExists(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* table morphs to keyNotExists                                               */
/*----------------------------------------------------------------------------*/
void Card_KeyNotExists::vMorph_table_To_keyNotExists(STORE::CardIter *pCI)
{
  _static_

  pCI->G.p_keyNotExists = pCI->G.p_table->pSub_keyNotExists();

  pCI->G.p_table->vRelegate_table(pCI, pCI->G.p_keyNotExists);

  // todo: this should be in index list
  pCI->G.p_index = pCI->G.p_keyNotExists->pSub_index();
  pCI->G.p_index->vSet_Choose_index();
  pCI->G.p_index->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);

  pCI->G.p_filter = pCI->G.p_keyNotExists->pSub_filter();
  pCI->G.p_filter->vCard(CK_CONSTRAINT);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_KeyNotExists::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(pCI);
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_KeyNotExists::vEarlyProject_recurse_add(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pCI->pPrevChild()->bIsCard(CK_PROJECT)) {

    pCI->Early.m_pOP_leaf = this;
    pCI->Prior.m_pProjection = pSub_projection();

    pCI->vPrintBoard();
    return;


  }

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  // check for additional/temporary transients
  vEarlyProject_add_transients(pCI);

  // insert new empty early project (has only needed) before this indexJoin
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->G_vInsertBefore(p_project_early, this);
  pCI->G_vMainResynch();

  pCI->vRenumberCards();
  pCI->vPrintBoard();

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  pCI->vPrintBoard();

  vUpdate_projection(pCI);

  vCorrectStmOdn(pCI->Prior.m_pProjection);

  pCI->vPrintBoard();

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;

  pCI->Prior.m_pProjection = pSub_projection();

  pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_KeyNotExists::vEarlyProject_add_transients(CardIter* pCI)
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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_KeyNotExists::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ( (pCI->G.p_projection = pSub_projection()) ) {
    vDeleteChild(pCI->G.p_projection);
  }

  if (pCI->Prior.m_pProjection) {
    pCI->G.p_projection = pCI->Prior.m_pProjection->pClone();
    vAppend(pCI->G.p_projection);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_KeyNotExists::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  if (1 == iDepth) {
    vValidate_mustHave(CK_LEVEL, CK_TABLE, CK_INDEXLIST, CK_NONE);
    RASSERT(pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  }

  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
