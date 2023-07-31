/*============================================================================*/
/* Card_ELIST.cpp -- Abstract Expression List                                 */
/*============================================================================*/

#include "Card_ELIST.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ELIST::Card_ELIST(eCK eCard)
: Card_LIST(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ELIST::~Card_ELIST(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vBump_keyOdn_In_rpnCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR)) {

    xCI.G.p_rpnCalc = xCI.G.p_expr->pSub_rpnCalc();

    if (! xCI.G.p_rpnCalc)
      continue;

    CardIter xCI2(xCI.G.p_rpnCalc);
    while ((xCI2.G_bEachMain(CK_TERM))) {
      Stz *szTerm = xCI2.G.p_term->szSymbol();

      if (xCI2.G.p_term->pSymbol()->bIsKeyOrdinal()) {
        xCI2.G.p_term->vBumpKeyOdn();
        xCI2.G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vBump_stmOdn_In_rpnCalc(CardIter* )
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR)) {

    xCI.G.p_rpnCalc = xCI.G.p_expr->pSub_rpnCalc();

    if (! xCI.G.p_rpnCalc)
      continue;

    CardIter xCI2(xCI.G.p_rpnCalc);
    while (xCI2.G_bEachMain(CK_TERM)) {
      Stz *szTerm = xCI2.G.p_term->szSymbol();

      if (xCI2.G.p_term->pSymbol()->bIsStreamOrdinal()) {
        xCI2.G.p_term->vBump_stmOdn();
        xCI2.G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR))
    xCI.G.p_expr->vBump_stmOdn(nBumpBy);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_ELIST::pContains(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI_this(this);
  while (xCI_this.G_bEachMain(CK_EXPR))
    if (xCI_this.G.p_expr->bSameAs(p_expr_arg))
      return xCI_this.G.p_expr;

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vConvertAll_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR))
    xCI.G.p_expr->vConvertAll_stmOdn_To_colOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* get expr relating to term                                                  */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_ELIST::pDecodeExpr(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr = _pCard_Expr(pGetChild_bySym(pSymbol));

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/* get number of expr in this projection                                      */
/*----------------------------------------------------------------------------*/
i8 Card_ELIST::nExpr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nExpr = nItems();

  return nExpr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_ELIST::pFindExpr(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nExpr = nChildCards();
  for (i8 i_Expr = 0; i_Expr < nExpr; i_Expr++) {
    if (!pGetChild(i_Expr)->bIsCard(CK_EXPR))
      break;
    Card_Expr* p_expr = pGetExpr(i_Expr);
    if (p_expr->bExpressionOfSymbol(pSymbol))
      return p_expr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expr of term                                                          */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_ELIST::pFind_expr_By_as(STORE::Symbol *pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nExpr = nChildCards();
  for (i8 i_Expr = 0; i_Expr < nExpr; i_Expr++) {
    if (! pGetChild(i_Expr)->bIsCard(CK_EXPR))
      break;

    Card_Expr* p_expr = pGetExpr(i_Expr);

    Card_As* p_as = p_expr->pSub_as();
    if (p_as && p_as->bSameSymbol(pSymbol->szSF__print()))
      return p_expr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expr of term                                                          */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_ELIST::pFind_expr_Of_term(STORE::Symbol *pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nExpr = nChildCards();
  for (i8 i_Expr = 0; i_Expr < nExpr; i_Expr++) {
    if (! pGetChild(i_Expr)->bIsCard(CK_EXPR))
      break;

    Card_Expr* p_expr = pGetExpr(i_Expr);

    if (p_expr->bExpressionOfSymbol(pSymbol))
      return p_expr;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a particular expr                                                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_ELIST::pGetExpr(i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr = _pCard_Expr(pGetChild(i_Expr));

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR)) {
    StzList* pStzList = new StzList;

    pStzList->vAppend(xCI.G.p_expr->szSymbol());

    CardIter xCI_expr(xCI.G.p_expr);
    xCI_expr.G.p_as        = nullptr;
    xCI_expr.G.p_boolCalc  = nullptr;
    xCI_expr.G.p_rpnCalc   = nullptr;

    while (xCI_expr.G_bEachMain()) {

      if ( ! (
          xCI_expr.G.p_MainCard->bIsCard(CK_AS)        ||
          xCI_expr.G.p_MainCard->bIsCard(CK_BOOLCALC)  ||
          xCI_expr.G.p_MainCard->bIsCard(CK_RPNCALC))) {
        vTrace("Unexpected %s"_Stz,xCI_expr.G.p_MainCard->szCardNm());
        NEVER_REACH
      }
    }

    if (xCI_expr.G.p_as) {
      pStzList->vAppend(xCI_expr.G.p_as->szSymbol());
    }
    pRibbon->vSprintDataList(pStzList, 20);
    pStzList->vRemoveAll();

    if (xCI_expr.G.p_rpnCalc) {
      xCI_expr.G.p_rpnCalc->vOntoStzList(pStzList);
      pRibbon->vSprintDataList(pStzList, 20);
      pStzList->vRemoveAll();
    }

    delete pStzList;
  }




  //vPrintCard_bySettings(pRibbon, iDepth);
}

/*----------------------------------------------------------------------------*/
/* renumber all expr                                                          */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vRenumberAll_expr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bIsKeyExpr = bIsCard(CK_KEY);

  i8 i_Expr = 1;
  for (Kid xCI(this); xCI.bEach(CK_EXPR); /**/) {
    if (bIsKeyExpr)
      xCI.p_expr()->vRenumberForm_keyOdn(i_Expr++);
    else
      xCI.p_expr()->vRenumberForm_stmOdn(i_Expr++);
  }
}

/*----------------------------------------------------------------------------*/
/* renumber all synthetic as (return number of synthetic as's)                */
/*----------------------------------------------------------------------------*/
i8 Card_ELIST::nRenumberAll_syntheticAs(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bIsKeyExpr = bIsCard(CK_KEY);

  i8 i_Col = 0;
  for (Kid xCI(this); xCI.bEach(CK_EXPR); /**/) {
    if (Card_As* p_as = xCI.p_expr()->pSub_as())
      if (p_as->pSymbol()->bIsSyntheticAs())
        p_as->pSymbol()->vSetSyntheticAs(++i_Col);
  }

  return i_Col;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
