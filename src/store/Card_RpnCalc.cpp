/*============================================================================*/
/* Card_RpnCalc.cpp -- RPN Calculation Sequence                               */
/*============================================================================*/

#include "Card_RpnCalc.hpp"
#include "Card_Term.hpp"
#include "Card_BoolPred.hpp"
#include "CardIter.hpp"
#include "Card_RpnExpr.hpp"
#include "Card_AndConj.hpp"
#include "Card_OrConj.hpp"
#include "Card_RlnConj.hpp"
#include "Card_NotConj.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_RpnCalc, CK_RPNCALC)
__aNewFunction(Card_RpnCalc, CN_CARD)
__vDeleteFunction(Card_RpnCalc, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnCalc::Card_RpnCalc(void)
: Card_TERMLIST(CK_RPNCALC, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnCalc::Card_RpnCalc(Symbol* pSymbol)
: Card_TERMLIST(CK_RPNCALC, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term = (new Card_Term) ->SYMPPY(pSymbol);
  vAppendChild(p_term);

  vApplyCast_p(p_term);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_RpnCalc::~Card_RpnCalc(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vBump_expCol(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(bSkipFoldExpr);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {

    if (xKid.p_term()->pSymbol()->bHasFacet(SF_EXP_COL)) {
      xKid.p_term()->vBump_ordCol(SF_EXP_COL,nBumpBy);
      xKid.p_term()->vMark1(MARK_CHANGED);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnCalc* Card_RpnCalc::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_RpnCalc(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* does this rpnCalc contain given term?                                      */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bContains(Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->pSymbol()->bMatches(pSymbol_arg))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xTerm(this); xTerm.bEach(CK_TERM); /**/) {
    xTerm.p_term()->vDeleteFacetIf(eFacet, eAlsoFacet, cTblSyn);
  }
}

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Cast xCast_last;
  xCast_last.m_xTypeId = TypeId();
  xCast_last.m_nTypeLength = 0;
  xCast_last.m_nTypeScale = 0;
  RASSERT(! xCast_last.m_xTypeId.bIsValid());

  TypeId xTypeId_save = xCast().m_xTypeId;
  if (! xTypeId_save.bIsValid()) {
    pCI->PRINTBOARD_dbg();
    vDump(__func__);
    NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // rpnCalc is a set of terms
  for (Kid xRpnCalc(this); xRpnCalc.bEach(CK_TERM); /**/) {
    pCI->G.p_term = xRpnCalc.p_term();
    pCI->G.p_term->vApplyCast(pCI, /*??*/p_Dic_expr, xCast_last);
  }
  pCI->G.p_term = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (xCast_last.m_xTypeId == pCI->G.p_rpnCalc->xCast().m_xTypeId) {
    if (xCast_last.m_nTypeLength   != pCI->G.p_rpnCalc->xCast().m_nTypeLength ||
        xCast_last.m_nTypeScale    != pCI->G.p_rpnCalc->xCast().m_nTypeScale  ||
        xCast_last.m_bTypeNullable != pCI->G.p_rpnCalc->xCast().m_bTypeNullable)
      vApplyCast_x(pCI->G.p_rpnCalc->xCast());
  }

  RASSERT(xTypeId_save == xCast().m_xTypeId);
}

/*----------------------------------------------------------------------------*/
/* return first key col in this rpn sequence                                  */
/*----------------------------------------------------------------------------*/
Symbol* Card_RpnCalc::pFirstColSym(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_EXP_COL) || xKid.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return xKid.p_term()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return first key col in this rpn sequence                                  */
/*----------------------------------------------------------------------------*/
Symbol* Card_RpnCalc::pFirstKeyCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_KEY_COL))
      return xKid.p_term()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return first key col in this rpn sequence                                  */
/*----------------------------------------------------------------------------*/
Symbol* Card_RpnCalc::pFirstNmdCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_NMD_COL))
      return xKid.p_term()->pSymbol();

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* return first stmCol in this rpn sequence                                   */
/*----------------------------------------------------------------------------*/
Symbol* Card_RpnCalc::pFirstStmCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_EXP_COL))
      return xKid.p_term()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return first tblCol in this rpn sequence                                   */
