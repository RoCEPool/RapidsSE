/*============================================================================*/
/* Card_Expr.cpp -- Typed Expression                                          */
/*============================================================================*/

#include "Card_Expr.hpp"

#include "Card_As.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolPred.hpp"
#include "CardIter.hpp"
#include "TypeSystem.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Expr,CK_EXPR)
__aNewFunction(Card_Expr,CN_CARD)
__vDeleteFunction(Card_Expr,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(void)
: Card_symbol(CK_EXPR, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Expr::Card_Expr(Symbol* pSymbol)
: Card_symbol(CK_EXPR, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc(pSymbol);
  vAppendChild(p_rpnCalc);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}



/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Expr::~Card_Expr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pGby_expr_From_expr_corr(Card_Expr* pExpr_corr)
{
  _static_

  RASSERT(CT_NONE != pExpr_corr->eCTVal());

  Card_Expr*    p_expr_gby = new Card_Expr;
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = pExpr_corr->pSub_term()->pClone();

  p_expr_gby->vAppendChild(p_rpnCalc);
  p_rpnCalc->vAppendChild(p_term);

  p_rpnCalc->vCast(pExpr_corr->xCast());
  p_term->vCast(pExpr_corr->xCast());
  p_expr_gby->vCast(pExpr_corr->xCast());

  return p_expr_gby;
}

/*----------------------------------------------------------------------------*/
/* form project expression from gby key expression                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_expr_From_Gby_expr(Card_Expr* p_expr_gby, Stz* szAs)
{
  _static_

  RASSERT(CT_NONE != p_expr_gby->eCTVal());

  Symbol xSymbol_Kn(SF_KEY_COL,p_expr_gby->i_Child() + 1);

  Card_Expr*    p_expr    = new Card_Expr;
  p_expr->pSymbol()->vAppendFacet(SF_NMD_COL,szAs);
  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  Card_Term*    p_term    = p_expr_gby->pSub_term()->pClone(&xSymbol_Kn);

  p_expr->vAppendChild(p_rpnCalc);
  p_rpnCalc->vAppendChild(p_term);

  p_rpnCalc->vCast(p_expr_gby->xCast());
  p_term->vCast(p_expr_gby->xCast());
  p_expr->vCast(p_expr_gby->xCast());

  return p_expr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pProj_Simple_expr_Of(Card_Expr* p_expr, i8 i_Col)
{
  _static_

  //p_expr->vDump();

  RASSERT(CT_NONE != p_expr->eCTVal());

  Symbol xSymbol_Dn(SF_EXP_COL, i_Col);

  Card_Expr* p_expr_simple = new Card_Expr;

  p_expr_simple->pSymbol()->vCastFacets(p_expr->pSymbol());

  Card_RpnCalc* p_rpnCalc = new Card_RpnCalc;
  p_rpnCalc->vCast(p_expr->xCast());

  //Card_Term* p_term = p_expr->pSub_term()->pClone(&xSymbol_Dn);
  //p_term->vCast(p_expr->xCast());
  //p_term->vDump();

  Card_Term* p_term = (new Card_Term) -> SYM(SF_EXP_COL,i_Col);

  p_rpnCalc->vAppendChild(p_term);


  p_expr_simple->vAppendChild(p_rpnCalc);
  p_expr_simple->vCast(p_rpnCalc);

  return p_expr_simple;
}

/*----------------------------------------------------------------------------*/
/* craft a casted expr for given column                                       */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr:: pCraft_expr_nmdCol(Column *pColumn, i8 iExpCol,
                                          char cTblSyn, i8 iTblCol)
{
  _static_

  Cast xCast;
  xCast.m_xTypeId       = pColumn->xColumnTypeId();
  xCast.m_nTypeLength   = pColumn->iColumnLength();
  xCast.m_nTypeScale    = pColumn->iColumnScale();
  xCast.m_bTypeNullable = pColumn->bIsNullable();

  Stz* szNmdCol = pColumn->szColNm();

  Card_Expr* p_expr_new = (new Card_Expr) -> SYM(SF_EXP_COL, iExpCol);
  p_expr_new->vCast(xCast);
  p_expr_new->pSymbol()->vAppendFacet(SF_NMD_COL, szNmdCol);

  Card_RpnCalc *p_rpnCalc_new = (new Card_RpnCalc);
  p_rpnCalc_new->vCast(xCast);
  p_expr_new->vAppendChild(p_rpnCalc_new);

  Card_Term *p_term_new = (new Card_Term) -> SYM(SF_TBL_COL, cTblSyn, iTblCol);
  p_term_new->vCast(xCast);
  p_rpnCalc_new->vAppendChild(p_term_new);

  p_expr_new->pSymbol()->vAppendFacets(p_term_new->pSymbol());

  return p_expr_new;
}

/*----------------------------------------------------------------------------*/
/* craft a casted expr for given expCol term                                  */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCast_expr_term_expCol(Card_Term* p_term)
{
  _static_

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vCast(p_term->xCast());

  Card_RpnCalc* p_rpnCalc_new = new Card_RpnCalc;
  p_rpnCalc_new->vCast(p_term->xCast());
  p_expr_new->vAppendChild(p_rpnCalc_new);

  Card_Term *p_term_new = p_term->pClone();
  p_rpnCalc_new->vAppendChild(p_term_new);

  return p_expr_new;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vBump_expCol(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vBump_expCol(nBumpBy, bSkipFoldExpr);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vBump_expCol(nBumpBy, bSkipFoldExpr);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_clone = _pCard_Expr(pDupCardRecursive());

  return p_clone;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bContains(Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    return p_boolPred->bContains(pSymbol_arg);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    return p_rpnCalc->bContains(pSymbol_arg);

  return false;
}

/*----------------------------------------------------------------------------*/
/* append a synthetic SF_NMD_COL                                              */
/*----------------------------------------------------------------------------*/
void Card_Expr::vAppend_nmdCol_synthetic(i8 iExpCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  s256 sColNm_synthetic;
  sColNm_synthetic.vSprint("COL%lld"_Stz,iExpCol);

  pSymbol()->vAppendFacet(SF_NMD_COL,sColNm_synthetic);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
}

/*----------------------------------------------------------------------------*/
/* ensure expression has a SF_NMD_COL                                         */
/*----------------------------------------------------------------------------*/
void Card_Expr::vEnsure_nmdCol(i8 iExpCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_expr = pSymbol();
  if (pSymbol_expr->bHasFacet(SF_NMD_COL))
    return;

  if (! bIsCompound()) {
    Card_Term* p_term0 = pGet_term0();
    if (p_term0->pSymbol()->bHasFacet(SF_NMD_COL)) {
      pSymbol_expr->vAppendFacet(SF_NMD_COL,p_term0->szFacet(SF_NMD_COL));
      return;
    }
  }

  vAppend_nmdCol_synthetic(iExpCol);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vEnsureTypeId_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_rpnCalc = pSub_rpnCalc();

  Symbol* pSymbol_expr = pSymbol();
  RASSERT(xCast().m_xTypeId.bIsValid());
  RASSERT(xCast().m_xTypeId == pCI->G.p_rpnCalc->xCast().m_xTypeId);

  TypeId xTypeId_save = xCast().m_xTypeId;

  pCI->G.p_rpnCalc->vEnsureTypeId_expCol(pCI);



//  if (pSymbol()->xCast().m_xTypeId != pCI->G.p_rpnCalc->xCast().m_xTypeId) {
//    vDump(__func__);
//    pCI->G.p_rpnCalc->vEnsureTypeId_expCol(pCI);
//    vDebugPoint();
//  }

//  if (! pSymbol_expr->xCast().m_xTypeId.bIsValid())
//    vCast(pCI->G.p_rpnCalc->xCast());
//  else
  if (pSymbol_expr->xCast().m_xTypeId == pCI->G.p_rpnCalc->xCast().m_xTypeId &&
      (pSymbol_expr->xCast().m_nTypeLength   != pCI->G.p_rpnCalc->xCast().m_nTypeLength ||
       pSymbol_expr->xCast().m_nTypeScale    != pCI->G.p_rpnCalc->xCast().m_nTypeScale  ||
       pSymbol_expr->xCast().m_bTypeNullable != pCI->G.p_rpnCalc->xCast().m_bTypeNullable))
    vCast(pCI->G.p_rpnCalc->xCast());

  //RASSERT(pSymbol()->xCast().m_xTypeId == pCI->G.p_rpnCalc->xCast().m_xTypeId);

//  if (pCI->G.p_rpnCalc->xCast().m_xTypeId.bIsValid())
//    vCast(pCI->G.p_rpnCalc->xCast());

  RASSERT(xTypeId_save == xCast().m_xTypeId);

  pCI->G.p_rpnCalc = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first stm col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_expCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* p_expCol = pSub_rpnCalc()->pFirstStmCol();

  return p_expCol;
}

/*----------------------------------------------------------------------------*/
/* get first key col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_keyCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* p_keyCol = pSub_rpnCalc()->pFirstKeyCol();

  return p_keyCol;
}

/*----------------------------------------------------------------------------*/
/* get first key col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_nmdCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* p_nmdCol = pSub_rpnCalc()->pFirstNmdCol();

  return p_nmdCol;
}

/*----------------------------------------------------------------------------*/
/* get first tbl col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_tblCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* p_tblCol = pSub_rpnCalc()->pFirstTblCol();

  return p_tblCol;
}

/*----------------------------------------------------------------------------*/
/* get symbol of given term                                                   */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pGet_Symbol(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_i = pSub_rpnCalc()->pGetTerm(i_Term);
  if (! p_term_i)
    return nullptr;

  Symbol* p_symbol_i = p_term_i->pSymbol();

  return p_symbol_i;
}

/*----------------------------------------------------------------------------*/
/* get given term                                                             */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::pGet_term(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_i = pSub_rpnCalc()->pGetTerm(i_Term);

  return p_term_i;
}

/*----------------------------------------------------------------------------*/
/* get term 0                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::pGet_term0(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term0 = pSub_rpnCalc()->pGetTerm(0);

  return p_term0;
}

/*----------------------------------------------------------------------------*/
/* induct this arriving expr                                                  */
/*----------------------------------------------------------------------------*/
Card_Expr * Card_Expr::pInduct(i8 iExp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RpnCalc* p_rpnCalc_arriving = pSub_rpnCalc();

  Card_Expr* p_expr_inducted = new Card_Expr;
  Symbol* pSymbol_expr_inducted = p_expr_inducted->pSymbol();

  //vDump(__func__);
  p_expr_inducted->vCast(this);

  Symbol* pSymbol_expr_arriving = pSymbol();

  RASSERT(pSymbol_expr_arriving->bHasFacet(SF_EXP_COL));

  if (pSymbol_expr_arriving->bHasFacet(SF_NMD_COL))
    pSymbol_expr_inducted->vAppendFacet(SF_NMD_COL,
                                         pSymbol_expr_arriving->szSF_chrSeq(SF_NMD_COL));
  else
    p_expr_inducted->vAppend_nmdCol_synthetic(iExp);

//  if (pSymbol_expr_arriving->bHasFacet(SF_KEY_COL))
//    pSymbol_expr_inducted->vAppendFacet(SF_KEY_COL,
//                                        pSymbol_expr_arriving->szSF_chrSeq(SF_KEY_COL));

  if (bIsScalarCol() && pGet_Symbol(0)->bHasFacet(SF_TBL_COL)) {
    pSymbol_expr_inducted->vAppendFacet(SF_TBL_COL,
                                        pGet_Symbol(0)->szSF_chrSeq(SF_TBL_COL));
  }




  Card_RpnCalc* p_rpnCalc_inducted = p_rpnCalc_arriving->pInduct(this);

  p_expr_inducted->vAppendChild(p_rpnCalc_inducted);

  return p_expr_inducted;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsDescending(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Direction* p_direction = pSub_direction())
    if (p_direction->pSymbol()->bSame("DESC"_Stz))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this an expression of a key                                             */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsExpressionOfKey(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol *pSymbol_0 = pGet_Symbol(0);

  if (pSymbol_0->bIs_Key_Col())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound expr?                                                   */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bCompound = false;
  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    bCompound = p_boolPred->bIsCompound();
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    bCompound = p_rpnCalc->bIsCompound();
  }

  return bCompound;
}

/*----------------------------------------------------------------------------*/
/* is this a literals expr?                                                   */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsLiterals(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bCompound = false;
  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    bCompound = p_boolPred->bIsLiterals();
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    bCompound = p_rpnCalc->bIsLiterals();
  }

  return bCompound;
}

/*----------------------------------------------------------------------------*/
/* is this expr a simple expression of a column?                              */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsScalarCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bScalarCol = false;
  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    bScalarCol = p_boolPred->bIsScalarCol();
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    bScalarCol = p_rpnCalc->bIsScalarCol();
  }

  return bScalarCol;
}

/*----------------------------------------------------------------------------*/
/* is this expr a simple expression of a key?                                 */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsScalarKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bScalarKey = false;
  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    bScalarKey = p_boolPred->bIsScalarKey();
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    bScalarKey = p_rpnCalc->bIsScalarKey();
  }

  return bScalarKey;
}

