/*============================================================================*/
/* Card_Aggreg.cpp -- Aggreg                                                  */
/*============================================================================*/

#include "Card_Aggreg.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Aggreg,CK_AGGREG)
__aNewFunction(Card_Aggreg,CN_CARD)
__vDeleteFunction(Card_Aggreg,CN_CARD)

__bIsCard(STORE,Card_Aggreg,CK_AGGREG)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Aggreg::Card_Aggreg(eCK eKind, eAL eAggLvl)
: Card_Project(CK_AGGREG)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAggLvl(eAggLvl);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Aggreg::~Card_Aggreg(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vDeleteAll();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
