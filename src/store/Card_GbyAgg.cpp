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
  pCI->StreamOrder.m_pProjection_running = pCI->G.p_projection;
  pCI->StreamOrder.m_pKey_running = pCI->G.p_key;
}

/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_GbyAgg::vEarly_project_root(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(bIsPartitionLevel());

  RASSERT(! pCI->EarlyProject.m_pNeeded);

  pCI->EarlyProject.m_pOP_root = (Card_OP*)this;

  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  // get the needed form of ordAgg's projection
  pCI->EarlyProject.m_pNeeded =
      pCI->G.p_projection->pCraftRoot_needed(pCI);

  pCI->vPrintBoard();

  Card_OP::vEarly_project_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

  pCI->vPrintBoard();

  pCI->G.p_key = pSub_key();
  pCI->G.p_key->vFlipTerms(pCI->EarlyProject.m_pNeeded);

  pCI->vPrintBoard();

  pCI->G.p_projection = pSub_projection();
  pCI->G.p_projection->vFlipTerms(pCI->EarlyProject.m_pNeeded);

  pCI->ProjectionAnalysis.m_pProjection_running = pCI->G.p_projection;

  pCI->vPrintBoard();

  vDebugPoint();
}
/*----------------------------------------------------------------------------*/
//void Card_GbyAgg::vEarly_project_root(CardIter* pCI)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  RASSERT(bIsPartitionLevel());
//
//  RASSERT(! pCI->EarlyProject.m_pProjection_needed);
//
//  pCI->EarlyProject.m_pOP_root = (Card_OP*)this;
//
//  pCI->G.p_key = pSub_key();
//
//  // get the needed form of ordAgg's projection
//  pCI->EarlyProject.m_pProjection_needed =
//      pSub_projection()->pCraftRoot_needed(pCI);
//
//  Card_OP::vEarly_project_backstep(pCI);
//
//  // todo: arrived back ...
//  pCI->vPrintBoard();
//
//  pCI->G.p_key = pSub_key();
//  pCI->G.p_key->vFlipTerms(pCI->EarlyProject.m_pProjection_needed);
//
//  pCI->vPrintBoard();
//
//  pCI->G.p_projection = pSub_projection();
//  pCI->G.p_projection->vFlipTerms(pCI->EarlyProject.m_pProjection_needed);
//
//  pCI->ProjectionAnalysis.m_pProjection_running = pCI->G.p_projection;
//
//  pCI->vPrintBoard();
//
//}

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

  pCI->G.p_gbyAgg->Y_vSet(P_Smoothed);

  if (bInject_foldExpr) {
    pCI->G.p_gbyAgg->vInject_foldExpr(pCI);
  }

  if (nBumpBy) {
    pCI->G.p_gbyAgg->vBump_Rightwards(nBumpBy);
  }

  pCI->ProjectionAnalysis.m_pProjection_running = pSub_projection();

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
