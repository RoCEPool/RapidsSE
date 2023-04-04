/*============================================================================*/
/* Card_CostNext.cpp -- Estimate cost for a JoinNext of a Query               */
/*============================================================================*/

#include "Card_CostNext.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CostNetwork.hpp"
#include "Card_CostUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CostNext, CK_COSTNEXT)
__aNewFunction(Card_CostNext, CN_CARD)
__vDeleteFunction(Card_CostNext, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CostNext::Card_CostNext(void)
: Card(CK_COSTNEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*-----------------------Ï-----------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CostNext::~Card_CostNext(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
