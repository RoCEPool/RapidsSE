/*============================================================================*/
/* Card_StreamJoin.cpp -- Stream Join (join in stream sequence)               */
/*============================================================================*/

#include "Card_StreamJoin.hpp"

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

__pCastFunction_Card(STORE, Card_StreamJoin, CK_STREAMJOIN)
__aNewFunction(Card_StreamJoin, CN_CARD)
__vDeleteFunction(Card_StreamJoin, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamJoin::Card_StreamJoin(void)
: Card(CK_STREAMJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamJoin::~Card_StreamJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate the various costs for this join locus (of join permutation seq)   */
/*----------------------------------------------------------------------------*/
i8 Card_StreamJoin::nEstLocusCosts(i8 i_JoinPermSeq,
                                   i8 i_Locus,
                                   i8 nBuff_prev_locus,
                                   i8 nFiltPred_prev,
                                   eXK eLocusCast,
                                   i8 nBuff_joining_with
                                  )
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (274 == i_JoinPermSeq)
    vDebugPoint();

  i8 nNodes = AM::PTNMAP::nKnownNodes();

  i8 nBuff_sent_over_network = 0;
  i8 nBuff_queued_by_unicast = 0;
  i8 nBuff_produced_byLocus  = 0;

  i8 nSend_toOtherNodes      = 0.0;
  d8 dRatio_toBeSent         = 0.0;

  d8 dNodes = nNodes;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  d8 dBuff_perNode = nBuff_prev_locus / dNodes;

  switch (eLocusCast) {

    case XK_BROADCAST:
      // no unicast buffers queued onto this locus
      nBuff_queued_by_unicast = 0;

      // a) calculate network buffers sent by broadcast to this locus

      // special first case
      if (0 == i_Locus && !nFiltPred_prev) {
        nBuff_prev_locus = nBuff_prev_locus / C_EXPANSION_COEFF;
        dBuff_perNode = nBuff_prev_locus / dNodes;
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

      // b) send is reduced by filter
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        vTrace("FILTERPRED: BROADCAST %lld "_Stz, nBuff_sent_over_network);
        while (nFiltPred_prev--)
          nBuff_sent_over_network = C_FILTER_COEFF * nBuff_sent_over_network;
        vTrace("  down to %lld \n"_Stz, nBuff_sent_over_network);
      }

      // c) how much produced by this join locus
      vTrace("BROADCAST joining with %lld "_Stz, nBuff_joining_with);
      if (nBuff_joining_with > nBuff_sent_over_network) {
        nBuff_produced_byLocus =
            C_JOIN_COEFF * nBuff_joining_with * C_EXPANSION_COEFF
              * ((i_Locus - 1) ? (i_Locus - 1) * C_JOIN_LONGER : 1.0);
      }
      else {
        nBuff_produced_byLocus =
            C_JOIN_COEFF * nBuff_sent_over_network * C_EXPANSION_COEFF;
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

      // b) send is reduced by filter
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        vTrace("FILTERPRED: HASHCAST %lld "_Stz, nBuff_sent_over_network);
        while (nFiltPred_prev--)
          nBuff_sent_over_network = C_FILTER_COEFF * nBuff_sent_over_network;
        vTrace("  down to %lld \n"_Stz, nBuff_sent_over_network);
      }

      // c) how much produced by this join locus
      if (nBuff_joining_with > nBuff_sent_over_network) {
        nBuff_produced_byLocus =
            C_JOIN_COEFF * (d8)nBuff_joining_with * C_EXPANSION_COEFF
              * ((i_Locus - 1) ? (i_Locus - 1) * C_JOIN_LONGER : 1.0);
      }
      else {
        nBuff_produced_byLocus =
            C_JOIN_COEFF * nBuff_sent_over_network * C_EXPANSION_COEFF;
      }


      break;

    case XK_UNICAST:
      // no buffers arriving to this locus because of network
      nBuff_sent_over_network = 0;

      // special first case
      if (1 == i_Locus && !nFiltPred_prev) {
        nBuff_prev_locus = nBuff_prev_locus / C_EXPANSION_COEFF;
        dBuff_perNode = nBuff_prev_locus / dNodes;
      }

      // a) calculate buffers arriving to this locus because of a unicast
      nBuff_queued_by_unicast = (i8)(1.0 * (nBuff_prev_locus));

      // b) send is reduced by filter
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        vTrace("FILTERPRED: UNICAST %lld "_Stz, nBuff_queued_by_unicast);
        while (nFiltPred_prev--)
          nBuff_queued_by_unicast = C_FILTER_COEFF * nBuff_queued_by_unicast;
        vTrace("  down to %lld \n"_Stz, nBuff_queued_by_unicast);
      }

      // b) how much produced by this join locus
      if (nBuff_joining_with > nBuff_queued_by_unicast) {
        nBuff_produced_byLocus =
            C_JOIN_COEFF * nBuff_joining_with * C_EXPANSION_COEFF
              * ((i_Locus - 1) ? (i_Locus - 1) * C_JOIN_LONGER : 1.0);

      }
      else {
        nBuff_produced_byLocus =
            C_JOIN_COEFF * nBuff_queued_by_unicast * C_EXPANSION_COEFF;
      }

      vTrace("UNICAST produces %lld\n"_Stz, nBuff_produced_byLocus);

      break;

    default:
      NEVER_REACH
  }

  vTrace("PRODUCED: %lld\n"_Stz, nBuff_produced_byLocus);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
