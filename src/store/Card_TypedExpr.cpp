/*============================================================================*/
/* Card_TypedExpr.cpp -- Typed Expression                                     */
/*============================================================================*/

#include "Card_TypedExpr.hpp"
#include "Card_BoolExpr.hpp"

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
: Card_SUB(CK_TYPEDEXPR)
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
Card_PredCmp* Card_TypedExpr::pFindBound(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_BoolExpr* pCard_BoolExpr = _pCard_BoolExpr(pFindSubCard(CK_BOOLEXPR));
  
  Card_PredCmp* pCard_PredCmp =
    pCard_BoolExpr->pFindBound(pIndex,eBound);
 
  return pCard_PredCmp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_TypedExpr::bHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_BoolExpr* pCard_BoolExpr = _pCard_BoolExpr(pFindSubCard(CK_BOOLEXPR));
  
  bool bHas = pCard_BoolExpr->bHasBounds(pIndex);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_TypedExpr::vReduceBoolExpr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_BoolExpr* pCard_BoolExpr = _pCard_BoolExpr(pFindSubCard(CK_BOOLEXPR));

  Card_PRED* pCard_ChildPRED = (Card_PRED*)pCard_BoolExpr->pGet(0);

  pCard_ChildPRED->vCullByKids();
  
  
  if (1 == pCard_ChildPRED->nItems()) {

    Card_PRED* pCard_KidPRED = (Card_PRED*)pCard_ChildPRED->pPop();

    pCard_BoolExpr->vReplace(pCard_ChildPRED,pCard_KidPRED);
    delete pCard_ChildPRED;
    
    pCard_ChildPRED = pCard_KidPRED;
  }
  
  
  if (pCard_ChildPRED->nItems())
    return;
  
  pCard_BoolExpr->vRemove(pCard_ChildPRED);
  vRemove(pCard_BoolExpr);

  delete pCard_BoolExpr;
  delete pCard_ChildPRED;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