/*----------------------------------------------------------------------------*/
/* does expression have term ?                                                */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bHasTerm(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bHas = false;

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    bHas = p_boolPred->bHas_term(pSymbol);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    bHas = p_rpnCalc->bHasTerm(pSymbol);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* is this an expression of given symbol ?                                    */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bMatches(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_arg = p_expr_arg->pGet_Symbol(0);
  Symbol* pSymbol_this = this->pGet_Symbol(0);

  if (pSymbol_this->bMatches(pSymbol_arg))
    return true;
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol = this->pSymbol();
  i8 nFacets = pSymbol->nFacets();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // primary facet

  s32 sType_short = szCT_char[eCTVal()];
  i8 i_Facet_primary = C_UNKNOWN;
  if (C_UNKNOWN != (i_Facet_primary = pSymbol->i_Facet(SF_KEY_COL))) {
    pRibbon->vSprintSnippet(false, "%c%c%s%lld]%s %s~%lld%s%lld"_Stz,
                            cMark1(),
                            cMark2(),
                            Stz::szDots(iDepth - 1),
                            m_i_Child,
                            szCardNm(),
                            szFacet(SF_KEY_COL),

                            i_Facet_primary,
                            sType_short.szStz(),
                            pSymbol->xCast().m_nTypeLength
    );

  }
  else
  if (C_UNKNOWN != (i_Facet_primary = pSymbol->i_Facet(SF_EXP_COL))) {
    pRibbon->vSprintSnippet(false, "%c%c%s%lld]%s %s~%lld%s%lld"_Stz,
                            cMark1(),
                            cMark2(),
                            Stz::szDots(iDepth - 1),
                            m_i_Child,
                            szCardNm(),
                            szFacet(SF_EXP_COL),

                            i_Facet_primary,
                            sType_short.szStz(),
                            pSymbol->xCast().m_nTypeLength
                            );
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // secondary facets

  for (i8 i_Facet_secondary = 0; i_Facet_secondary < nFacets; i_Facet_secondary++) {
    if (i_Facet_secondary == i_Facet_primary)
      continue;
    Facet* aFacet = pSymbol->aFacet(i_Facet_secondary);
    pRibbon->vSprintSnippet(true, "%s~%lld"_Stz,
                            aFacet->m_sChrSeq.szStz(),
                            i_Facet_secondary);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberForm_keyCol(i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  SYM(SF_KEY_COL,i_Expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vRenumberForm_expCol(i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  SYM(SF_EXP_COL, i_Expr);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bSameAs(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

//  if (xTypeId() != p_expr_arg->xTypeId())
//    return false;

  Card_RpnCalc* p_rpnCalc_this = this->pSub_rpnCalc();
  Card_RpnCalc* p_rpnCalc_arg  = p_expr_arg->pSub_rpnCalc();
  if (!p_rpnCalc_this->bSameAs(p_rpnCalc_arg))
    return false;

//  if (Card_Direction* p_direction_this = pSub_direction()) {
//    if (Card_Direction *p_direction_arg = p_expr_arg->pSub_direction()) {
//      if (! p_direction_this->pSymbol()->bSame(p_direction_arg->pSymbol()))
//        return false;
//    }
//    else {
//      return false;
//    }
//  }
//  else
//  if (p_expr_arg->pSub_direction()) {
//    return false;
//  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Expr::vShortForm(String* pString, Stz* szSep)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_BoolPred* p_boolPred = pSub_boolPred())
    p_boolPred->vShortForm(pString);
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc())
    p_rpnCalc->vShortForm(pString,szSep);

}

/*----------------------------------------------------------------------------*/
/* convert stmCol to tblCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Expr::vX_expCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_boolPred = nullptr;
  pCI->G.p_rpnCalc  = nullptr;

  if ((pCI->G.p_boolPred = pSub_boolPred()))
    pCI->G.p_boolPred->vX_expCol_To_tblCol(pCI);
  else
  if ((pCI->G.p_rpnCalc = pSub_rpnCalc()))
    pCI->G.p_rpnCalc->vX_expCol_To_tblCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Expr::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nAncestors = pCI->nAncestors();
  pCI->vAncestorPush(this);

  if (Card_BoolPred* p_boolPred = pSub_boolPred()) {
    p_boolPred->vX_tblCol_To_expCol(pCI);
    p_boolPred = nullptr;
  }
  else
  if (Card_RpnCalc* p_rpnCalc = pSub_rpnCalc()) {
    p_rpnCalc->vX_tblCol_To_expCol(pCI);
    p_rpnCalc = nullptr;
  }

  pCI->vAncestorPop();
  RASSERT(pCI->nAncestors() == nAncestors);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSymbol_expr = pSymbol();
  RASSERT(pSymbol_expr->xCast().m_xTypeId.bIsValid());
  if (m_xSymbol.xTypeId() == TypeId::xIdOf(CT_DECIMAL)) {
    if (m_xSymbol.nTypeLength() == 0) {
      vMark("?"_Stz,"l"_Stz);
      pCI->pAncestorParent()->vMark("?"_Stz,"l"_Stz);
      pCI->vPrintBoard(__func__,__LINE__,true);
      vDump(__func__);
      vValidationFailed("Decimal length is 0 : %p"_Stz, this);
    }
  }

  (void)Card::eValidate_parentEither(pCI, CK_t_EMIT, CK_c_FOLD, CK_s_INJECTABLE,
                                     CK_s_KEY, CK_s_NEEDED, CK_STREAMKEY, CK_NONE);

  if (pCI->pAncestorFind(CK_s_KEY)) {
    //vDump(__func__);
    if (! m_xSymbol.bHasFacet(SF_KEY_COL) || ! m_xSymbol.bValidate()) {
      vDump(__func__);
      vValidationFailed("SF_KEY_COL Missing : %p"_Stz,this);
    }
  }

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
