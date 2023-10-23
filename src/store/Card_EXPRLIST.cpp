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

      if (xCI2.p_term()->pSymbol()->bIs_Key_Col()) {
        xCI2.p_term()->vBumpKeyCol();
        xCI2.p_term()->vMark1(MARK_CHANGED);
        continue;
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vBump_stmCol_In_rpnCalc(CardIter* )
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR)) {

    Card_RpnCalc* p_rpnCalc = xKid.p_expr()->pSub_rpnCalc();

    if (! p_rpnCalc)
      continue;

    Kid xCI2(p_rpnCalc);
    while (xCI2.bEach(CK_TERM)) {
//      Stz *szTerm = xCI2.G.p_term->szFacet(SF_NONE);

      if (xCI2.p_term()->pSymbol()->bIs_Stm_Col()) {
        xCI2.p_term()->vBump_stmCol();
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
    xKid.p_expr()->vBump_stmCol(nBumpBy);
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
/* return term0 of <expr> with term0 matching given $n stream-column          */
/*----------------------------------------------------------------------------*/
Card_Term* Card_EXPRLIST::pDecode_term0_Same_expr_stmCol(Symbol* pSymbol_stmCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(pSymbol_stmCol->bIs_Stm_Col());

  if (Card_Expr* p_expr_stmCol = pGet_expr(pSymbol_stmCol)) {
    Card_Term* p_term0 = p_expr_stmCol->pGet_term(0);
    return p_term0;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xCI_this(this);
  while (xCI_this.bEach(CK_EXPR))
    xCI_this.p_expr()->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
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
Card_Expr* Card_EXPRLIST::pFind_expr_Same_stmCol(Symbol* pSymbol_stmCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(pSymbol_stmCol->bIs_Stm_Col());

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (xKid.p_expr()->pSymbol()->bSame(pSymbol_stmCol))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expr of term                                                          */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Via_as(Symbol *pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    Card_As* p_as = xKid.p_expr()->pSub_as();
    if (p_as && p_as->bSymbol_Same(pSymbol))
      return xKid.p_expr();

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expr of term                                                          */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Via_as_stripItem(Symbol *pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Symbol xSymbol_arg_stripped;
  Symbol xSymbol_as_stripped;

  xSymbol_arg_stripped.vFacets(pSymbol);
  xSymbol_arg_stripped.vItemStrip();

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    Card_As* p_as = xKid.p_expr()->pSub_as();
    if (! p_as)
      continue;

    xSymbol_as_stripped.vFacets(p_as->pSymbol());
    xSymbol_as_stripped.vItemStrip();

    if (xSymbol_as_stripped.bMatches(&xSymbol_arg_stripped))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expression which expresses the given symbol                           */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Via_term0(Symbol* pSymbol, eSF* aeFacet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    if (xKid.p_expr()->bMatching_term0(pSymbol, aeFacet))
      return xKid.p_expr();
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find expression which expresses the given symbol                           */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pFind_expr_Via_term0(Symbol* pSymbol, Kid* aKid)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  while (aKid->bEach(CK_EXPR)) {
    if (aKid->p_expr()->bMatching_term0(pSymbol))
      return aKid->p_expr();
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
/* get expr relating to term                                                  */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_EXPRLIST::pGet_expr(Symbol* pSymbol_ordinal)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr = _pCard_Expr(pGetChild_bySym(pSymbol_ordinal));

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
      xKid.p_expr()->vRenumberForm_KeyCol(i_Expr++);
    else
      xKid.p_expr()->vRenumberForm_stmCol(i_Expr++);
  }
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
/* convert to streamDef                                                       */
/*----------------------------------------------------------------------------*/
StreamDef* Card_EXPRLIST::pToStreamDef(ColMap* pColMap,
                                         Card_Key* p_key,
                                         Card* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  StreamDef* pStreamDef = new StreamDef;

  s32 sColNm;
  s32 sColNm_mapped;
  eCT eColType;



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_GBYAGG == p_OP->eCard()) {

    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szFacet(SF_NONE);
      else
        sColNm = p_expr->pGet_term(0)->szFacet(SF_NONE);

      p_key->vX();

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_SORT == p_OP->eCard()) {

    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szFacet(SF_NONE);
      else
        sColNm = p_expr->pGet_term(0)->szFacet(SF_NONE);

      p_key->vX();

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_TOP == p_OP->eCard()) {

    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szFacet(SF_NONE);
      else
        sColNm = p_expr->pGet_term(0)->szFacet(SF_NONE);

      p_key->vX();

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_EmittedCol = 1;
  UNUSED(i_EmittedCol);
  if (CK_PROJECT == p_OP->eCard()) {
    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); i_EmittedCol++) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szFacet(SF_NONE);
      else
        sColNm = p_expr->pGet_term(0)->szFacet(SF_NONE);

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);

    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  i_EmittedCol = 1;
  if (CK_SCAN == p_OP->eCard()) {
    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); i_EmittedCol++) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szFacet(SF_NONE);
      else
        sColNm = p_expr->pGet_term(0)->szFacet(SF_NONE);

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(CK_GBYAGG != p_OP->eCard());
  i_EmittedCol = 1;


  for (Kid xProjection(this); xProjection.bEach(CK_EXPR); i_EmittedCol++) {
    Card_Expr *p_expr = xProjection.p_expr();

    if (CK_PROJECT == p_OP->eCard()) {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szFacet(SF_NONE);
      else
        sColNm = p_expr->pGet_term(0)->szFacet(SF_NONE);

      sColNm_mapped = sColNm;

    }
    else
    if (CK_SORT == p_OP->eCard()) {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szFacet(SF_NONE);
      else
        sColNm = p_expr->pGet_term(0)->szFacet(SF_NONE);

      sColNm_mapped = sColNm;

    }
    else {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szFacet(SF_NONE);
      else
        sColNm = p_expr->pGet_term(0)->szFacet(SF_NONE);

      sColNm_mapped = sColNm;
    }


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream def column

    eColType = p_expr->eColumnType();

    pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);

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
/* convert stmCol to tblCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vX_stmCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR))
    xKid.p_expr()->vX_stmCol_To_tblCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_EXPRLIST::vX_tblCol_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_EXPR))
    xKid.p_expr()->vX_tblCol_To_stmCol(pCI);
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
