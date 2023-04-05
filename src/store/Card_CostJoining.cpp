/*============================================================================*/
/* Card_LoadJoined.cpp -- Load of the actual Join                              */
/*============================================================================*/

#include "Card_CostJoining.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CostNetwork.hpp"
#include "Card_CostUnicast.hpp"
#include "Card_Size.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CostJoining, CK_COSTJOINING)
__aNewFunction(Card_CostJoining, CN_CARD)
__vDeleteFunction(Card_CostJoining, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CostJoining::Card_CostJoining(void)
: Card(CK_COSTJOINING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CostJoining::~Card_CostJoining(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate buffers produced by the join of this locus                        */
/*----------------------------------------------------------------------------*/

i8 Card_CostJoining::nEstJoiningBufferVolume(i8 nLocus,
                                             i8 nBuff_emitted_prior,
                                             eXK eLocusCast,
                                             i8 nBuff_joining_with,
                                             i8 nFiltPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nBuff_produced_locus = C_UNKNOWN;

  if (nBuff_joining_with > nBuff_emitted_prior) {
    nBuff_produced_locus =
        2 * nBuff_joining_with;
  }
  else {
    nBuff_produced_locus =
        2 * nBuff_emitted_prior;
  }

//  while (nFiltPred--)
//    nBuff_produced_locus = 0.2 * nBuff_produced_locus;

  vValue(nBuff_produced_locus);

  return nBuff_produced_locus;
}

/*----------------------------------------------------------------------------*/

//i8 Card_CostJoining::nEstJoiningBufferVolume(i8 nLocus,
//                                             i8 nBuff_emitted_prior,
//                                             eXK eLocusCast,
//                                             i8 nBuff_joining_with)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  i8 nBuff_produced_locus = C_UNKNOWN;
//
//  if (nBuff_joining_with > nBuff_emitted_prior) {
//    nBuff_produced_locus =
//        (nLocus + 2) * nBuff_joining_with;
//  }
//  else {
//    nBuff_produced_locus =
//        nBuff_emitted_prior + (2 * nBuff_emitted_prior / nLocus);
//  }
//
//  vValue(nBuff_produced_locus);
//
//  return nBuff_produced_locus;
//}

/*----------------------------------------------------------------------------*/

//i8 Card_CostJoining::nEstJoiningBufferVolume(i8 nLocus,
//                                             i8 nBuff_emitted_prior,
//                                             eXK eLocusCast,
//                                             i8 nBuff_joining_with)
//
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  i8 nBuff_produced_locus = C_UNKNOWN;
//
//  if (nBuff_joining_with > nBuff_emitted_prior) {
//    nBuff_produced_locus = (nLocus + 2) * nBuff_joining_with;
//  }
//  else {
//    nBuff_produced_locus = (nLocus + 2) * nBuff_emitted_prior;
//  }
//
//  vValue(nBuff_produced_locus);
//
//  return nBuff_produced_locus;
//}

/*----------------------------------------------------------------------------*/

//i8 Card_CostJoining::nEstJoiningBufferVolume(i8 nLocus,
//                                             i8 nBuff_emitted_prior,
//                                             eXK eLocusCast,
//                                             i8 nBuff_joining_with)
//
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  i8 nBuff_produced_locus = (nLocus + 2) * nBuff_joining_with;
//
//  vValue(nBuff_produced_locus);
//
//  return nBuff_produced_locus;
//}

/*----------------------------------------------------------------------------*/

//i8 Card_CostJoining::nEstJoiningBufferVolume(i8 nLocus,
//                                             i8 nBuff_emitted_prior,
//                                             eXK eLocusCast,
//                                             i8 nBuff_joining_with)
//
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
//
//  i8 nBuff_produced_locus = C_UNKNOWN;
//
//  if (nBuff_emitted_prior > nBuff_joining_with) {
//    d8 dExpansionCoeff = (d8) nBuff_emitted_prior / (d8) nBuff_joining_with;
//    nBuff_produced_locus =
//        (i8) (dExpansionCoeff * (d8)(nBuff_emitted_prior + nBuff_joining_with));
//  }
//  else {
//    d8 dExpansionCoeff = (d8) nBuff_joining_with / (d8) nBuff_emitted_prior;
//    nBuff_produced_locus =
//        (i8) (dExpansionCoeff * (d8)(nBuff_emitted_prior + nBuff_joining_with));
//  }
//
//  vValue(nBuff_produced_locus);
//
//  return nBuff_produced_locus;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
