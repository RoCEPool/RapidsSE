/*============================================================================*/
/* Card_JoinNext.cpp -- Join Head (head of join sequence)                     */
/*============================================================================*/

#include "Card_JoinNext.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_Cast.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Load.hpp"
#include "Card_LoadJoined.hpp"
#include "Card_LoadNetwork.hpp"
#include "Card_LoadUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"

#include "eXK.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_JoinNext, CK_JOINNEXT)
__aNewFunction(Card_JoinNext, CN_CARD)
__vDeleteFunction(Card_JoinNext, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinNext::Card_JoinNext(void)
: Card(CK_JOINNEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinNext::~Card_JoinNext(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate the arriving network, arriving unicast and join volumes           */
/*----------------------------------------------------------------------------*/
i8 Card_JoinNext::nEstVolumes(i8 nNodes,
                              eXK eNextCast,
                              i8 nBuff_arriving,
                              Card_TableList* p_tableList,
                              char cTblSyn_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  Card_Load*        p_loadJoin    = new Card_Load;
  Card_Cast*        p_cast        = new Card_Cast;
  Card_LoadNetwork* p_loadNetwork = new Card_LoadNetwork;
  Card_LoadUnicast* p_loadUnicast = new Card_LoadUnicast;
  Card_LoadJoined*  p_loadJoined  = new Card_LoadJoined;

  p_cast->vValue(aXK[eNextCast]);

  // estimate arriving network volume
  p_loadNetwork->nEstNetworkVol(nNodes, eNextCast, nBuff_arriving,
                                p_tableList, cTblSyn_r);

  // estimate arriving unicast volume
  p_loadUnicast->nEstUnicastVol(nNodes, eNextCast, nBuff_arriving,
                                p_tableList, cTblSyn_r);

  // estimate buffers produced by join
  i8 nBuff_produced  =
      p_loadJoined->nEstJoinVol(nNodes, nBuff_arriving,
                                p_tableList, cTblSyn_r);

  p_loadJoin->vAppend(p_cast);
  p_loadJoin->vAppend(p_loadNetwork);
  p_loadJoin->vAppend(p_loadUnicast);
  p_loadJoin->vAppend(p_loadJoined);
  vAppend(p_loadJoin);

  return nBuff_produced;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
