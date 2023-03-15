/*============================================================================*/
/* Card_ELIST.cpp -- Abstract Expression List                                 */
/*============================================================================*/

#include "Card_ELIST.hpp"
#include "Card_As.hpp"
#include "Card_StmCol.hpp"
#include "Card_OrdElmOdn.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ELIST::Card_ELIST(eCK eCard)
: Card(eCard)
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
  while ((pCI->G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    pCI->G.p_rpnCalc = pCI->G.p_expr->pSub_rpnCalc();

    if (! pCI->G.p_rpnCalc)
      continue;

    CardIter xCI2(pCI->G.p_rpnCalc);
    while ((pCI->G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = pCI->G.p_term->szValue();

      if (Card::bIsKeyOrdinal(szTerm)) {
        pCI->G.p_term->vBump_KeyOrdinal();
        pCI->G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vBump_stmOdn_In_rpnCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while ((pCI->G.p_expr = _pCard_Expr(xCI.pEachChild()))) {

    pCI->G.p_rpnCalc =pCI->G.p_expr->pSub_rpnCalc();

    if (! pCI->G.p_rpnCalc)
      continue;

    CardIter xCI2(pCI->G.p_rpnCalc);
    while ((pCI->G.p_term = _pCard_Term(xCI2.pEachChild()))) {
      Stz *szTerm = pCI->G.p_term->szValue();

      if (Card::bIsStreamColOrdinal(szTerm)) {
        pCI->G.p_term->vBump_StreamColOrdinal();
        pCI->G.p_term->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_ELIST::iRpnCalc_Has_stmOrdOdn(Card_OrdElmOdn* p_stmOrdOdn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szStrOrdOdn = p_stmOrdOdn->szValue();

  CardIter xCI_elist(this);
  while (Card_Expr* p_expr = _pCard_Expr(xCI_elist.pEachChild())) {

    Card_RpnCalc* p_rpnCalc = p_expr->pSub_rpnCalc();
    if (! p_rpnCalc)
      continue;

    if (p_rpnCalc->nChildCards() > 1)
      continue;

    CardIter xCI_rpnCalc(p_rpnCalc);
    while (Card_Term* p_term = _pCard_Term(xCI_rpnCalc.pEachChild())) {
      if (p_term->bIsValue(szStrOrdOdn))
        return xCI_elist.i_Child();
    }
  }

  return C_UNKNOWN;
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vBump_Rightwards(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR))
    xCI.G.p_expr->vBump_Rightwards(nBumpBy);
}

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR))
    xCI.G.p_expr->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
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
Card_Expr* Card_ELIST::pFindExpr_BasedOn_projectedCol(CardIter* pCI, Stz* szPrjCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  i8 nKeyExpr = nChildCards();
  for (i8 i_KeyExpr = 0; i_KeyExpr < nKeyExpr; i_KeyExpr++) {
    Card_Expr *p_expr_key = pGetExpr(i_KeyExpr);
    Stz* szPrjCol_expr_key = p_expr_key->szProjectedCol();
    if (szPrjCol_expr_key && szPrjCol_expr_key->bSame(szPrjCol))
      return p_expr_key;
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
/* get expr relating to term                                                  */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_ELIST::pGetExpr(Card_Term* p_term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Expr  = p_term->iGetOdnNo() - 1;

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

    pStzList->vAppend(xCI.G.p_expr->szValue());

    CardIter xCI_expr(xCI.G.p_expr);
    xCI_expr.G.p_as        = nullptr;
    xCI_expr.G.p_boolCalc  = nullptr;
    xCI_expr.G.p_colNm     = nullptr;
    xCI_expr.G.p_rpnCalc   = nullptr;
    xCI_expr.G.p_tblColOdn = nullptr;
    xCI_expr.G.p_type      = nullptr;

    while (xCI_expr.G_bEachMain()) {

      if ( ! (
          xCI_expr.G.p_MainCard->bIsCard(CK_AS)        ||
          xCI_expr.G.p_MainCard->bIsCard(CK_BOOLCALC)  ||
          xCI_expr.G.p_MainCard->bIsCard(CK_COLNM)     ||
          xCI_expr.G.p_MainCard->bIsCard(CK_RPNCALC)   ||
          xCI_expr.G.p_MainCard->bIsCard(CK_TBLCOLODN) ||
          xCI_expr.G.p_MainCard->bIsCard(CK_TYPE))) {
        vTrace("Unexpected %s"_Stz,xCI_expr.G.p_MainCard->szCardNm());
        NEVER_REACH
      }
    }

    if (xCI_expr.G.p_as) {
      pStzList->vAppend(xCI_expr.G.p_as->szValue());
    }
    if (xCI_expr.G.p_colNm) {
      if (! xCI_expr.G.p_as)
        pStzList->vAppend(xCI_expr.G.p_colNm->szValue());
    }
//    pRibbon->vSprintList(pStzList, 20);
//    pStzList->vRemoveAll();
//
//    pStzList->vAppend(" "_Stz);
    if (xCI_expr.G.p_tblColOdn) {
      pStzList->vAppend(xCI_expr.G.p_tblColOdn->szValue());
    }
    if (xCI_expr.G.p_type) {
      pStzList->vAppend(xCI_expr.G.p_type->szTypeShort()); //szValue());
    }
    pRibbon->vSprintList(pStzList, 20);
    pStzList->vRemoveAll();

    if (xCI_expr.G.p_rpnCalc) {
      xCI_expr.G.p_rpnCalc->vOntoStzList(pStzList);
      pRibbon->vSprintList(pStzList, 20);
      pStzList->vRemoveAll();
    }

    delete pStzList;
  }




  //vPrintCard_bySettings(pRibbon, iDepth);
}



/*----------------------------------------------------------------------------*/
/* renumber all expr                                                          */
/*----------------------------------------------------------------------------*/
void Card_ELIST::vRenumberAllExpr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bIsKeyExpr = (bIsCard(CK_KEY));

  i8 i_Expr = 1;
  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR))
    xCI.G.p_expr->vRenumber(i_Expr++, bIsKeyExpr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
