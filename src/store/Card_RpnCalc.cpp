/*============================================================================*/
/* Card_RpnCalc.cpp -- RPN Calculation Sequence                               */
/*============================================================================*/

#include "Card_RpnCalc.hpp"
#include "Card_Term.hpp"
#include "Card_BoolPred.hpp"
#include "CardIter.hpp"
#include "Card_RpnTerms.hpp"
#include "Card_AndConj.hpp"
#include "Card_OrConj.hpp"
#include "Card_CmpConj.hpp"
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

    if (xKid.p_term()->pSymbol()->bIs_Stm_Col()) {
      xKid.p_term()->vBump_stmCol(nBumpBy);
      xKid.p_term()->vMark1(MARK_CHANGED);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCT Card_RpnCalc::eColumnType(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCT eColType = pSymbol()->eColType();

  return eColType;
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vCorrectStmCol(Card_Emitted* p_emitted_prior)
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
      //pCI->Prior.m_pEmitted->vDump(__func__);
      p_expr_found = pCI->Prior.m_pEmitted->pGet_expr(iOdn - 1);
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
    p_expr_ColSym->pSymbol()->vColType(p_term_ColSym->pSymbol()->eColType());

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
    if (xKid.p_term()->bSymbol_Same(pSymbol))
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

  Card_Term* p_term =(new Card_Term) -> SYM(SF_STM_COL, i_Expr + 1);
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

  eSF eSymbol = Symbol::eEvalSymbol(p_expr_encasing->szSymbol(SF_NONE));
  RASSERT(SF_STM_COL == eSymbol);
  Card_Term* p_term_propagated = (new Card_Term) ->SYM(SF_STM_COL, p_expr_encasing->szSymbol(SF_NONE));
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
    Stz* szThis = p_term_this->szSymbol(SF_NONE);
    Card_Term* p_term_arg = p_rpnCalc_arg->pGetTerm(i_Child);
    Stz* szArg = p_term_arg->szSymbol(SF_NONE);

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

    Card_CmpConj* p_cmpConj = new Card_CmpConj;
    p_cmpConj->vTransferChildren(this);

    p_boolPred->vAppendChild(p_cmpConj);

    return p_boolPred;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_AndConj* p_andPred = nullptr;
  Card_CmpConj* p_cmpConj = nullptr;
  Card_NotConj* p_notPred = nullptr;
  Card_OrConj*  p_orPred  = nullptr;

  Card*  p_childA  = nullptr;
  Card*  p_childB  = nullptr;

  Card_RpnTerms*  p_TermSeries = new Card_RpnTerms;

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
        p_cmpConj = new Card_CmpConj;
        {
          eSF eSymbol = Symbol::eEvalSymbol(p_term_next->szSymbol(SF_NONE));
          RASSERT(SF_RLN_OP == eSymbol);
        }
        p_cmpConj->SYM(SF_RLN_OP,p_term_next->szSymbol(SF_NONE));
        p_cmpConj->vTransferChildren(p_TermSeries);
        RASSERT(p_cmpConj->bIsRelnOp())
        //p_cmpConj->vDump();
        if (p_cmpConj->bContains_callToSubQuery(pCI)) {
          p_cmpConj->vCard(CK_CMPCONJ);
          RASSERT(p_cmpConj->bIsRelnOp())
        }
        else
        if (!p_cmpConj->pSymbol()->bSame("=="_Stz)) { // todo: more work
          p_cmpConj->vCard(CK_CMPCONJ);
          RASSERT(p_cmpConj->bIsRelnOp())
        }
        //p_boolPred->vPushChild(p_cmpConj);
        p_boolPred->vAppendChild(p_cmpConj);
        p_cmpConj = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolPred->vDump(0,"COMPARE"_Stz);
        break;

      case TE_EXISTS:
      case TE_NOTEXISTS:
        // <boolPred> = <exists><boolPred>
        p_cmpConj = new Card_CmpConj;
        {
          eSF eSymbol = Symbol::eEvalSymbol(p_term_next->szSymbol(SF_NONE));
          RASSERT(SF_SQY_OP == eSymbol);
        }
        p_cmpConj->SYM(SF_SQY_OP,p_term_next->szSymbol(SF_NONE));
        p_cmpConj->vTransferChildren(p_TermSeries);
        RASSERT(p_cmpConj->bIsSqyOp())
        p_boolPred->vAppendChild(p_cmpConj);
        p_cmpConj = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolPred->vDump(0,"EXISTS"_Stz);
        break;

      case TE_IN:
        // <boolPred> = <in><boolPred>
        if (p_TermSeries->bIsInList()) {
          p_cmpConj = new Card_CmpConj;
          p_cmpConj->SYM(SF_SQY_OP,p_term_next->szSymbol(SF_NONE));
          p_cmpConj->vTransferChildren(p_TermSeries);
          p_boolPred->vAppendChild(p_cmpConj);
          RASSERT(p_cmpConj->bIsSqyOp())
          p_cmpConj = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolPred->vDump(0,"BOOLFN/IN"_Stz);
        }
        else {
          p_cmpConj = new Card_CmpConj;
          p_cmpConj->SYM(SF_SQY_OP,"in"_Stz);
          p_cmpConj->vTransferChildren(p_TermSeries);
          RASSERT(p_cmpConj->bIsSqyOp())
          p_boolPred->vAppendChild(p_cmpConj);
          p_cmpConj = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolPred->vDump(0,"IN"_Stz);
        }
        break;

      case TE_NOTIN:
        // <boolPred> = <in><boolPred>
        if (p_TermSeries->bIsInList()) {
          p_cmpConj = new Card_CmpConj;
          p_cmpConj->SYM(SF_SQY_OP,"in"_Stz);
          p_cmpConj->vTransferChildren(p_TermSeries);
          RASSERT(p_cmpConj->bIsBoolFn())
          p_boolPred->vAppendChild(p_cmpConj);
          p_cmpConj = nullptr;
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
          p_cmpConj = new Card_CmpConj;
          p_cmpConj->SYM(SF_SQY_OP,p_term_next->szSymbol(SF_NONE));
          p_cmpConj->vTransferChildren(p_TermSeries);
          RASSERT(p_cmpConj->bIsSqyOp())
          p_boolPred->vAppendChild(p_cmpConj);
          p_cmpConj = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolPred->vDump(0,"NOTIN"_Stz);
        }
        break;

      case TE_BOOLFN:
        p_cmpConj = new Card_CmpConj;
        p_cmpConj->SYM(SF_BOL_FN,p_term_next->szSymbol(SF_NONE));
        p_cmpConj->vTransferChildren(p_TermSeries);
        RASSERT(p_cmpConj->bIsBoolFn())
        p_boolPred->vAppendChild(p_cmpConj);
        p_cmpConj = nullptr;
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

    p_cmpConj = new Card_CmpConj;
    p_cmpConj->vTransferChildren(p_TermSeries);

    p_boolPred->vAppendChild(p_cmpConj);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCT Card_RpnCalc::eColType(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCT eColType = pSymbol()->eColType();
  return eColType;
}

/*----------------------------------------------------------------------------*/
/* convert stmCol to tblCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vX_stmCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xKid(this);
  while (xKid.bEach(CK_TERM)) {
    if (!xKid.p_term()->pSymbol()->bIs_Stm_Col())
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

  vValidate_mustHave(CK_TERM, CK_NONE);

  RASSERT(pParent->eCard() != CK_WHERE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
