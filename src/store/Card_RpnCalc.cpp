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

  vColType(p_term);

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
void Card_RpnCalc::vBump_stmCol(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(bSkipFoldExpr);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {

    if (xKid.p_term()->pSymbol()->bIs_Exp_Col()) {
      xKid.p_term()->vBump_stmCol(nBumpBy);
      xKid.p_term()->vMark1(MARK_CHANGED);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
TypeId Card_RpnCalc::xColTypeId(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

	TypeId xTypeId = pSymbol()->xColTypeId();

  return xTypeId;
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
void Card_RpnCalc::vCorrectStmCol(Card_Emit* p_emitted_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    xKid.p_term()->vCorrectStmCol(p_emitted_prior);
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
void Card_RpnCalc::vEnsureTypesFor_stmCol(CardIter* pCI)
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
    if (xTerm.p_term()->bIsStmCol()) {
      //pCI->Flux.m_pArriving->vDump(__func__);
      p_expr_found = pCI->Flux.m_pArriving->pGet_expr(iOdn - 1);
    }
    else
      continue;

    Card_RpnCalc *p_rpnCalc_found = p_expr_found->pSub_rpnCalc();

    xTerm.p_term()->vColType(p_rpnCalc_found);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vFactorAll_ColSym_IntoDistinct_expr(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(! pCI->G.p_subSet);

  Card_SubSet* p_subSet = new Card_SubSet;

  // an rpnCalc is a linear sequence of terms, some of which are columns
  for (Kid x_term(this); x_term.bEach(CK_TERM); /**/) {

     if (!x_term.p_term()->bIsColSym())
      continue;

    Card_Term* p_term_ColSym = x_term.p_term();

    Card_Expr* p_expr_ColSym = new Card_Expr(p_term_ColSym->pSymbol());
    p_expr_ColSym->pSymbol()->vColTypeId(p_term_ColSym->pSymbol()->xColTypeId());

    p_subSet->vAppendChild(p_expr_ColSym);
  }

  pCI->G.p_subSet = p_subSet;
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
    if (xKid.p_term()->bIsStmCol())
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

  s32 sType_short = szCT_char[pSymbol()->xColTypeId().eCTVal()];

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
  p_rpnCalc_reduced->vColType(this);

  Card_Term* p_term =(new Card_Term) -> SYM(SF_EXP_COL, i_Expr + 1);
  p_term->vColType(this);

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

  p_rpnCalc_propagated->vColType(this);

  eSF eSymbol = Symbol::eEvalSymbol(p_expr_encasing->szFacet(SF_NONE));
  RASSERT(SF_EXP_COL == eSymbol);
  Card_Term* p_term_propagated = (new Card_Term) ->SYM(SF_EXP_COL, p_expr_encasing->szFacet(SF_NONE));
  p_term_propagated->vColType(this);

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
Card_BoolPred* Card_RpnCalc::pTo_boolPred(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Symbol xWhen(SF_WHEN,"WHEN"_Stz);

  Card_BoolPred* p_boolPred = new Card_BoolPred;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the rpn calc have a when in it?

  if (this->bHasTerm(&xWhen)) {

//    Card_RlnConj* p_rlnConj = new Card_RlnConj;
//    p_rlnConj->vTransferChildren(this);
//
//    p_boolPred->vAppendChild(p_rlnConj);
//
//
//    Card* p_last = p_rlnConj->pPeekChild_last();
//    if (p_last->pSymbol()->bIs_Rln_Op()) {
//      p_rlnConj->pSymbol()->vFacet(p_last->pSymbol()->eFacet(),p_last->pSymbol()->szSF_chrSeq(SF_RLN_OP));
//      p_last = p_rlnConj->pPopChild_tail();
//      delete p_last;
//    }

    p_boolPred->vTransferChildren(this);

    return p_boolPred;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_AndConj* p_andPred = nullptr;
  Card_RlnConj* p_rlnConj = nullptr;
  Card_NotConj* p_notPred = nullptr;
  Card_OrConj*  p_orPred  = nullptr;

  Card*  p_childA  = nullptr;
  Card*  p_childB  = nullptr;

  Card_RpnExpr*  p_TermSeries = new Card_RpnExpr;

  while (this->nChildCards()) {
    Card_Term* p_term_next = _pCard_Term(this->pPopChild());

    eTE eTerm_next = p_term_next->eEvalTerm();

    switch (eTerm_next) {

      case TE_AND:
        // <boolPred> = <and>/<boolPred>
        p_childB = p_boolPred->pPopChild_tail();
        p_childA = p_boolPred->pPopChild_tail();

        p_andPred = new Card_AndConj;
        p_andPred->vAppendChild(p_childA);
        p_andPred->vAppendChild(p_childB);

        p_boolPred->vAppendChild(p_andPred);
        p_andPred = nullptr;
        p_childA  = nullptr;
        p_childB  = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolPred->vDump(0,"AND"_Stz);
        break;

      case TE_OR:
        // <boolPred> = <or>/<boolPred>
        p_childB = p_boolPred->pPopChild_tail();
        p_childA = p_boolPred->pPopChild_tail();

        p_orPred = new Card_OrConj;
        p_orPred->vAppendChild(p_childA);
        p_orPred->vAppendChild(p_childB);

        p_boolPred->vAppendChild(p_orPred);
        p_orPred = nullptr;
        p_childA = nullptr;
        p_childB = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolPred->vDump(0,"OR"_Stz);
        break;

      case TE_NOT:
        p_childA = p_boolPred->pPopChild_tail();

        p_notPred = new Card_NotConj;
        p_notPred->vAppendChild(p_childA);

        p_boolPred->vAppendChild(p_notPred);
        p_notPred = nullptr;
        p_childA  = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolPred->vDump(0,"NOT"_Stz);
        break;

      case TE_RELNOP:
        // <boolPred> = <cmpPred><termSeries><boolPred>
        RASSERT(p_TermSeries->nChildCards() > 0);
        p_rlnConj = new Card_RlnConj;
        {
          eSF eSymbol = Symbol::eEvalSymbol(p_term_next->szFacet(SF_NONE));
          RASSERT(SF_RLN_OP == eSymbol);
        }
        p_rlnConj->SYM(SF_RLN_OP, p_term_next->szFacet(SF_NONE));
        p_rlnConj->vTransferChildren(p_TermSeries);
        RASSERT(p_rlnConj->bIsRelnOp())
        //p_rlnConj->vDump();
        if (p_rlnConj->bContains_callToSubQuery(pCI)) {
          p_rlnConj->vCard(CK_RLNCONJ);
          RASSERT(p_rlnConj->bIsRelnOp())
        }
        else
        if (!p_rlnConj->pSymbol()->bSame("=="_Stz)) { // todo: more work
          p_rlnConj->vCard(CK_RLNCONJ);
          RASSERT(p_rlnConj->bIsRelnOp())
        }
        //p_boolPred->vPushChild(p_rlnConj);
        p_boolPred->vAppendChild(p_rlnConj);
        p_rlnConj = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolPred->vDump(0,"COMPARE"_Stz);
        break;

      case TE_EXISTS:
      case TE_NOTEXISTS:
        // <boolPred> = <exists><boolPred>
        p_rlnConj = new Card_RlnConj;
        {
          eSF eSymbol = Symbol::eEvalSymbol(p_term_next->szFacet(SF_NONE));
          RASSERT(SF_SQY_OP == eSymbol);
        }
        p_rlnConj->SYM(SF_SQY_OP, p_term_next->szFacet(SF_NONE));
        p_rlnConj->vTransferChildren(p_TermSeries);
        RASSERT(p_rlnConj->bIsSqyOp())
        p_boolPred->vAppendChild(p_rlnConj);
        p_rlnConj = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolPred->vDump(0,"EXISTS"_Stz);
        break;

      case TE_IN:
        // <boolPred> = <in><boolPred>
        if (p_TermSeries->bIsInList()) {
          p_rlnConj = new Card_RlnConj;
          p_rlnConj->SYM(SF_SQY_OP, p_term_next->szFacet(SF_NONE));
          p_rlnConj->vTransferChildren(p_TermSeries);
          p_boolPred->vAppendChild(p_rlnConj);
          RASSERT(p_rlnConj->bIsSqyOp())
          p_rlnConj = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolPred->vDump(0,"BOOLFN/IN"_Stz);
        }
        else {
          p_rlnConj = new Card_RlnConj;
          p_rlnConj->SYM(SF_SQY_OP,"in"_Stz);
          p_rlnConj->vTransferChildren(p_TermSeries);
          RASSERT(p_rlnConj->bIsSqyOp())
          p_boolPred->vAppendChild(p_rlnConj);
          p_rlnConj = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolPred->vDump(0,"IN"_Stz);
        }
        break;

      case TE_NOTIN:
        // <boolPred> = <in><boolPred>
        if (p_TermSeries->bIsInList()) {
          p_rlnConj = new Card_RlnConj;
          p_rlnConj->SYM(SF_SQY_OP,"in"_Stz);
          p_rlnConj->vTransferChildren(p_TermSeries);
          RASSERT(p_rlnConj->bIsBoolFn())
          p_boolPred->vAppendChild(p_rlnConj);
          p_rlnConj = nullptr;
          DELETE_CARD(p_term_next);

          p_childA = p_boolPred->pPopChild_tail();
          p_notPred = new Card_NotConj;
          p_notPred->vAppendChild(p_childA);
          p_boolPred->vAppendChild(p_notPred);
          p_notPred = nullptr;
          p_childA  = nullptr;
          //p_boolPred->vDump(0,"BOOLFN/NOTIN"_Stz);
        }
        else {
          p_rlnConj = new Card_RlnConj;
          p_rlnConj->SYM(SF_SQY_OP, p_term_next->szFacet(SF_NONE));
          p_rlnConj->vTransferChildren(p_TermSeries);
          RASSERT(p_rlnConj->bIsSqyOp())
          p_boolPred->vAppendChild(p_rlnConj);
          p_rlnConj = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolPred->vDump(0,"NOTIN"_Stz);
        }
        break;

      case TE_BOOLFN:
        p_rlnConj = new Card_RlnConj;
        p_rlnConj->SYM(SF_BOL_FN, p_term_next->szFacet(SF_NONE));
        p_rlnConj->vTransferChildren(p_TermSeries);
        RASSERT(p_rlnConj->bIsBoolFn())
        p_boolPred->vAppendChild(p_rlnConj);
        p_rlnConj = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolPred->vDump(0,"BOOLFN"_Stz);
        break;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // <termSeries>

      case TE_EXECUTE:
        DELETE_CARD(p_term_next);
        break;

      case TE_OTHER:
        p_TermSeries->vAppendChild(p_term_next);
        //p_TermSeries->vDump(0,"TERM_SERIES"_Stz);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  if (p_TermSeries->nItems()) {
    RASSERT(0 == p_boolPred->nChildCards());
    //p_boolPred->vDump(0,"boolPred"_Stz);
    //p_TermSeries->vDump(0,"termSeries"_Stz);

    p_rlnConj = new Card_RlnConj;
    p_rlnConj->vTransferChildren(p_TermSeries);

    p_boolPred->vAppendChild(p_rlnConj);
  }

  RASSERT(0 == p_TermSeries->nChildCards());
  delete p_TermSeries;

  //p_boolPred->vDump();

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
void Card_RpnCalc::vX_stmCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    if (!xKid.p_term()->pSymbol()->bIs_Exp_Col())
      continue;

    xKid.p_term()->vX_stmCol_To_tblCol(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vX_tblCol_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    if (!xKid.p_term()->pSymbol()->bIs_Tbl_Col())
      continue;

    xKid.p_term()->vX_tblCol_To_stmCol(pCI);
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
