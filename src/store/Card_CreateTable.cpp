/*============================================================================*/
/* Card_CreateTable.cpp -- Create Table                                       */
/*============================================================================*/

#include "Card_CreateTable.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CreateTable,CK_CREATETABLE)
__aNewFunction(Card_CreateTable,CN_CARD)
__vDeleteFunction(Card_CreateTable,CN_CARD)

__bIsCard(STORE,Card_CreateTable,CK_CREATETABLE)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CreateTable::Card_CreateTable(eCK eKind)
: Card(CK_CREATETABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CreateTable::~Card_CreateTable(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
