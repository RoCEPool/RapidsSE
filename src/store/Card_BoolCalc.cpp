/*============================================================================*/
/* Card_BoolCalc.cpp -- Boolean Calculation                                   */
/*============================================================================*/

#include "Card_BoolCalc.hpp"

#include "Card_PRED.hpp"
#include "Card_BolPred.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"
#include "Card_AndPred.hpp"
#include "Card_OuterColNm.hpp"
#include "Card_TblColOdn.hpp"
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
void Card_BoolCalc::vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(this->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,nBumpBy,bSkipFoldExpr](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (bSkipFoldExpr && p_term->bIsFoldExpr())
      return C_CONTINUE_RECURSION;

    if (Card::bIsStreamColOrdinal(p_term->szValue())) {
      p_term->vBump_StreamColOrdinal(nBumpBy);
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
void Card_BoolCalc::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(this->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,pCI](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    p_term->vConvert_stmColOdn_To_tblColOdn(pCI);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vCopyMarkedPredToTable(Card_Deck* p_deck)
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

    pMainCard->vAddPredUnder_main((Card_PRED*)p_pred->pDupCardAndSubCards());

    // todo: is this needed?
    Card_Filter* p_filter = pMainCard->pSub_filter();
    p_filter->vMark1(p_table->cMark1());
    p_filter->vMark2(p_table->cMark2());

  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vCullPred(char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! this->nChildCards())
    return;

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(this->pGetChild(0));

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
  else
  if (CK_ROPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vRemovePred(p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }
  else
  if (CK_BOLPRED == eCard_pred) {
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
/* inspect <boolCalc>> and mark any <pred> which could be moved               */
/*----------------------------------------------------------------------------*/
Card_PRED* Card_BoolCalc::pFindRemove_CorrPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_PRED* p_pred_corr =
      static_cast<Card_PRED*>(pFindPpyRecursive(P_CorrPred,CK_CMPPRED));

  if (! p_pred_corr)
    return nullptr;

  vRemovePred(p_pred_corr);

  return p_pred_corr;
}

/*----------------------------------------------------------------------------*/
/* inspect <boolCalc>> and mark any <pred> which could be moved               */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vMarkWrt_table(Card_Deck* p_deck)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

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

    if (p_card->bIsCard(CK_BOLPRED)) {
      p_card->vMark1('b');
      xCR.vDontDescend();
      continue;
    }

    if (p_card->bIsCard(CK_CMPPRED)) {
      p_card->vMark1('?');
      xCR.vDontDescend();
      char cWhichTable = ((Card_PRED *) p_card)->cTable_WhichTableFor_Pred(p_deck);
      if (C_UNKNOWN != cWhichTable) {
        p_card->vMark1(MARK_AT);
        p_card->vMark2(cWhichTable);
      }
      continue;
    }

    if (p_card->bIsCard(CK_ORPRED)) {
      p_card->vMark1('|');
      xCR.vDontDescend();
      continue;
    }

    if (p_card->bIsCard(CK_ROPRED))
      continue;

    if (p_card->bIsCard(CK_TERM))
      continue;

    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/* move <where> to <table>/<filter>                                           */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vPush_Correlated_cmpPred_Onto_mq_boolCalc(Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

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

  Card* pParentCard = this->pFindParent(p_pred);

  pParentCard->vRemoveSubCard(p_pred);

  if (CK_ANDPRED == pParentCard->eCard() || CK_ORPRED == pParentCard->eCard()) {

    if (0 == pParentCard->nChildCards()) {
      this->vRemoveSubCard(pParentCard);
    }
    else {
      Card* pSiblingCard = pParentCard->pGetChild(0);
      Card* pGrandCard = this->pFindParent((pParentCard));
      pParentCard->vRemoveSubCard(pSiblingCard);
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

    if (p_card->bNeedsBrace()) {
      bBraced = true;
      return C_TERMINATE_RECURSION;
    }

    return C_CONTINUE_RECURSION;
  };

  bApplyRecursive(CK_NONE,bTestBraced);

  return bBraced;
}

/*----------------------------------------------------------------------------*/
/* resolve the parameters terms in given boolCalc                             */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vSubstitute_Parameters(Card_ParamList* p_paramList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(this->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfXYZ = [this,p_paramList](Card_PRED* p_pred, bool bUpwards)
  {
    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! Card::bIsParamOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Card_OuterColNm* p_outerColNm = _pCard_OuterColNm(p_paramList->pFindPpyRecursive(P_ParamOdn));
    if (! p_outerColNm)
      return false;

    //Card_StmColOdn* p_stmColOrd = p_outerColNm->pSub_stmColOdn();
    //M_vCard_Set_Value(p_term,p_stmColOrd->szValue(),MARK_CHANGED);
    Card_TblColOdn* p_tblColOrd = p_outerColNm->pSub_tblColOdn();
    p_term->vSetValueMark1(p_tblColOrd->szValue(), MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bfXYZ);
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
    p_test->vDump();

    if (p_test->bIsPRED()) {
      if (p_test->cMark1() == cWithMark) {
        p_pred_yanked = static_cast<Card_PRED*>(p_test);
        p_boolCalc->vRemovePred(p_pred_yanked);
        RASSERT(p_pred_yanked);
        return true;
      }
    }

    CardIter xCI(p_test);
    while (Card* p_child = xCI.pEachChild()) {
      if (bYank(p_boolCalc,cWithMark,p_child,p_pred_yanked,bYank)) {
        RASSERT(p_pred_yanked);
        return true;
      }
    }

    RASSERT(! p_pred_yanked);
    return false;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  CardIter xCI(this);

  while (Card* p_child = xCI.pEachChild()) {
    if (bYank(this,cWithMark,p_child,p_pred_yanked,bYank)) {
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

  if (p_test->bIsPRED()) {
    if (p_test->cMark1() == cWithMark) {
      p_pred_yanked = static_cast<Card_PRED*>(p_test);
      this->vRemovePred(p_pred_yanked);
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  CardIter xCI(this);
  while (Card* p_child = xCI.pEachChild()) {
    if (this->bYank_PRED(cWithMark, p_child, p_pred_yanked)) {
      RASSERT(p_pred_yanked);
      return true;
    }
  }

  RASSERT(! p_pred_yanked);
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
