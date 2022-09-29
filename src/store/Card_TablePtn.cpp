/*============================================================================*/
/* Card_TablePtn.cpp -- Table Partition Key                                   */
/*============================================================================*/

#include "Card_TablePtn.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_TablePtn,CK_TABLEPTN)
__aNewFunction(Card_TablePtn,CN_CARD)
__vDeleteFunction(Card_TablePtn,CN_CARD)

__bIsCard(STORE,Card_TablePtn,CK_TABLEPTN)
__pGetSubFunction_Card(STORE,Card_TablePtn,CK_TABLEPTN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TablePtn::Card_TablePtn(void)
: Card(CK_TABLEPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TablePtn::~Card_TablePtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
