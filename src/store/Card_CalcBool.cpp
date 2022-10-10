/*============================================================================*/
/* Card_CalcBool.cpp -- Boolean Calculation                                   */
/*============================================================================*/

#include "Card_CalcBool.hpp"
#include "Card_PRED.hpp"
#include "Card_PredCmp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CalcBool,CK_CALCBOOL)
__aNewFunction(Card_CalcBool,CN_CARD)
__vDeleteFunction(Card_CalcBool,CN_CARD)

__bIsCard(STORE,Card_CalcBool,CK_CALCBOOL)
__pSubFunction_Card(STORE,CalcBool,CK_CALCBOOL)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CalcBool::Card_CalcBool(void)
: Card(CK_CALCBOOL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CalcBool::~Card_CalcBool(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card_CalcBool::vCullPred(char cWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_)

  Card_PRED* pCard_pred = static_cast<Card_PRED*>(pGetChild(0));

  while (pCard_pred && bCullPred(pCard_pred,cWithMark) && nChildCards()) {
    pCard_pred = static_cast<Card_PRED*>(pGetChild(0));
  }
}

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
void Card_CalcBool::vRemovePred(Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_)

  Card* pParentCard = pFindParent(pCard_pred);

  pParentCard->vRemoveSubCard(pCard_pred);

  if (CK_PREDAND == pParentCard->eCard() || CK_PREDOR == pParentCard->eCard()) {

    if (0 == pParentCard->nChildCards()) {
      vRemoveSubCard(pParentCard);
    }
    else {
      Card* pSiblingCard = pParentCard->pGetChild(0);
      Card* pGrandCard = pFindParent((pParentCard));
      pParentCard->vRemoveSubCard(pSiblingCard);
      pGrandCard->vReplaceChild(pParentCard, pSiblingCard);
    }

    delete pParentCard;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove given predicate                                                     */
/*----------------------------------------------------------------------------*/
bool Card_CalcBool::bCullPred(Card_PRED* pCard_pred, char cWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_)

  eCK eCard_pred = pCard_pred->eCard();

  if (CK_PREDAND == eCard_pred) {
    if (cWithMark == pCard_pred->cMark1()) {
       vRemovePred(pCard_pred);
       return true;
    }
  }
  else
  if (CK_PREDOR == eCard_pred) {
    if (cWithMark == pCard_pred->cMark1()) {
      vRemovePred(pCard_pred);
      return true;
    }
  }
  else
  if (CK_PREDCMP == eCard_pred) {
    if (cWithMark == pCard_pred->cMark1()) {
      vRemovePred(pCard_pred);
      return true;
    }
    return false;
  }

  RASSERT (CK_PREDAND == eCard_pred || CK_PREDOR == eCard_pred);

  i8 nChildren = pCard_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_PRED* pCard_pred_child = static_cast<Card_PRED*>(pCard_pred->pGetChild(i_Child));
    if (bCullPred(pCard_pred_child,cWithMark))
      return true;
  }

  return  false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
