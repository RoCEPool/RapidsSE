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

  Kid xEXPRLIST(this);
  while (xEXPRLIST.bEach(CK_EXPR)) {

    for (pCI->vBeginTarget(xEXPRLIST.p_expr(),CK_TERM); pCI->bEachTarget(); /**/) {
      pCI->E.p_term = pCI->E.xTarget.p_term();

      if (pCI->E.p_term->pSymbol()->bHasFacet(SF_KEY_COL)) {
        pCI->E.p_term->vBump_ordCol(SF_KEY_COL);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vBump_expCol_In_rpnCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xEXPRLIST(this);
  while (xEXPRLIST.bEach(CK_EXPR)) {

    for (pCI->vBeginTarget(xEXPRLIST.p_expr(),CK_TERM); pCI->bEachTarget(); /**/) {
      if (pCI->E.xTarget.p_term()->pSymbol()->bHasFacet(SF_EXP_COL)) {
        pCI->E.p_term = pCI->E.xTarget.p_term();
        pCI->E.p_term->vBump_ordCol(SF_EXP_COL);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* does this expression list contain given term?                              */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pContains(CardIter* pCI,Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xCI_this(this);
  while (xCI_this.bEach(CK_EXPR))
    if (xCI_this.p_expr()->bContains(pCI,pSymbol_arg))
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
Card_Expr* Card_EXPRLIST::pDecode_expr(Symbol* pSymbol_expCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(pSymbol_expCol->bHasFacet(SF_EXP_COL));

  i8 i_Child  = pSymbol_expCol->iTo_ordinal(SF_EXP_COL) - 1;

  Card_Expr* p_expr_expCol = pGet_expr(i_Child);

  return p_expr_expCol;
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
void Card_EXPRLIST::vDeleteFacetIf(CardIter* pCI, eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xEXPRLIST(this);
  while (xEXPRLIST.bEach(CK_EXPR))
    xEXPRLIST.p_expr()->vDeleteFacetIf(pCI, eFacet, eAlsoFacet, cTblSyn);
}

/*----------------------------------------------------------------------------*/
/* ensure each expression has a SF_NMD_COL                                    */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vEnsure_expr_Has_nmdCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xEXPRLIST(this);
  while (xEXPRLIST.bEach(CK_EXPR))
    xEXPRLIST.p_expr()->vEnsure_expr_Has_nmdCol(xEXPRLIST.i_Child());
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

  for (Kid xEXPRLIST(this); xEXPRLIST.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr = xEXPRLIST.p_expr();

    if (p_expr->pSymbol()->bMatches(pSymbol_arg))
      return p_expr;

    if (pSymbol_arg->bHasFacet(SF_TBL_COL) && p_expr->bIsScalarCol()) {
      Symbol* pSym_tblCol = p_expr->pPeekChild()->pSymbol();
      if (pSym_tblCol->bMatches(SF_TBL_COL,pSymbol_arg))
        return p_expr;
    }

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Matches(eSF eFacet_arg, Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xEXPRLIST(this); xEXPRLIST.bEach(CK_EXPR); /**/) {
    if (xEXPRLIST.p_expr()->pSymbol()->bMatches(eFacet_arg, pSymbol_arg))
      return xEXPRLIST.p_expr();
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
    Card_Expr* p_expr = aKid->p_expr();

    if (p_expr->pSymbol()->bMatches(pSymbol_arg))
      return p_expr;

    if (pSymbol_arg->bHasFacet(SF_TBL_COL) && p_expr->bIsScalarCol()) {
      Symbol* pSym_tblCol = p_expr->pPeekChild()->pSymbol();
      if (pSym_tblCol->bMatches(pSymbol_arg))
        return p_expr;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find <expr> which has same symbol as given $n stream-column                */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Matches_ignoring(Symbol *pSymbol_arg, eSF eFacet_ignore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xEXPRLIST(this); xEXPRLIST.bEach(CK_EXPR); /**/) {
    if (xEXPRLIST.p_expr()->pSymbol()->bMatches_ignoring(pSymbol_arg, eFacet_ignore))
      return xEXPRLIST.p_expr();
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
void Card_EXPRLIST::vRenumberAll_expr(eSF eFacet)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

/**Z_KEY_DIRECTION renumber*/
  for (Kid xEXPRLIST(this); xEXPRLIST.bEach(CK_EXPR); /**/)
    xEXPRLIST.p_expr()->pSymbol()->vApplyFacet(eFacet,xEXPRLIST.i_Child()+1);
}

/*----------------------------------------------------------------------------*/
/* convert to streamDef                                                       */
/*----------------------------------------------------------------------------*/
StreamDef* Card_EXPRLIST::pTo_StreamDef(ColMap* pColMap,
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

      i8 i_Cell       = p_expr->pSymbol()->Y_iGet(P_Cell);
      i8 i_EmittedCol = p_expr->pSymbol()->Y_iGet(P_EmittedCol);
      bool bPayload = ! p_expr->pSymbol()->Y_bHas(P_Key);
      pColMap->vAdd3(bPayload, i_Cell, /* -> */ i_EmittedCol);
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

  for (Kid xEXPRLIST(this); xEXPRLIST.bEach(); /**/) {
    Card_Expr* p_expr_this = xEXPRLIST.p_expr();
    if (!p_expr_this->bIsScalarCol())
      continue;

    Symbol* pSymbol = p_expr_this->pSymbol();

    Card_Expr* p_expr_ref = p_exprList_ref->pFind_expr_Matches(pSymbol);

    if (p_expr_ref) {
      p_expr_this->vApplyCast_x(p_expr_ref->xCast());
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

  Kid xEXPRLIST(this);
  while (xEXPRLIST.bEach(CK_EXPR)) {
    pCI->E.p_expr = xEXPRLIST.p_expr();
    pCI->E.p_expr->vX_expCol_To_tblCol(pCI);
  }

  pCI->E.p_expr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (CK_o_GBYAGG == pCI->E.e_Op) {
    vDebugPoint();
  }

  Kid xEXPRLIST(this);
  while (xEXPRLIST.bEach(CK_EXPR)) {
    pCI->E.p_expr = xEXPRLIST.p_expr();
    pCI->E.p_expr->vX_tblCol_To_expCol(pCI);
  }

  pCI->E.p_expr = nullptr;
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
