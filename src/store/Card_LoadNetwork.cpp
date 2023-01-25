/*============================================================================*/
/* Card_LoadNetwork.cpp -- Load of Network (in load of join)                  */
/*============================================================================*/

#include "Card_LoadNetwork.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_LoadNetwork, CK_LOADNETWORK)
__aNewFunction(Card_LoadNetwork, CN_CARD)
__vDeleteFunction(Card_LoadNetwork, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_LoadNetwork::Card_LoadNetwork(void)
: Card(CK_LOADNETWORK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_LoadNetwork::~Card_LoadNetwork(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate network load                                                     */
/*----------------------------------------------------------------------------*/
void Card_LoadNetwork::vCalculateLoad(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // todo: complete...

  vValue((i8)0);
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
