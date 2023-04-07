/*============================================================================*/
/* Card_JoinLocus.cpp -- Join Head (head of join sequence)                     */
/*============================================================================*/

#include "Card_JoinLocus.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_LocusCast.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CostLocus.hpp"
#include "Card_CostJoining.hpp"
#include "Card_CostNetwork.hpp"
#include "Card_CostUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"

#include "eXK.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_JoinLocus, CK_JOINLOCUS)
__aNewFunction(Card_JoinLocus, CN_CARD)
__vDeleteFunction(Card_JoinLocus, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinLocus::Card_JoinLocus(void)
: Card(CK_JOINLOCUS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinLocus::~Card_JoinLocus(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate the various costs for this join locus                             */
/*----------------------------------------------------------------------------*/
i8 Card_JoinLocus::nEstLocusCosts(i8 nLocus,
                                  i8 nBuff_emitted_prior,
                                  eXK eLocusCast,
                                  i8 nBuff_joining_with,
                                  i8 nFiltPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LocusCast* p_locusCast = new Card_LocusCast;
  p_locusCast->vValue(aXK[eLocusCast]);

  Card_CostLocus*   p_costLocus   = new Card_CostLocus;
  Card_CostUnicast* p_costUnicast = new Card_CostUnicast;
  Card_CostNetwork* p_costNetwork = new Card_CostNetwork;
  Card_CostJoining* p_costJoining = new Card_CostJoining;

  // calculate the possible unicast cost for this join locus
  p_costUnicast->vEstUnicastBufferVolume(nLocus, nBuff_emitted_prior, eLocusCast);

  // calculate the possible network cost for this join locus
  p_costNetwork->vEstNetworkBufferVolume(nLocus, nBuff_emitted_prior, eLocusCast);

  // calculate buffers produced by this join locus
  i8 nBuff_produced_locus =
      p_costJoining->nEstJoiningBufferVolume(nLocus, nBuff_emitted_prior, eLocusCast,
                                             nBuff_joining_with, nFiltPred);

  p_costLocus->vAppend(p_locusCast);
  p_costLocus->vAppend(p_costNetwork);
  p_costLocus->vAppend(p_costUnicast);
  p_costLocus->vAppend(p_costJoining);
  vAppend(p_costLocus);

  return nBuff_produced_locus;
}
/*----------------------------------------------------------------------------*/
i8 Card_JoinLocus::nEstLocusCosts_v2(i8 nLocus,
                                     i8 nBuff_emitted_prior,
                                     eXK eLocusCast,
                                     i8 nBuff_joining_with,
                                     i8 nFiltPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_LocusCast* p_locusCast = new Card_LocusCast;
  p_locusCast->vValue(aXK[eLocusCast]);

  Card_CostLocus*   p_costLocus   = new Card_CostLocus;
  Card_CostUnicast* p_costUnicast = new Card_CostUnicast;
  Card_CostNetwork* p_costNetwork = new Card_CostNetwork;
  Card_CostJoining* p_costJoining = new Card_CostJoining;

  i8 nBuff_produced_locus = 0;

  i8 nPtnsPerNode = AM::PTNMAP::nPtnsPerNode();
  i8 nNodes = AM::PTNMAP::nKnownNodes();

  i8 nBuff_network = 0;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 nBuff_unicast = 0;

  switch (eLocusCast) {

    case XK_UNICAST:
      // buffers arriving to this locus because of a unicast
      nBuff_unicast = (i8)(1.0 * (nBuff_emitted_prior));

      // no network associated with a unicast to this locus
      break;

    case XK_HASHCAST:
      // no unicast buffers arriving to this locus via hashcast

      // network buffers arriving at this locus because of a hashcast
      nBuff_network = (i8)(1.0 * nBuff_emitted_prior);
      break;

    case XK_BROADCAST:
      // no unicast buffers arriving to this locus via broadcast

      // network buffers arriving at this locus because of a broadcast
      if (1 == nNodes)
        nBuff_network = (i8)(1.0 * (nBuff_emitted_prior));
      else
        nBuff_network = (i8)(1.0 * (nBuff_emitted_prior * (nNodes - 1)));
      break;

    default:
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // estimate buffers produced by the join of this locus

  if (nBuff_joining_with > nBuff_emitted_prior) {
    nBuff_produced_locus =
        2 * nBuff_joining_with;
  }
  else {
    nBuff_produced_locus =
        2 * nBuff_emitted_prior;
  }

  if (AM::JOB::bHasTag(C_FILTERPRED)) {
    while (nFiltPred--)
      nBuff_produced_locus = 0.2 * nBuff_produced_locus;
  }


  p_costUnicast->vValue(nBuff_unicast);
  p_costNetwork->vValue(nBuff_network);
  p_costJoining->vValue(nBuff_produced_locus);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_costLocus->vAppend(p_locusCast);
  p_costLocus->vAppend(p_costNetwork);
  p_costLocus->vAppend(p_costUnicast);
  p_costLocus->vAppend(p_costJoining);
  vAppend(p_costLocus);

  return nBuff_produced_locus;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
