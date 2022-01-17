/*============================================================================*/
/* Card_OrdAgg.cpp -- List of Ordered Aggregate                           */
/*============================================================================*/

#include "Card_OrdAgg.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_OrdAgg,CK_ORDAGG)
__aNewFunction(Card_OrdAgg,CN_CARD)
__vDeleteFunction(Card_OrdAgg,CN_CARD)

__bIsCard(STORE,Card_OrdAgg,CK_ORDAGG)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OrdAgg::Card_OrdAgg(eCK eKind, Stz* szLevel)
: Card_Project(CK_ORDAGG)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
    
  vLevel(szLevel);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OrdAgg::~Card_OrdAgg(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  vDeleteAll();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
