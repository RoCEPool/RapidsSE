/*============================================================================*/
/* Card_CostNetwork.cpp -- Load of Network (in load of join)                  */
/*============================================================================*/

#include "Card_CostNetwork.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Size.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CostNetwork, CK_COSTNETWORK)
__aNewFunction(Card_CostNetwork, CN_CARD)
__vDeleteFunction(Card_CostNetwork, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CostNetwork::Card_CostNetwork(void)
: Card(CK_COSTNETWORK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CostNetwork::~Card_CostNetwork(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate possible buffers arriving at this locus over the network          */
/*----------------------------------------------------------------------------*/
void Card_CostNetwork::vEstNetworkBufferVolume(i8 nBuff_emitted_prior,
                                               eXK eLocusCast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // todo: complete...

  i8 nPtnsPerNode = AM::PTNMAP::nPtnsPerNode();
  i8 nNodes = AM::PTNMAP::nKnownNodes();

  i8 nBuff_network = 0;

  switch (eLocusCast) {
    case XK_UNICAST:
      // no network costs associated with a unicast
      break;

    case XK_HASHCAST:
      // network cost for a hashcast to this join locus
      nBuff_network = (i8)(1.0 * nBuff_emitted_prior);
      break;

    case XK_BROADCAST:
      // network cost for a broadcast to this join locus
      nBuff_network = (i8)(1.0 * (nBuff_emitted_prior * nNodes));
      break;

    default:
      NEVER_REACH
  }

  vValue(nBuff_network);
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
