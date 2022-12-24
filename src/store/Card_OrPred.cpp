/*============================================================================*/
/* Card_OrPred.cpp -- Or Predicate                                            */
/*============================================================================*/

#include "Card_OrPred.hpp"
#include "Card_PRED.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_OrPred, CK_ORPRED)
__aNewFunction(Card_OrPred, CN_CARD)
__vDeleteFunction(Card_OrPred, CN_CARD)


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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find a comparePred which is the E_eTerm_Bound of given index                 */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_OrPred::pOrPredFindBound(Index* pIndex, ePB eBound)
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
/* does orPred have some bound on given index?                                */
/*----------------------------------------------------------------------------*/
bool Card_OrPred::bOrPredHasBounds(Index* pIndex)
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
