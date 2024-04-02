/*============================================================================*/
/* Card_Expr.cpp -- Typed Expression                                          */
/*============================================================================*/

#include "Card_Expr.hpp"

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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vAssemble_injCrlRsl(CardIter* pCI)
{
  _static_

  eAL e_Level     = (eAL) pCI->E.p_level->pSymbol()->Y_iGet(P_Level);

  if (AL_PARTITION == e_Level) {
    pCI->E.p_expr = Card_Expr::pCraft_expr_keyCol_Of_term(pCI->E.p_expr->pGet_term0());
  }
  else {
    Symbol xSymbol_KeyCol(SF_KEY_COL,1);

    pCI->vNewMember(CK_EXPR);
    i8 i_Board = pCI->Flat.m_pSqy_inner->pBoard()->i_Board();
    pCI->E.p_expr->pSymbol()->vApplyFacet_varg(SF_NMD_COL, C_CRLKEY, i_Board);
    pCI->vAppendNewChild(pCI->E.p_expr, CK_RPNCALC);
    pCI->vAppendNewChild(pCI->E.p_rpnCalc, CK_TERM, &xSymbol_KeyCol);

    pCI->E.p_rpnCalc->vApplyCast_x(pCI->E.p_key->xCast());
    pCI->E.p_term->vApplyCast_x(pCI->E.p_key->xCast());
    pCI->E.p_expr->vApplyCast_x(pCI->E.p_key->xCast());

    pCI->E.p_emit->vBump_expCol_In_rpnCalc(pCI);
    pCI->E.p_key->pSymbol()->Y_vSet(P_ProjBumped);
  }

  pCI->E.p_expr->pSymbol()->Y_vSet(P_InjCrlKey);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_Of_expr(Card_Expr* p_expr, i8 iExpCol)
{
  _static_

  RASSERT(CT_NONE != p_expr->eCTVal());

  Symbol xSymbol_ExpCol(SF_EXP_COL, iExpCol, p_expr->xCast());
  Card_Expr* p_expr_new = pCraft_expr_Of_symbol(&xSymbol_ExpCol);

  return p_expr_new;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_Of_symbol(Symbol* pSymbol)
{
  _static_

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vApplyCast_x(pSymbol->xCast());

  Card_RpnCalc* p_rpnCalc_new = new Card_RpnCalc;
  p_rpnCalc_new->vApplyCast_x(pSymbol->xCast());
  p_expr_new->vAppendChild(p_rpnCalc_new);

  Card_Term* p_term_new = (new Card_Term) ->SYM(pSymbol);
  p_term_new->vApplyCast_x(pSymbol->xCast());
  p_rpnCalc_new->vAppendChild(p_term_new);

  return p_expr_new;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_Of_term(Card_Term* p_term)
{
  _static_

  Cast xCast = p_term->pSymbol()->xCast();

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vApplyCast_x(xCast);

  Card_RpnCalc* p_rpnCalc_new = new Card_RpnCalc;
  p_rpnCalc_new->vApplyCast_x(xCast);
  p_expr_new->vAppendChild(p_rpnCalc_new);

  Card_Term* p_term_new = p_term->pClone();
  p_term_new->vApplyCast_x(xCast);
  p_rpnCalc_new->vAppendChild(p_term_new);

  return p_expr_new;
}

/*----------------------------------------------------------------------------*/
/* form project expression from gby key expression                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_keyCol_Of_term(Card_Term* p_term)
{
  _static_

  Card_Expr* p_expr = pCraft_expr_Of_term(p_term);

  Symbol xSymbol_Kn(SF_KEY_COL, C_UNKNOWN);
  p_expr->pGet_term0()->pSymbol()->vApplyFacets(&xSymbol_Kn);

  return p_expr;
}



/*----------------------------------------------------------------------------*/
/* craft a casted expr for given column                                       */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Expr::pCraft_expr_of_nmdCol(Column *pColumn, i8 iExpCol,
                                            char cTblSyn, i8 iTblCol)
{
  _static_

  Cast xCast;
  xCast.m_xTypeId       = pColumn->xColumnTypeId();
  xCast.m_nTypeLength   = pColumn->iColumnLength();
  xCast.m_nTypeScale    = pColumn->iColumnScale();
  xCast.m_bTypeNullable = pColumn->bIsNullable();

  Stz* szNmdCol = pColumn->szColNm();

  Card_Expr* p_expr_new = new Card_Expr;
  p_expr_new->vApplyCast_x(xCast);
  p_expr_new->pSymbol()->vApplyFacet(SF_NMD_COL, szNmdCol);
  p_expr_new->pSymbol()->vApplyFacet(SF_EXP_COL, iExpCol);

  Card_RpnCalc *p_rpnCalc_new = (new Card_RpnCalc);
  p_rpnCalc_new->vApplyCast_x(xCast);
  p_expr_new->vAppendChild(p_rpnCalc_new);

  Card_Term *p_term_new = (new Card_Term) -> SYM(SF_TBL_COL, cTblSyn, iTblCol);
  p_term_new->vApplyCast_x(xCast);
  p_rpnCalc_new->vAppendChild(p_term_new);

  p_expr_new->pSymbol()->vApplyFacets(p_term_new->pSymbol());

  return p_expr_new;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->E.p_rpnCalc = pSub_rpnCalc();

  Symbol* pSymbol_expr = pSymbol();
  if (! xCast().m_xTypeId.bIsValid()) {
    pCI->PRINTBOARD_dbg();
    vDump(__func__ );
    NEVER_REACH
  }
  RASSERT(xCast().m_xTypeId.bIsValid());
  RASSERT(xCast().m_xTypeId == pCI->E.p_rpnCalc->xCast().m_xTypeId);

  TypeId xTypeId_save = xCast().m_xTypeId;

  //pCI->G.p_rpnCalc->vApplyCast(pCI,/*??*/p_Dic_expr);

  {
    Cast xCast_last;
    xCast_last.m_xTypeId = TypeId();
    xCast_last.m_nTypeLength = 0;
    xCast_last.m_nTypeScale = 0;
    RASSERT(!xCast_last.m_xTypeId.bIsValid());

    TypeId xTypeId_save = xCast().m_xTypeId;
    if (!xTypeId_save.bIsValid()) {
      pCI->PRINTBOARD_dbg();
      vDump(__func__);
      NEVER_REACH
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    // rpnCalc is a set of terms
    for (Kid xRpnCalc(pSub_rpnCalc()); xRpnCalc.bEach(CK_TERM); /**/) {
      pCI->E.p_term = xRpnCalc.p_term();
      pCI->E.p_term->vApplyCast(pCI, /*??*/p_Dic_expr, xCast_last);
    }
    pCI->E.p_term = nullptr;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (xCast_last.m_xTypeId == pCI->E.p_rpnCalc->xCast().m_xTypeId) {
      if (xCast_last.m_nTypeLength != pCI->E.p_rpnCalc->xCast().m_nTypeLength ||
          xCast_last.m_nTypeScale != pCI->E.p_rpnCalc->xCast().m_nTypeScale ||
          xCast_last.m_bTypeNullable != pCI->E.p_rpnCalc->xCast().m_bTypeNullable)
        vApplyCast_x(pCI->E.p_rpnCalc->xCast());
    }

    RASSERT(xTypeId_save == xCast().m_xTypeId);
  }

//  if (pSymbol()->xCast().m_xTypeId != pCI->G.p_rpnCalc->xCast().m_xTypeId) {
//    vDump(__func__);
//    pCI->G.p_rpnCalc->vApplyCast(pCI);
//    vDebugPoint();
//  }

//  if (! pSymbol_expr->xCast().m_xTypeId.bIsValid())
//    vApplyCast(pCI->G.p_rpnCalc->xCast());
//  else
  if (pSymbol_expr->xCast().m_xTypeId == pCI->E.p_rpnCalc->xCast().m_xTypeId &&
      (pSymbol_expr->xCast().m_nTypeLength   != pCI->E.p_rpnCalc->xCast().m_nTypeLength ||
       pSymbol_expr->xCast().m_nTypeScale    != pCI->E.p_rpnCalc->xCast().m_nTypeScale ||
       pSymbol_expr->xCast().m_bTypeNullable != pCI->E.p_rpnCalc->xCast().m_bTypeNullable))
    vApplyCast_x(pCI->E.p_rpnCalc->xCast());

  //RASSERT(pSymbol()->xCast().m_xTypeId == pCI->G.p_rpnCalc->xCast().m_xTypeId);

//  if (pCI->G.p_rpnCalc->xCast().m_xTypeId.bIsValid())
//    vApplyCast(pCI->G.p_rpnCalc->xCast());

  RASSERT(xTypeId_save == xCast().m_xTypeId);

  pCI->E.p_rpnCalc = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vBump_expCol(i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //p_rpnCalc->vBump_expCol(nBumpBy);
  Kid xRpnCalc(pSub_rpnCalc());
  while (xRpnCalc.bEach(CK_TERM)) {

    if (xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_EXP_COL)) {
      xRpnCalc.p_term()->vBump_ordCol(SF_EXP_COL, nBumpBy);
      xRpnCalc.p_term()->vMark1(MARK_CHANGED);
    }
  }

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

  for (Kid xRpnCalc(pSub_rpnCalc()); xRpnCalc.bEach(CK_TERM); /**/)
    if (xRpnCalc.p_term()->pSymbol()->bMatches(pSymbol_arg))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xTerm(pSub_rpnCalc()); xTerm.bEach(CK_TERM); /**/) {
    xTerm.p_term()->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
  }
}

/*----------------------------------------------------------------------------*/
/* ensure expression has a SF_NMD_COL                                         */
/*----------------------------------------------------------------------------*/
void Card_Expr::vEnsure_expr_Has_nmdCol(i8 iExpCol)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol* pSym_expr = pSymbol();
  if (pSym_expr->bHasFacet(SF_NMD_COL))
    return;

  if (! bIsCompound()) {
    Card_Term* p_term0 = pGet_term0();
    Symbol* pSym_term0 = p_term0->pSymbol();
    if (pSym_term0->bHasFacet(SF_NMD_COL)) {
      pSym_expr->vApplyFacet(SF_NMD_COL, pSym_term0);
      return;
    }
  }

  // synthetic column name
  pSym_expr->vApplyFacet(SF_NMD_COL,iExpCol);
}

/*----------------------------------------------------------------------------*/
/* return first key col in this rpn sequence                                  */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_colSym(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xRpnCalc(pSub_rpnCalc()); xRpnCalc.bEach(CK_TERM); /**/)
    if (xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_EXP_COL) || xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return xRpnCalc.p_term()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first stm col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_expCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xRpnCalc(pSub_rpnCalc()); xRpnCalc.bEach(CK_TERM); /**/)
    if (xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_EXP_COL))
      return xRpnCalc.p_term()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first key col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_keyCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xRpnCalc(pSub_rpnCalc()); xRpnCalc.bEach(CK_TERM); /**/)
    if (xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_KEY_COL))
      return xRpnCalc.p_term()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first tbl col                                                          */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pFirst_tblCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xRpnCalc(pSub_rpnCalc()); xRpnCalc.bEach(CK_TERM); /**/)
    if (xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return xRpnCalc.p_term()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get symbol of given term                                                   */
/*----------------------------------------------------------------------------*/
Symbol* Card_Expr::pGet_Symbol(i8 i_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term_i = pGet_term(i_Term);
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

  if (i_Term >= pSub_rpnCalc()->nChildCards())
    return nullptr;

  Card_Term* p_term = _pCard_Term(pSub_rpnCalc()->pGetChild(i_Term));

  return p_term;
}

/*----------------------------------------------------------------------------*/
/* get term 0 (of a scalar expr)                                              */
/*----------------------------------------------------------------------------*/
Card_Term* Card_Expr::pGet_term0(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(1 == pSub_rpnCalc()->nChildCards());
  Card_Term* p_term0 = pGet_term(0);

  return p_term0;
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
  else {
    Kid xRpnCalc(pSub_rpnCalc());
    while (xRpnCalc.bEach(CK_TERM)) {
      if (xRpnCalc.p_term()->pSymbol()->bMatches(pSymbol))
        return true;
    }

    return false;

  }

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* induct this arriving expr                                                  */
/*----------------------------------------------------------------------------*/
Card_Expr * Card_Expr::pInduct(i8 iExp)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  //Card_RpnCalc* p_rpnCalc_arriving = pSub_rpnCalc();

  Card_Expr* p_expr_inducted = new Card_Expr;
  Symbol* pSymbol_expr_inducted = p_expr_inducted->pSymbol();

  //vDump(__func__);
  p_expr_inducted->vApplyCast_p(this);

  Symbol* pSymbol_expr_arriving = pSymbol();

  RASSERT(pSymbol_expr_arriving->bHasFacet(SF_EXP_COL));

  if (pSymbol_expr_arriving->bHasFacet(SF_NMD_COL))
    pSymbol_expr_inducted->vApplyFacet(SF_NMD_COL,pSymbol_expr_arriving);
  else
    p_expr_inducted->pSymbol()->vApplyFacet(SF_NMD_COL,iExp);

  if (pSymbol_expr_arriving->bHasFacet(SF_SQY_RSL))
    pSymbol_expr_inducted->vApplyFacet(SF_SQY_RSL,pSymbol_expr_arriving);


  if (bIsScalarCol() && pGet_Symbol(0)->bHasFacet(SF_TBL_COL)) {
    pSymbol_expr_inducted->vApplyFacet(SF_TBL_COL,pGet_Symbol(0));
  }

//  Card_RpnCalc* p_rpnCalc_inducted = p_rpnCalc_arriving->pInduct(this);
//  p_expr_inducted->vAppendChild(p_rpnCalc_inducted);




  Card_RpnCalc* p_rpnCalc_inducted = new Card_RpnCalc;

  p_rpnCalc_inducted->vApplyCast_p(this);

  Card_Term* p_term_inducted = new Card_Term;
  p_term_inducted->vApplyCast_p(this);
  p_rpnCalc_inducted->vAppendChild(p_term_inducted);

  if (bIsScalarCol() && pFirst_colSym()->bHasFacet(SF_TBL_COL)) {
    p_term_inducted ->SYM(SF_TBL_COL, pFirst_colSym()->szFacet(SF_TBL_COL));
  }
  if (pSymbol()->bHasFacet(SF_EXP_COL)) {
    p_term_inducted ->SYM(SF_EXP_COL, szFacet(SF_EXP_COL));
  }





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
    if (p_direction->pSymbol()->bHasFacet(C_DESC))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this an expression of a key                                             */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsKeyCol(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Symbol *pSymbol_0 = pGet_Symbol(0);

  if (pSymbol_0->bHasFacet(SF_KEY_COL))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound expr?                                                   */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSub_rpnCalc()->nChildCards() > 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a literals expr?                                                   */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsLiterals(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xRpnCalc(pSub_rpnCalc());
  while (xRpnCalc.bEach(CK_TERM)) {
    if (xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_EXP_COL) ||
        xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this expr a simple expression of a column?                              */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bIsScalarCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pSub_rpnCalc()->nChildCards() > 1)
    return false;

  Card_Term* p_term_0 = pGet_term(0);
  if (! p_term_0->pSymbol()->bHasFacet(SF_EXP_COL) &&  !p_term_0->pSymbol()->bHasFacet(SF_TBL_COL))
    return false;

  return true;
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
bool Card_Expr::bNeedsBrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xRpnCalc(pSub_rpnCalc());
  while (xRpnCalc.bEach(CK_TERM)) {
    if (xRpnCalc.p_term()->pSymbol()->bNeedsBrace())
      return true;
  }

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

  vPrintProperties(pRibbon, iDepth);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // secondary facets

  for (i8 i_Facet_secondary = 0; i_Facet_secondary < nFacets; i_Facet_secondary++) {
    if (i_Facet_secondary == i_Facet_primary)
      continue;
    Facet* aFacet = pSymbol->aFacet(i_Facet_secondary);
    pRibbon->vSprintSnippet(true, "%s~%lld"_Stz,
                            aFacet->m_sFacet.szStz(),
                            i_Facet_secondary);
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Expr::bSameAs(Card_Expr* p_expr_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren = pSub_rpnCalc()->nChildCards();

  if (nChildren != p_expr_arg->pSub_rpnCalc(C_NOT_NULL)->nChildCards())
    return false;

  for (i8 i_Child = 0; i_Child != nChildren; i_Child++) {
    Card_Term* p_term_this = pGet_term(i_Child);
    Card_Term* p_term_arg = p_expr_arg->pGet_term(i_Child);

    if (p_term_this->bSymbolMatches(p_term_arg))
      continue;

    return false;
  }


  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vTransfer(STORE::Card *pChild_src, eSF eSymbol_limit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RpnCalc* p_rpn_calc_this = pSub_rpnCalc();

  bool bAppendAnd = false;
  if (p_rpn_calc_this->nChildCards())
    bAppendAnd = true;

  p_rpn_calc_this->vTransferChildren(pChild_src, eSymbol_limit);

  if (bAppendAnd) {
    Card_Term *p_term_and = (new Card_Term) ->SYM(SF_BOL_OP,"and"_Stz);
    p_rpn_calc_this->vAppendChild(p_term_and);
  }

  p_rpn_calc_this->vApplyCast_x(pChild_src->xCast());

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Expr::vUpdate_expCol(Card_Emit* p_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xRpnCalc(pSub_rpnCalc()); xRpnCalc.bEach(CK_TERM); /**/)
    xRpnCalc.p_term()->vUpdate_expCol(p_arriving);
}

/*----------------------------------------------------------------------------*/
/* convert stmCol to tblCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Expr::vX_expCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->E.p_boolPred = nullptr;
  pCI->E.p_rpnCalc  = nullptr;

  if ((pCI->E.p_boolPred = pSub_boolPred()))
    pCI->E.p_boolPred->vX_expCol_To_tblCol(pCI);
  else
  if ((pCI->E.p_rpnCalc = pSub_rpnCalc())) {
    //vDump(__func__);
    Kid xRpnCalc(pSub_rpnCalc());
    while (xRpnCalc.bEach(CK_TERM)) {
      if (!xRpnCalc.p_term()->pSymbol()->bHasFacet(SF_EXP_COL))
        continue;

      pCI->E.p_term = xRpnCalc.p_term();
      pCI->E.p_term->vX_expCol_To_tblCol(pCI);
    }

    pCI->E.p_term = nullptr;

  }
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
  else {
    for (Kid xRpnCalc(pSub_rpnCalc());xRpnCalc.bEach(CK_TERM); /**/) {
      pCI->E.p_term = xRpnCalc.p_term();
      pCI->E.p_term->vX_tblCol_To_expCol(pCI);
    }

    pCI->E.p_term = nullptr;

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
      pCI->PRINTBOARD_dbg();
      vDump(__func__);
      vValidationFailed("Decimal length is 0 : %p"_Stz, this);
    }
  }

/**Z_KEY_DIRECTION validation*/
  (void)Card::eValidate_parentEither(pCI,
                                     CK_s_CONSTRAINT,
                                     CK_DIRECTION,
                                     CK_t_EMIT,
                                     CK_o_FILTER,
                                     CK_s_KEY,

                                     CK_c_FLAT,
                                     CK_c_FOLD,
                                     CK_s_INJECTABLE,
                                     CK_s_NEEDED,
                                     CK_STREAMKEY,
                                     CK_NONE);

  eValidate_mustEither(CK_BOOLPRED, CK_RPNCALC, CK_NONE);

  if (pCI->bAncestor(CK_s_KEY)) {
    //vDump(__func__);
    if (! m_xSymbol.bHasFacet(SF_KEY_COL) || ! m_xSymbol.bValidate()) {
      vDump(__func__);
      vValidationFailed("SF_KEY_COL Missing : %p"_Stz,this);
    }

    RASSERT(bIsScalarCol());
  }

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
