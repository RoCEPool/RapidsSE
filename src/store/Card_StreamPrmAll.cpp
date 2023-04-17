/*============================================================================*/
/* Card_StreamPrmAll.cpp -- Join Permutation Set (of lists)                    */
/*============================================================================*/

#include "Card_StreamPrmAll.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_StreamHead.hpp"
#include "Card_StreamPer.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"
#include "PermGen.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_StreamPrmAll, CK_STREAMPRMALL)
__aNewFunction(Card_StreamPrmAll, CN_CARD)
__vDeleteFunction(Card_StreamPrmAll, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamPrmAll::Card_StreamPrmAll(void)
: Card(CK_STREAMPRMALL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamPrmAll::~Card_StreamPrmAll(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute the set of stream join permutations                                */
/*----------------------------------------------------------------------------*/
Card_StreamPrmAll* Card_StreamPrmAll::pComputeAllPermutations(Card_TableList* p_tableList,
                                                              Card_JoinPredList* p_joinPredList)
{
  _static_

  Card_StreamPrmAll* p_streamPrmAll = new Card_StreamPrmAll;

  i8 nTables = p_tableList->nChildCards();
  PermGen* pPermGen = new PermGen(nTables);

  i8 i_Permutation = 0;
  while (i_Permutation < pPermGen->nPermutations(nTables)) {

    // calc the join sequence for this stream permutation
    pPermGen->vCalcPermutation(i_Permutation);

    Card_StreamPer* p_streamPer = new Card_StreamPer;
    p_streamPer->pSymbol()->Y_vSet(P_Permutation, i_Permutation);

    // create the permuted join sequence for the stream, testing if sequence is valid
    i8 i_InvalidAtJoin = p_streamPer->iCreateJoinSeq(p_tableList,
                                                     p_joinPredList,
                                                     pPermGen);
    p_streamPrmAll->vAppend(p_streamPer);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // valid permutation

    if (C_UNKNOWN == i_InvalidAtJoin) {
      //vTrace("VALID PERMUTATION %lld\n"_Stz, i_Permutation);
      i_Permutation++;
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // invalid permutation

    //vTrace("PERMUTATION INVALID %lld at %lld\n"_Stz, i_Permutation,i_InvalidAtJoin);

    i_Permutation = pPermGen->iNextValid(i_InvalidAtJoin, i_Permutation);
    //vTrace("RESTARTING FROM %lld\n"_Stz, i_Permutation);
    vDebugPoint();
  }

  delete pPermGen;

  return p_streamPrmAll;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute the load of all permutations                                       */
/*----------------------------------------------------------------------------*/
void Card_StreamPrmAll::vEstCostsForAllPermutations(Card_TableList*    p_tableList,
                                                    Card_FiltPredList* p_filtPredList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain())
    xCI.G.p_streamPrm->vEstStreamCost(p_tableList, p_filtPredList);
}

/*----------------------------------------------------------------------------*/
/* pick given order                                                           */
/*----------------------------------------------------------------------------*/
Card_StreamPer* Card_StreamPrmAll::pJoinOrder(i8 iJoinOrder)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iPermutation;
  Card_StreamPer* p_joinPermSeq = nullptr;

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_STREAMPER)) {
    p_joinPermSeq = xCI.G.p_streamPrm;
    iPermutation = p_joinPermSeq->pSymbol()->Y_iGet(P_Permutation);
    if (iJoinOrder == iPermutation)
      return (p_joinPermSeq);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* pick permutation with lowest cost                                          */
/*----------------------------------------------------------------------------*/
Card_StreamPer* Card_StreamPrmAll::pLowestCost(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_StreamPer* pLowest = nullptr;

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_STREAMPER)) {
    if (! pLowest)
      pLowest = xCI.G.p_streamPrm;
    else
    if (xCI.G.p_streamPrm->bIsLessCost(pLowest))
      pLowest = xCI.G.p_streamPrm;
  }

  return pLowest;
}

/*----------------------------------------------------------------------------*/
/* compute the set of join permutations                                       */
/*----------------------------------------------------------------------------*/
void Card_StreamPrmAll::vRemoveInvalid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    if (xCI.G.p_streamPrm->pSymbol()->Y_bHas(P_Invalid))
      xCI.G_vRemoveDeleteMain(CK_STREAMPER);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
