/*============================================================================*/
/* Card_Projection.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Projection.hpp"

#include "Card_RpnCalc.hpp"

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
/* projection analysis (post fold)                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vProjection_Analysis_v2(CardIter* pCI)
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
      pCI->G.p_projection->vRenumberAllExpr();
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
/* as review                                                                  */
/*----------------------------------------------------------------------------*/
void Card_Projection::vAs_Review(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nPrjExpr = nChildCards();
  for (i8 i_PrjExpr = 0; i_PrjExpr < nPrjExpr; i_PrjExpr++) {

    // get <gbyAgg>/<projection>/<expr>
    Card_Expr *p_expr_prj = pGetExpr(i_PrjExpr);

    Stz* szPrjCol_prj = p_expr_prj->szProjectedCol();
    if ((! szPrjCol_prj || (! Card::bIsKeyOrdinal(szPrjCol_prj))))
      continue;

    // the <gbyAgg>/<projection>/<expr> is a single projected column

    // get corresponding <gbyAgg>/<key>/<expr>
    Card_Term* pTerm_prj = p_expr_prj->szProjectedTerm();
    Card_Expr* p_expr_key = pCI->G.p_key->pGetExpr(pTerm_prj);

    if (! p_expr_key)
      continue;
    else
    if (p_expr_key->pSub_as()) {

      if (p_expr_prj->pSub_as()) {
        p_expr_key->vRemoveDeleteChild(pCI->G.p_as);
        p_expr_key->vAppend(p_expr_prj->pSub_as()->pDupCardAndSubCards());
      }
      else {
        p_expr_prj->vAppend(p_expr_key->pSub_as()->pDupCardAndSubCards());
      }
    }
    else
    if (p_expr_prj->pSub_as()) {
      p_expr_key->vAppend(p_expr_prj->pSub_as()->pDupCardAndSubCards());
    }

    vDebugPoint();
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vDelete_key(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_key = pSub_key()))
    vRemoveDeleteChild(pCI->G.p_key);

  pCI->G.p_key = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pFind_expr_By_projectedCol(CardIter* pCI, Stz* szCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nExpr = nChildCards();
  for (i8 i_Expr = 0; i_Expr < nExpr; i_Expr++) {
    Card_Expr* p_expr = pGetExpr(i_Expr);
    if (p_expr->bIsProjectedCol(szCol))
      return p_expr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* derive the key projection of this projection (of running key)              */
/*----------------------------------------------------------------------------*/
void Card_Projection::vKeyProjection(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Key* p_key_projection = pCI->G.p_projection->pSub_key();

  if (! pCI->StreamOrder.m_pKey_running)
    return;

  i8 nKeyExpr = pCI->StreamOrder.m_pKey_running->nChildCards();
  for (i8 i_RunKeyExpr = 0; i_RunKeyExpr < nKeyExpr; i_RunKeyExpr++) {
    Card_Expr* p_expr_runkey = pCI->StreamOrder.m_pKey_running->pGetExpr(i_RunKeyExpr);

    Stz* szPrjCol_runkey = p_expr_runkey->szProjectedCol();

    Card_Expr* p_expr_prjCol = pFindExpr_BasedOn_projectedCol(pCI,szPrjCol_runkey);
    if (p_expr_prjCol) {

      // annotation or direction in passing
      if (p_expr_runkey->pSub_direction() && ! p_expr_prjCol->pSub_direction()) {
        p_expr_prjCol->vAppend(p_expr_runkey->pSub_direction()->pDupCardAndSubCards());
      }

      pCI->G.p_key->vAdd_key_expr_BasedOn_expr(p_expr_prjCol,false);
    }

  }
}

/*----------------------------------------------------------------------------*/
/* re-projection                                                              */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pReProjection(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);


  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the carry form of this projection                                      */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pToCarry_v2(CardIter* pCI, Card_Key* p_key)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // 1) make a copy of the key
  Card_Projection* p_projection_carry = new Card_Projection;

  i8 nKeyExpr = p_key->nChildCards();
  for (i8 i_KeyExpr = 0; i_KeyExpr < nKeyExpr; i_KeyExpr++) {
    Card_Expr *p_expr_key = p_key->pGetExpr(i_KeyExpr);

    Card_Expr *p_expr_carry = p_expr_key->pToCarry_key();

    p_projection_carry->vAppend(p_expr_carry);
  }

  p_projection_carry->vRenumberAllExpr();
  p_projection_carry->vDump();

  // 2) append the referenced col's of this projection
  i8 nPrjExpr = nChildCards();
  for (i8 i_PrjExpr = 0; i_PrjExpr < nPrjExpr; i_PrjExpr++) {
    Card *pChild = pGetChild(i_PrjExpr);
    if (! pChild->bIsCard(CK_EXPR))
      break;

    Card_Expr *p_expr_prj = pGetExpr(i_PrjExpr);

    if (Card_RpnCalc* p_rpnCalc = p_expr_prj->pSub_rpnCalc()) {
      i8 nRpnTerm = p_rpnCalc->nChildCards();
      for (i8 i_RpnTerm = 0; i_RpnTerm < nRpnTerm; i_RpnTerm++) {
        Card_Term *p_term_rpn = p_rpnCalc->pGetTerm(i_RpnTerm);

        if (p_term_rpn->bIsColOdn()) {
          if (!p_projection_carry->pFind_expr_By_projectedCol(pCI, p_term_rpn->szValue()))
            if (Card_Expr *p_expr_col = p_term_rpn->pExpr_For_col(pCI))
              p_projection_carry->vAppend(p_expr_col->pDupCardAndSubCards());
        }
      }
    }
  }
  p_projection_carry->vRenumberAllExpr();
  p_projection_carry->vDump();

  return p_projection_carry;
}
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pToCarry(Card_Key* p_key)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // temp: take a copy of this projection
  Card_Projection* p_projection_carry =
      _pCard_Projection(pDupCardAndSubCards());

  return p_projection_carry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
