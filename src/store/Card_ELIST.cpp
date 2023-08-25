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
Card_ELIST::Card_ELIST(eCK eCard, i8 iFixedDepth)
: Card_LIST(eCard, iFixedDepth)
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
  UNUSED(pCI);

  Kid xCI(this);
  while (xCI.bEach(CK_EXPR)) {

    Card_RpnCalc* p_rpnCalc = xCI.p_expr()->pSub_rpnCalc();

    if (! p_rpnCalc)
      continue;

    Kid xCI2(p_rpnCalc);
    while ((xCI2.bEach(CK_TERM))) {

      if (xCI2.p_term()->pSymbol()->bIs_Key_Col()) {
        xCI2.p_term()->vBumpKeyOdn();
        xCI2.p_term()->vMark1(MARK_CHANGED);
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

  Kid xCI(this);
  while (xCI.bEach(CK_EXPR)) {

    Card_RpnCalc* p_rpnCalc = xCI.p_expr()->pSub_rpnCalc();

    if (! p_rpnCalc)
      continue;

    Kid xCI2(p_rpnCalc);
    while (xCI2.bEach(CK_TERM)) {
//      Stz *szTerm = xCI2.G.p_term->szSymbol(SF_NONE);

      if (xCI2.p_term()->pSymbol()->bIs_Col_No()) {
        xCI2.p_term()->vBump_stmOdn();
        xCI2.p_term()->vMark1(MARK_CHANGED);
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

  Kid xCI(this);
  while (xCI.bEach(CK_EXPR))
    xCI.p_expr()->vBump_stmOdn(nBumpBy);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_ELIST::pContains(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xCI_this(this);
  while (xCI_this.bEach(CK_EXPR))
    if (xCI_this.p_expr()->bSameAs(p_expr_arg))
      return xCI_this.p_expr();

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vConvertAll_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xCI(this);
  while (xCI.bEach(CK_EXPR))
    xCI.p_expr()->vConvertAll_stmOdn_To_colOdn(pCI);
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
    if (p_as && p_as->bSameSymbol(pSymbol))
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  RASSERT(2 == iDepth);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
