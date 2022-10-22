/*============================================================================*/
/* Card_PtnColList.cpp -- Stream Column List                               */
/*============================================================================*/

#include "Card_PtnColList.hpp"
#include "Card_Expr.hpp"
#include "Card_ObyElist.hpp"
#include "Card_AggElist.hpp"

#include "Card_StmCol.hpp"
#include "Card_As.hpp"
#include "Card_Type.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_PtnColList,CK_PTNCOLLIST)
__aNewFunction(Card_PtnColList,CN_CARD)
__vDeleteFunction(Card_PtnColList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PtnColList::Card_PtnColList(void)
: Card(CK_PTNCOLLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PtnColList::~Card_PtnColList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
