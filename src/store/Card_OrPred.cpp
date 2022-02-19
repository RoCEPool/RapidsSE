/*============================================================================*/
/* Card_OrPred.cpp -- Or Predicate                                            */
/*============================================================================*/

#include "Card_OrPred.hpp"
#include "Card_PRED.hpp"
#include "Card_ComparePred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_OrPred,CK_ORPRED)
__aNewFunction(Card_OrPred,CN_CARD)
__vDeleteFunction(Card_OrPred,CN_CARD)

__bIsCard(STORE,Card_OrPred,CK_ORPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrPred::Card_OrPred(void)
: Card_PRED(CK_ORPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OrPred::~Card_OrPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Card_OrPred::pFindBound_or(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (Card_ComparePred* pFound = pSubPred0->pFindBound_pred(pIndex,eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (Card_ComparePred* pFound = pSubPred1->pFindBound_pred(pIndex,eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card_OrPred::bHasBounds_or(Index* pIndex)
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
bool Card_OrPred::bRemoveBound_or(Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (pSubPred0 == pCard_ComparePred) {
    vRemove(pCard_ComparePred);
    //vDeleteComparePred(pCard_ComparePred);
    delete pCard_ComparePred;
    return true;
  }

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (pSubPred1 == pCard_ComparePred) {
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
