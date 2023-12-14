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
  pCI->G.p_emit->vDelete_streamKey(pCI);


  if (pCI->StreamOrder.m_pStreamKey) {
    pCI->G.p_streamKey = _pCard_StreamKey(pCI->StreamOrder.m_pStreamKey->pDupCardRecursive());
  }
  else {
    pCI->G.p_streamKey = new Card_StreamKey;
  }

  pCI->G.p_index->vAdd_streamKey_exprs(pCI);

  pCI->G.p_emit->vAppendChild(pCI->G.p_streamKey);


  // set running projection for use by next op
  pCI->StreamOrder.m_pEmitted = pCI->G.p_emit;
  pCI->StreamOrder.m_pStreamKey  = pCI->G.p_streamKey;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vEarlyProject_recurse_add(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pCI->pPrevChild()->bIsCard(CK_o_GBYAGG) ||
      pCI->pPrevChild()->bIsCard(CK_o_PROJECT)) {

    pCI->Early.m_pOP_leaf = this;
    pCI->Flux.m_pArriving = pSub_emit();

   // pCI->vPrintBoard();
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

 // pCI->vRenumberCards();
 // pCI->vPrintBoard();

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

//  pCI->vRenumberCards();
//  pCI->vPrintBoard();

  pCI->G.p_table = pSub_table();
  vUpdate_emit_OP(pCI);
  //vRecreate_emit(pCI);

//  pCI->vRenumberCards();
//  pCI->vPrintBoard();

  //pCI->Flux.m_pArriving->vDump(__func__);
  vCorrect_expCol(pCI->Flux.m_pArriving);

 // pCI->vPrintBoard();

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;

  pCI->Flux.m_pArriving = pSub_emit();



 // pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vEarlyProject_add_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  pCI->G.p_indexList = pSub_indexList();
  pCI->G.p_index     = pCI->G.p_indexList->pFindChosenIndex();
  pCI->G.p_bounds    = pCI->G.p_index->pSub_bounds();
  pCI->G.p_bound     = pCI->G.p_bounds->pFindChosenBound();
  //pCI->G.p_bound->vDump();
  pCI->G.p_term      = pCI->G.p_bound->pGet_IfNot_idxCol();

  RASSERT(pCI->G.p_term);
  //pCI->G.p_term->vDump();


  if (!pCI->Early.m_pNeeded_later->pFind_expr_Via_term0(pCI->G.p_term->pSymbol())) {

    if (pCI->G.p_term->bIs_expCol()) {
      Card_Expr* p_expr_expCol = Card_Expr::pCast_expr_term_expCol(pCI->G.p_term);
      pCI->Early.m_pNeeded_later->vAppendChild(p_expr_expCol);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }
    else
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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambda = [pCI] (Card_Term* p_term) {

    if (!p_term->bIsColSym())
      return C_CONTINUE_RECURSION;

    if (pCI->Early.m_pNeeded_later->pFind_expr_Via_term0(p_term->pSymbol()))
      return C_CONTINUE_RECURSION;

    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(p_term)) {
      //p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      pCI->Early.m_pNeeded_later->vAppendChild(p_expr_clone);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }

    return C_CONTINUE_RECURSION;
  };

  if (! (pCI->G.p_filter = pSub_filter()))
    return;

  if ((pCI->G.p_boolPred = pCI->G.p_filter->pSub_boolPred()))
    pCI->G.p_boolPred->bApplyRecursive_term(bLambda);

}
/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vEarlyProject_root(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->Early.m_pNeeded_later);

  if (pCI->pPrevChild()->bIsCard(CK_o_PROJECT))
    return;

  pCI->G.p_key = pSub_key();
  pCI->G.p_emit = pSub_emit();

  // get the root needed form of ordAgg's projection
  pCI->Early.m_pOP_root = this;
  pCI->Early.m_pOP_leaf = nullptr;
  pCI->Early.m_pNeeded_later  = pCI->G.p_emit->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this ordAgg
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->G_vInsertBefore(p_project_early, this);
  pCI->G_vMainResynch();

  pCI->vRenumberCards();
  pCI->vPrintBoard();


  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  vCorrect_expCol(pCI->Flux.m_pArriving);

 // pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();

 // pCI->vPrintBoard();

  pCI->G.p_emit = pSub_emit();

  pCI->Flux.m_pArriving = pCI->G.p_emit;

 // pCI->vPrintBoard();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* table morphs to indexJoin                                                  */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vMorph_table_To_indexJoin(STORE::CardIter *pCI)
{
  _static_

  pCI->G.p_indexJoin = pCI->G.p_table->pSub_indexJoin();
  pCI->G.p_indexList = pCI->G.p_table->pSub_indexList();

  pCI->G.p_bounds = pCI->G.p_table->pSub_bounds();

  if (! pCI->G.p_bounds) {
    s256 sErrorMsg;
    sErrorMsg.vSprint("No index for table(%c) %s"_Stz,
                      pCI->G.p_table->pSymbol()->cSF_char(),
                      pCI->G.p_table->pSub_tableNm()->szFacet(SF_NONE));
    Error xError(EC_MISSING_JOIN_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

//  pCI->G_vNewMember(CK_o_INDEXJOIN);
//  pCI->G.p_table->vRelegate_table(pCI, pCI->G.p_indexJoin);
  pCI->G.p_indexJoin = _pCard_IndexJoin(pCI->G.p_from->pCard(CK_o_INDEXJOIN));

  pCI->G.p_indexList->vChoose_index_For_indexJoin();
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

  vValidate_mustHave(CK_s_LEVEL, CK_s_TABLE, CK_NONE);

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
