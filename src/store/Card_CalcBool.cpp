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
__pGetSubFunction_Card(STORE,Card_CalcBool,CK_CALCBOOL)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CalcBool::Card_CalcBool(void)
: Card_EXPRESSION(CK_CALCBOOL)
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
void Card_CalcBool::vRemovePred(Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_)

  Card* pParentCard = pFindParent(pCard_pred);

  pParentCard->vRemoveSubCard(pCard_pred);

  if (CK_PREDAND == pParentCard->eCard() || CK_PREDOR == pParentCard->eCard()) {

    if (0 == pParentCard->nSubCards()) {
      vRemoveSubCard(pParentCard);
    }
    else {
      Card* pSiblingCard = pParentCard->pGetSubCard(0);
      Card* pGrandCard = pFindParent((pParentCard));
      pParentCard->vRemoveSubCard(pSiblingCard);
      pGrandCard->vReplaceSubCard(pParentCard,pSiblingCard);
    }

    delete pParentCard;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
