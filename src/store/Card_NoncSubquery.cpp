/*============================================================================*/
/* Card_NoncSubquery.cpp -- Non-Correlated Subquery                           */
/*============================================================================*/

#include "Card_NoncSubquery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_NoncSubquery,CK_NONCSUBQUERY)
__aNewFunction(Card_NoncSubquery,CN_CARD_NONCSUBQUERY)
__vDeleteFunction(Card_NoncSubquery,CN_CARD_NONCSUBQUERY)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_NoncSubquery::Card_NoncSubquery(eCK eKind, Stz* szQueryNm)
: Card(CK_NONCSUBQUERY)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
        
  vQueryNm(szQueryNm);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_NoncSubquery::~Card_NoncSubquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
