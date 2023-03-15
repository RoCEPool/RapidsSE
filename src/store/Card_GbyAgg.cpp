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
: Card_Project(CK_GBYAGG)
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

  pCI->G.p_projection->vAs_Review(pCI);
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
/* convert terms which are stmCol to tblCol                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_projection = pSub_projection()))
    pCI->G.p_projection->vConvert_stmColOdn_terms_To_tblColOdn(pCI);

  if ((pCI->G.p_key = pSub_key()))
    pCI->G.p_key->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* create a new <gbyAgg>/<projection>/<key> by copying <gbyAgg>/<key>         */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vDerive_Projection_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bPeg_Projection(pCI));
  pCI->G.p_projection->vDelete_key(pCI);

  RASSERT(bPeg_Key(pCI));

  pCI->G.p_key = _pCard_Key(pCI->G.p_key->pDupCardAndSubCards());
  pCI->G.p_projection->vAppend(pCI->G.p_key);

  // set running projection for use by next op
  pCI->m_pProjection_running = pCI->G.p_projection;
  pCI->m_pKey_running = pCI->G.p_key;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vEarly_project(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! bIsPartitionLevel())
    return;

  RASSERT(! pCI->m_pProjection_carry);

  // get the carry form of gbyAgg's projection
  pCI->m_pProjection_carry =
      pSub_projection()->pToCarry(pSub_key());

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

  Card_FoldElist::vPush_foldExpr_Into_gbyAgg_projection(pCI,pCI->m_pExpr_fold);
  Card_FoldElist::vPush_foldExpr_Into_gbyAgg_key(pCI,pCI->m_pExpr_fold);

  pCI->G.p_key->vRenumberAllExpr();
  pCI->G.p_projection->vRenumberAllExpr();
}

/*----------------------------------------------------------------------------*/
/* promote gbyAgg to ordAgg                                                   */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vPromote_gbyAgg_To_ordAgg(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! (pCI->G.p_level = pSub_level()) )
    return;

  if (AL_PARTITION != pCI->G.p_level->Y_iGet(P_Level))
    return;

  if (! pCI->m_pKey_running)
    return;

  pCI->G.p_key = pSub_key();

  if (! pCI->G.p_key->bIsEqual(pCI->m_pKey_running))
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

  pCI->G.p_gbyAgg->Y_vSet(P_Smoothed);

  if (bInject_foldExpr) {
    pCI->G.p_gbyAgg->vInject_foldExpr(pCI);
  }

  if (nBumpBy) {
    pCI->G.p_gbyAgg->vBump_Rightwards(nBumpBy);
  }

  pCI->G.pDeck->vRenumberRecursive();
}

/*----------------------------------------------------------------------------*/
/* smooth                                                                     */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vSmooth(CardIter* pCI, CarryForward* pCarryForward)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_gbyAgg->Y_vSet(P_Smoothed);

  if (pCarryForward->GbyAgg.m_bInject_foldExpr) {
    pCI->G.p_gbyAgg->vInject_foldExpr(pCI);
  }

  if (pCarryForward->GbyAgg.m_nBumpBy) {
    pCI->G.p_gbyAgg->vBump_Rightwards(pCarryForward->GbyAgg.m_nBumpBy);
    pCarryForward->GbyAgg.m_nBumpBy = 0;
  }

  pCI->G.pDeck->vRenumberRecursive();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
