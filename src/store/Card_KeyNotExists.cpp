/*============================================================================*/
/* Card_KeyNotExists.cpp -- Key Not Exists                                    */
/*============================================================================*/

#include "Card_KeyNotExists.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_KeyNotExists,CK_KEYNOTEXISTS)
__aNewFunction(Card_KeyNotExists,CN_CARD)
__vDeleteFunction(Card_KeyNotExists,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_KeyNotExists::Card_KeyNotExists(void)
: Card(CK_KEYNOTEXISTS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_KeyNotExists::~Card_KeyNotExists(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
