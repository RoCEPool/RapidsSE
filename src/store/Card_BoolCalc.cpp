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
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vBoolCalc_CullPred(char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! this->nChildCards())
    return;

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(this->pGetChild(0));

  while (pCard_pred &&
      bBoolCalc_CullPred(pCard_pred, cWithMark) &&
      this->nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(this->pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Card_BoolCalc::bBoolCalc_CullPred(Card_PRED* p_pred, char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  eCK eCard_pred = p_pred->eCard();

  if (CK_ANDPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vBoolCalc_RemovePred(p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_ORPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vBoolCalc_RemovePred(p_pred);
      delete p_pred;
      return true;
    }
  }
  else
  if (CK_CMPPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vBoolCalc_RemovePred(p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }
  else
  if (CK_ROPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vBoolCalc_RemovePred(p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }
  else
  if (CK_BOLPRED == eCard_pred) {
    if (cWithMark == p_pred->cMark1()) {
      this->vBoolCalc_RemovePred(p_pred);
      delete p_pred;
      return true;
    }
    return false;
  }

  RASSERT (CK_ANDPRED == eCard_pred || CK_ORPRED == eCard_pred || CK_NOTPRED == eCard_pred);

  i8 nChildren = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(p_pred->pGetChild(i_Child));
    if (this->bBoolCalc_CullPred(pCard_pred_child, cWithMark))
      return true;
  }

  return  false;
}

/*----------------------------------------------------------------------------*/
/* inspect <boolCalc>> and mark any <pred> which could be moved               */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vBoolCalc_MarkWrt_table(Card_Deck* p_deck)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolCalc::vBoolCalc_PushPred(Card_PRED* p_pred_toBeAdded)
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
void Card_BoolCalc::vBoolCalc_RemovePred(Card_PRED* p_pred)
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
