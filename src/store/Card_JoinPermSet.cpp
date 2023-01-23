/*============================================================================*/
/* Card_JoinPermSet.cpp -- Join Permutation Set (of lists)                    */
/*============================================================================*/

#include "Card_JoinPermSet.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_JoinPerm.hpp"
#include "Card_JoinPermList.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"
#include "PermGen.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPermSet,CK_JOINPERMSET)
__aNewFunction(Card_JoinPermSet,CN_CARD)
__vDeleteFunction(Card_JoinPermSet,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPermSet::Card_JoinPermSet(void)
: Card(CK_JOINPERMSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPermSet::~Card_JoinPermSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* compute the set of join permutations                                       */
/*----------------------------------------------------------------------------*/
Card_JoinPermSet* Card_JoinPermSet::pAllPermutedJoins(Card_TableList* p_tableList,
                                                      Card_JoinPredList* p_joinPredList)
{
  _static_

  Card_JoinPermSet* p_joinPermSet = new Card_JoinPermSet;

  i8 nTables = p_tableList->nChildCards();
  PermGen* pPermGen = new PermGen(nTables);

  while (pPermGen->bNextPermutation()) {

    Card_JoinPermList* p_joinPermList = new Card_JoinPermList;
    p_joinPermSet->vAppendMain(p_joinPermList);
    p_joinPermList->Y_vSet(P_Permutation, pPermGen->nPermutations() - 1);

    p_joinPermList->vOnePermutedJoin(p_tableList,
                                     p_joinPredList,
                                     p_joinPermSet,
                                     pPermGen);

//    vTrace("%lld :"_Stz,pPermGen->nPermutations());
//    for (i8 i_Item = 0; i_Item < nTables; i_Item++) {
//      i8 i_Table = pPermGen->iItem(i_Item);
//      Card_Table* p_table = _pCard_Table(p_tableList->pGetChild(i_Table));
//      if (i_Item)
//        vTrace(", "_Stz,i_Table);
//      vTrace("%c"_Stz,p_table->cValue());
//    }
//    vTrace("\n"_Stz);
  }

  delete pPermGen;

  return p_joinPermSet;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
