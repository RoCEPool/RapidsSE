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

  vDeleteSub(CK_p_STREAMKEY);
  pCI->StreamOrder.m_pStreamKey = Card_StreamKey::pTo_streamKey(pCI->E.p_key);
  vAppendChild(pCI->StreamOrder.m_pStreamKey);

  // set running projection for use by next op
  pCI->StreamOrder.m_pEmit = pCI->E.p_emit;
}


/*----------------------------------------------------------------------------*/
/* inject sub-query result into this gbyAgg                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vInject_foldExpr(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->E.p_key = pCI->E.p_gbyAgg->pSub_key();

  pCI->E.p_key->vBump_expCol(pCI);

  pCI->E.p_emit = pCI->E.p_gbyAgg->pSub_emit();
  pCI->E.p_emit->vBump_keyCol(pCI);
  pCI->E.p_emit->vBump_expCol(pCI);

  pCI->E.p_emit->vPush_foldExpr(pCI, pCI->Flat.m_pExpr_fold);
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

  if (!pCI->E.p_key->bEqualTo(pCI, pCI->StreamOrder.m_pStreamKey))
    return false;

  vCard(CK_o_ORDAGG);

  vDebugPoint();

  return true;
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vSmooth(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

 // pCI->PRINTBOARD_dbg();

  pCI->E.p_gbyAgg->pSymbol()->Y_vSet(P_Smoothed);

  if (pCI->Smooth.GbyAgg.m_bInject_foldExpr) {
    pCI->E.p_gbyAgg->vInject_foldExpr(pCI);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_child_mustBe(CK_p_LEVEL, CK_p_KEY, CK_p_EMIT, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  Card_OP::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
