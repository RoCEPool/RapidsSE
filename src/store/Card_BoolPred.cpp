/*============================================================================*/
/* Card_BoolPred.cpp -- Boolean Predicate                                     */
/*============================================================================*/

#include "Card_BoolPred.hpp"

#include "Card_CONJUNCTION.hpp"
#include "Card_RlnConj.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"
#include "Card_AndConj.hpp"
#include "Card_OuterColNm.hpp"
#include "Card_Filter.hpp"
#include "Card_Table.hpp"
#include "Card_Deck.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_BoolPred, CK_BOOLPRED)
__aNewFunction(Card_BoolPred, CN_CARD)
__vDeleteFunction(Card_BoolPred, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BoolPred::Card_BoolPred(void)
: Card_CONJSUBROOT(CK_BOOLPRED, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_BoolPred::~Card_BoolPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert <rpnCalc> to <boolPred>                                            */
/*----------------------------------------------------------------------------*/
Card_BoolPred* Card_BoolPred::pTo_boolPred(CardIter* pCI, Card_RpnCalc* p_rpnCalc)
{
  _static_

  Symbol xWhen(SF_WHEN,"WHEN"_Stz);

  Card_BoolPred* p_boolPred = new Card_BoolPred;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // does the rpn calc have a when in it?

  if (p_rpnCalc->bHasTerm(&xWhen)) {

    p_boolPred->vTransferChildren(p_rpnCalc);

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

  while (p_rpnCalc->nChildCards()) {
    Card_Term* p_term_next = _pCard_Term(p_rpnCalc->pPopChild());

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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vBump_stmCol(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJUNCTION* p_conj_ROOT = this->pConj_root();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [nBumpBy,bSkipFoldExpr](Card_CONJUNCTION* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);
    if (bSkipFoldExpr && p_term->pSymbol()->bSame("$1"_Stz))
      return C_CONTINUE_RECURSION;

    if (p_term->pSymbol()->bIs_Exp_Col()) {
      p_term->vBump_stmCol(nBumpBy);
      p_term->vMark1(MARK_CHANGED);
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
TypeId Card_BoolPred::xColTypeId(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return TypeId::xIdOf(CT_BOOLEAN);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CONJUNCTION* Card_BoolPred::pConj_root(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! nChildCards())
    return nullptr;

  Card_CONJUNCTION* p_conj = static_cast<Card_CONJUNCTION*>(pGetChild(0));

  return p_conj;
}
/*----------------------------------------------------------------------------*/
/* does this boolPred contain given term?                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bContains(Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_conj_ROOT = this->pConj_root();

  bool bDoesContain = false;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pSymbol_arg,&bDoesContain](Card_CONJUNCTION* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);

    if (p_term->pSymbol()->bMatches(pSymbol_arg)) {
      bDoesContain = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);

  return bDoesContain;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <conjCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vCopyMarkedConjToTable(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Deck* pDeck = pCI->G.pDeck;
  Card *pMainCard;

  pCI->vRecurseBegin(this);
  s32 sTimes;
  for (i8 nTimes = 0; Card_CONJUNCTION* p_conj = (Card_CONJUNCTION*)pCI->pRecurseEach(); nTimes++) {

    sTimes = nTimes;
    //p_conj->vDump(__func__,0,sTimes);

    if (! p_conj->bMark1(MARK_AT))
      continue;

    char cTableSyn = p_conj->cMark2();
    Card_Table *p_table = pDeck->pFindTableCard(cTableSyn, pMainCard);
    if (! p_table)
      continue;

    ((Card_OP *) pMainCard)->vAdd_Conj_Under_filter((Card_CONJUNCTION *) p_conj->pDupCardRecursive());

    // todo: is this needed?
    Card_Filter* p_filter = pMainCard->pSub_filter();
    p_filter->vMark1(p_table->cMark1());
    p_filter->vMark2(p_table->cMark2());

  }

  pCI->vRecurseEnd();

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vCorrectStmCol(Card_Emit* p_emitted_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [p_emitted_prior](Card* p_card)
  {
    if (p_card->pSymbol()->bIs_Exp_Col())
      _pCard_Term(p_card)->vCorrectStmCol(p_emitted_prior);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* remove given conjunction                                                   */
/*----------------------------------------------------------------------------*/
i8 Card_BoolPred::nCullConj(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nCulled = 0;

  if (! this->nChildCards())
    return 0;

  Card_CONJUNCTION* pCard_conj = this->pConj_root();

  while (pCard_conj) {

    if (!bCullConj(pCard_conj, szWithMark))
      break;

    nCulled++;

    if (! this->nChildCards())
      break;

    pCard_conj = static_cast<Card_CONJUNCTION*>(this->pGetChild(0));
  }

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/* remove given conjunction                                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bCullConj(Card_CONJUNCTION* p_conj, Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard_conj = p_conj->eCard();

  if (CK_TERM == eCard_conj)
    return false;

  if (CK_ANDCONJ == eCard_conj) {
    if (p_conj->bMark1(szWithMark)) {
      this->vRemove_conj(p_conj);
      delete p_conj;
      return true;
    }
  }
  else
  if (CK_ORCONJ == eCard_conj) {
    if (p_conj->bMark1(szWithMark)) {
      this->vRemove_conj(p_conj);
      delete p_conj;
      return true;
    }
  }
  else
  if (CK_RLNCONJ == eCard_conj) {
    if (p_conj->bMark1(szWithMark)) {
      this->vRemove_conj(p_conj);
      delete p_conj;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDCONJ == eCard_conj || CK_ORCONJ == eCard_conj || CK_NOTCONJ == eCard_conj);

  i8 nChildren = p_conj->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CONJUNCTION* pCard_conj_child = static_cast<Card_CONJUNCTION*>(p_conj->pGetChild(i_Child));
    if (this->bCullConj(pCard_conj_child, szWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* delete facet if                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  auto fvDeleteFacetIf = [eFacet,eAlsoFacet,cTblSyn](Card* pCard_term)
  {
    Card_Term* p_term = _pCard_Term(pCard_term);

    p_term->vDeleteFacetIf(eFacet,eAlsoFacet,cTblSyn);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,fvDeleteFacetIf);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* ensure all terms have a type                                               */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vEnsureTypesFor_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_conj = static_cast<Card_CONJUNCTION*>(pGetChild(0));

  if (p_conj)
    p_conj->vEnsureTypesFor_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* inspect <boolPred>> and mark any <conj> which could be moved               */
/*----------------------------------------------------------------------------*/
Card_CONJUNCTION* Card_BoolPred::pFindRemove_conj_corr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_conj_corr =
      static_cast<Card_CONJUNCTION*>(pFindPpyRecursive(P_CrlRslTest, CK_RLNCONJ));

  if (! p_conj_corr)
    return nullptr;

  vRemove_conj(p_conj_corr);

  return p_conj_corr;
}

/*----------------------------------------------------------------------------*/
/* is this a compound boolPred                                                */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound boolPred                                                */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsLiterals(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  bool bLiterals = false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  auto fbTestLiterals = [&bLiterals](Card* p_card)
  {
    Card_Term* p_term = _pCard_Term(p_card);

    if (p_term->bIsColSym()) {
      bLiterals = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,fbTestLiterals);
  UNUSED(bIgnore);

  return bLiterals;
}

/*----------------------------------------------------------------------------*/
/* is this boolPred a simple expression of a column                           */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsScalarCol(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card *p_card_0 = pGetChild(0);
  if (!p_card_0->bIsCard(CK_TERM))
    return false;

  Card_Term *p_term_0 = _pCard_Term(p_card_0);
  if (!p_term_0->bIsTblCol())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this boolPred a simple expression of a key                              */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsScalarKey(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card *p_card_0 = pGetChild(0);
  if (!p_card_0->bIsCard(CK_TERM))
    return false;

  Card_Term *p_term_0 = _pCard_Term(p_card_0);
  if (!p_term_0->bIsKeyCol())
    return false;

  return true;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bHasTerm(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bHas = false;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pSymbol, &bHas](Card* p_card)
  {
    if (p_card->pSymbol()->bMatches(pSymbol)) {
      bHas = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,bLambdaFn);
  UNUSED(bIgnore);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* inspect <boolPred>> and mark any <conj> which could be moved               */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vMarkWrt_table(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->vRecurseBegin(this);
  while (Card* p_card = pCI->pRecurseEach()) {

    //p_card->vDump(__func__);

    if (p_card->bIsCard(CK_NOTCONJ)) {
      p_card->vMark1(MARK_NOT);
      pCI->vDontDescend(true);
      continue;
    }
    if (p_card->bIsCard(CK_ANDCONJ)) {
      p_card->vMark1(MARK_AND);
      continue;
    }

    if (p_card->bIsCard(CK_RLNCONJ)) {
      p_card->vMark1(MARK_COMPARE);
      pCI->vDontDescend(true);
      if (((Card_CONJUNCTION *)p_card)->bContains_callToSubQuery(pCI))
        continue;
      Symbol* pWhichTable = ((Card_CONJUNCTION *) p_card)->pTable_WhichTableFor_Conj(pCI);
      if (pWhichTable) {
        p_card->vMark1(MARK_AT);
        p_card->vMark2(pWhichTable->cSF_char());
      }
      continue;
    }

    if (p_card->bIsCard(CK_ORCONJ)) {
      p_card->vMark1(MARK_OR);
      pCI->vDontDescend(true);
      continue;
    }


    if (p_card->bIsCard(CK_TERM))
      continue;

    NEVER_REACH
  }

  pCI->vRecurseEnd();

}

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vPush_CrlKeyConj_As_CrlKeyTest(Card_CONJUNCTION *p_conj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  p_conj->pSymbol()->Y_vCut(P_CrlKeyPred);
  p_conj->pSymbol()->Y_vSet(P_CrlKeyTest);

  if (this->nChildCards()) {
    this->vPushConj(p_conj);
  }
  else {
    this->vAppendChild(p_conj);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vPushConj(Card_CONJUNCTION* p_conj_toBeAdded)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJUNCTION* p_conj_oldTop = static_cast<Card_CONJUNCTION*>(this->pPopChild());
  RASSERT(! this->nChildCards());

  Card_AndConj* p_andPred_new = new Card_AndConj;
  p_andPred_new->vAppendChild(p_conj_oldTop);
  p_andPred_new->vAppendChild(p_conj_toBeAdded);

  this->vPushChild(p_andPred_new);
}

/*----------------------------------------------------------------------------*/
/* remove given conjunction                                                     */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vRemove_conj(Card_CONJUNCTION* p_conj)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pParentCard = this->pFindParentRecursive(p_conj);

  pParentCard->vRemoveSub(p_conj);

  if (CK_ANDCONJ == pParentCard->eCard() || CK_ORCONJ == pParentCard->eCard()) {

    if (0 == pParentCard->nChildCards()) {
      this->vRemoveSub(pParentCard);
    }
    else {
      Card* pSiblingCard = pParentCard->pGetChild(0);
      Card* pGrandCard = this->pFindParentRecursive((pParentCard));
      pParentCard->vRemoveSub(pSiblingCard);
      pGrandCard->vReplaceChild(pParentCard, pSiblingCard);
    }

    delete pParentCard;
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_RlnConj* Card_BoolPred::pRlnConj_whichCallsSubQuery(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RlnConj* pCmpConj = nullptr;

  auto bLambdaFn = [&pCmpConj,pCI](Card_CONJUNCTION* p_conj, bool bUpwards)
  {
    UNUSED(bUpwards);

    if (p_conj->bContains_callToSubQuery(pCI)) {
      pCmpConj = _pCard_RlnConj(p_conj);
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJUNCTION* p_conj_ROOT = this->pConj_root();

  bool bIgnore = p_conj_ROOT->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);

  return pCmpConj;
}

/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <boolPred>                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vRpn_boolPred(String* psRpnExpr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsBraced())
    psRpnExpr->vConcat(" }"_Stz);

  vRpn_boolPred_conj(psRpnExpr, this);
}

/*----------------------------------------------------------------------------*/
/* append part                                                                */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vRpn_boolPred_conj(String* psRpnExpr,
                                       Card_CONJSUBROOT *p_CONJSUBROOT)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(p_CONJSUBROOT); xKid.bEach(); /**/) {
    Card *pChild = xKid.pChild();

    eCK eChild = pChild->eCard();
    switch (eChild) {
      case CK_ANDCONJ:
        vRpn_boolPred_conj(psRpnExpr, _pCard_AndConj(pChild));
        psRpnExpr->vCatSprint(" and"_Stz);
        break;
      case CK_NOTCONJ:
        vRpn_boolPred_conj(psRpnExpr, _pCard_NotConj(pChild));
        psRpnExpr->vCatSprint(" not"_Stz);
        break;
      case CK_ORCONJ:
        vRpn_boolPred_conj(psRpnExpr, _pCard_OrConj(pChild));
        psRpnExpr->vCatSprint(" or"_Stz);
        break;
      case CK_RLNCONJ:
        vRpn_boolPred_conj(psRpnExpr, _pCard_RlnConj(pChild));
        psRpnExpr->vCatSprint(" %s"_Stz, pChild->szFacet(SF_NONE));
        break;
      case CK_TERM:
        if (pChild->pSymbol()->bIs_Exp_Col() ||
            pChild->pSymbol()->bIs_Tbl_Col()) {

          // TODO: this rule should be in validate?
          if (!pChild->pSymbol()->bHasType()) {
            //p_part->vDump();
            //pChild->vDump();
            RASSERT(pChild->pSymbol()->bHasType());
          }
        }
        psRpnExpr->vCatSprint(" %s"_Stz, pChild->szFacet(SF_NONE));
        break;
      default:
        NEVER_REACH
    }
  }
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! nChildCards())
    return;

  Card_CONJUNCTION* p_CONJUNCTION = static_cast<Card_CONJUNCTION*>(pGetChild(0));

  p_CONJUNCTION->vShortForm(pString);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bIsBraced(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bBraced = false;
  auto bTestBraced=[&bBraced](Card* p_card) {

    if (p_card->pSymbol()->bNeedsBrace()) {
      bBraced = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  bApplyRecursive(CK_NONE,bTestBraced);

  return bBraced;
}

/*----------------------------------------------------------------------------*/
/* remove given conjunction                                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bYank_conj(Stz* szWithMark, Card_CONJUNCTION*& p_conj_yanked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  p_conj_yanked = nullptr;

  if (! this->nChildCards()) {
    RASSERT(! p_conj_yanked);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  auto bYank = [](Card_BoolPred* p_boolPred,
                  Stz* szWithMark,
                  Card* p_test,
                  Card_CONJUNCTION*& p_conj_yanked,
                  auto&& bYank /*oddity permits recursive lambda*/)
  {
    //p_test->vDump();

    if (Card_CONJUNCTION::bIsConj(p_test)) {
      if (p_test->bMark1(szWithMark)) {
        p_conj_yanked = static_cast<Card_CONJUNCTION*>(p_test);
        p_boolPred->vRemove_conj(p_conj_yanked);
        RASSERT(p_conj_yanked);
        return true;
      }
    }

    Kid xKid(p_test);
    while (xKid.bEach()) {
      if (bYank(p_boolPred,szWithMark,xKid.pChild(),p_conj_yanked,bYank)) {
        RASSERT(p_conj_yanked);
        return true;
      }
    }

    RASSERT(! p_conj_yanked);
    return false;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Kid xKid(this);
  while (xKid.bEach()) {
    if (bYank(this, szWithMark, xKid.pChild(), p_conj_yanked, bYank)) {
      RASSERT(p_conj_yanked);
      return true;
    }
  }

  RASSERT(! p_conj_yanked);
  return false;
}

/*----------------------------------------------------------------------------*/
/* remove given conjunction                                                   */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bYank_conj(Stz* szWithMark, Card* p_test,
                               Card_CONJUNCTION*& p_conj_yanked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_CONJUNCTION::bIsConj(p_test)) {
    if (p_test->bMark1(szWithMark)) {
      p_conj_yanked = static_cast<Card_CONJUNCTION*>(p_test);
      this->vRemove_conj(p_conj_yanked);
      RASSERT(p_conj_yanked);
      return true;
    }
  }

  Kid xKid(this);
  while (xKid.bEach()) {
    if (this->bYank_conj(szWithMark, xKid.pChild(), p_conj_yanked)) {
      RASSERT(p_conj_yanked);
      return true;
    }
  }

  RASSERT(! p_conj_yanked);
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vX_stmCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_conj_ROOT = this->pConj_root();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_CONJUNCTION* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);
    Symbol* pSymbol = p_term->pSymbol();
    if (!pSymbol->bIs_Exp_Col())
      return C_CONTINUE_RECURSION;

    p_term->vX_stmCol_To_tblCol(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_ROOT->bConjRecursive(bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vX_tblCol_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_conj_conj = this->pConj_root();
  if (! p_conj_conj)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfTo_stmCol = [pCI](Card_CONJUNCTION* p_conj, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_conj->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_conj);
    Symbol* pSymbol = p_term->pSymbol();
    if (!pSymbol->bIs_Tbl_Col())
      return C_CONTINUE_RECURSION;

    p_term->vX_tblCol_To_stmCol(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_conj_conj->bConjRecursive(bfTo_stmCol);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

//  Card::vValidate_hasChildren();
  if (nChildCards())
    Card::eValidate_mustSome(CK_ANDCONJ,CK_NOTCONJ,CK_ORCONJ,CK_RLNCONJ,
                               CK_TERM,CK_NONE);

  (void)Card::eValidate_parentEither(pCI,CK_CONSTRAINT,CK_FILTER,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
