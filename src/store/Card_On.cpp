/*============================================================================*/
/* Card_On.cpp -- On Card                                                     */
/*============================================================================*/

#include "Card_On.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_On,CK_ON)
__aNewFunction(Card_On,CN_CARD)
__vDeleteFunction(Card_On,CN_CARD)

__bIsCard(STORE,Card_On,CK_ON)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_On::Card_On(Column* pColumn1, Table* pTable1,
                 Column* pColumn2, Table* pTable2)
: Card(CK_ON)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vColumn1(pColumn1);
  vTable1(pTable1);

  vColumn2(pColumn2);
  vTable2(pTable2);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_On::~Card_On(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
