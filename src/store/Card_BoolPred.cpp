/*============================================================================*/
/* Card_BoolPred.cpp -- Boolean Predicate                                     */
/*============================================================================*/

#include "Card_BoolPred.hpp"
#include "Card_PRED.hpp"
#include "Card_ComparePred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_BoolPred,CK_BOOLPRED)
__aNewFunction(Card_BoolPred,CN_CARD)
__vDeleteFunction(Card_BoolPred,CN_CARD)

__bIsCard(STORE,Card_BoolPred,CK_BOOLPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_BoolPred::Card_BoolPred(void)
: Card_PRED(CK_BOOLPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_BoolPred::~Card_BoolPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Card_BoolPred::pFindBound(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pPred = (Card_PRED*)pGet(0);

  Card_ComparePred* pCard_ComparePred = pPred->pFindBound_pred(pIndex,eBound);
  
  return pCard_ComparePred;
}

/*----------------------------------------------------------------------------*/
/* does filterPred have some bound wrt given index?                           */
/*----------------------------------------------------------------------------*/
bool Card_BoolPred::bHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pPred = (Card_PRED*)pGet(0);
  if (pPred->bHasBounds_pred(pIndex))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_BoolPred::vReduceFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pSubPred = (Card_PRED*)pGet(0);
  if (pSubPred->bReduciblePred()) {
    vReducePred(pSubPred);
    return;
  }
  

  Card_PRED* pCard0 = (Card_PRED*)pGet(0);
  //vReduceFilter_pred(pCard0);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
