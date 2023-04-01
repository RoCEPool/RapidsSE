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
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_RpnCalc::~Card_RpnCalc(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vAppendRpnCalc(STORE::Card_RpnCalc *p_rpnCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bAddAnd = false;
  if (nChildCards())
    bAddAnd = true;

  vTransferChildren(p_rpnCalc);

  if (bAddAnd) {
    Card_Term *p_term_and = new Card_Term("and"_Stz);
    vAppend(p_term_and);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {

    if (SymVal::bIsStreamColOrdinal(xCI.G.p_term->szValue())) {
      xCI.G.p_term->vBump_StreamColOrdinal(nBumpBy);
      xCI.G.p_term->vMark1(MARK_CHANGED);
    }
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

    if (xCI.G.p_term->bSame("exists"_Stz)) {

      RASSERT(xCI.G_bEachMain(CK_TERM));
      Card_Term* p_term_next = xCI.G.p_term;
      if (p_term_next->bSame("not"_Stz)) {
        xCI.vRewind();
        this->vRemoveChild(p_term_next);
        p_term->vValue("notExists"_Stz);
        continue;
      }
      continue;
    }

    if (xCI.G.p_term->bSame("in"_Stz)) {

      RASSERT(xCI.G_bEachMain(CK_TERM));
      Card_Term* p_term_next = xCI.G.p_term;
      if (p_term_next->bSame("not"_Stz)) {
        xCI.vRewind();
        this->vRemoveChild(p_term_next);
        p_term->vValue("notIn"_Stz);
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
void Card_RpnCalc::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {
    xCI.G.p_term->vConvert_stmColOdn_To_tblColOdn(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* get the one (and only) stmColOdn from rpnCalc                              */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RpnCalc::pGetSingle_stmColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (1 != this->nChildCards())
    return nullptr;

  Card_Term* p_term = this->pSub_term();
  Stz* szTerm = p_term->szValue();
  if (! SymVal::bIsStreamColOrdinal(szTerm))
    return nullptr;

  return p_term;
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
bool Card_RpnCalc::bIsBraced(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {
    if (xCI.G.p_term->bNeedsBrace())
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsSame(Card_RpnCalc* p_rpnCalc_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();

  if (nChildren != p_rpnCalc_arg->nChildCards())
    return false;

  for (i8 i_Child = 0; i_Child != nChildren; i_Child++) {
    Card_Term* p_term_this = this->pGetTerm(i_Child);
    Stz* szThis = p_term_this->szValue();
    Card_Term* p_term_arg = p_rpnCalc_arg->pGetTerm(i_Child);
    Stz* szArg = p_term_arg->szValue();

    if (szThis->bSame(szArg))
      continue;

    Stz* szArg_tblCol = p_term_arg->Y_szGet(P_TblCol);
    if (szArg_tblCol && szThis->bSame(szArg_tblCol))
      continue;

    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bIsSingleCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  if (1 != nChildren)
    return false;

  Card* pChild = pGetChild(0);
  Stz* szVal = pChild->szValue();

  if (SymVal::bIsColOrdinal(szVal))
    return true;

  return true;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_RpnCalc::bHasWhen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {
    if (xCI.G.p_term->bSame("WHEN"_Stz))
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
/* get the one (and only) projected column from rpnCalc                       */
/*----------------------------------------------------------------------------*/
Stz* Card_RpnCalc::szProjectedCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Term* p_term_0 = this->pProjectedTerm();
  if (! p_term_0)
    return nullptr;

  Stz* szVal = p_term_0->szValue();

  return szVal;
}

/*----------------------------------------------------------------------------*/
/* get the one (and only) projected column from rpnCalc                       */
/*----------------------------------------------------------------------------*/
Card_Term* Card_RpnCalc::pProjectedTerm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (1 != this->nChildCards())
    return nullptr;

  Card_Term* p_term_0 = this->pGetTerm(0);

  if (p_term_0->bIsKeyOdn()    ||
      p_term_0->bIsStmColOdn() ||
      p_term_0->bIsTblColOdn())
    return p_term_0;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vFlipTerms(Card_Projection* p_projection_needed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI_this(this);
  while (xCI_this.G_bEachMain(CK_TERM))
    xCI_this.G.p_term->vFlip(p_projection_needed);
}

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolCalc>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolCalc* Card_RpnCalc::pToBoolCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolCalc* p_boolCalc = new Card_BoolCalc;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the rpn calc have a when in it?

  if (this->bHasWhen()) {

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

    eTE eTerm_next = p_term_next->eTermEval();

    //vTrace("%s %s\n"_Stz, aTE[eTerm_next], p_term_next->szValue());

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
        p_cmpPred->vValue(p_term_next->szValue());
        p_cmpPred->vTransferChildren(p_TermSeries);
        RASSERT(p_cmpPred->bIsRelnOp())
        //p_cmpPred->vDump();
        if (p_cmpPred->bContains_callToSubQuery(pCI)) {
          p_cmpPred->vCard(CK_CMPPRED);
          RASSERT(p_cmpPred->bIsRelnOp())
        }
        else
        if (! p_cmpPred->bIsValue("=="_Stz)) { // todo: more work
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
        p_cmpPred->vValue(p_term_next->szValue());
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
          p_cmpPred->vValue(p_term_next->szValue());
          p_cmpPred->vTransferChildren(p_TermSeries);
          p_boolCalc->vAppend(p_cmpPred);
          RASSERT(p_cmpPred->bIsSubQueryOp())
          p_cmpPred = nullptr;
          DELETE_CARD(p_term_next);
          //p_boolCalc->vDump(0,"BOOLFN/IN"_Stz);
        }
        else {
          p_cmpPred = new Card_CmpPred;
          p_cmpPred->vValue("in"_Stz);
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
          p_cmpPred->vValue("in"_Stz);
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
          p_cmpPred->vValue(p_term_next->szValue());
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
        p_cmpPred->vValue(p_term_next->szValue());
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
        if (StrLib::bBeginsWith(p_term_next->szValue()->azChar(), "__"))
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
    p_boolCalc->vDump(0,"boolCalc"_Stz);
    p_TermSeries->vDump(0,"termSeries"_Stz);

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
/* remove any term with matching value                                        */
/*----------------------------------------------------------------------------*/
i8 Card_RpnCalc::nTrimTerm(Stz* szValue)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nTerms = 0;
  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_TERM)) {
    if (xCI.G.p_MainCard->bIsValue(szValue)) {
      xCI.G_vRemoveDeleteMain(CK_TERM);
      nTerms++;
    }
  }

  return nTerms;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_RpnCalc::vTypes_For_stmColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_TERM); /**/) {

    if (xKid.p_term()->Y_bHas(P_Type))
      continue;

    if (SymVal::bIsKeyOrdinal(xKid.p_term()->szValue())) {

      Card_Expr *p_expr_found =
          pCI->G.p_key->pGetExpr(xKid.p_term()->iGetOdnNo() - 1);
      xKid.p_term()->Y_vSet(P_Type, szCT_char[Card::eTo_eCT(p_expr_found->pSub_type()->szValue())]);

      continue;
    }

    if (SymVal::bIsStreamColOrdinal(xKid.p_term()->szValue())) {

      Card_Expr *p_expr_found =
          pCI->Analysis.m_pProjection_prior->pGetExpr(xKid.p_term()->iGetOdnNo() - 1);
      xKid.p_term()->Y_vSet(P_Type, szCT_char[Card::eTo_eCT(p_expr_found->pSub_type()->szValue())]);

      continue;
    }
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_RpnCalc::szP_Type(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szP_Type = Y_szGet(P_Type);
  if (szP_Type)
    return szP_Type;

  return szCT_char[CT_NONE];
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
