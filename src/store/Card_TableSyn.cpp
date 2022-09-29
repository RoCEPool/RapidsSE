/*============================================================================*/
/* Card_TableSyn.cpp -- Table                                                    */
/*============================================================================*/

#include "Card_TableSyn.hpp"

#include "Card_IndexPtr.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexList.hpp"

#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_TableSyn, CK_TABLESYN)
__aNewFunction(Card_TableSyn, CN_CARD)
__vDeleteFunction(Card_TableSyn, CN_CARD)

__bIsCard(STORE, Card_TableSyn, CK_TABLESYN)
__pGetSubFunction_Card(STORE, Card_TableSyn, CK_TABLESYN)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TableSyn::Card_TableSyn(void)
: Card(CK_TABLESYN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TableSyn::~Card_TableSyn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