/*----------------------------------------------------------------------------*/
Symbol* Card_RpnCalc::pFirstTblCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return xKid.p_term()->pSymbol();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a term                                                                 */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RpnCalc::pGetTerm(i8 i_Term)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (i_Term >= this->nChildCards())
    return nullptr;

  Card_Term* p_term = _pCard_Term(pGetChild(i_Term));

  return p_term;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bHasTerm(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
//    if (xKid.p_term()->bSymbolMatches(pSymbol))
    if (xKid.p_term()->pSymbol()->bMatches(pSymbol))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* induct this arriving rpcCalc                                               */
/*----------------------------------------------------------------------------*/
Card_RpnCalc* Card_RpnCalc::pInduct(Card_Expr* p_expr_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RpnCalc* p_rpnCalc_inducted = new Card_RpnCalc;

  p_rpnCalc_inducted->vApplyCast_p(this);

  Card_Term* p_term_inducted = new Card_Term;
  p_term_inducted->vApplyCast_p(this);
  p_rpnCalc_inducted->vAppendChild(p_term_inducted);

  if (bIsScalarCol() && pFirstColSym()->bHasFacet(SF_TBL_COL)) {
    p_term_inducted ->SYM(SF_TBL_COL, pFirstColSym()->szFacet(SF_TBL_COL));
  }
  if (p_expr_arriving->pSymbol()->bHasFacet(SF_EXP_COL)) {
    p_term_inducted ->SYM(SF_EXP_COL, p_expr_arriving->szFacet(SF_EXP_COL));
  }

  return p_rpnCalc_inducted;
}

/*----------------------------------------------------------------------------*/
/* is this rpnCalc an aggregate function sequence ?                           */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsAggCalc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() < 0)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  if (! p_term_0->pSymbol()->bHasFacet((SF_PARENTHESIS)))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound rpnCalc?                                                */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a literals rpnCalc?                                                */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsLiterals(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    if (xKid.p_term()->pSymbol()->bHasFacet(SF_EXP_COL) || xKid.p_term()->pSymbol()->bHasFacet(SF_TBL_COL))
      return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this rpnCalc a simple expression of a column ?                          */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsScalarCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  if (! p_term_0->pSymbol()->bHasFacet(SF_EXP_COL) &&  !p_term_0->pSymbol()->bHasFacet(SF_TBL_COL))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this rpnCalc a simple expression of a key ?                             */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsScalarKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  if (! p_term_0->pSymbol()->bHasFacet(SF_KEY_COL))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bNeedsBrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    if (xKid.p_term()->pSymbol()->bNeedsBrace())
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sType_short = szCT_char[eCTVal()];

  pRibbon->vSprintSnippet(false,"%c%c%s%lld]%s %s%lld"_Stz,
                          cMark1(),
                          cMark2(),
                          Stz::szDots(iDepth - 1),
                          m_i_Child,
                          szCardNm(),
                          sType_short.szStz(),
                          m_xSymbol.xCast().m_nTypeLength);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bSameAs(Card_RpnCalc* p_rpnCalc_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();

  if (nChildren != p_rpnCalc_arg->nChildCards())
    return false;

  for (i8 i_Child = 0; i_Child != nChildren; i_Child++) {
    Card_Term* p_term_this = this->pGetTerm(i_Child);
    Card_Term* p_term_arg = p_rpnCalc_arg->pGetTerm(i_Child);

    if (p_term_this->bSymbolMatches(p_term_arg))
      continue;

    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vShortForm(String* pString, Stz* szSep)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    xKid.p_term()->vShortForm(pString,szSep);
    szSep = " "_Stz;
  }
}

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolPred>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolPred* Card_RpnCalc::pTo_boolPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolPred* p_boolPred = Card_BoolPred::pTo_boolPred(this);
  return p_boolPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vTransfer(STORE::Card_RpnCalc *p_rpnCalc, eSF eSymbol_limit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bAppendAnd = false;
  if (nChildCards())
    bAppendAnd = true;

  vTransferChildren(p_rpnCalc,eSymbol_limit);

  if (bAppendAnd) {
    Card_Term *p_term_and = (new Card_Term) ->SYM(SF_BOL_OP,"and"_Stz);
    vAppendChild(p_term_and);
  }
}

/*----------------------------------------------------------------------------*/
/* convert stmCol to tblCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vX_expCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    if (!xKid.p_term()->pSymbol()->bHasFacet(SF_EXP_COL))
      continue;

    pCI->G.p_term = xKid.p_term();
    pCI->G.p_term->vX_expCol_To_tblCol(pCI);
  }

  pCI->G.p_term = nullptr;
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xRpnCalc(this);xRpnCalc.bEach(CK_TERM); /**/) {
    pCI->G.p_term = xRpnCalc.p_term();
    pCI->G.p_term->vX_tblCol_To_expCol(pCI);
  }

  pCI->G.p_term = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  (void)Card::eValidate_parentEither(pCI,
                                     CK_EXPR,

                                     CK_NONE);

  vValidate_mustHave(CK_TERM, CK_NONE);

  Card::vValidate_hasType();
  Card::vValidate_hasChildren();

  if (xCast().m_xTypeId != pCI->pAncestor(CK_EXPR)->xCast().m_xTypeId) {
    pCI->PRINTBOARD_dbg();
    vDump(__func__);
    NEVER_REACH;
  }

  vValidate_mustHave(CK_TERM, CK_NONE);

  (void)Card::eValidate_parentEither(pCI,CK_EXPR,CK_NONE);

  if (pCI->bAncestor(CK_s_INJECTABLE)) {
    if (!m_xSymbol.bValidType()) {
      vDump(__func__);
      vValidationFailed("Type Missing : %p"_Stz,this);
    }
  }

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
