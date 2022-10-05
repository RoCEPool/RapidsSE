/*============================================================================*/
/* Card_ColumnRef.cpp -- Column Reference                                     */
/*============================================================================*/

#include "Card_ColumnRef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_ColumnRef, CK_COLUMNREF)
__aNewFunction(Card_ColumnRef, CN_CARD)
__vDeleteFunction(Card_ColumnRef, CN_CARD)

__bIsCard(STORE, Card_ColumnRef, CK_COLUMNREF)
__pGetSubFunction_Card(STORE,ColumnRef, CK_COLUMNREF)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ColumnRef::Card_ColumnRef(void)
: Card(CK_COLUMNREF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ColumnRef::~Card_ColumnRef(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Card_ColumnRef::cTableSyn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return Card::cColumnRef_tableSyn();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_ColumnRef::vTableSyn(String* psTableSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  psTableSyn->vSprint("%c"_Stz,cTableSyn());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
