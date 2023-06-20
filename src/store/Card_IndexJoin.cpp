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
  pCI->G.p_projection->vDeletePrjKey(pCI);

  if (pCI->StreamOrder.m_pKey_running) {
    pCI->G.p_key = _pCard_Key(pCI->StreamOrder.m_pKey_running->pDupCardRecursive());
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
void Card_IndexJoin::vEarlyProject_recurse_add(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pCI->pPrevChild()->bIsCard(CK_GBYAGG) ||
      pCI->pPrevChild()->bIsCard(CK_PROJECT)) {

    pCI->Early.m_pOP_leaf = this;
    pCI->Analysis.m_pProjection_prior = pSub_projection();

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

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard();

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  pCI->vPrintBoard();

  vUpdate_projection(pCI);

  vCorrectStmOdn(pCI->Analysis.m_pProjection_prior);

  pCI->vPrintBoard();

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
void Card_IndexJoin::vEarlyProject_add_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  pCI->G.p_indexList = pSub_indexList();
  pCI->G.p_index     = pCI->G.p_indexList->pFindChosenIndex();
  pCI->G.p_bounds    = pCI->G.p_index->pSub_bounds();
  pCI->G.p_bound     = pCI->G.p_bounds->pFindChosenBound();
  //pCI->G.p_bound->vDump();
  pCI->G.p_term      = pCI->G.p_bound->pGet_colOdn();

  RASSERT(pCI->G.p_term);
  //pCI->G.p_term->vDump();


  if (!pCI->Early.m_pNeeded_later->pFindExpr(pCI->G.p_term->pSymbol())) {

    if (pCI->G.p_term->bIsStmOdn()) {
      Card_Expr* p_expr_stmCol = new Card_Expr;
      Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
      p_rpnCalc->pSymbol()->Y_vSet(P_Type,pCI->G.p_term->pSymbol()->Y_szGet(P_Type));
      p_expr_stmCol->vAppend(p_rpnCalc);
      p_rpnCalc->vAppend(pCI->G.p_term->pDupCardRecursive());
      pCI->Early.m_pNeeded_later->vAppend(p_expr_stmCol);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }
    else
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

  auto bLambda = [pCI] (Card_Term* p_term) {

    if (!p_term->bIsValOdn())
      return C_CONTINUE_RECURSION;

    if (pCI->Early.m_pNeeded_later->pFindExpr(p_term->pSymbol()))
      return C_CONTINUE_RECURSION;

    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(p_term)) {
      //p_expr_defn->vDump();
      Card_Expr* p_expr_clone = p_expr_defn->pClone();
      pCI->Early.m_pNeeded_later->vAppend(p_expr_clone);
      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
    }

    return C_CONTINUE_RECURSION;
  };

  if (! (pCI->G.p_filter = pSub_filter()))
    return;

  CardIter xCI_filter(pCI->G.p_filter);
  while (xCI_filter.G_bEachMain(CK_BOOLCALC)) {

    xCI_filter.G.p_boolCalc->bApplyRecursive_term(bLambda);

  }

}
//void Card_IndexJoin::vEarlyProject_add_transients(CardIter* pCI)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
//
//
//  pCI->G.p_indexList = pSub_indexList();
//  pCI->G.p_index     = pCI->G.p_indexList->pFindChosenIndex();
//  pCI->G.p_bounds    = pCI->G.p_index->pSub_bounds();
//  pCI->G.p_bound     = pCI->G.p_bounds->pFindChosenBound();
//  //pCI->G.p_bound->vDump();
//  pCI->G.p_term      = pCI->G.p_bound->pGet_colOdn();
//
//  RASSERT(pCI->G.p_term);
//  //pCI->G.p_term->vDump();
//
//
//  if (!pCI->Early.m_pNeeded_later->pFindExpr(pCI->G.p_term->pSymbol())) {
//
//    if (pCI->G.p_term->bIsStmOdn()) {
//      Card_Expr* p_expr_stmCol = new Card_Expr;
//      Card_As* p_as = new Card_As(pCI->Early.m_pNeeded_later->nChildCards());
//      p_expr_stmCol->vAppend(p_as);
//      Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
//      p_rpnCalc->vSetType(pCI->G.p_term);
//      p_expr_stmCol->vAppend(p_rpnCalc);
//      p_expr_stmCol->vSetType(p_rpnCalc);
//      p_rpnCalc->vAppend(pCI->G.p_term->pDupCardRecursive());
//      pCI->Early.m_pNeeded_later->vAppend(p_expr_stmCol);
//      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
//    }
//    else
//    if (Card_Expr* p_expr_defn = pCI->pResolvePrior_expr(pCI->G.p_term)) {
//      //p_expr_defn->vDump();
//      Card_Expr* p_expr_clone = p_expr_defn->pClone();
//      pCI->Early.m_pNeeded_later->vAppend(p_expr_clone);
//      pCI->Early.m_pNeeded_later->vRenumberAll_expr();
//    }
//
//    //pCI->Early.m_pNeeded_later->vDump();
//
//    vDebugPoint();
//  }
//
//  vDebugPoint();
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_IndexJoin::vEarlyProject_root(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->Early.m_pNeeded_later);

  if (pCI->pPrevChild()->bIsCard(CK_PROJECT))
    return;

  pSymbol()->Y_vSet(P_BreakPt);

  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  // get the root needed form of ordAgg's projection
  pCI->Early.m_pOP_root = this;
  pCI->Early.m_pOP_leaf = nullptr;
  pCI->Early.m_pNeeded_later  = pCI->G.p_projection->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this ordAgg
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->G_vInsertBefore(p_project_early, this);
  pCI->G_vMainResynch();

  pCI->G.pDeck->vRenumberRecursive();
  pCI->vPrintBoard();


  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  vCorrectStmOdn(pCI->Analysis.m_pProjection_prior);

  pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();

  pCI->vPrintBoard();

  pCI->G.p_projection = pSub_projection();

  pCI->Analysis.m_pProjection_prior = pCI->G.p_projection;

  pCI->vPrintBoard();

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
                      pCI->G.p_table->cSymbol(),
                      pCI->G.p_table->pSub_tableNm()->szSymbol());
    Error xError(EC_MISSING_JOIN_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
    throw xError;
  }

  pCI->G_vNewMember(CK_INDEXJOIN);
  pCI->G.p_table->vRelegate_table(pCI, pCI->G.p_indexJoin);

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

  vValidate_mustHave(CK_LEVEL,CK_TABLE,CK_INDEXLIST,CK_NONE);

  pSub_level()->vValidate(pCI,iDepth+1,this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
