/*============================================================================*/
/* Card_StreamPrmAll.cpp -- Join Permutation Set (of lists)                    */
/*============================================================================*/

#include "Card_StreamPrmAll.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_StreamHead.hpp"
#include "Card_StreamPrm.hpp"
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
/* compute the set of join permutations                                       */
/*----------------------------------------------------------------------------*/
Card_StreamPrmAll* Card_StreamPrmAll::pAllPermutedJoins(Card_TableList* p_tableList,
                                                        Card_JoinPredList* p_joinPredList)
{
  _static_

  Card_StreamPrmAll* p_joinPermAll = new Card_StreamPrmAll;

  i8 nTables = p_tableList->nChildCards();
  PermGen* pPermGen = new PermGen(nTables);

  i8 i_Permutation = 0;
  while (i_Permutation < pPermGen->nPermutations(nTables)) {

    pPermGen->vCalcPermutation(i_Permutation);

    Card_StreamPrm* p_joinPermSeq = new Card_StreamPrm;
    p_joinPermSeq->pSymbol()->Y_vSet(P_Permutation, i_Permutation);

    i8 i_JoinInvalid = p_joinPermSeq->iOnePermutedJoin(p_tableList,
                                                       p_joinPredList,
                                                       pPermGen);

    p_joinPermAll->vAppend(p_joinPermSeq);

    if (C_UNKNOWN == i_JoinInvalid) {
      //vTrace("VALID PERMUTATION %lld\n"_Stz, i_Permutation);
      i_Permutation++;
      continue;
    }

    //vTrace("PERMUTATION INVALID %lld at %lld\n"_Stz, i_Permutation,i_JoinInvalid);

    i_Permutation = pPermGen->iNextValid(i_JoinInvalid,i_Permutation);
    //vTrace("RESTARTING FROM %lld\n"_Stz, i_Permutation);
    vDebugPoint();
  }

  delete pPermGen;

  return p_joinPermAll;
}
/*----------------------------------------------------------------------------*/
//Card_StreamPrmAll* Card_StreamPrmAll::pAllPermutedJoins(Card_TableList* p_tableList,
//                                                      Card_JoinPredList* p_joinPredList)
//{
//  _static_
//
//  Card_StreamPrmAll* p_joinPermAll = new Card_StreamPrmAll;
//
//  i8 nTables = p_tableList->nChildCards();
//  PermGen* pPermGen = new PermGen(nTables);
//
//  while (pPermGen->bNextPermutation()) {
//
//    Card_StreamPrm* p_joinPermSeq = new Card_StreamPrm;
//    p_joinPermSeq->Y_vSet(P_Permutation, pPermGen->nPermutations() - 1);
//    p_joinPermAll->vAppendMain(p_joinPermSeq);
//
//    p_joinPermSeq->iOnePermutedJoin(p_tableList,
//                                    p_joinPredList,
//                                    pPermGen);
//  }
//
//  delete pPermGen;
//
//  return p_joinPermAll;
//}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute the load of all permutations                                       */
/*----------------------------------------------------------------------------*/
void Card_StreamPrmAll::vEstCostsForAllJoinPerm(Card_TableList*    p_tableList,
                                                Card_FiltPredList* p_filtPredList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain())
    xCI.G.p_joinPermSeq->vEstQueryCost(p_tableList,p_filtPredList);
}

/*----------------------------------------------------------------------------*/
/* pick given order                                                           */
/*----------------------------------------------------------------------------*/
Card_StreamPrm* Card_StreamPrmAll::pJoinOrder(i8 iJoinOrder)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iPermutation;
  Card_StreamPrm* p_joinPermSeq = nullptr;

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_JOINPERMSEQ)) {
    p_joinPermSeq = xCI.G.p_joinPermSeq;
    iPermutation = p_joinPermSeq->pSymbol()->Y_iGet(P_Permutation);
    if (iJoinOrder == iPermutation)
      return (p_joinPermSeq);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* pick join permseq with lowest load                                         */
/*----------------------------------------------------------------------------*/
Card_StreamPrm* Card_StreamPrmAll::pLowestLoad(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_StreamPrm* pLowest = nullptr;

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_JOINPERMSEQ)) {
    if (! pLowest)
      pLowest = xCI.G.p_joinPermSeq;
    else
    if (xCI.G.p_joinPermSeq->bIsLessLoad(pLowest))
      pLowest = xCI.G.p_joinPermSeq;
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
    if (xCI.G.p_joinPermSeq->pSymbol()->Y_bHas(P_Invalid))
      xCI.G_vRemoveDeleteMain(CK_JOINPERMSEQ);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
