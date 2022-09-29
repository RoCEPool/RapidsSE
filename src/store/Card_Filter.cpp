/*============================================================================*/
/* Card_Filter.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Filter.hpp"

#include "Card_Expr.hpp"
#include "Card_CalcBool.hpp"
#include "Card_PredCmp.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Filter,CK_FILTER)
__aNewFunction(Card_Filter,CN_CARD)
__vDeleteFunction(Card_Filter,CN_CARD)

__bIsCard(STORE,Card_Filter,CK_FILTER)
__pGetSubFunction_Card(STORE,Card_Filter,CK_FILTER)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Filter::Card_Filter(void)
: Card(CK_FILTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Filter::~Card_Filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bCull(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Expr* pCard_Expr =
    _pCard_Expr(pFindSubCard(CK_EXPR));

  if (pCard_Expr->pFindSubCard(CK_EXPRBOOL))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vCullPred(char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);

    eCK eCard = pChildCard->eCard();
    if (eCard != CK_EXPR)
      continue;

    Card_Expr* p_expr = _pCard_Expr(pChildCard);

    p_expr->vCullPred(cWithMark);
  }
}


/*----------------------------------------------------------------------------*/
/* to expr                                                                    */
/*----------------------------------------------------------------------------*/
void Card_Filter::vToExpr(Stz* szExpr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  szExpr->vCopy(""_Stz);
  
  Card_Type* pCard_Type = _pCard_Type(pGet(0));

  szExpr->vConcat("type "_Stz);
  szExpr->vConcat(pCard_Type->szType());

  Card_PredCmp* pCard_PredCmp = _pCard_PredCmp(pGet(1));
  
  i8 nChildren = pCard_PredCmp->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_Term* pCard_Term = _pCard_Term(pCard_PredCmp->pGetChild(i_Child));
    
    Stz* szTerm = pCard_Term->szTerm();
    
    szExpr->vConcat(" "_Stz);

    szExpr->vConcat(szTerm);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_PredCmp* Card_Filter::pFindBound(Index* pIndex, ePB eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Expr* pCard_Expr =
    _pCard_Expr(pFindSubCard(CK_EXPR));

  Card_PredCmp* pCard_PredCmp = pCard_Expr->pFindBound(pIndex,eBound);
  
  return pCard_PredCmp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Expr* pCard_Expr =
    _pCard_Expr(pFindSubCard(CK_EXPR));
  
  bool bHas = pCard_Expr->bHasBounds(pIndex);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bIsEmptyFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CalcBool* pCard_calcBool =
      _pCard_CalcBool(pFindSubCard(CK_CALCBOOL));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vReduceFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_Expr* pCard_Expr =
    _pCard_Expr(pFindSubCard(CK_EXPR));

  pCard_Expr->vReduceBoolExpr();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bRemoveBound(Card_PredCmp* pCard_PredCmp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
    
  vRemoveSubCard(pCard_PredCmp);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
