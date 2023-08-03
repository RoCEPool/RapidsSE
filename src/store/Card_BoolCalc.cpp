/*============================================================================*/
/* Card_BoolCalc.cpp -- Boolean Calculation                                   */
/*============================================================================*/

#include "Card_BoolCalc.hpp"

#include "Card_PRED.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"
#include "Card_AndPred.hpp"
#include "Card_OuterColNm.hpp"
#include "Card_Filter.hpp"
#include "Card_Table.hpp"
#include "Card_Deck.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_BoolCalc, CK_BOOLCALC)
__aNewFunction(Card_BoolCalc, CN_CARD)
__vDeleteFunction(Card_BoolCalc, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BoolCalc::Card_BoolCalc(void)
: Card(CK_BOOLCALC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_BoolCalc::~Card_BoolCalc(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_BoolCalc::pCmpPred_whichCallsSubQuery(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CmpPred* pCmpPred = nullptr;

  auto bLambdaFn = [&pCmpPred,pCI](Card_PRED* p_pred, bool bUpwards)
  {
    if (p_pred->bContains_callToSubQuery(pCI)) {
      pCmpPred = _pCard_CmpPred(p_pred);
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_PRED* p_pred_ROOT = this->pRootPred();

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);

  return pCmpPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vBump_stmOdn(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* p_pred_ROOT = this->pRootPred();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,nBumpBy,bSkipFoldExpr](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (bSkipFoldExpr && p_term->bSameSymbol("$1"_Stz))
      return C_CONTINUE_RECURSION;

    if (p_term->pSymbol()->bIsStreamOrdinal()) {
      p_term->vBump_stmOdn(nBumpBy);
      p_term->vMark1(MARK_CHANGED);
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eCT Card_BoolCalc::eColumnType(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return CT_BOOLEAN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vConvertAll_stmOdn_To_colOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_PRED* p_pred_ROOT = this->pRootPred();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,pCI](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    p_term->vConvertIf_stmOdn_To_colOdn(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vCopyMarkedPredsToTable(Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card *pMainCard;

  CardIter xCR(this);
  while (Card_PRED* p_pred = (Card_PRED*)xCR.pRecurseEach()) {
    if (MARK_AT != p_pred->cMark1())
      continue;

    char cTableSyn = p_pred->cMark2();
    Card_Table *p_table = p_deck->pFindTableCard(cTableSyn, pMainCard);
    if (! p_table)
      continue;

    ((Card_OP*)pMainCard)->vAdd_Pred_Under_filter((Card_PRED*) p_pred->pDupCardRecursive());

    // todo: is this needed?
    Card_Filter* p_filter = pMainCard->pSub_filter();
    p_filter->vMark1(p_table->cMark1());
    p_filter->vMark2(p_table->cMark2());

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vCorrectStmOdn(Card_Projection* p_projection_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [p_projection_prior](Card* p_card)
  {
    if (p_card->pSymbol()->bIsStreamOrdinal())
      _pCard_Term(p_card)->vCorrectStmOdn(p_projection_prior);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,bLambdaFn);

}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vCullPred(char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! this->nChildCards())
    return;

  Card_PRED* pCard_pred = this->pRootPred();

  while (pCard_pred &&
      bCullPred(pCard_pred, cWithMark) &&
         this->nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(this->pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bCullPred(Card_PRED* p_pred, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vRemovePred(p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vRemovePred(p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vRemovePred(p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDPRED == eCard_pred || CK_ORPRED == eCard_pred || CK_NOTPRED == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(p_pred->pGetChild(i_Child));
    if (this->bCullPred(pCard_pred_child, cWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* find predicate with given mark                                             */
/*----------------------------------------------------------------------------*/
Card_PRED* Card_BoolCalc::pFindMarkedPred(char cWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCR(this);
  while (Card_PRED *p_pred = (Card_PRED *) xCR.pRecurseEach()) {
    if (cWithMark == p_pred->cMark1())
      return p_pred;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* inspect <boolCalc>> and mark any <pred> which could be moved               */
/*----------------------------------------------------------------------------*/
Card_PRED* Card_BoolCalc::pFindRemove_CorrPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_PRED* p_pred_corr =
      static_cast<Card_PRED*>(pFindPpyRecursive(P_CrlRslTest, CK_CMPPRED));

  if (! p_pred_corr)
    return nullptr;

  vRemovePred(p_pred_corr);

  return p_pred_corr;
}

/*----------------------------------------------------------------------------*/
/* is this boolCalc a simple expression of a column                           */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bIsScalarCol(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card *p_card_0 = pGetChild(0);
  if (!p_card_0->bIsCard(CK_TERM))
    return false;

  Card_Term *p_term_0 = _pCard_Term(p_card_0);
  if (!p_term_0->bIsColOdn())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* is this boolCalc a simple expression of a key                              */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bIsScalarKey(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return false;

  Card *p_card_0 = pGetChild(0);
  if (!p_card_0->bIsCard(CK_TERM))
    return false;

  Card_Term *p_term_0 = _pCard_Term(p_card_0);
  if (!p_term_0->bIsKeyOdn())
    return false;

  return true;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bHasTerm(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bHas = false;
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pSymbol, &bHas](Card* p_card)
  {
    if (p_card->pSymbol()->bSame(pSymbol)) {
      bHas = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,bLambdaFn);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* inspect <boolCalc>> and mark any <pred> which could be moved               */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vMarkWrt_table(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Deck* p_deck = pCI->G.pDeck;

  CardIter xCR(this);
  while (Card* p_card = xCR.pRecurseEach()) {

    if (p_card->bIsCard(CK_NOTPRED)) {
      p_card->vMark1('!');
      xCR.vDontDescend();
      continue;
    }
    if (p_card->bIsCard(CK_ANDPRED)) {
      p_card->vMark1('&');
      continue;
    }

    if (p_card->bIsCard(CK_CMPPRED)) {
      p_card->vMark1('b');
      xCR.vDontDescend();
      if (((Card_PRED *)p_card)->bContains_callToSubQuery(pCI))
        continue;
      Symbol* pWhichTable = ((Card_PRED *)p_card)->pTable_WhichTableFor_Pred(pCI);
      if (pWhichTable) {
        p_card->vMark1(MARK_AT);
        p_card->vMark2(pWhichTable->cSF_synonym());
      }
      continue;
    }

    if (p_card->bIsCard(CK_ORPRED)) {
      p_card->vMark1('|');
      xCR.vDontDescend();
      continue;
    }


    if (p_card->bIsCard(CK_TERM))
      continue;

    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vPush_CrlKeyPred_As_CrlKeyTest(Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  p_PRED->pSymbol()->Y_vCut(P_CrlKeyPred);
  p_PRED->pSymbol()->Y_vSet(P_CrlKeyTest);

  if (this->nChildCards()) {
    this->vPushPred(p_PRED);
  }
  else {
    this->vAppend(p_PRED);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vPushPred(Card_PRED* p_pred_toBeAdded)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* p_pred_oldTop = static_cast<Card_PRED*>(this->pPop());
  RASSERT(! this->nChildCards());

  Card_AndPred* p_andPred_new = new Card_AndPred;
  p_andPred_new->vAppend(p_pred_oldTop);
  p_andPred_new->vAppend(p_pred_toBeAdded);

  this->vPush(p_andPred_new);
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vRemovePred(Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pParentCard = this->pFindParentRecursive(p_pred);

  pParentCard->vRemoveSub(p_pred);

  if (CK_ANDPRED == pParentCard->eCard() || CK_ORPRED == pParentCard->eCard()) {

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
Card_PRED* Card_BoolCalc::pRootPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_PRED* p_PRED = static_cast<Card_PRED*>(pGetChild(0));

  return p_PRED;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bIsBraced(void)
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
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bYank_PRED(char cWithMark, Card_PRED*& p_pred_yanked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(' ' != cWithMark);

  p_pred_yanked = nullptr;

  if (! this->nChildCards()) {
    RASSERT(! p_pred_yanked);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  auto bYank = [](Card_BoolCalc* p_boolCalc,
                  char cWithMark,
                  Card* p_test,
                  Card_PRED*& p_pred_yanked,
                  auto&& bYank /*oddity permits recursive lambda*/)
  {
    //p_test->vDump();

    if (Card_PRED::bIsPRED(p_test)) {
      if (p_test->cMark1() == cWithMark) {
        p_pred_yanked = static_cast<Card_PRED*>(p_test);
        p_boolCalc->vRemovePred(p_pred_yanked);
        RASSERT(p_pred_yanked);
        return true;
      }
    }

    CardIter xCI(p_test);
    while (xCI.G_bEachMain()) {
      if (bYank(p_boolCalc,cWithMark,xCI.G.p_MainCard,p_pred_yanked,bYank)) {
        RASSERT(p_pred_yanked);
        return true;
      }
    }

    RASSERT(! p_pred_yanked);
    return false;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    if (bYank(this,cWithMark,xCI.G.p_MainCard,p_pred_yanked,bYank)) {
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  RASSERT(! p_pred_yanked);
  return false;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bYank_PRED(char cWithMark, Card* p_test,
                               Card_PRED*& p_pred_yanked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(' ' != cWithMark);

  if (Card_PRED::bIsPRED(p_test)) {
    if (p_test->cMark1() == cWithMark) {
      p_pred_yanked = static_cast<Card_PRED*>(p_test);
      this->vRemovePred(p_pred_yanked);
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    if (this->bYank_PRED(cWithMark, xCI.G.p_MainCard, p_pred_yanked)) {
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  RASSERT(! p_pred_yanked);
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Stz* Card_BoolCalc::szP_Type(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return szCT_char[CT_BOOLEAN];
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
