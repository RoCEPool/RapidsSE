/*============================================================================*/
/* Card_AndPred.cpp -- And Predicate                                          */
/*============================================================================*/

#include "Card_AndPred.hpp"
#include "Card_PRED.hpp"
#include "Card_CmpPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_AndPred, CK_ANDPRED)
__aNewFunction(Card_AndPred, CN_CARD)
__vDeleteFunction(Card_AndPred, CN_CARD)


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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_AndPred::pAndPredFindBound(Index* pIndex, ePB eBound)
{
_precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

Card_PRED* pSubPred0 = (Card_PRED*)this->pGet(0);
if (Card_CmpPred* pFound = pSubPred0->pPredFindBound(pIndex, eBound))
return pFound;

Card_PRED* pSubPred1 = (Card_PRED*)this->pGet(1);
if (Card_CmpPred* pFound = pSubPred1->pPredFindBound(pIndex, eBound))
return pFound;

return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this andPred have some bound on index?                                */
/*----------------------------------------------------------------------------*/
bool Card_AndPred::bAndPredHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_PRED* pSubPred0 = (Card_PRED*)this->pGet(0);
  if (pSubPred0->bPredHasBounds(pIndex))
    return true;

  Card_PRED* pSubPred1 = (Card_PRED*)this->pGet(1);
  if (pSubPred1->bPredHasBounds(pIndex))
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
