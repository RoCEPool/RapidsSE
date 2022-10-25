/*============================================================================*/
/* Card_PtndJoin.cpp -- Generate                                              */
/*============================================================================*/

#include "Card_PtndJoin.hpp"
#include "Card_SubSet.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Size.hpp"

#include "Table.hpp"
#include "StreamDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_PtndJoin,CK_PTNDJOIN)
__aNewFunction(Card_PtndJoin,CN_CARD)
__vDeleteFunction(Card_PtndJoin,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PtndJoin::Card_PtndJoin(void)
: Card(CK_PTNDJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PtndJoin::~Card_PtndJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
