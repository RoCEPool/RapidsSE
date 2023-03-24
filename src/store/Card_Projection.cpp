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
/* craft projection for a new early project to be inserted after current op   */
/* from this denoted projection of current op                                 */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pCraft_projection_early(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Projection* p_projection_early = new Card_Projection;

  pCI->EarlyProject.m_pProjection_needed->vDump();

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    xKid.p_expr()->vDump();

    if (! pCI->EarlyProject.m_pProjection_needed->pHasSame(xKid.p_expr()))
      continue;
    if (p_projection_early->pHasSame(xKid.p_expr()))
      continue;

    // denoted expr (of this denoted projection of denoted main op)
    //   is needed and not yet in early projection

    Card_Expr* p_expr_early = xKid.p_expr()->pCraft_expr_early(pCI);
    p_projection_early->vAppend(p_expr_early);
    p_expr_early->vRenumber(p_projection_early->nChildCards());
  }


  p_projection_early->vDump();

  return p_projection_early;
}
/*----------------------------------------------------------------------------*/
//Card_Projection* Card_Projection::pCraft_projection_early(CardIter* pCI)
//{
//  _static_
//
//  Card_Projection* p_projection_early = new Card_Projection;
//
//  pCI->EarlyProject.m_pProjection_needed->vDump();
//
//  CardIter xCI_projectionOfOp(pCI->G.p_projection);
//  while (xCI_projectionOfOp.G_bEachMain(CK_EXPR)) {
//    xCI_projectionOfOp.G.p_expr->vDump();
//
//    if (! pCI->EarlyProject.m_pProjection_needed->pHasSame(xCI_projectionOfOp.G.p_expr))
//      continue;
//    if (p_projection_early->pHasSame(xCI_projectionOfOp.G.p_expr))
//      continue;
//
//    // denoted expr (of this denoted projection of denoted main op)
//    //   is needed and not yet in early projection
//
//    Card_Expr* p_expr_early = xCI_projectionOfOp.G.p_expr->pCraft_expr_early(pCI);
//    p_projection_early->vAppend(p_expr_early);
//    p_expr_early->vRenumber(p_projection_early->nChildCards());
//  }
//
//
//  p_projection_early->vDump();
//
//  return p_projection_early;
//}

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
  pCI->ProjectionAnalysis.m_pProjection_running = pCI->G.p_projection = nullptr;

  while (pCI->G_bEachMain()) {

    if (CK_FOLD == pCI->G.e_MainCard) {
      bPostFold = true;
      continue;
    }

    if (CK_TABLE == pCI->G.e_MainCard) {
      pCI->G.p_table->vProjectionDeduction(pCI);
      continue;
    }

    if (pCI->G.p_MainCard->pSub_projection())
      pCI->ProjectionAnalysis.m_pProjection_running = pCI->G.p_MainCard->pSub_projection();
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

  while (xCI.G_bEachMain_reverse()) {
    if (Card_Projection* p_projection = xCI.G.p_MainCard->pSub_projection())
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
    Card_Term* pTerm_prj = p_expr_prj->pProjectedTerm();
    Card_Expr* p_expr_key = pCI->G.p_key->pDecodeExpr(pTerm_prj);

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
Card_Projection* Card_Projection::pClone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Projection(pDupCardAndSubCards());
}


/*----------------------------------------------------------------------------*/
/* deduce reduced form of this projection needed for crafting early projects  */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pCraftProjectionNeeded(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // this projection will be in a gbyAgg or ordAgg
  RASSERT(pCI->G.p_MainCard->bIsCard(CK_GBYAGG) || pCI->G.p_MainCard->bIsCard(CK_ORDAGG));

  Card_Projection* p_projection_needed = new Card_Projection;
  p_projection_needed->Y_vSet(P_Needed);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) make a copy of the key as start of needed projection

  CardIter xCI_key(pCI->G.p_key);
  while (xCI_key.G_bEachMain(CK_EXPR)) {
    Card_Expr *p_expr_needed = xCI_key.G.p_expr->pClone(true);
    p_projection_needed->vAppend(p_expr_needed);
  }

  p_projection_needed->vRenumberAllExpr();
  p_projection_needed->vDump();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) append the referenced col's in rpnCalc of this projection into needed projection

  CardIter xCI_projection(this);
  while (xCI_projection.G_bEachMainWithSub(CK_EXPR,CK_RPNCALC)) {

    CardIter xCI_rpnCalc(xCI_projection.G.p_rpnCalc);
    while (xCI_rpnCalc.G_bEachMain(CK_TERM)) {
      if (! xCI_rpnCalc.G.p_term->bIsColOdn())
        continue;
      if (p_projection_needed->pFind_expr_By_projectedCol(xCI_rpnCalc.G.p_term->szValue()))
        continue;

      if (Card_Expr *p_expr_col = xCI_rpnCalc.G.p_term->pExpr_For_col(pCI))
        p_projection_needed->vAppend(p_expr_col->pClone(true));
    }
  }
  p_projection_needed->vRenumberAllExpr();
  p_projection_needed->vDump();

  return p_projection_needed;
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
Card_Expr* Card_Projection::pFind_expr_By_projectedCol(Stz* szCol)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pFind_expr_With_projectedCol(Card_Term* p_term_col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(p_term_col->bIsColOdn());

  for (Kid xKid(this); xKid.bEach(); /**/) {
    Card_Expr* p_expr = xKid.p_expr();
    if (p_expr->bHasProjectedCol(p_term_col))
      return p_expr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pHasSame(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI_this(this);
  while (xCI_this.G_bEachMain(CK_EXPR))
    if (xCI_this.G.p_expr->bIsSame(p_expr_arg))
      return xCI_this.G.p_expr;

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

    Card_Expr* p_expr_prjCol = pFindExpr(szPrjCol_runkey);
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
Card_Projection* Card_Projection::pDeduce(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Projection* p_projection_deduced = new Card_Projection;

  CardIter xCI_this(this);
  while (xCI_this.G_bEachMain(CK_EXPR)) {
    Card_Expr* p_expr_deduced = xCI_this.G.p_expr->pDeduce(pCI);
    p_projection_deduced->vAppend(p_expr_deduced);
    p_expr_deduced->vRenumber(p_projection_deduced->nChildCards());
  }

  return p_projection_deduced;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
