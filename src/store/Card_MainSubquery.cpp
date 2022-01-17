/*============================================================================*/
/* Card_MainSubquery.cpp -- Main Subquery                                     */
/*============================================================================*/

#include "Card_MainSubquery.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_MainSubquery,CK_MAINSUBQUERY)
__aNewFunction(Card_MainSubquery,CN_CARD)
__vDeleteFunction(Card_MainSubquery,CN_CARD)

__bIsCard(STORE,Card_MainSubquery,CK_MAINSUBQUERY)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_MainSubquery::Card_MainSubquery(eCK eKind, Stz* szQueryNm)
: Card(CK_MAINSUBQUERY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vQueryNm(szQueryNm);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_MainSubquery::~Card_MainSubquery(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
