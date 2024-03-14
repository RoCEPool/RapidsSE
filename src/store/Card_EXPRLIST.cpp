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
void Card_EXPRLIST::vBump_KeyCol_In_rpnCalc(CardIter* pCI)
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

      if (xCI2.p_term()->pSymbol()->bHasFacet(SF_KEY_COL)) {
        xCI2.p_term()->vBump_ordCol(SF_KEY_COL);
        xCI2.p_term()->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vBump_expCol_In_rpnCalc(CardIter* )
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR)) {

    Card_RpnCalc* p_rpnCalc = xKid.p_expr()->pSub_rpnCalc();

    if (! p_rpnCalc)
      continue;

    Kid xCI2(p_rpnCalc);
    while (xCI2.bEach(CK_TERM)) {
      if (xCI2.p_term()->pSymbol()->bHasFacet(SF_EXP_COL)) {
        xCI2.p_term()->vBump_ordCol(SF_EXP_COL);
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
    xKid.p_expr()->vBump_expCol(nBumpBy);
}

/*----------------------------------------------------------------------------*/
/* does this expression list contain given term?                              */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pContains(Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xCI_this(this);
  while (xCI_this.bEach(CK_EXPR))
    if (xCI_this.p_expr()->bContains(pSymbol_arg))
      return xCI_this.p_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pContains(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xCI_this(this);
  while (xCI_this.bEach(CK_EXPR))
    if (xCI_this.p_expr()->bMatches(p_expr_arg))
      return xCI_this.p_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return term0 of <expr> with term0 matching given $n stream-column          */
/*----------------------------------------------------------------------------*/
Card_Term* Card_EXPRLIST::pDecode_expr_term0(Symbol* pSymbol_expCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(pSymbol_expCol->bHasFacet(SF_EXP_COL));

  i8 i_Child  = pSymbol_expCol->iTo_ordinal(SF_EXP_COL) - 1;

  if (Card_Expr* p_expr_expCol = pGet_expr(i_Child)) {
    if (p_expr_expCol->bIsScalarCol()) {
      Card_Term *p_term0 = p_expr_expCol->pGet_term(0);
      return p_term0;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR))
    xKid.p_expr()->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
}

/*----------------------------------------------------------------------------*/
/* ensure each expression has a SF_NMD_COL                                    */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vEnsure_expr_Has_nmdCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR))
    xKid.p_expr()->vEnsure_expr_Has_nmdCol(xKid.i_Child());
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
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Matches(Symbol *pSymbol_arg)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (xKid.p_expr()->pSymbol()->bMatches(pSymbol_arg))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Matches(eSF eFacet_arg, Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (xKid.p_expr()->pSymbol()->bMatches(eFacet_arg, pSymbol_arg))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expression which expresses the given symbol                           */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Matches(Symbol* pSymbol_arg, Kid* aKid)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  while (aKid->bEach(CK_EXPR)) {
    if (aKid->p_expr()->pSymbol()->bMatches(pSymbol_arg))
      return aKid->p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Matches_ignoring(Symbol *pSymbol_arg, eSF eFacet_ignore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (xKid.p_expr()->pSymbol()->bMatches_ignoring(pSymbol_arg, eFacet_ignore))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a particular expr                                                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pGet_expr(i8 i_Expr)
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

  bool bIsKeyExpr = bIsCard(CK_s_KEY);

  i8 i_Expr = 1;
  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (bIsKeyExpr)
      xKid.p_expr()->vRenumberForm_keyCol(i_Expr++);
    else
      xKid.p_expr()->vRenumberForm_expCol(i_Expr++);
  }
}

/*----------------------------------------------------------------------------*/
/* same ?                                                                     */
/*----------------------------------------------------------------------------*/
bool Card_EXPRLIST::bSame(Card_EXPRLIST* p_EXPRLIST_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nExpr = this->nChildCards();
  if (nExpr != p_EXPRLIST_arg->nChildCards())
    return false;

  for (i8 i_Expr = 0; i_Expr < nExpr; i_Expr++) {
    Card_Expr* p_expr_this = pGet_expr(i_Expr);
    Card_Expr* p_expr_arg  = p_EXPRLIST_arg->pGet_expr(i_Expr);

    if (! p_expr_this->bSameAs(p_expr_arg))
      return false;

    if (Card_Direction* p_direction_this = p_expr_this->pSub_direction()) {
      if (Card_Direction *p_direction_arg = p_expr_arg->pSub_direction()) {
        if (!p_direction_this->pSymbol()->bMatches(p_direction_arg->pSymbol()))
          return false;
      }
      else {
        return false;
      }
    }
    else
    if (p_expr_arg->pSub_direction()) {
      return false;
    }

  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szSep = ""_Stz;
  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    xKid.p_expr()->vShortForm(pString,szSep);
    szSep = ","_Stz;
  }

}

/*----------------------------------------------------------------------------*/
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
bool Card_EXPRLIST::bTestFind_expr_Matching(Card_Expr* p_expr_arg,
                                            Symbol *pSymbol_arg, eSF eFacet_ignore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (xKid.p_expr()->pSymbol()->bMatches_ignoring(pSymbol_arg, eFacet_ignore) &&
        xKid.p_expr() == p_expr_arg)
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* convert to streamDef                                                       */
/*----------------------------------------------------------------------------*/
StreamDef* Card_EXPRLIST::pToStreamDef(ColMap* pColMap,
                                       Card_Key* p_key,
                                       Card* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  StreamDef* pStreamDef = new StreamDef;

  s256 sColNm;


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_o_GBYAGG == p_OP->eCard()) {

    for (Kid xExprList(this); xExprList.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xExprList.p_expr();

      //p_expr->vDump(__func__,"GBYAGG"_Stz);
      sColNm = p_expr->szFacet(SF_ITM_COL);

      Cast xCast = p_expr->xCast();
      pStreamDef->vAddColumn(sColNm,
                             xCast.m_xTypeId,
                             xCast.m_nTypeLength,
                             xCast.m_nTypeScale,
                             xCast.m_bTypeNullable);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_o_SORT == p_OP->eCard()) {

    for (Kid xExprList(this); xExprList.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xExprList.p_expr();

      sColNm = p_expr->szFacet(SF_ITM_COL);

      Cast xCast = p_expr->xCast();
      pStreamDef->vAddColumn(sColNm,
                             xCast.m_xTypeId,
                             xCast.m_nTypeLength,
                             xCast.m_nTypeScale,
                             xCast.m_bTypeNullable);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_o_TOP == p_OP->eCard()) {

    for (Kid xExprList(this); xExprList.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xExprList.p_expr();

      sColNm = p_expr->szFacet(SF_ITM_COL);

      Cast xCast = p_expr->xCast();
      pStreamDef->vAddColumn(sColNm,
                             xCast.m_xTypeId,
                             xCast.m_nTypeLength,
                             xCast.m_nTypeScale,
                             xCast.m_bTypeNullable);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_EmittedCol = 1;
  UNUSED(i_EmittedCol);
  if (CK_o_PROJECT == p_OP->eCard()) {
    for (Kid xExprList(this); xExprList.bEach(CK_EXPR); i_EmittedCol++) {
      Card_Expr *p_expr = xExprList.p_expr();

      sColNm = p_expr->szFacet(SF_ITM_COL);

      Cast xCast = p_expr->xCast();
      pStreamDef->vAddColumn(sColNm,
                             xCast.m_xTypeId,
                             xCast.m_nTypeLength,
                             xCast.m_nTypeScale,
                             xCast.m_bTypeNullable);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  i_EmittedCol = 1;
  if (CK_o_SCAN == p_OP->eCard()) {
    for (Kid xExprList(this); xExprList.bEach(CK_EXPR); i_EmittedCol++) {
      Card_Expr *p_expr = xExprList.p_expr();

      sColNm = p_expr->szFacet(SF_ITM_COL);

      Cast xCast = p_expr->xCast();
      pStreamDef->vAddColumn(sColNm,
                             xCast.m_xTypeId,
                             xCast.m_nTypeLength,
                             xCast.m_nTypeScale,
                             xCast.m_bTypeNullable);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(CK_o_GBYAGG != p_OP->eCard());
  i_EmittedCol = 1;

  for (Kid xExprList(this); xExprList.bEach(CK_EXPR); i_EmittedCol++) {
    Card_Expr *p_expr = xExprList.p_expr();

    if (CK_o_PROJECT == p_OP->eCard()) {
      sColNm = p_expr->szFacet(SF_ITM_COL);

    }
    else
    if (CK_o_SORT == p_OP->eCard()) {
      sColNm = p_expr->szFacet(SF_ITM_COL);
    }
    else {
      sColNm = p_expr->szFacet(SF_ITM_COL);
    }


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream def column

		Cast xCast = p_expr->xCast();

    pStreamDef->vAddColumn(sColNm,
                           xCast.m_xTypeId,
                           xCast.m_nTypeLength,
                           xCast.m_nTypeScale,
                           xCast.m_bTypeNullable);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // col map (gbyagg) column

    if (pColMap) {
      RASSERT(p_key);

      if (p_expr->pSymbol()->Y_bHas(P_Key))
        pColMap->vAdd(p_expr->pSymbol()->Y_iGet(P_EmittedCol),
                      false,
                      p_expr->pSymbol()->Y_iGet(P_Cell));
      else
        pColMap->vAdd(p_expr->pSymbol()->Y_iGet(P_EmittedCol),
                      true,
                      p_expr->pSymbol()->Y_iGet(P_Cell));
    }
  }

  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* update the casts of this exprList                                          */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vUpdate_cast(Card_EXPRLIST *p_exprList_ref)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vDump(__func__,"vUpdate_cast"_Stz);
  p_exprList_ref->vDump(__func__,"p_exprList_ref"_Stz);

  for (Kid xKid(this); xKid.bEach(); /**/) {
    Card_Expr* p_expr_this = xKid.p_expr();
    if (!p_expr_this->bIsScalarCol())
      continue;

    Symbol* pSymbol = p_expr_this->pSymbol();

    Card_Expr* p_expr_ref = p_exprList_ref->pFind_expr_Matches(pSymbol);

    if (p_expr_ref) {
      p_expr_this->vApplyCast_x(p_expr_ref->xCast());
      p_expr_this->pSub_rpnCalc()->vApplyCast_x(p_expr_ref->xCast());
      p_expr_this->pGet_term(0)->vApplyCast_x(p_expr_ref->xCast());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* convert stmCol to tblCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vX_expCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR)) {
    pCI->G.p_expr = xKid.p_expr();
    pCI->G.p_expr->vX_expCol_To_tblCol(pCI);
  }

  pCI->G.p_expr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR)) {
    pCI->G.p_expr = xKid.p_expr();
    pCI->G.p_expr->vX_tblCol_To_expCol(pCI);
  }

  pCI->G.p_expr = nullptr;
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
