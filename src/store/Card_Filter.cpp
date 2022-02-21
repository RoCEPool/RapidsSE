/*============================================================================*/
/* Card_Filter.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Filter.hpp"

#include "Card_ComparePred.hpp"
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
: Card_UNIT(CK_FILTER)
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
  
  Card_TypedExpr* pCard_TypedExpr = _pCard_TypedExpr(pGet(0));

  if (pCard_TypedExpr->pFindSubCard(CK_BOOLPRED))
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

  Card_ComparePred* pCard_ComparePred = _pCard_ComparePred(pGet(1));
  
  i8 nSubCards = pCard_ComparePred->nItems();
  for (i8 i_Sub = 0; i_Sub < nSubCards; i_Sub++) {
    Card_Term* pCard_Term = _pCard_Term(pCard_ComparePred->pGet(i_Sub));
    
    Stz* szTerm = pCard_Term->szTerm();
    
      szExpr->vConcat(" "_Stz);

    szExpr->vConcat(szTerm);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_ComparePred* Card_Filter::pFindBound(Index* pIndex, eBK eBound)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_TypedExpr* pCard_TypedExpr = _pCard_TypedExpr(pGet(0));
  
  Card_ComparePred* pCard_ComparePred = pCard_TypedExpr->pFindBound(pIndex,eBound);
  
  return pCard_ComparePred;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bHasBounds(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_TypedExpr* pCard_TypedExpr = _pCard_TypedExpr(pGet(0));
  
  bool bHas = pCard_TypedExpr->bHasBounds(pIndex);
  
  return bHas;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vReduceFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_TypedExpr* pCard_TypedExpr = _pCard_TypedExpr(pGet(0));
  
  pCard_TypedExpr->vReduceFilter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bRemoveBound(Card_ComparePred* pCard_ComparePred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  Card_TypedExpr* pCard_TypedExpr = _pCard_TypedExpr(pGet(0));
  
  bool bRemoved = pCard_TypedExpr->bRemoveBound(pCard_ComparePred);
  
  return bRemoved;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
