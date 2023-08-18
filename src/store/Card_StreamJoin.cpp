/*============================================================================*/
/* Card_StreamJoin.cpp -- Stream Join (join in stream sequence)               */
/*============================================================================*/

#include "Card_StreamJoin.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_JoinCast.hpp"
#include "Card_CmpPred.hpp"
#include "Card_JoinEstimate.hpp"
#include "Card_CostJoining.hpp"
#include "Card_CostNetwork.hpp"
#include "Card_CostUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"

#include "eXK.h"
#include "CardIter.hpp"

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
: Card_symbol(CK_STREAMJOIN, C_UNKNOWN)
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
i8 Card_StreamJoin::nEstJoinCost(i8 i_Permutation,
                                 i8 i_Join,
                                 i8 nUnfiltBuff_prev_join,
                                 i8 nFiltPred_prev,
                                 eXK eJoinCast,
                                 i8 nBuff_joining_with)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (274 == i_Permutation)
    vDebugPoint();

  i8 nNodes = AM::PTNMAP::nKnownNodes();

  i8 nBuff_recv_over_network = 0;
  i8 nBuff_popped_by_unicast = 0;
  i8 nUnfiltBuff_produced_by_join  = 0;

  d8 dFilterReduction = 1.0;
  i8 nOtherNodes      = 0.0;
  d8 dRatio_sent         = 0.0;

  d8 dNodes = nNodes;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  d8 dPrevBuff_perNode = nUnfiltBuff_prev_join / dNodes;

  switch (eJoinCast) {

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case XK_BROADCAST:
      // no unicast buffers
      nBuff_popped_by_unicast = 0;

      // a) calculate network buffers sent by broadcast to this locus

      // special first case
      if (0 == i_Join && !nFiltPred_prev) {
        nUnfiltBuff_prev_join = nUnfiltBuff_prev_join / C_EXPANSION_COEFF;
        dPrevBuff_perNode = nUnfiltBuff_prev_join / dNodes;
      }

      //
      if (1 == nNodes)
        nBuff_recv_over_network = 0;
      else {
        nOtherNodes = nNodes - 1; // 1 2 3
        dRatio_sent = 1.0;        // 1
        nBuff_recv_over_network =
            nNodes * (dPrevBuff_perNode * dRatio_sent * nOtherNodes);
      }

      // b) send is reduced by filter
      dFilterReduction = 1.0;
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        while (nFiltPred_prev--)
          dFilterReduction *= C_FILTER_COEFF;
      }
      nBuff_recv_over_network *= dFilterReduction;

      // c) how much produced by this join
      if (nBuff_joining_with > nBuff_recv_over_network) {
        nUnfiltBuff_produced_by_join =
            C_JOIN_COEFF * nBuff_joining_with * C_EXPANSION_COEFF
              * ((i_Join) ? (i_Join) * C_JOIN_LONGER : 1.0);
      }
      else {
        nUnfiltBuff_produced_by_join =
            C_JOIN_COEFF * nBuff_recv_over_network * C_EXPANSION_COEFF;
      }

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case XK_HASHCAST:
      // no unicast buffers queued onto this locus
      nBuff_popped_by_unicast = 0;

      // a) calculate network buffers sent by hashcast to this locus
      if (1 == nNodes)
        nBuff_recv_over_network = 0;
     else {
        nOtherNodes = nNodes - 1; // 1 2 3
        dRatio_sent = (dNodes - 1.0) / (dNodes); // 1/2 2/3 3/4
        nBuff_recv_over_network =
            nNodes * (dPrevBuff_perNode * dRatio_sent * nOtherNodes);
      }

      // b) send is reduced by filter
      dFilterReduction = 1.0;
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        while (nFiltPred_prev--)
          dFilterReduction *= C_FILTER_COEFF;
      }
      nBuff_recv_over_network *= dFilterReduction;

      // c) how much produced by this join locus
      if (nBuff_joining_with > nBuff_recv_over_network) {
        nUnfiltBuff_produced_by_join =
            C_JOIN_COEFF * (d8)nBuff_joining_with * C_EXPANSION_COEFF
              * ((i_Join) ? (i_Join ) * C_JOIN_LONGER : 1.0);
      }
      else {
        nUnfiltBuff_produced_by_join =
            C_JOIN_COEFF * nBuff_recv_over_network * C_EXPANSION_COEFF;
      }

      break;

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case XK_UNICAST:
      // no buffers arriving to this locus because of network
      nBuff_recv_over_network = 0;

      // special first case
      if (1 == i_Join && !nFiltPred_prev) {
        nUnfiltBuff_prev_join = nUnfiltBuff_prev_join / C_EXPANSION_COEFF;
        dPrevBuff_perNode = nUnfiltBuff_prev_join / dNodes;
      }

      // a) calculate buffers arriving to this locus because of a unicast
      nBuff_popped_by_unicast = (i8)(1.0 * (nUnfiltBuff_prev_join));

      // b) send is reduced by filter
      dFilterReduction = 1.0;
      if (AM::JOB::bHasTag(C_FILTERPRED)) {
        while (nFiltPred_prev--)
          dFilterReduction *= C_FILTER_COEFF;
      }
      nBuff_popped_by_unicast *= dFilterReduction;

      // b) how much produced by this join locus
      if (nBuff_joining_with > nBuff_popped_by_unicast) {
        nUnfiltBuff_produced_by_join =
            C_JOIN_COEFF * nBuff_joining_with * C_EXPANSION_COEFF
              * ((i_Join) ? (i_Join) * C_JOIN_LONGER : 1.0);

      }
      else {
        nUnfiltBuff_produced_by_join =
            C_JOIN_COEFF * nBuff_popped_by_unicast * C_EXPANSION_COEFF;
      }

      break;

    default:
      NEVER_REACH
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_JoinCast* p_joinCast = new Card_JoinCast;
  p_joinCast->SYM(SF_XK_CAST,aXK[eJoinCast]);

  Card_CostUnicast*  p_costUnicast  = new Card_CostUnicast;
  Card_CostNetwork*  p_costNetwork  = new Card_CostNetwork;
  Card_CostJoining*  p_costJoining  = new Card_CostJoining;

  p_costUnicast->SYM(SF_INT_LIT,nBuff_popped_by_unicast);
  p_costNetwork->SYM(SF_INT_LIT,nBuff_recv_over_network);
  p_costJoining->SYM(SF_INT_LIT,nUnfiltBuff_produced_by_join);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_JoinEstimate* p_joinEstimate = new Card_JoinEstimate;

  p_joinEstimate->vAppend(p_joinCast);
  p_joinEstimate->vAppend(p_costNetwork);
  p_joinEstimate->vAppend(p_costUnicast);
  p_joinEstimate->vAppend(p_costJoining);

  vAppend(p_joinEstimate);

  return nUnfiltBuff_produced_by_join;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_StreamJoin::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
