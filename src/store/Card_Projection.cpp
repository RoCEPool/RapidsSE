/*============================================================================*/
/* Card_Projection.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Projection.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Projection, CK_PROJECTION)
__aNewFunction(Card_Projection, CN_CARD)
__vDeleteFunction(Card_Projection, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Projection::Card_Projection(void)
: Card_ELIST(CK_PROJECTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Projection::~Card_Projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* projection analysis                                                        */
/*----------------------------------------------------------------------------*/
void Card_Projection::vProjection_Analysis(CardIter* pCI)
{
  _static_

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_TABLE))
    if ((pCI->G.p_projection = pCI->G.p_table->pSub_projection()))
      pCI->G.p_table->vRemoveDeleteChild(pCI->G.p_projection);


  pCI->vRestart();
  pCI->G.p_projection = nullptr;
  while (pCI->G_bEachMain(CK_TABLE))
    pCI->G.p_table->vProjectionAnalysis(pCI);

}

/*----------------------------------------------------------------------------*/
/* projection analysis (post fold)                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vProjection_Analysis_postFold(CardIter* pCI)
{
  _static_

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_TABLE))
    if ((pCI->G.p_projection = pCI->G.p_table->pSub_projection()))
      pCI->G.p_table->vRemoveDeleteChild(pCI->G.p_projection);

  bool bPostFold = false;

  pCI->vRestart();
  pCI->G.p_projection = nullptr;
  while (pCI->G_bEachMain()) {
    if (CK_FOLD == pCI->G.e_MainCard) {
      bPostFold = true;
      continue;
    }

    if (CK_TABLE == pCI->G.e_MainCard) {
      pCI->G.p_table->vProjectionAnalysis(pCI);
      continue;
    }

    if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection()))
      pCI->G.p_projection->vNumberExpr();
  }
}

/*----------------------------------------------------------------------------*/
/* relabel expressions                                                        */
/*----------------------------------------------------------------------------*/
void Card_Projection::vRelabelExpressions(CardIter* pCI)
{
  _static_

  i8 i_Expr;
  s32 sLabel;

  pCI->G_vRestart();
  while (pCI->G_bEachMainWith2Sub(CK_GBYAGG, CK_KEY, CK_PROJECTION)) {

    i_Expr = 0;
    CardIter xCI_g(pCI->G.p_key);
    while ((pCI->G.p_expr = _pCard_Expr(xCI_g.pEachChild()))) {
      sLabel.vSprint("$K%lld"_Stz,++i_Expr);
      pCI->G.p_expr->vValue(sLabel.szStz());
    }

    i_Expr = 0;
    CardIter xCI_p(pCI->G.p_projection);
    while ((pCI->G.p_expr = _pCard_Expr(xCI_p.pEachChild()))) {
      sLabel.vSprint("$%lld"_Stz,++i_Expr);
      pCI->G.p_expr->vValue(sLabel.szStz());
    }

  }

  i8 i_StmCol;

  pCI->G_vRestart();
  while (pCI->G_bEachMain()) {
    if (! (pCI->G.p_stream = pCI->G.p_MainCard->pSub_stream()))
      continue;

    i_StmCol = pCI->G.p_stream->Y_iGet(P_OdnLo);

    pCI->G.p_stmColList = pCI->G.p_stream->pSub_stmColList();

    CardIter xCI_s(pCI->G.p_stmColList);
    while ((pCI->G.p_stmCol = _pCard_StmCol(xCI_s.pEachChild()))) {
      sLabel.vSprint("$%lld"_Stz,i_StmCol++);
      pCI->G.p_stmCol->vValue(sLabel.szStz());
    }

  }

}

/*----------------------------------------------------------------------------*/
/* resolve (right-left) projection prior to main card                         */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pResolvePriorProjection(CardIter* pCI)
{
  _static_

  CardIter xCI(pCI);
  xCI.vSet(pCI->G.i_MainCard,pCI->G.p_MainCard);

  while (Card* p_card = xCI.pEachChild_reverse()) {
    if (Card_Projection* p_projection = p_card->pSub_projection())
      return p_projection;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* re-projection                                                              */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pReProjection(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return nullptr;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
