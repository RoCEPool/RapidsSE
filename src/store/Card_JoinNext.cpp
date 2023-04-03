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
/* estimate cost of next join                                                 */
/*----------------------------------------------------------------------------*/
i8 Card_JoinNext::nEstNextCosts(eXK eNextCast,
                                i8 nBuff_arriving,
                                i8 nBuff_joiningTo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  Card_Load*        p_load        = new Card_Load;
  Card_Cast*        p_cast        = new Card_Cast;
  Card_LoadNetwork* p_loadNetwork = new Card_LoadNetwork;
  Card_LoadUnicast* p_loadUnicast = new Card_LoadUnicast;
  Card_LoadJoined*  p_loadJoined  = new Card_LoadJoined;

  p_cast->vValue(aXK[eNextCast]);

  p_loadNetwork->vEstNetworkVolume(eNextCast, nBuff_arriving, nBuff_joiningTo);

  p_loadUnicast->vEstUnicastVolume(eNextCast, nBuff_arriving, nBuff_joiningTo);

  i8 nBuff_produced = p_loadJoined->nEstJoinVolume(eNextCast, nBuff_arriving, nBuff_joiningTo);

  p_load->vAppend(p_cast);
  p_load->vAppend(p_loadNetwork);
  p_load->vAppend(p_loadUnicast);
  p_load->vAppend(p_loadJoined);
  vAppend(p_load);

  return nBuff_produced;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
