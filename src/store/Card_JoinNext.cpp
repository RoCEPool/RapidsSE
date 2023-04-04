/*============================================================================*/
/* Card_JoinNext.cpp -- Join Head (head of join sequence)                     */
/*============================================================================*/

#include "Card_JoinNext.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CastNext.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CostNext.hpp"
#include "Card_CostJoining.hpp"
#include "Card_CostNetwork.hpp"
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
/* estimate cost of next join                                                 */
/*----------------------------------------------------------------------------*/
i8 Card_JoinNext::nEstNextCosts(eXK eNextCast,
                                i8 nBuff_arriving,
                                i8 nBuff_joiningTo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  Card_CostNext*    p_costNext    = new Card_CostNext;
  Card_CastNext*    p_castNext    = new Card_CastNext;
  Card_CostNetwork* p_costNetwork = new Card_CostNetwork;
  Card_LoadUnicast* p_loadUnicast = new Card_LoadUnicast;
  Card_CostJoining* p_costJoining = new Card_CostJoining;

  p_castNext->vValue(aXK[eNextCast]);

  p_costNetwork->vEstNetworkVolume(eNextCast, nBuff_arriving, nBuff_joiningTo);

  p_loadUnicast->vEstUnicastVolume(eNextCast, nBuff_arriving, nBuff_joiningTo);

  i8 nBuff_produced = p_costJoining->nEstJoinVolume(eNextCast, nBuff_arriving, nBuff_joiningTo);

  p_costNext->vAppend(p_castNext);
  p_costNext->vAppend(p_costNetwork);
  p_costNext->vAppend(p_loadUnicast);
  p_costNext->vAppend(p_costJoining);
  vAppend(p_costNext);

  return nBuff_produced;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
