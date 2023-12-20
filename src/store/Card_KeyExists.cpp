/*============================================================================*/
/* Card_KeyExists.cpp -- Key Exists                                           */
/*============================================================================*/

#include "Card_KeyExists.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_KeyExists, CK_o_KEYEXISTS)
__aNewFunction(Card_KeyExists,CN_CARD)
__vDeleteFunction(Card_KeyExists,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_KeyExists::Card_KeyExists(void)
: Card_CONDITION(CK_o_KEYEXISTS, C_UNKNOWN)
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
/* from morphs to keyExists                                                   */
/*----------------------------------------------------------------------------*/
void Card_KeyExists::vMorph_from_To_keyExists(CardIter *pCI)
{
  _static_

  pCI->G.p_keyExists = pCI->G.p_from->pSub_keyExists();

  pCI->G.p_from->vDeleteChild(pCI->G.p_keyExists);

  pCI->G.p_keyExists = _pCard_KeyExists(pCI->G.p_from->pCard(CK_o_KEYEXISTS));

  // todo: this should be in index list
  pCI->G.p_index = pCI->G.p_keyExists->pSub_index();
  pCI->G.p_index->vSet_Choose_index();
  pCI->G.p_index->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);

  pCI->G.p_filter = pCI->G.p_keyExists->pSub_filter();
  pCI->G.p_filter->vCard(CK_s_CONSTRAINT);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_KeyExists::vEarlyProject_recurse_add(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pCI->pPrevChild()->bIsCard(CK_o_PROJECT)) {

    pCI->Early.m_pOP_leaf = this;
    pCI->Flux.m_pArriving = pSub_emit();

   // pCI->vPrintBoard();
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

 // pCI->vRenumberCards();
 // pCI->vPrintBoard();

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...


 // pCI->vPrintBoard();

  pCI->G.p_OP = pCI->G.p_keyExists;
  pCI->G.p_keyExists->vRecreate_emit(pCI);

 // pCI->vPrintBoard();

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;
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
  pCI->G.p_term      = pCI->G.p_bound->pGet_tblCol();

  RASSERT(pCI->G.p_term);
  //pCI->G.p_term->vDump();

  /*
   * todo:
   *
   * i) is the bound $t.c in the needed projection?
   *    n: find defn expr for $t.c
   *       add expr (defn expr for $t.c) into needed projection
   */

  if (!pCI->Early.m_pNeeded_later->pFind_expr_Via_term0(pCI->G.p_term->pSymbol())) {

    //pCI->Early.m_pNeeded_later->vDump();

    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(pCI->G.p_term)) {
      //p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      pCI->Early.m_pNeeded_later->vAppendChild(p_expr_clone);
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
void Card_KeyExists::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pParent);

  if (1 == iDepth) {
    vValidate_mustHave(CK_s_LEVEL, CK_s_TABLE, CK_s_CONSTRAINT, CK_NONE);
    RASSERT(pFindPpyRecursive(P_ChosenIndex, CK_INDEX));
    RASSERT(pSub_bound());
  }

  Card::vValidate(pCI,iDepth,pParent);
  if (1 == iDepth)
    Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
