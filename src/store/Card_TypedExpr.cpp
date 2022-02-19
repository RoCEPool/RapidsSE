/*============================================================================*/
/* Card_TypedExpr.cpp -- Typed Expression                                     */
/*============================================================================*/

#include "Card_TypedExpr.hpp"
#include "Card_BoolPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_TypedExpr,CK_TYPEDEXPR)
__aNewFunction(Card_TypedExpr,CN_CARD)
__vDeleteFunction(Card_TypedExpr,CN_CARD)

__bIsCard(STORE,Card_TypedExpr,CK_TYPEDEXPR)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TypedExpr::Card_TypedExpr(void)
: Card(CK_TYPEDEXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TypedExpr::~Card_TypedExpr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Card_TypedExpr::pFindBound(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_BoolPred* pCard_BoolPred = _pCard_BoolPred(pFindSubCard(CK_BOOLPRED));
  
  Card_ComparePred* pCard_ComparePred =
    pCard_BoolPred->pFindBound(pIndex,eBound);
 
  return pCard_ComparePred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_TypedExpr::bHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_BoolPred* pCard_BoolPred = _pCard_BoolPred(pFindSubCard(CK_BOOLPRED));
  
  bool bHas = pCard_BoolPred->bHasBounds(pIndex);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_TypedExpr::vReduceFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_BoolPred* pCard_BoolPred = _pCard_BoolPred(pFindSubCard(CK_BOOLPRED));
  
  pCard_BoolPred->vReduceFilter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_TypedExpr::bRemoveBound(Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_BoolPred* pCard_BoolPred = _pCard_BoolPred(pFindSubCard(CK_BOOLPRED));
  
  bool bRemoved = pCard_BoolPred->bRemoveBound(pCard_ComparePred);
  
  return bRemoved;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
