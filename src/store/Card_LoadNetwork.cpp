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
/* estimate arriving network volume                                           */
/*----------------------------------------------------------------------------*/
void Card_LoadNetwork::vEstNetworkVol(eXK eNextCast,
                                      i8 nBuff_arriving,
                                      Card_TableList* p_tableList,
                                      char cTblSyn_r,
                                      i8 iSize_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nNodes = AM::PTNMAP::nKnownNodes();


  vValue(nBuff_arriving);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  i8 nPtnsPerNode = AM::PTNMAP::nPtnsPerNode();

  i8 nBuff_produced = 0;

  switch (eNextCast) {

    case XK_UNICAST:
      break;

    case XK_HASHCAST:
      nBuff_produced = (i8)(1.0 * (nBuff_arriving * iSize_r));
      break;

    case XK_BROADCAST:
      nBuff_produced = (i8)(1.0 * (nBuff_arriving * iSize_r * nPtnsPerNode));
      break;

    default:
      NEVER_REACH
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
