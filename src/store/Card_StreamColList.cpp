/*============================================================================*/
/* Card_StreamColList.cpp -- Stream Column List                               */
/*============================================================================*/

#include "Card_StreamColList.hpp"
#include "Card_Expr.hpp"
#include "Card_ObyElist.hpp"
#include "Card_StreamElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamColList,CK_STREAMCOLLIST)
__aNewFunction(Card_StreamColList,CN_CARD)
__vDeleteFunction(Card_StreamColList,CN_CARD)

__bIsCard(STORE,Card_StreamColList,CK_STREAMCOLLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamColList::Card_StreamColList(void)
: Card_SUB(CK_STREAMCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamColList::~Card_StreamColList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
