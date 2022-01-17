/*============================================================================*/
/* Card_Predicate.cpp -- Predicate                                            */
/*============================================================================*/

#include "Card_Predicate.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Predicate,CK_PREDICATE)
__aNewFunction(Card_Predicate,CN_CARD_PREDICATE)
__vDeleteFunction(Card_Predicate,CN_CARD_PREDICATE)

__bIsClass(STORE,Card_Predicate,CN_CARD_PREDICATE)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Predicate::Card_Predicate(eCK eKind)
: Card(CK_PREDICATE)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
      
  m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Predicate::~Card_Predicate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
