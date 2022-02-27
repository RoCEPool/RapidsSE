/*============================================================================*/
/* Card_ExprBool.cpp -- Boolean Predicate                                     */
/*============================================================================*/

#include "Card_ExprBool.hpp"
#include "Card_PRED.hpp"
#include "Card_PredCmp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_ExprBool,CK_EXPRBOOL)
__aNewFunction(Card_ExprBool,CN_CARD)
__vDeleteFunction(Card_ExprBool,CN_CARD)

__bIsCard(STORE,Card_ExprBool,CK_EXPRBOOL)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ExprBool::Card_ExprBool(void)
: Card_EXPRESSION(CK_EXPRBOOL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
      
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ExprBool::~Card_ExprBool(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_PredCmp* Card_ExprBool::pFindBound(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pPred = (Card_PRED*)pGet(0);

  Card_PredCmp* pCard_PredCmp = pPred->pFindBound_pred(pIndex,eBound);
  
  return pCard_PredCmp;
}

/*----------------------------------------------------------------------------*/
/* does filterPred have some bound wrt given index?                           */
/*----------------------------------------------------------------------------*/
bool Card_ExprBool::bHasBounds(Index* pIndex)
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
void Card_ExprBool::vReduceExpr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_PRED* pCard_PRED = (Card_PRED*)pGet(0);
  if (! pCard_PRED->bReduciblePred())
    return;
  
  //vReducePred(pSubPred);

  
  this->vDump();
  pCard_PRED->vDump();
  
  // the child will only have one subcard
  i8 nChildSubCards = pCard_PRED->nItems();
  RASSERT(1 == nChildSubCards);
  
  //pCard_PRED_parent->vRemove(pCard_PRED_child);
  //pCard_PRED_parent->vDump();
  
  Card_PRED* pCard_PRED_kid = (Card_PRED*)pCard_PRED->pPop();
  pCard_PRED_kid->vDump();

  this->vReplace(pCard_PRED,pCard_PRED_kid);
  this->vDump();

  delete pCard_PRED;
  
  this->vDump();

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
