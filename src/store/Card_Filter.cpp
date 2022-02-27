/*============================================================================*/
/* Card_Filter.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Filter.hpp"

#include "Card_PredCmp.hpp"
#include "Card_Term.hpp"
#include "Card_TypedExpr.hpp"
#include "Card_Type.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Filter,CK_FILTER)
__aNewFunction(Card_Filter,CN_CARD)
__vDeleteFunction(Card_Filter,CN_CARD)

__bIsCard(STORE,Card_Filter,CK_FILTER)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Filter::Card_Filter(void)
: Card_MAIN(CK_FILTER)
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
  
  Card_TypedExpr* pCard_TypedExpr =
    _pCard_TypedExpr(pFindSubCard(CK_TYPEDEXPR));

  if (pCard_TypedExpr->pFindSubCard(CK_BOOLEXPR))
    return false;
  
  return true;
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
  
  i8 nSubCards = pCard_PredCmp->nItems();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_Term* pCard_Term = _pCard_Term(pCard_PredCmp->pGet(i_Sub));
    
    Stz* szTerm = pCard_Term->szTerm();
    
      szExpr->vConcat(" "_Stz);

    szExpr->vConcat(szTerm);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_PredCmp* Card_Filter::pFindBound(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_TypedExpr* pCard_TypedExpr =
    _pCard_TypedExpr(pFindSubCard(CK_TYPEDEXPR));

  Card_PredCmp* pCard_PredCmp = pCard_TypedExpr->pFindBound(pIndex,eBound);
  
  return pCard_PredCmp;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_TypedExpr* pCard_TypedExpr =
    _pCard_TypedExpr(pFindSubCard(CK_TYPEDEXPR));
  
  bool bHas = pCard_TypedExpr->bHasBounds(pIndex);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vReduceFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_TypedExpr* pCard_TypedExpr =
    _pCard_TypedExpr(pFindSubCard(CK_TYPEDEXPR));

  pCard_TypedExpr->vReduceBoolExpr();
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
