/*============================================================================*/
/* Card_CorrSubquery.cpp -- Correlated Subquery                               */
/*============================================================================*/

#include "Card_CorrSubquery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CorrSubquery,CK_CORRSUBQUERY)
__aNewFunction(Card_CorrSubquery,CN_CARD)
__vDeleteFunction(Card_CorrSubquery,CN_CARD)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CorrSubquery::Card_CorrSubquery(eCK eKind, Stz* szQueryNm)
: Card(CK_CORRSUBQUERY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vQueryNm(szQueryNm);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CorrSubquery::~Card_CorrSubquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
