/*============================================================================*/
/* Card_RpnCalc.cpp -- RPN Calculation Sequence                               */
/*============================================================================*/

#include "Card_RpnCalc.hpp"
#include "Card_Term.hpp"
#include "Card_BoolCalc.hpp"
#include "CardIter.hpp"
#include "Card_RpnTerms.hpp"
#include "Card_AndPred.hpp"
#include "Card_OrPred.hpp"
#include "Card_CmpPred.hpp"
#include "Card_NotPred.hpp"

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
: Card(CK_RPNCALC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_RpnCalc::Card_RpnCalc(Symbol* pSymbol)
: Card(CK_RPNCALC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term = new Card_Term(pSymbol);
  vAppend(p_term);

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
void Card_RpnCalc::vBump_stmOdn(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {

    if (xCI.G.p_term- pSymbol()->bIsStreamOrdinal()) {
      xCI.G.p_term->vBump_stmOdn(nBumpBy);
      xCI.G.p_term->vMark1(MARK_CHANGED);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCT Card_RpnCalc::eColumnType(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Stz *szType = pSymbol()->szColType()) {
    eCT eColType = Symbol::eTo_eCT_char(szType);
    return eColType;
  }

  return CT_NONE;
}

Card_RpnCalc* Card_RpnCalc::pClone(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return _pCard_RpnCalc(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vConcat(STORE::Card_RpnCalc *p_rpnCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bAppendAnd = false;
  if (nChildCards())
    bAppendAnd = true;

  vTransferChildren(p_rpnCalc);

  if (bAppendAnd) {
    Card_Term *p_term_and = new Card_Term("and"_Stz);
    vAppend(p_term_and);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vConvert_sqyOp_not_To_notSqyOp(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {

    Card_Term* p_term = xCI.G.p_term;

    if (! xCI.bMore())
      break;

    if (xCI.G.p_term->bSameSymbol("exists"_Stz)) {

      RASSERT(xCI.G_bEachMain(CK_TERM));
      Card_Term* p_term_next = xCI.G.p_term;
      if (p_term_next->bSameSymbol("not"_Stz)) {
        xCI.vRewind();
        this->vRemoveChild(p_term_next);
        p_term->vSymbol("notExists"_Stz);
        continue;
      }
      continue;
    }

    if (xCI.G.p_term->bSameSymbol("in"_Stz)) {

      RASSERT(xCI.G_bEachMain(CK_TERM));
      Card_Term* p_term_next = xCI.G.p_term;
      if (p_term_next->bSameSymbol("not"_Stz)) {
        xCI.vRewind();
        this->vRemoveChild(p_term_next);
        p_term->vSymbol("notIn"_Stz);
        DELETE_CARD(p_term_next);
        continue;
      }
      continue;
    }

  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vConvertAll_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {
    xCI.G.p_term->vConvertIf_stmOdn_To_colOdn(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vCorrectStmOdn(Card_Projection* p_projection_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    xKid.p_term()->vCorrectStmOdn(p_projection_prior);
}

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vEnsureTypesFor_stmOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr *p_expr_found;

  // rpnCalc is a set of terms
  for (Kid xTerm(this); xTerm.bEach(CK_TERM); /**/) {
    if (xTerm.p_term()->pSymbol()->Y_bHas(P_Type))
      continue;

    i8 iOdn = xTerm.iGetOdn();

    if (xTerm.p_term()->bIsKeyOdn())
      p_expr_found = pCI->G.p_key->pGetExpr(iOdn - 1);
    else
    if (xTerm.p_term()->bIsStmOdn())
      p_expr_found = pCI->Prior.m_pProjection->pGetExpr(iOdn - 1);
    else
      continue;

    Card_RpnCalc *p_rpnCalc_found = p_expr_found->pSub_rpnCalc();

    xTerm.p_term()->vColType(p_rpnCalc_found);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vFactorAll_valOdn_IntoDistinct_expr(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(! pCI->G.p_subSet);

  Card_SubSet* p_subSet = new Card_SubSet;

  // an rpnCalc is a linear sequence of terms, some of which are columns
  for (Kid x_term(this); x_term.bEach(CK_TERM); /**/) {

     if (!x_term.p_term()->bIsValOdn())
      continue;

    Card_Term* p_term_valOdn = x_term.p_term();

    Card_Expr* p_expr_valOdn = new Card_Expr(p_term_valOdn->pSymbol());
    p_expr_valOdn->pSymbol()->Y_vSet(P_Type,p_term_valOdn->pSymbol()->Y_szGet(P_Type));

    p_subSet->vAppend(p_expr_valOdn);
  }

  pCI->G.p_subSet = p_subSet;
}

/*----------------------------------------------------------------------------*/
/* return first key col in this rpn sequence                                  */
/*----------------------------------------------------------------------------*/
Symbol* Card_RpnCalc::pFirstKeyCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/)
    if (xKid.p_term()->bIsKeyOdn())
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
    if (xKid.p_term()->bIsStmOdn())
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
    if (xKid.p_term()->bIsColOdn())
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

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {
    if (xCI.G.p_term->bSameSymbol(pSymbol->szSF__print()))
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
/* is this rpnCalc a simple expression of a column ?                          */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsScalarCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card_Term* p_term_0 = pGetTerm(0);
  if (! p_term_0->bIsValOdn())
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
  if (! p_term_0->bIsKeyOdn())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bNeedsBrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {
    if (xCI.G.p_term->pSymbol()->bNeedsBrace())
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vOntoStzList(StzList* pStzList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pStzList->vAppend(":"_Stz);

  ChildCards::vOntoStzList(pStzList);
}

/*----------------------------------------------------------------------------*/
/* return a reduced copy of this rpnCalc                                      */
/*----------------------------------------------------------------------------*/
Card_RpnCalc * Card_RpnCalc::pReducedCopy(i8 i_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (1 == nChildCards()) {
    Card_RpnCalc* p_rpnCalc_reduced = pClone();
    return p_rpnCalc_reduced;
  }

  Card_RpnCalc* p_rpnCalc_reduced = new Card_RpnCalc;
  p_rpnCalc_reduced->vColType(this);

  Symbol xExprNo;
  xExprNo.vSetStreamOrdinal(i_Expr + 1);

  Card_Term* p_term = new Card_Term(xExprNo.szSF__print());
  p_term->vColType(this);

  p_rpnCalc_reduced->vAppend(p_term);

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

  Card_Term* p_term_propagated = new Card_Term(p_expr_encasing->szSymbol());
  p_term_propagated->vColType(this);

  p_rpnCalc_propagated->vAppend(p_term_propagated);

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
    Stz* szThis = p_term_this->szSymbol();
    Card_Term* p_term_arg = p_rpnCalc_arg->pGetTerm(i_Child);
    Stz* szArg = p_term_arg->szSymbol();

    if (szThis->bSame(szArg))
      continue;

    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolCalc>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolCalc* Card_RpnCalc::pToBoolCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Symbol xWhen("WHEN"_Stz,SF_OTHER);

  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the rpn calc have a when in it?

  if (this->bHasTerm(&xWhen)) {

    Card_CmpPred* p_cmpPred = new Card_CmpPred;
    p_cmpPred->vTransferChildren(this);

    p_boolCalc->vAppend(p_cmpPred);

    return p_boolCalc;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Stz* szSqyNm;
  Card_AndPred* p_andPred = nullptr;
  Card_CmpPred* p_cmpPred = nullptr;
  Card_NotPred* p_notPred = nullptr;
  Card_OrPred*  p_orPred  = nullptr;

  Card*  p_childA  = nullptr;
  Card*  p_childB  = nullptr;

  bool bSawSqy = false;

  Card_Term*    p_subQueryNm = nullptr;

  Card_RpnTerms*  p_TermSeries = new Card_RpnTerms;

  while (this->nChildCards()) {
    Card_Term* p_term_next = _pCard_Term(this->pPop());

    eTE eTerm_next = p_term_next->eEvalTerm();

    //vTrace("%s %s\n"_Stz, aTE[eTerm_next], p_term_next->szSF__print());

    switch (eTerm_next) {

      case TE_AND:
        // <boolCalc> = <and>/<boolCalc>
        p_childB = p_boolCalc->pPlopChild();
        p_childA = p_boolCalc->pPlopChild();

        p_andPred = new Card_AndPred;
        p_andPred->vAppend(p_childA);
        p_andPred->vAppend(p_childB);

        p_boolCalc->vAppend(p_andPred);
        p_andPred = nullptr;
        p_childA  = nullptr;
        p_childB  = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"AND"_Stz);
        break;

      case TE_OR:
        // <boolCalc> = <or>/<boolCalc>
        p_childB = p_boolCalc->pPlopChild();
        p_childA = p_boolCalc->pPlopChild();

        p_orPred = new Card_OrPred;
        p_orPred->vAppend(p_childA);
        p_orPred->vAppend(p_childB);

        p_boolCalc->vAppend(p_orPred);
        p_orPred = nullptr;
        p_childA = nullptr;
        p_childB = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"OR"_Stz);
        break;

      case TE_NOT:
        p_childA = p_boolCalc->pPlopChild();

        p_notPred = new Card_NotPred;
        p_notPred->vAppend(p_childA);

        p_boolCalc->vAppend(p_notPred);
        p_notPred = nullptr;
        p_childA  = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"NOT"_Stz);
        break;

      case TE_RELNOP:
        // <boolCalc> = <cmpPred><termSeries><boolCalc>
        RASSERT(p_TermSeries->nChildCards() > 0);
        p_cmpPred = new Card_CmpPred;
        p_cmpPred->vSymbol(p_term_next->szSymbol());
        p_cmpPred->vTransferChildren(p_TermSeries);
        RASSERT(p_cmpPred->bIsRelnOp())
        //p_cmpPred->vDump();
        if (p_cmpPred->bContains_callToSubQuery(pCI)) {
          p_cmpPred->vCard(CK_CMPPRED);
          RASSERT(p_cmpPred->bIsRelnOp())
        }
        else
        if (!p_cmpPred->bSameSymbol("=="_Stz)) { // todo: more work
          p_cmpPred->vCard(CK_CMPPRED);
          RASSERT(p_cmpPred->bIsRelnOp())
        }
        //p_boolCalc->vPush(p_cmpPred);
        p_boolCalc->vAppend(p_cmpPred);
        p_cmpPred = nullptr;
        DELETE_CARD(p_term_next);
        bSawSqy = false;
        //p_boolCalc->vDump(0,"COMPARE"_Stz);
        break;

      case TE_EXISTS:
      case TE_NOTEXISTS:
        // <boolCalc> = <exists><boolCalc>
        p_cmpPred = new Card_CmpPred;
        p_cmpPred->vSymbol(p_term_next->szSymbol());
        p_cmpPred->vTransferChildren(p_TermSeries);
        RASSERT(p_cmpPred->bIsSubQueryOp())
        p_boolCalc->vAppend(p_cmpPred);
        p_cmpPred = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"EXISTS"_Stz);
        break;

      case TE_IN:
        // <boolCalc> = <in><boolCalc>
        if (p_TermSeries->bIsInList()) {
          p_cmpPred = new Card_CmpPred;
          p_cmpPred->vSymbol(p_term_next->szSymbol());
          p_cmpPred->vTransferChildren(p_TermSeries);
          p_boolCalc->vAppend(p_cmpPred);
          RASSERT(p_cmpPred->bIsSubQueryOp())
          p_cmpPred = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolCalc->vDump(0,"BOOLFN/IN"_Stz);
        }
        else {
          p_cmpPred = new Card_CmpPred;
          p_cmpPred->vSymbol("in"_Stz);
          p_cmpPred->vTransferChildren(p_TermSeries);
          RASSERT(p_cmpPred->bIsSubQueryOp())
          p_boolCalc->vAppend(p_cmpPred);
          p_cmpPred = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolCalc->vDump(0,"IN"_Stz);
        }
        break;

      case TE_NOTIN:
        // <boolCalc> = <in><boolCalc>
        if (p_TermSeries->bIsInList()) {
          p_cmpPred = new Card_CmpPred;
          p_cmpPred->vSymbol("in"_Stz);
          p_cmpPred->vTransferChildren(p_TermSeries);
          RASSERT(p_cmpPred->bIsBoolFn())
          p_boolCalc->vAppend(p_cmpPred);
          p_cmpPred = nullptr;
          DELETE_CARD(p_term_next);

          p_childA = p_boolCalc->pPlopChild();
          p_notPred = new Card_NotPred;
          p_notPred->vAppend(p_childA);
          p_boolCalc->vAppend(p_notPred);
          p_notPred = nullptr;
          p_childA  = nullptr;
          //p_boolCalc->vDump(0,"BOOLFN/NOTIN"_Stz);
        }
        else {
          p_cmpPred = new Card_CmpPred;
          p_cmpPred->vSymbol(p_term_next->szSymbol());
          p_cmpPred->vTransferChildren(p_TermSeries);
          RASSERT(p_cmpPred->bIsSubQueryOp())
          p_boolCalc->vAppend(p_cmpPred);
          p_cmpPred = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolCalc->vDump(0,"NOTIN"_Stz);
        }
        break;

      case TE_BOOLFN:
        p_cmpPred = new Card_CmpPred;
        p_cmpPred->vSymbol(p_term_next->szSymbol());
        p_cmpPred->vTransferChildren(p_TermSeries);
        RASSERT(p_cmpPred->bIsBoolFn())
        p_boolCalc->vAppend(p_cmpPred);
        p_cmpPred = nullptr;
        DELETE_CARD(p_term_next);
        //p_boolCalc->vDump(0,"BOOLFN"_Stz);
        break;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // <termSeries>

      case TE_EXECUTE:
        DELETE_CARD(p_term_next);
        break;

      case TE_OTHER:
        if (StrLib::bBeginsWith(p_term_next->szSymbol()->azChar(), "__"))
          bSawSqy = true;
        p_TermSeries->vAppend(p_term_next);
        //p_TermSeries->vDump(0,"TERM_SERIES"_Stz);
        break;

      default:
        NEVER_REACH;
        break;
    }
  }

  if (p_TermSeries->nItems()) {
    RASSERT(0 == p_boolCalc->nChildCards());
    //p_boolCalc->vDump(0,"boolCalc"_Stz);
    //p_TermSeries->vDump(0,"termSeries"_Stz);

    p_cmpPred = new Card_CmpPred;
    p_cmpPred->vTransferChildren(p_TermSeries);

    p_boolCalc->vAppend(p_cmpPred);
  }

  RASSERT(0 == p_TermSeries->nChildCards());
  delete p_TermSeries;

  //p_boolCalc->vDump();

  return p_boolCalc;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_RpnCalc::szP_Type(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szP_Type = pSymbol()->Y_szGet(P_Type);
  if (szP_Type)
    return szP_Type;

  return szCT_char[CT_NONE];
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_TERM, CK_NONE);

  Kid xRpnCalc(this);
  while (xRpnCalc.bEach(CK_TERM)) {
    Card_Term *p_term = xRpnCalc.p_term();
    p_term->vValidate(pCI, iDepth + 1, this);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
