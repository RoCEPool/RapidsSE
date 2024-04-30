/*============================================================================*/
/* Card_IndexJoin.cpp -- Index Join                                           */
/*============================================================================*/

#include "Card_IndexJoin.hpp"
#include "CardIter.hpp"
#include "Board.hpp"
#include "Error.hpp"

#include "XF_Optimise_Early_projection.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_IndexJoin, CK_o_INDEXJOIN)
__aNewFunction(Card_IndexJoin,CN_CARD)
__vDeleteFunction(Card_IndexJoin,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexJoin::Card_IndexJoin(void)
: Card_OP(CK_o_INDEXJOIN, C_UNKNOWN)
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
void Card_IndexJoin::vDerive_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // extends the projection key

  RASSERT(bPeg_Projection(pCI));
  RASSERT(bPeg_Chosen_index(pCI));

   pCI->E.p_emit->vDeleteSub(CK_p_STREAMKEY);

  if (pCI->StreamOrder.m_pStreamKey) {
    pCI->E.p_streamKey = _pCard_StreamKey(pCI->StreamOrder.m_pStreamKey->pDupCardRecursive());
  }
  else {
    pCI->E.p_streamKey = new Card_StreamKey;
  }

  pCI->E.p_index->vAdd_streamKey_exprs(pCI);

  pCI->E.p_Op->vAppendChild(pCI->E.p_streamKey);

  // set running projection for use by next op
  pCI->StreamOrder.m_pEmit = pCI->E.p_emit;
  pCI->StreamOrder.m_pStreamKey  = pCI->E.p_streamKey;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vEarlyProject_add_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  pCI->E.p_indexList = pSub_indexList();
  pCI->E.p_index     = pCI->E.p_indexList->pFindChosenIndex();
  pCI->E.p_bounds    = pCI->E.p_index->pSub_bounds();
  pCI->E.p_bound     = pCI->E.p_bounds->pFindChosenBound();
  //pCI->G.p_bound->vDump();
  pCI->E.p_term      = pCI->E.p_bound->pGet_IfNot_idxCol();

  RASSERT(pCI->E.p_term);
  //pCI->G.p_term->vDump();


  if (!pCI->Early.m_pNeeded_later->pFind_expr(pCI->E.p_term->pSymbol())) {

    if (pCI->E.p_term->pSymbol()->bHasFacet(SF_EXP_COL)) {
      Card_Expr* p_expr_expCol = Card_Expr::pCraft_expr_Of_term(pCI->E.p_term);
      pCI->Early.m_pNeeded_later->vAppendChild(p_expr_expCol);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }
    else
    if (Card_Expr* p_expr_defn = pCI->pPrior_expr(pCI->E.p_term->pSymbol())) {
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      //p_expr_clone->vDump(__func__);
      p_expr_clone->pGet_Symbol(0)->vDeleteFacet(SF_EXP_COL);
      //p_expr_clone->vDump(__func__);
      pCI->Early.m_pNeeded_later->vAppendChild(p_expr_clone);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }

    //pCI->Early.m_pNeeded_later->vDump();

    vDebugPoint();
  }

  pCI->E.p_term      = pCI->E.p_bound->pGet_idxCol();

  vDebugPoint();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambda = [pCI] (Card_Term* p_term) {

    if (!p_term->pSymbol()->bHasFacet(SF_EXP_COL) && ! p_term->pSymbol()->bHasFacet(SF_TBL_COL))
      return C_CONTINUE_RECURSION;

    if (pCI->Early.m_pNeeded_later->pFind_expr_ignoring(p_term->pSymbol(), SF_EXP_COL)) {
      return C_CONTINUE_RECURSION;
    }

    if (Card_Expr* p_expr_defn = pCI->pPrior_expr(p_term->pSymbol())) {
      //p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      if (p_expr_clone->pGet_Symbol(0)->nFacets() > 1)
        p_expr_clone->pGet_Symbol(0)->vDeleteFacet(SF_EXP_COL);
      pCI->Early.m_pNeeded_later->vAppendChild(p_expr_clone);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }

    return C_CONTINUE_RECURSION;
  };

  if (! (pCI->E.p_filter = pSub_filter()))
    return;

  if ((pCI->E.p_predicate = pCI->E.p_filter->pSub_predicate()))
    pCI->E.p_predicate->bLambdaWalk_term(bLambda);

}
/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
bool Card_IndexJoin::bEarlyProject_root(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->Early.m_pNeeded_later);

  if (pCI->pPeekPrev()->bIsCard(CK_o_PROJECT))
    return false;

  // get the root needed form of ordAgg's projection
  pCI->Early.m_pNeeded_later  = pCI->E.p_emit->pCraftRoot_needed(pCI);


  return true;
}

/*----------------------------------------------------------------------------*/
/* table morphs to indexJoin                                                  */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vMorph_table_To_indexJoin(STORE::CardIter *pCI)
{
  _static_

  pCI->E.p_indexJoin = pCI->E.p_table->pSub_indexJoin();
  pCI->E.p_indexList = pCI->E.p_table->pSub_indexList();

  pCI->E.p_bounds = pCI->E.p_table->pSub_bounds();

  if (! pCI->E.p_bounds) {
    s256 sErrorMsg;
    sErrorMsg.vSprint("No index for table(%c) %s"_Stz,
                      pCI->E.p_table->pSymbol()->cTblSyn(),
                      pCI->E.p_table->pSub_tableNm()->szFacet(SF_TBL_NM));
    Error xError(EC_MISSING_JOIN_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

//  pCI->vNewMember(CK_o_INDEXJOIN);
//  pCI->G.p_table->vRelegate_table(pCI, pCI->G.p_indexJoin);
  pCI->E.p_indexJoin = _pCard_IndexJoin(pCI->E.p_from->pCard(CK_o_INDEXJOIN));

  pCI->E.p_indexList->vChoose_index_For_indexJoin();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_child_mustBe(CK_p_LEVEL, CK_p_TABLE, CK_NONE);

  RASSERT(pSub_table()->pSub_indexList());

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
