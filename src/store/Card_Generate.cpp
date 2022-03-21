/*============================================================================*/
/* Card_Generate.cpp -- Generate                                              */
/*============================================================================*/

#include "Card_Generate.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Generate,CK_GENERATE)
__aNewFunction(Card_Generate,CN_CARD)
__vDeleteFunction(Card_Generate,CN_CARD)

__bIsCard(STORE,Card_Generate,CK_GENERATE)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Generate::Card_Generate(i8 iGenerate)
: Card_SUB(CK_GENERATE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vGenerate(iGenerate);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Generate::~Card_Generate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
