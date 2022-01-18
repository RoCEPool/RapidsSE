/*============================================================================*/
/* Card_DropTable.cpp -- Drop Table                                           */
/*============================================================================*/

#include "Card_DropTable.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_DropTable,CK_DROPTABLE)
__aNewFunction(Card_DropTable,CN_CARD)
__vDeleteFunction(Card_DropTable,CN_CARD)

__bIsCard(STORE,Card_DropTable,CK_DROPTABLE)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_DropTable::Card_DropTable(void)
: Card(CK_DROPTABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_DropTable::~Card_DropTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
