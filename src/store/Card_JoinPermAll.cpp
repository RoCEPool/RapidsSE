/*============================================================================*/
/* Card_JoinPermAll.cpp -- Join Permutation Set (of lists)                    */
/*============================================================================*/

#include "Card_JoinPermAll.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_JoinPerm.hpp"
#include "Card_JoinPermOne.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"
#include "PermGen.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_JoinPermAll, CK_JOINPERMALL)
__aNewFunction(Card_JoinPermAll, CN_CARD)
__vDeleteFunction(Card_JoinPermAll, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPermAll::Card_JoinPermAll(void)
: Card(CK_JOINPERMALL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPermAll::~Card_JoinPermAll(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute the set of join permutations                                       */
/*----------------------------------------------------------------------------*/
Card_JoinPermAll* Card_JoinPermAll::pAllPermutedJoins(Card_TableList* p_tableList,
                                                      Card_JoinPredList* p_joinPredList)
{
  _static_

  Card_JoinPermAll* p_joinPermAll = new Card_JoinPermAll;

  i8 nTables = p_tableList->nChildCards();
  PermGen* pPermGen = new PermGen(nTables);

  while (pPermGen->bNextPermutation()) {

    Card_JoinPermOne* p_joinPermOne = new Card_JoinPermOne;
    p_joinPermOne->Y_vSet(P_Permutation, pPermGen->nPermutations() - 1);
    p_joinPermAll->vAppendMain(p_joinPermOne);

    p_joinPermOne->vOnePermutedJoin(p_tableList,
                                    p_joinPredList,
                                    pPermGen);
  }

  delete pPermGen;

  return p_joinPermAll;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute the set of join permutations                                       */
/*----------------------------------------------------------------------------*/
void Card_JoinPermAll::vRemoveInvalid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain()) {
    if (xCI.G.p_joinPermOne->Y_bHas(P_Invalid))
      xCI.G_vRemoveDeleteMain(CK_JOINPERMONE);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
