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
                                  i8 nBuff_prior,
                                  eXK eLocusCast,
                                  i8 nBuff_joining_with,
                                  i8 nFiltPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nNodes = AM::PTNMAP::nKnownNodes();

  i8 nBuff_sent_over_network = 0;
  i8 nBuff_queued_by_unicast = 0;
  i8 nBuff_produced_byLocus  = 0;

  i8 nSend_toOtherNodes      = 0.0;
  d8 dRatio_toBeSent         = 0.0;

  d8 dNodes = nNodes;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  d8 dBuff_perNode = nBuff_prior / dNodes;

  switch (eLocusCast) {

    case XK_BROADCAST:
      // no unicast buffers queued onto this locus
      nBuff_queued_by_unicast = 0;

      // a) calculate network buffers sent by broadcast to this locus

      // special first case
      if (1 == nLocus) {
        nBuff_prior = nBuff_prior / C_EXPANSION_COEFF;
        dBuff_perNode = nBuff_prior / dNodes;
      }

      //
      if (1 == nNodes)
        nBuff_sent_over_network = 0;
      else {
        nSend_toOtherNodes        =  // 1 2 3
            nNodes - 1;
        dRatio_toBeSent    =  // 1
            1.0;
        nBuff_sent_over_network =  //
            nNodes * (dBuff_perNode * dRatio_toBeSent * nSend_toOtherNodes);
      }

      // b) how much produced by this join locus
      if (nBuff_joining_with > nBuff_sent_over_network) {
        nBuff_produced_byLocus =
            2 * nBuff_joining_with * C_EXPANSION_COEFF;
      }
      else {
        nBuff_produced_byLocus =
            2 * nBuff_sent_over_network * C_EXPANSION_COEFF;
      }

      // c) produced is then reduced by filter
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        vTrace("FILTERPRED: BROADCAST %lld "_Stz, nBuff_produced_byLocus);
        while (nFiltPred--)
          nBuff_produced_byLocus = C_FILTER_COEFF * nBuff_produced_byLocus;
        vTrace("  down to %lld \n"_Stz, nBuff_produced_byLocus);
      }

      break;

    case XK_HASHCAST:
      // no unicast buffers queued onto this locus
      nBuff_queued_by_unicast = 0;

      // a) calculate network buffers sent by hashcast to this locus
      if (1 == nNodes)
        nBuff_sent_over_network = 0;
     else {
        nSend_toOtherNodes        = // 1 2 3
            nNodes - 1;
        dRatio_toBeSent    = // 1/2 2/3 3/4
            (dNodes - 1.0) / (dNodes);
        nBuff_sent_over_network = //
            nNodes * (dBuff_perNode * dRatio_toBeSent * nSend_toOtherNodes);
      }

      // b) how much produced by this join locus
      if (nBuff_joining_with > nBuff_sent_over_network) {
        nBuff_produced_byLocus =
            2 * nBuff_joining_with * C_EXPANSION_COEFF;
      }
      else {
        nBuff_produced_byLocus =
            2 * nBuff_sent_over_network* C_EXPANSION_COEFF;
      }

      // c) produced is then reduced by filter
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        vTrace("FILTERPRED: HASHCAST %lld "_Stz, nBuff_produced_byLocus);
        while (nFiltPred--)
          nBuff_produced_byLocus = C_FILTER_COEFF * nBuff_produced_byLocus;
        vTrace("  down to %lld \n"_Stz, nBuff_produced_byLocus);
      }

      break;

    case XK_UNICAST:
      // no buffers arriving to this locus because of network
      nBuff_sent_over_network = 0;

      // a) calculate buffers arriving to this locus because of a unicast
      nBuff_queued_by_unicast = (i8)(1.0 * (nBuff_prior));

      // b) how much produced by this join locus
      if (nBuff_joining_with > nBuff_queued_by_unicast) {
        nBuff_produced_byLocus =
            2 * nBuff_joining_with;
      }
      else {
        nBuff_produced_byLocus =
            2 * nBuff_queued_by_unicast;
      }

      // c) produced is then reduced by filter
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        if (nFiltPred)
          nBuff_produced_byLocus = 0.2 * nBuff_produced_byLocus;
      }

      break;

    default:
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_LocusCast* p_locusCast = new Card_LocusCast;
  p_locusCast->vValue(aXK[eLocusCast]);

  Card_CostLocus*   p_costLocus   = new Card_CostLocus;
  Card_CostUnicast* p_costUnicast = new Card_CostUnicast;
  Card_CostNetwork* p_costNetwork = new Card_CostNetwork;
  Card_CostJoining* p_costJoining = new Card_CostJoining;


  p_costUnicast->vValue(nBuff_queued_by_unicast);
  p_costNetwork->vValue(nBuff_sent_over_network);
  p_costJoining->vValue(nBuff_produced_byLocus);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_costLocus->vAppend(p_locusCast);
  p_costLocus->vAppend(p_costNetwork);
  p_costLocus->vAppend(p_costUnicast);
  p_costLocus->vAppend(p_costJoining);
  vAppend(p_costLocus);

  return nBuff_produced_byLocus;
}
/*----------------------------------------------------------------------------*/
//i8 Card_JoinLocus::nEstLocusCosts(i8 nLocus,
//                                  i8 nBuff_emitted_prior,
//                                  eXK eLocusCast,
//                                  i8 nBuff_joining_with,
//                                  i8 nFiltPred)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  i8 nNodes = AM::PTNMAP::nKnownNodes();
//
//  i8 nBuff_produced_locus = 0;
//  i8 nBuff_network = 0;
//  i8 nBuff_unicast = 0;
//
//  i8 nEachNodeSends = 0.0;
//  d8 dRatio_toOthersNodes = 0.0;
//
//
//
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // estimate buffers arriving to this join locus
//
//  d8 dNodes = nNodes;
//  d8 dData_prior = nBuff_emitted_prior;
//  d8 dDataPerNode = nBuff_emitted_prior / dNodes;
//
//  switch (eLocusCast) {
//
//    case XK_BROADCAST:
//      // network buffers arriving at this locus because of a broadcast
//
//      if (1 == nNodes)
//        nBuff_network = 0;
//      else {
//        nEachNodeSends       =  // 1 2 3
//            nNodes - 1;
//        dRatio_toOthersNodes =  // 1
//            1.0;
//        nBuff_network        =  //
//            nNodes * nEachNodeSends * (dDataPerNode * dRatio_toOthersNodes);
//      }
//
//      if (nBuff_joining_with > nBuff_network) {
//        nBuff_produced_locus =
//            2 * nBuff_joining_with;
//      }
//      else {
//        nBuff_produced_locus =
//            2 * nBuff_network;
//      }
//
//      if (AM::JOB::bHasTag(C_FILTERPRED)) {
//        if (nFiltPred)
//          nBuff_produced_locus = 0.2 * nBuff_produced_locus;
//      }
//
//      break;
//
//    case XK_HASHCAST:
//      // no unicast buffers arriving to this locus via hashcast
//
//      // network buffers arriving at this locus because of a hashcast
//      if (1 == nNodes)
//        nBuff_network = 0;
//      else {
//        nEachNodeSends       = // 1 2 3
//            nNodes - 1;
//        dRatio_toOthersNodes = // 1/2 2/3 3/4
//            (dNodes - 1.0) / (dNodes);
//        nBuff_network        =
//            nNodes * nEachNodeSends * (dDataPerNode * dRatio_toOthersNodes);
//      }
//
//      if (nBuff_joining_with > nBuff_network) {
//        nBuff_produced_locus =
//            2 * nBuff_joining_with;
//      }
//      else {
//        nBuff_produced_locus =
//            2 * nBuff_network;
//      }
//
//      if (AM::JOB::bHasTag(C_FILTERPRED)) {
//        if (nFiltPred)
//          nBuff_produced_locus = 0.2 * nBuff_produced_locus;
//      }
//
//      break;
//
//    case XK_UNICAST:
//      // buffers arriving to this locus because of a unicast
//      nBuff_unicast = (i8)(1.0 * (nBuff_emitted_prior));
//
//      // no network associated with a unicast to this locus
//
//
//      if (nBuff_joining_with > nBuff_unicast) {
//        nBuff_produced_locus =
//            2 * nBuff_joining_with;
//      }
//      else {
//        nBuff_produced_locus =
//            2 * nBuff_unicast;
//      }
//
//      if (AM::JOB::bHasTag(C_FILTERPRED)) {
//        if (nFiltPred)
//          nBuff_produced_locus = 0.2 * nBuff_produced_locus;
//      }
//
//      break;
//
//    default:
//      NEVER_REACH
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // estimate buffers produced by this join locus
//
//
//  Card_LocusCast* p_locusCast = new Card_LocusCast;
//  p_locusCast->vValue(aXK[eLocusCast]);
//
//  Card_CostLocus*   p_costLocus   = new Card_CostLocus;
//  Card_CostUnicast* p_costUnicast = new Card_CostUnicast;
//  Card_CostNetwork* p_costNetwork = new Card_CostNetwork;
//  Card_CostJoining* p_costJoining = new Card_CostJoining;
//
//
//  p_costUnicast->vValue(nBuff_unicast);
//  p_costNetwork->vValue(nBuff_network);
//  p_costJoining->vValue(nBuff_produced_locus);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  p_costLocus->vAppend(p_locusCast);
//  p_costLocus->vAppend(p_costNetwork);
//  p_costLocus->vAppend(p_costUnicast);
//  p_costLocus->vAppend(p_costJoining);
//  vAppend(p_costLocus);
//
//  return nBuff_produced_locus;
//}
/*----------------------------------------------------------------------------*/
//i8 Card_JoinLocus::nEstLocusCosts(i8 nLocus,
//                                  i8 nBuff_emitted_prior,
//                                  eXK eLocusCast,
//                                  i8 nBuff_joining_with,
//                                  i8 nFiltPred)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  Card_LocusCast* p_locusCast = new Card_LocusCast;
//  p_locusCast->vValue(aXK[eLocusCast]);
//
//  Card_CostLocus*   p_costLocus   = new Card_CostLocus;
//  Card_CostUnicast* p_costUnicast = new Card_CostUnicast;
//  Card_CostNetwork* p_costNetwork = new Card_CostNetwork;
//  Card_CostJoining* p_costJoining = new Card_CostJoining;
//
//  i8 nNodes = AM::PTNMAP::nKnownNodes();
//
//  i8 nBuff_produced_locus = 0;
//  i8 nBuff_network = 0;
//  i8 nBuff_unicast = 0;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // estimate buffers arriving to this join locus
//
//  switch (eLocusCast) {
//
//    case XK_BROADCAST:
//      // no unicast buffers arriving to this locus via broadcast
//
//      // network buffers arriving at this locus because of a broadcast
//      if (1 == nNodes)
//        nBuff_network = (i8)(1.0 * (nBuff_emitted_prior));
//      else
//        nBuff_network = (i8)(1.0 * (nBuff_emitted_prior * (nNodes - 1)));
//      break;
//
//    case XK_HASHCAST:
//      // no unicast buffers arriving to this locus via hashcast
//
//      // network buffers arriving at this locus because of a hashcast
//      if (1 == nNodes)
//        nBuff_network = (i8)(1.0 * (nBuff_emitted_prior));
//      else
//        nBuff_network = (i8)(1.0 * (nBuff_emitted_prior * (nNodes - 1)) / nNodes);
//      break;
//
//    case XK_UNICAST:
//      // buffers arriving to this locus because of a unicast
//      nBuff_unicast = (i8)(1.0 * (nBuff_emitted_prior));
//
//      // no network associated with a unicast to this locus
//      break;
//
//    default:
//      NEVER_REACH
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // estimate buffers produced by this join locus
//
//  if (nBuff_joining_with > nBuff_emitted_prior) {
//    nBuff_produced_locus =
//        2 * nBuff_joining_with;
//  }
//  else {
//    nBuff_produced_locus =
//        2 * nBuff_emitted_prior;
//  }
//
//  if (AM::JOB::bHasTag(C_FILTERPRED)) {
//    //while (nFiltPred--)
//    if (nFiltPred)
//      nBuff_produced_locus = 0.2 * nBuff_produced_locus;
//  }
//
//
//  p_costUnicast->vValue(nBuff_unicast);
//  p_costNetwork->vValue(nBuff_network);
//  p_costJoining->vValue(nBuff_produced_locus);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  p_costLocus->vAppend(p_locusCast);
//  p_costLocus->vAppend(p_costNetwork);
//  p_costLocus->vAppend(p_costUnicast);
//  p_costLocus->vAppend(p_costJoining);
//  vAppend(p_costLocus);
//
//  return nBuff_produced_locus;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
