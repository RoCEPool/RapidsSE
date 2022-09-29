/*============================================================================*/
/* Card_Permutation.cpp -- Permutation                                        */
/*============================================================================*/

#include "Card_Permutation.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Permutation,CK_FILTER)
__aNewFunction(Card_Permutation,CN_CARD)
__vDeleteFunction(Card_Permutation,CN_CARD)

__bIsCard(STORE,Card_Permutation,CK_FILTER)
__pGetSubFunction_Card(STORE,Card_Permutation,CK_FILTER)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Permutation::Card_Permutation(void)
: Card(CK_FILTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Permutation::~Card_Permutation(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
