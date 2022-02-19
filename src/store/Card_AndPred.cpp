/*============================================================================*/
/* Card_AndPred.cpp -- And Predicate                                          */
/*============================================================================*/

#include "Card_AndPred.hpp"
#include "Card_PRED.hpp"
#include "Card_ComparePred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_AndPred,CK_ANDPRED)
__aNewFunction(Card_AndPred,CN_CARD)
__vDeleteFunction(Card_AndPred,CN_CARD)

__bIsCard(STORE,Card_AndPred,CK_ANDPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AndPred::Card_AndPred(void)
: Card_PRED(CK_ANDPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AndPred::~Card_AndPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Card_AndPred::pFindBound_and(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (Card_ComparePred* pFound = pSubPred0->pFindBound_pred(pIndex,eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (Card_ComparePred* pFound = pSubPred1->pFindBound_pred(pIndex,eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool Card_AndPred::bHasBounds_and(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (pSubPred0->bHasBounds_pred(pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (pSubPred1->bHasBounds_pred(pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* remove given comparePred                                                   */
/*----------------------------------------------------------------------------*/
bool Card_AndPred::bRemoveBound_and(Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pCard0 = (Card_PRED*)pGet(0);
  if (pCard0 == pCard_ComparePred) {
    vRemove(pCard_ComparePred);
    //vDeleteComparePred(pCard_ComparePred);
    delete pCard_ComparePred;
    return true;
  }

  Card_PRED* pCard1 = (Card_PRED*)pGet(1);
  if (pCard1 == pCard_ComparePred) {
    vRemove(pCard_ComparePred);
    //vDeleteComparePred(pCard_ComparePred);
    delete pCard_ComparePred;
    return true;
  }

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
