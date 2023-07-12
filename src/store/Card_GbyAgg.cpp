/*============================================================================*/
/* Card_GbyAgg.cpp -- Aggreg-GroupBy                                          */
/*============================================================================*/

#include "Card_GbyAgg.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_GbyAgg, CK_GBYAGG)
__aNewFunction(Card_GbyAgg, CN_CARD)
__vDeleteFunction(Card_GbyAgg, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_GbyAgg::Card_GbyAgg(void)
: Card_AGG(CK_GBYAGG)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_GbyAgg::~Card_GbyAgg(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* as review                                                                  */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vAs_Review(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  pCI->G.p_projection->vReviewAs(pCI);
}

/*----------------------------------------------------------------------------*/
/* bump key & projection of this gbyAgg rightwards by given amount            */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_Key* p_key = pSub_key())
    p_key->vBump_Rightwards(nBumpBy);

  if (Card_Projection* p_projection = pSub_projection())
    p_projection->vBump_Rightwards(nBumpBy);
}

/*----------------------------------------------------------------------------*/
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vColumn_Mapping(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 i_Cell_key;
  i8 i_Cell_payload;
  i8 nPayload = 0;

  i8 i_Emitted = 1;

  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  for (Kid xKey(pCI->G.p_key); xKey.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr_key = xKey.p_expr();
    Symbol* pSymbol_key = p_expr_key->pSymbol();

    Card_Expr* p_expr_projection = pCI->G.p_projection->pDecodeExpr(pSymbol_key);
    if (! p_expr_projection)
      continue;

    i_Cell_key = pSymbol_key->iExtractOrdinal();

    p_expr_projection->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
    p_expr_projection->pSymbol()->Y_vSet(P_Key);
    p_expr_projection->pSymbol()->Y_vSet(P_Cell, i_Cell_key);

  }

}
/*----------------------------------------------------------------------------*/
//void Card_GbyAgg::vColumn_Mapping(CardIter *pCI)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  i8 i_Cell_key;
//  i8 i_Cell_payload;
//  i8 nPayload = 0;
//
//  i8 i_Emitted = 1;
//
//  pCI->G.p_key = pSub_key();
//  pCI->G.p_projection = pSub_projection();
//
//  for (Kid xProjection(pCI->G.p_projection); xProjection.bEach(CK_EXPR); /**/) {
//    Card_Expr* p_expr = xProjection.p_expr();
//
//    if (p_expr->bIsExpressionOfKey()) {
//
//      Symbol* pSymbol_key = p_expr->pFirstKeyCol();
////      Card_Expr* p_expr_key = pCI->G.p_key->pDecodeExpr(pSymbol_key);
////      Symbol* pSymbol_key_stmCol = p_expr_key->pFirstStmCol();
////      i_Cell_key = pSymbol_key_stmCol->iExtractOrdinal();
//      i_Cell_key = pSymbol_key->iExtractOrdinal();
//
//      p_expr->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
//      p_expr->pSymbol()->Y_vSet(P_Key);
//      p_expr->pSymbol()->Y_vSet(P_Cell, i_Cell_key);
//
//    }
//    else {
//
//      if (Symbol* pSymbol_payload_stmCol = p_expr->pFirstStmCol())
//        i_Cell_payload = ++nPayload; //p_expr->pSymbol()->iExtractOrdinal();
//      else
//        i_Cell_payload = -1;
//
//      p_expr->pSymbol()->Y_vSet(P_EmittedCol, i_Emitted++);
//      p_expr->pSymbol()->Y_vSet(P_Payload);
//      p_expr->pSymbol()->Y_vSet(P_Cell, i_Cell_payload);
//    }
//  }
//
//}

