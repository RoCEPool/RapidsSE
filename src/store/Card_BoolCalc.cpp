/*============================================================================*/
/* Card_BoolCalc.cpp -- Boolean Calculation                                   */
/*============================================================================*/

#include "Card_BoolCalc.hpp"

#include "Card_CONJUNCTION.hpp"
#include "Card_CmpConj.hpp"
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

__pCastFunction_Card(STORE, Card_BoolCalc, CK_BOOLCALC)
__aNewFunction(Card_BoolCalc, CN_CARD)
__vDeleteFunction(Card_BoolCalc, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BoolCalc::Card_BoolCalc(void)
: Card_TERMLIST(CK_BOOLCALC, C_UNKNOWN)
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
Card_CmpConj* Card_BoolCalc::pCmpPred_whichCallsSubQuery(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CmpConj* pCmpPred = nullptr;

  auto bLambdaFn = [&pCmpPred,pCI](Card_CONJUNCTION* p_pred, bool bUpwards)
  {
    UNUSED(bUpwards);

    if (p_pred->bContains_callToSubQuery(pCI)) {
      pCmpPred = _pCard_CmpConj(p_pred);
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CONJUNCTION* p_pred_ROOT = this->pRootPred();

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
  UNUSED(bIgnore);

  return pCmpPred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vBump_stmCol(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJUNCTION* p_pred_ROOT = this->pRootPred();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [nBumpBy,bSkipFoldExpr](Card_CONJUNCTION* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (bSkipFoldExpr && p_term->pSymbol()->bSame("$1"_Stz))
      return C_CONTINUE_RECURSION;

    if (p_term->pSymbol()->bIs_Stm_Col()) {
      p_term->vBump_stmCol(nBumpBy);
      p_term->vMark1(MARK_CHANGED);
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
  UNUSED(bIgnore);
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
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vCopyMarkedPredsToTable(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Deck* pDeck = pCI->G.pDeck;
  Card *pMainCard;

  pCI->vRecurseBegin(this);
  s32 sTimes;
  for (i8 nTimes = 0; Card_CONJUNCTION* p_pred = (Card_CONJUNCTION*)pCI->pRecurseEach(); nTimes++) {

    sTimes = nTimes;
    //p_pred->vDump(__func__,0,sTimes);

    if (! p_pred->bMark1(MARK_AT))
      continue;

    char cTableSyn = p_pred->cMark2();
    Card_Table *p_table = pDeck->pFindTableCard(cTableSyn, pMainCard);
    if (! p_table)
      continue;

    ((Card_OP*)pMainCard)->vAdd_Pred_Under_filter((Card_CONJUNCTION*) p_pred->pDupCardRecursive());

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
void Card_BoolCalc::vCorrectStmCol(Card_Emitted* p_emitted_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [p_emitted_prior](Card* p_card)
  {
    if (p_card->pSymbol()->bIs_Stm_Col())
      _pCard_Term(p_card)->vCorrectStmCol(p_emitted_prior);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = bApplyRecursive(CK_TERM,bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
i8 Card_BoolCalc::nCullPred(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nCulled = 0;

  if (! this->nChildCards())
    return 0;

  Card_CONJUNCTION* pCard_pred = this->pRootPred();

  while (pCard_pred) {

    if (! bCullPred(pCard_pred, szWithMark))
      break;

    nCulled++;

    if (! this->nChildCards())
      break;

    pCard_pred = static_cast<Card_CONJUNCTION*>(this->pGetChild(0));
  }

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bCullPred(Card_CONJUNCTION* p_pred, Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDCONJ == eCard_pred) {
    if (p_pred->bMark1(szWithMark)) {
      this->vRemovePred(p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORCONJ == eCard_pred) {
    if (p_pred->bMark1(szWithMark)) {
      this->vRemovePred(p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPCONJ == eCard_pred) {
    if (p_pred->bMark1(szWithMark)) {
      this->vRemovePred(p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDCONJ == eCard_pred || CK_ORCONJ == eCard_pred || CK_NOTCONJ == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CONJUNCTION* pCard_pred_child = static_cast<Card_CONJUNCTION*>(p_pred->pGetChild(i_Child));
    if (this->bCullPred(pCard_pred_child, szWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* delete facet if                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn)
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
void Card_BoolCalc::vEnsureTypesFor_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_PRED = static_cast<Card_CONJUNCTION*>(pGetChild(0));

  if (p_PRED)
    p_PRED->vEnsureTypesFor_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* inspect <boolCalc>> and mark any <pred> which could be moved               */
/*----------------------------------------------------------------------------*/
Card_CONJUNCTION* Card_BoolCalc::pFindRemove_CorrPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_pred_corr =
      static_cast<Card_CONJUNCTION*>(pFindPpyRecursive(P_CrlRslTest, CK_CMPCONJ));

  if (! p_pred_corr)
    return nullptr;

  vRemovePred(p_pred_corr);

  return p_pred_corr;
}

/*----------------------------------------------------------------------------*/
/* is this a compound boolCalc                                                */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bIsCompound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (nChildCards() > 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound boolCalc                                                */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bIsLiterals(void)
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
  if (!p_term_0->bIsTblCol())
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
  if (!p_term_0->bIsKeyCol())
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
  UNUSED(bIgnore);

  return bHas;
}

/*----------------------------------------------------------------------------*/
/* inspect <boolCalc>> and mark any <pred> which could be moved               */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vMarkWrt_table(CardIter* pCI)
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

    if (p_card->bIsCard(CK_CMPCONJ)) {
      p_card->vMark1(MARK_COMPARE);
      pCI->vDontDescend(true);
      if (((Card_CONJUNCTION *)p_card)->bContains_callToSubQuery(pCI))
        continue;
      Symbol* pWhichTable = ((Card_CONJUNCTION *)p_card)->pTable_WhichTableFor_Pred(pCI);
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
void Card_BoolCalc::vPush_CrlKeyPred_As_CrlKeyTest(Card_CONJUNCTION *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  p_PRED->pSymbol()->Y_vCut(P_CrlKeyPred);
  p_PRED->pSymbol()->Y_vSet(P_CrlKeyTest);

  if (this->nChildCards()) {
    this->vPushPred(p_PRED);
  }
  else {
    this->vAppendChild(p_PRED);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vPushPred(Card_CONJUNCTION* p_pred_toBeAdded)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CONJUNCTION* p_pred_oldTop = static_cast<Card_CONJUNCTION*>(this->pPopChild());
  RASSERT(! this->nChildCards());

  Card_AndConj* p_andPred_new = new Card_AndConj;
  p_andPred_new->vAppendChild(p_pred_oldTop);
  p_andPred_new->vAppendChild(p_pred_toBeAdded);

  this->vPushChild(p_andPred_new);
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vRemovePred(Card_CONJUNCTION* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card* pParentCard = this->pFindParentRecursive(p_pred);

  pParentCard->vRemoveSub(p_pred);

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
Card_CONJUNCTION* Card_BoolCalc::pRootPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! nChildCards())
    return nullptr;

  Card_CONJUNCTION* p_PRED = static_cast<Card_CONJUNCTION*>(pGetChild(0));

  return p_PRED;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vShortForm(String* pString)
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
bool Card_BoolCalc::bYank_PRED(Stz* szWithMark, Card_CONJUNCTION*& p_pred_yanked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  p_pred_yanked = nullptr;

  if (! this->nChildCards()) {
    RASSERT(! p_pred_yanked);
    return false;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //
  auto bYank = [](Card_BoolCalc* p_boolCalc,
                  Stz* szWithMark,
                  Card* p_test,
                  Card_CONJUNCTION*& p_pred_yanked,
                  auto&& bYank /*oddity permits recursive lambda*/)
  {
    //p_test->vDump();

    if (Card_CONJUNCTION::bIsPRED(p_test)) {
      if (p_test->bMark1(szWithMark)) {
        p_pred_yanked = static_cast<Card_CONJUNCTION*>(p_test);
        p_boolCalc->vRemovePred(p_pred_yanked);
        RASSERT(p_pred_yanked);
        return true;
      }
    }

    Kid xKid(p_test);
    while (xKid.bEach()) {
      if (bYank(p_boolCalc,szWithMark,xKid.pChild(),p_pred_yanked,bYank)) {
        RASSERT(p_pred_yanked);
        return true;
      }
    }

    RASSERT(! p_pred_yanked);
    return false;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Kid xKid(this);
  while (xKid.bEach()) {
    if (bYank(this,szWithMark,xKid.pChild(),p_pred_yanked,bYank)) {
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
bool Card_BoolCalc::bYank_PRED(Stz* szWithMark, Card* p_test,
                               Card_CONJUNCTION*& p_pred_yanked)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_CONJUNCTION::bIsPRED(p_test)) {
    if (p_test->bMark1(szWithMark)) {
      p_pred_yanked = static_cast<Card_CONJUNCTION*>(p_test);
      this->vRemovePred(p_pred_yanked);
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  Kid xKid(this);
  while (xKid.bEach()) {
    if (this->bYank_PRED(szWithMark, xKid.pChild(), p_pred_yanked)) {
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
eCT Card_BoolCalc::eColType(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  return CT_BOOLEAN;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vX_stmCol_To_tblCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_pred_ROOT = this->pRootPred();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI](Card_CONJUNCTION* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    Symbol* pSymbol = p_term->pSymbol();
    if (!pSymbol->bIs_Stm_Col())
      return C_CONTINUE_RECURSION;

    p_term->vX_stmCol_To_tblCol(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vX_tblCol_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_CONJUNCTION* p_pred_ROOT = this->pRootPred();
  if (! p_pred_ROOT)
    return;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfTo_stmCol = [pCI](Card_CONJUNCTION* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    Symbol* pSymbol = p_term->pSymbol();
    if (!pSymbol->bIs_Tbl_Col())
      return C_CONTINUE_RECURSION;

    p_term->vX_tblCol_To_stmCol(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bfTo_stmCol);
  UNUSED(bIgnore);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
