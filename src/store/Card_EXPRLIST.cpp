/*============================================================================*/
/* Card_EXPRLIST.cpp -- Abstract Expression List                                */
/*============================================================================*/

#include "Card_EXPRLIST.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_EXPRLIST::Card_EXPRLIST(eCK eCard, i8 iFixedDepth)
: Card_LIST(eCard, iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_EXPRLIST::~Card_EXPRLIST(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vBump_keyOdn_In_rpnCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR)) {

    Card_RpnCalc* p_rpnCalc = xKid.p_expr()->pSub_rpnCalc();

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
void Card_EXPRLIST::vBump_stmOdn_In_rpnCalc(CardIter* )
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR)) {

    Card_RpnCalc* p_rpnCalc = xKid.p_expr()->pSub_rpnCalc();

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
void Card_EXPRLIST::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR))
    xKid.p_expr()->vBump_stmOdn(nBumpBy);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pContains(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xCI_this(this);
  while (xCI_this.bEach(CK_EXPR))
    if (xCI_this.p_expr()->bSameAs(p_expr_arg))
      return xCI_this.p_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* convert colOdn to stmOdn                                                   */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vConvert_colOdn_To_stmOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR))
    xKid.p_expr()->vConvert_colOdn_To_stmOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* convert stmOdn to colOdn                                                   */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vConvert_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR))
    xKid.p_expr()->vConvert_stmOdn_To_colOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* get expr relating to term                                                  */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pDecodeExpr(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr = _pCard_Expr(pGetChild_bySym(pSymbol));

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/* get number of expr in this projection                                      */
/*----------------------------------------------------------------------------*/
i8 Card_EXPRLIST::nExpr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nExpr = nItems();

  return nExpr;
}

/*----------------------------------------------------------------------------*/
/* find expression which expresses the given symbol                           */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Of_symbol(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (xKid.p_expr()->bExpressionOfSymbol(pSymbol))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expression which matches given $n ordinal                             */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_By_Ordinal(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(pSymbol->bIs_Col_No());

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (xKid.p_expr()->pSymbol()->bSame(pSymbol))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expr of term                                                          */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_By_as(Symbol *pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    Card_As* p_as = xKid.p_expr()->pSub_as();
    if (p_as && p_as->bSameSymbol(pSymbol))
      return xKid.p_expr();

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expr of term                                                          */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_By_as_stripItem(Symbol *pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Symbol xSymbol_arg_stripped;
  Symbol xSymbol_as_stripped;

  xSymbol_arg_stripped.vSym(pSymbol);
  xSymbol_arg_stripped.vItemStrip();

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    Card_As* p_as = xKid.p_expr()->pSub_as();
    if (! p_as)
      continue;

    xSymbol_as_stripped.vSym(p_as->pSymbol());
    xSymbol_as_stripped.vItemStrip();

    if (xSymbol_as_stripped.bSame(&xSymbol_arg_stripped))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a particular expr                                                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pGetExpr(i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr = _pCard_Expr(pGetChild(i_Expr));

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/* renumber all expr                                                          */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vRenumberAll_expr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bIsKeyExpr = bIsCard(CK_KEY);

  i8 i_Expr = 1;
  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (bIsKeyExpr)
      xKid.p_expr()->vRenumberForm_keyOdn(i_Expr++);
    else
      xKid.p_expr()->vRenumberForm_stmOdn(i_Expr++);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
