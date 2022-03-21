/*============================================================================*/
/* Card_PredOr.cpp -- Or Predicate                                            */
/*============================================================================*/

#include "Card_PredOr.hpp"
#include "Card_PRED.hpp"
#include "Card_PredCmp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_PredOr,CK_PREDOR)
__aNewFunction(Card_PredOr,CN_CARD)
__vDeleteFunction(Card_PredOr,CN_CARD)

__bIsCard(STORE,Card_PredOr,CK_PREDOR)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PredOr::Card_PredOr(void)
: Card_PRED(CK_PREDOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PredOr::~Card_PredOr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* find a comparePred which is the eBound of given index                      */
/*----------------------------------------------------------------------------*/
Card_PredCmp* Card_PredOr::pFindBound_or(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pSubPred0 = (Card_PRED*)pGet(0);
  if (Card_PredCmp* pFound = pSubPred0->pFindBound_pred(pIndex,eBound))
    return pFound;

  Card_PRED* pSubPred1 = (Card_PRED*)pGet(1);
  if (Card_PredCmp* pFound = pSubPred1->pFindBound_pred(pIndex,eBound))
    return pFound;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card_PredOr::bHasBounds_or(Index* pIndex)
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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
