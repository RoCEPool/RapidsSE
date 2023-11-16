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

  vCast(p_term);

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

    if (xKid.p_term()->pSymbol()->bIs_Exp_Col()) {
      xKid.p_term()->vBump_expCol(nBumpBy);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vCorrect_expCol(Card_Emit* p_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    xKid.p_term()->vCorrect_expCol(p_arriving);
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
void Card_RpnCalc::vEnsureTypeId_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr *p_expr_found;

  // rpnCalc is a set of terms
  for (Kid xTerm(this); xTerm.bEach(CK_TERM); /**/) {
    if (xTerm.p_term()->pSymbol()->bHasType())
      continue;

    i8 iOdn = xTerm.iGetOdn();

    if (xTerm.p_term()->bIsKeyCol())
      p_expr_found = pCI->G.p_key->pGet_expr(iOdn - 1);
    else
    if (xTerm.p_term()->bIs_expCol()) {
      //pCI->Flux.m_pArriving->vDump(__func__);
      p_expr_found = pCI->Flux.m_pArriving->pGet_expr(iOdn - 1);
    }
    else
      continue;

    Card_RpnCalc *p_rpnCalc_found = p_expr_found->pSub_rpnCalc();

    xTerm.p_term()->vCast(p_rpnCalc_found);
  }
}

/*----------------------------------------------------------------------------*/
/* return first key col in this rpn sequence                                  */
/*----------------------------------------------------------------------------*/
Symbol* Card_RpnCalc::pFirstColSym(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->bIsColSym())
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
    if (xKid.p_term()->bIsKeyCol())
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
    if (xKid.p_term()->bIs_expCol())
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
    if (xKid.p_term()->bIsTblCol())
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
/* is this rpnCalc an aggregate function sequence ?                           */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsAggCalc(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() < 0)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  if (! p_term_0->pSymbol()->bSame(')'))
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
    if (xKid.p_term()->bIsColSym())
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
  if (! p_term_0->bIsColSym())
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
  if (! p_term_0->bIsKeyCol())
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

  s32 sType_short = szCT_char[xTypeId().eCTVal()];

  pRibbon->vSprintSnippet(false,"%c%c%s%lld]%s %s"_Stz,
                          cMark1(),
                          cMark2(),
                          Stz::szDots(iDepth - 1),
                          m_i_Child,
                          szCardNm(),
                          sType_short.szStz());
}

/*----------------------------------------------------------------------------*/
/* return a reduced copy of this rpnCalc                                      */
/*----------------------------------------------------------------------------*/
Card_RpnCalc * Card_RpnCalc::pReducedCopy(i8 i_Expr, bool bIs_SQCRLKEY)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsScalarCol() &&
      (bIs_SQCRLKEY || pGetTerm(0)->pSymbol()->bIs_Tbl_Col())) {
    Card_RpnCalc* p_rpnCalc_reduced = pClone();
    return p_rpnCalc_reduced;
  }

  Card_RpnCalc* p_rpnCalc_reduced = new Card_RpnCalc;
  p_rpnCalc_reduced->vCast(this);

  Card_Term* p_term =(new Card_Term) -> SYM(SF_EXP_COL, i_Expr + 1);
  p_term->vCast(this);

  p_rpnCalc_reduced->vAppendChild(p_term);

  return p_rpnCalc_reduced;
}

/*----------------------------------------------------------------------------*/
/* return a reduced copy of this rpnCalc                                      */
/*----------------------------------------------------------------------------*/
Card_RpnCalc * Card_RpnCalc::pReducedCopy_key(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(1 == nChildCards());

  Card_RpnCalc* p_rpnCalc_reduced = pClone();
  return p_rpnCalc_reduced;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RpnCalc* Card_RpnCalc::pPropagate(Card_Expr* p_expr_encasing)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_RpnCalc* p_rpnCalc_propagated = new Card_RpnCalc;

  p_rpnCalc_propagated->vCast(this);

  eSF eSymbol = Symbol::eEvalSymbol(p_expr_encasing->szFacet(SF_NONE));
  RASSERT(SF_EXP_COL == eSymbol);
  Card_Term* p_term_propagated = (new Card_Term) ->SYM(SF_EXP_COL, p_expr_encasing->szFacet(SF_NONE));
  p_term_propagated->vCast(this);

  p_rpnCalc_propagated->vAppendChild(p_term_propagated);

  return p_rpnCalc_propagated;
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
    Stz* szThis = p_term_this->szFacet(SF_NONE);
    Card_Term* p_term_arg = p_rpnCalc_arg->pGetTerm(i_Child);
    Stz* szArg = p_term_arg->szFacet(SF_NONE);

    if (szThis->bSame(szArg))
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
    if (!xKid.p_term()->pSymbol()->bIs_Exp_Col())
      continue;

    xKid.p_term()->vX_expCol_To_tblCol(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    if (!xKid.p_term()->pSymbol()->bIs_Tbl_Col())
      continue;

    xKid.p_term()->vX_tblCol_To_expCol(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card::vValidate_hasType();
  Card::vValidate_hasChildren();

  vValidate_mustHave(CK_TERM, CK_NONE);

  (void)Card::eValidate_parentEither(pCI,CK_EXPR,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
