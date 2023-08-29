/*============================================================================*/
/* Card_StreamSet.cpp -- Join Permutation Set (of lists)                    */
/*============================================================================*/

#include "Card_StreamSet.hpp"

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

__pCastFunction_Card(STORE, Card_StreamSet, CK_STREAMSET)
__aNewFunction(Card_StreamSet, CN_CARD)
__vDeleteFunction(Card_StreamSet, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamSet::Card_StreamSet(void)
: Card_symbol(CK_STREAMSET, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamSet::~Card_StreamSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute the set of stream join permutations                                */
/*----------------------------------------------------------------------------*/
Card_StreamSet* Card_StreamSet::pComputeAllPermutations(Card_TableList* p_tableList,
                                                        Card_JoinPredList* p_joinPredList)
{
  _static_

  Card_StreamSet* p_streamPrmAll = new Card_StreamSet;

  i8 nTables = p_tableList->nChildCards();
  PermGen* pPermGen = new PermGen(nTables);

  i8 i_Permutation = 0;
  while (i_Permutation < pPermGen->nPermutations(nTables)) {

    // calc the join sequence for this stream permutation
    pPermGen->vCalcPermutation(i_Permutation);

    Card_StreamPer* p_streamPer = new Card_StreamPer;
    p_streamPer->pSymbol()->Y_vSet(P_Permutation, i_Permutation);

    // create the permuted join sequence for the stream, testing if sequence is valid
    i8 i_InvalidAtJoin = p_streamPer->iCreatePerSeq(p_tableList,
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
void Card_StreamSet::vEstCostsForAllPermutations(Card_TableList*    p_tableList,
                                                 Card_FiltPredList* p_filtPredList)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xCI(this);
  while (xCI.bEach())
    xCI.p_streamPer()->vEstStreamCost(p_tableList, p_filtPredList);
}

/*----------------------------------------------------------------------------*/
/* pick permutation with highest cost                                         */
/*----------------------------------------------------------------------------*/
Card_StreamPer* Card_StreamSet::pHighestCost(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_StreamPer* pHighest = nullptr;

  Kid xCI(this);
  while (xCI.bEach(CK_STREAMPER)) {
    if (! pHighest)
      pHighest = xCI.p_streamPer();
    else
    if (xCI.p_streamPer()->bIsHigherCost(pHighest))
      pHighest = xCI.p_streamPer();
  }

  return pHighest;
}

/*----------------------------------------------------------------------------*/
/* pick given order                                                           */
/*----------------------------------------------------------------------------*/
Card_StreamPer* Card_StreamSet::pJoinOrder(i8 iJoinOrder)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 iPermutation;
  Card_StreamPer* p_joinPermSeq = nullptr;

  Kid xCI(this);
  while (xCI.bEach(CK_STREAMPER)) {
    p_joinPermSeq = xCI.p_streamPer();
    iPermutation = p_joinPermSeq->pSymbol()->Y_iGet(P_Permutation);
    if (iJoinOrder == iPermutation)
      return (p_joinPermSeq);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* pick permutation with lowest cost                                          */
/*----------------------------------------------------------------------------*/
Card_StreamPer* Card_StreamSet::pLowestCost(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_StreamPer* pLowest = nullptr;

  Kid xCI(this);
  while (xCI.bEach(CK_STREAMPER)) {
    if (! pLowest)
      pLowest = xCI.p_streamPer();
    else
    if (xCI.p_streamPer()->bIsLowerCost(pLowest))
      pLowest = xCI.p_streamPer();
  }

  return pLowest;
}

/*----------------------------------------------------------------------------*/
/* compute the set of join permutations                                       */
/*----------------------------------------------------------------------------*/
void Card_StreamSet::vRemoveInvalid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Kid xCI(this);
  while (xCI.bEach()) {
    if (xCI.p_streamPer()->pSymbol()->Y_bHas(P_Invalid)) {
      Card_StreamPer* p_streamPer = xCI.p_streamPer();
      xCI.vRemove();
      DELETE_CARD(p_streamPer);
    }
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_StreamSet::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