/*----------------------------------------------------------------------------*/
/* convert terms which are stmCol to tblCol                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_projection = pSub_projection()))
    pCI->G.p_projection->vConvertAll_stmOdn_To_colOdn(pCI);

  if ((pCI->G.p_key = pSub_key()))
    pCI->G.p_key->vConvertAll_stmOdn_To_colOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* create a new <gbyAgg>/<projection>/<key> by copying <gbyAgg>/<key>         */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bPeg_Projection(pCI));
  pCI->G.p_projection->vDeletePrjKey(pCI);

  RASSERT(bPeg_Key(pCI));

  pCI->G.p_key = _pCard_Key(pCI->G.p_key->pDupCardRecursive());
  pCI->G.p_projection->vAppend(pCI->G.p_key);

  // set running projection for use by next op
  pCI->StreamOrder.m_pProjection_running = pCI->G.p_projection;
  pCI->StreamOrder.m_pKey_running = pCI->G.p_key;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vEarlyProject_root(CardIter* pCI)
{
  //vDump(0,(Stz*)__func__);

  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->Early.m_pNeeded_later);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // if the immediate previous card is a project then rely on that project
  //   to have determined what projections are needed

  if (pCI->pPrevChild()->bIsCard(CK_PROJECT)) {

    pCI->Early.m_pOP_leaf = this;
    pCI->Analysis.m_pProjection_prior = pSub_projection();

    pCI->vPrintBoard();
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pSymbol()->Y_vSet(P_BreakPt);

  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  // get the root needed form of gbyAgg's projection
  pCI->Early.m_pOP_root = (Card_OP*)this;
  pCI->Early.m_pNeeded_later  = pCI->G.p_projection->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this gbyAgg
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->G_vInsertBefore(p_project_early, this);
  pCI->G_vMainResynch();

  pCI->vRenumberCards();
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
/* inject sub-query result into this gbyAgg                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vInject_foldExpr(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_key = pCI->G.p_gbyAgg->pSub_key();
  pCI->G.p_key->vBump_stmOdn_In_rpnCalc(pCI);

  pCI->G.p_projection = pCI->G.p_gbyAgg->pSub_projection();
  pCI->G.p_projection->vBump_keyOdn_In_rpnCalc(pCI);
  pCI->G.p_projection->vBump_stmOdn_In_rpnCalc(pCI);

  Card_FoldElist::vPush_foldExpr_Into_gbyAgg_projection(pCI,pCI->Sqy.m_pExpr_fold);
  Card_FoldElist::vPush_foldExpr_Into_gbyAgg_key(pCI,pCI->Sqy.m_pExpr_fold);

  pCI->G.p_key->vRenumberAll_expr();
  pCI->G.p_projection->vRenumberAll_expr();
}

/*----------------------------------------------------------------------------*/
/* promote gbyAgg to ordAgg                                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vPromote_gbyAgg_To_ordAgg(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! (pCI->G.p_level = pSub_level()) )
    return;

  if (AL_PARTITION != pCI->G.p_level->pSymbol()->Y_iGet(P_Level))
    return;

  if (! pCI->StreamOrder.m_pKey_running)
    return;

  pCI->G.p_key = pSub_key();

  if (! pCI->G.p_key->bIsEqual(pCI->StreamOrder.m_pKey_running))
    return;

  vCard(CK_ORDAGG);

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vSmooth(CardIter* pCI, bool bInject_foldExpr, i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_gbyAgg->pSymbol()->Y_vSet(P_Smoothed);

  if (bInject_foldExpr) {
    pCI->G.p_gbyAgg->vInject_foldExpr(pCI);
  }

  if (nBumpBy) {
    pCI->G.p_gbyAgg->vBump_Rightwards(nBumpBy);
  }

  pCI->Analysis.m_pProjection_prior = pSub_projection();

  pCI->vRenumberCards();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->vPrintBoard(__LINE__);

  pCI->G.p_gbyAgg->pSymbol()->Y_vSet(P_Smoothed);

  if (pCarryForward->GbyAgg.m_bInject_foldExpr) {
    pCI->G.p_gbyAgg->vInject_foldExpr(pCI);
  }

  pCI->vPrintBoard(__LINE__);


  if (pCarryForward->GbyAgg.m_nBumpBy) {
    pCI->G.p_gbyAgg->vBump_Rightwards(pCarryForward->GbyAgg.m_nBumpBy);
    pCarryForward->GbyAgg.m_nBumpBy = 0;
  }

  pCI->vRenumberCards();

  pCI->vPrintBoard(__LINE__);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_key = pCI->G.p_MainCard->pSub_key())) {
    if (pCI->Analysis.m_pProjection_prior) {
      pCI->G.p_key->vTypes_For_stmColOdn(pCI);
      pCI->G.p_key->vRenumberAll_expr();
    }
  }

  if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection())) {

    if (pCI->Analysis.m_pProjection_prior) {
      pCI->G.p_projection->vEnsureTypes_For_stmColOdn(pCI);
      pCI->G.p_projection->vRenumberAll_expr();
    }

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_LEVEL,CK_KEY,CK_PROJECTION,CK_NONE);

  pSub_level()->vValidate(pCI,iDepth+1,this);
  pSub_key()->vValidate(pCI,iDepth+1,this);
  pSub_projection()->vValidate(pCI,iDepth+1,this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
