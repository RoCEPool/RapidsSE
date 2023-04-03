/*============================================================================*/
/* Card_LoadNetwork.cpp -- Load of Network (in load of join)                  */
/*============================================================================*/

#include "Card_LoadNetwork.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Size.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
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
/* estimate network volume (as part of cost estimate of next join)            */
/*----------------------------------------------------------------------------*/
void Card_LoadNetwork::vEstNetworkVolume(eXK eNextCast,
                                         i8 nBuff_arriving,
                                         i8 nBuff_joiningTo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // todo: complete...

  i8 nPtnsPerNode = AM::PTNMAP::nPtnsPerNode();
  i8 nNodes = AM::PTNMAP::nKnownNodes();

  i8 iLoad = 0;
  i8 nBuffersOut = 0;

  switch (eNextCast) {
    case XK_UNICAST:
      break;

    case XK_HASHCAST:
      iLoad = (i8)(1.0 * nBuff_arriving);
      nBuffersOut = (i8)(1.0 * (nBuff_arriving * nBuff_joiningTo));
      break;

    case XK_BROADCAST:
      iLoad = (i8)(1.0 * (nBuff_arriving * nPtnsPerNode));
      nBuffersOut = (i8)(1.0 * (nBuff_arriving * nBuff_joiningTo * nPtnsPerNode));
      break;

    default:
      NEVER_REACH
  }

  vValue((i8)iLoad);
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
