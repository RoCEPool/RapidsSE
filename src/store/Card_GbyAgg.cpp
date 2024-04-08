/*============================================================================*/
/* Card_GbyAgg.cpp -- Aggreg-GroupBy                                          */
/*============================================================================*/

#include "Card_GbyAgg.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_GbyAgg, CK_o_GBYAGG)
__aNewFunction(Card_GbyAgg, CN_CARD)
__vDeleteFunction(Card_GbyAgg, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_GbyAgg::Card_GbyAgg(void)
: Card_AGGREGATE(CK_o_GBYAGG, 1)
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
/* create a new <gbyAgg>/<projection>/<key> by copying <gbyAgg>/<key>         */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vDerive_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bPeg_Projection(pCI));
  RASSERT(bPeg_Key(pCI));
  pCI->E.p_emit->vDelete_streamKey(pCI);

  pCI->StreamOrder.m_pStreamKey = Card_StreamKey::pTo_streamKey(pCI->E.p_key);
  pCI->E.p_emit->vAppendChild(pCI->StreamOrder.m_pStreamKey);


  // set running projection for use by next op
  pCI->StreamOrder.m_pEmitted = pCI->E.p_emit;
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

  if (pCI->pPeekPrev()->bIsCard(CK_o_PROJECT)) {

    pCI->Early.m_pOP_leaf = this;
    pCI->Dic.m_p_expr = pCI->Flux.m_pArriving = pSub_emit();

   // pCI->PRINTBOARD_dbg();
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->E.p_key = pSub_key();
  pCI->E.p_emit = pSub_emit();

  // get the root needed form of gbyAgg's projection
  pCI->Early.m_pOP_root = (Card_OP*)this;
  if (1 == m_i_Child)
    vDebugPoint();

  pCI->Early.m_pNeeded_later  = pCI->E.p_emit->pCraftRoot_needed(pCI);

  // insert empty early projection (has only needed) before this gbyAgg
  Card_Project* p_project_early = pCI->Early.m_pNeeded_later->pNew_project_early(pCI);
  pCI->vInsertBefore(p_project_early, this);
  pCI->vResynchOp();

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  RASSERT(pCI->Early.m_pOP_leaf);

  pCI->E.p_gbyAgg->vRestate_emit(pCI);

 // pCI->PRINTBOARD_dbg();
}

/*----------------------------------------------------------------------------*/
/* craft needed                                                               */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vCraft_needed_root(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->Early.m_pNeeded_later = pCI->E.p_emit->pCraft_needed_root(pCI);
  DELETE_CARD(pCI->Early.m_pNeeded_later);
}

/*----------------------------------------------------------------------------*/
/* inject sub-query result into this gbyAgg                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vInject_foldExpr(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->E.p_key = pCI->E.p_gbyAgg->pSub_key();
//Z_KEY_DIRECTION
  for (pCI->vBeginTarget(pCI->E.p_key,CK_EXPR); pCI->bEachTarget(); /**/) {
    Card_Expr* p_expr = pCI->E.xTarget.p_expr();
    for (pCI->vBeginTarget(p_expr,CK_TERM); pCI->bEachTarget(); /**/) {
      Card_Term* p_term = pCI->E.xTarget.p_term();
      if (p_term->pSymbol()->bHasFacet(SF_EXP_COL)) {
        p_term->vBump_ordCol(SF_EXP_COL);
        p_term->vMark1(MARK_CHANGED);
       }
    }
  }


  pCI->E.p_emit = pCI->E.p_gbyAgg->pSub_emit();
  pCI->E.p_emit->vBump_KeyCol_In_rpnCalc(pCI);
  pCI->E.p_emit->vBump_expCol_In_rpnCalc(pCI);

  pCI->E.p_emit->vPush_foldExpr_Into_emit(pCI, pCI->Flat.m_pExpr_fold);
  pCI->E.p_key->vAdd_key_expr_BasedOn_expr(pCI,pCI->Flat.m_pExpr_fold,true,false);

  pCI->E.p_key->vRenumberAll_expr(SF_KEY_COL);
  pCI->E.p_emit->vRenumberAll_expr();
}

/*----------------------------------------------------------------------------*/
/* replace gbyAgg by ordAgg                                                   */
/*----------------------------------------------------------------------------*/
bool Card_GbyAgg::bMorph_gbyAgg_To_ordAgg(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! (pCI->E.p_level = pSub_level()) )
    return false;

  if (AL_PARTITION != pCI->E.p_level->pSymbol()->Y_iGet(P_Level))
    return false;

  if (! pCI->StreamOrder.m_pStreamKey)
    return false;

  pCI->E.p_key = pSub_key();

  if (! pCI->E.p_key->bIsEqual(pCI,pCI->StreamOrder.m_pStreamKey))
    return false;

  vCard(CK_o_ORDAGG);

  vDebugPoint();

  return true;
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

 // pCI->PRINTBOARD_dbg();

  pCI->E.p_gbyAgg->pSymbol()->Y_vSet(P_Smoothed);

  if (pCarryForward->GbyAgg.m_bInject_foldExpr) {
    pCI->E.p_gbyAgg->vInject_foldExpr(pCI);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_s_LEVEL, CK_s_KEY, CK_s_STREAM, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
