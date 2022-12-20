/*============================================================================*/
/* Card_JoinPtnList.cpp -- Cross-Partition List                              */
/*============================================================================*/

#include "Card_PtndJoinSet.hpp"

#include "Card_CoPtnJoinList.hpp"
#include "Card_Deck.hpp"
#include "Card_Note.hpp"
#include "Card_Permutation.hpp"
#include "Card_PtndJoin.hpp"
#include "Card_SubSet.hpp"
#include "PermGen.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_PtndJoinSet,CK_PTNDJOINSET)
__aNewFunction(Card_PtndJoinSet,CN_CARD)
__vDeleteFunction(Card_PtndJoinSet,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_PtndJoinSet::Card_PtndJoinSet(void)
: Card(CK_PTNDJOINSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_PtndJoinSet::~Card_PtndJoinSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_PtndJoinSet* Card_PtndJoinSet::pExtract(Card_CoPtnJoinList* p_coPtnJoinList,
                                             Card_CrossPtnJoinList* p_crossPtnJoinList)
{
  _static_

  Card_PtndJoinSet* p_ptndJoinSet = new Card_PtndJoinSet;

  Card_SubSet* p_subSet = p_coPtnJoinList->pDupSubCards();
  p_ptndJoinSet->vTransferChildren(p_subSet);
  DELETE_CARD(p_subSet);

  p_subSet = p_crossPtnJoinList->pDupSubCards();
  p_ptndJoinSet->vTransferChildren(p_subSet);
  DELETE_CARD(p_subSet);

  i8 nChildren = p_ptndJoinSet->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pCard_child = p_ptndJoinSet->pGetChild(i_Child);
    pCard_child->vCard(CK_PTNDJOIN);
    pCard_child->Y_vSet(P_PtndJoin, i_Child);
  }

  return p_ptndJoinSet;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_PtndJoinSet::vPermute(Card_Deck* pDeck)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  PermGen* pPermGen = new PermGen(nChildCards());

  Card_SubSet* p_subSet = new Card_SubSet;

  while (pPermGen->bNextPermutation()) {
    Card_Permutation* p_permutation = new Card_Permutation;
    pDeck->vAppendMain(p_permutation);
    p_permutation->Y_vSet(P_Permutation, pPermGen->nPermutations() - 1);

    RASSERT(0 == p_subSet->nChildCards());
    for (i8 i_Item = 0; i_Item < pPermGen->nItems(); i_Item++) {
      i8 i_Child = pPermGen->iItem(i_Item);
      Card* pCard_child = pGetChild(i_Child);
      pDeck->vAppendMain(pCard_child->pDupCardAndSubCards());

      Card_PtndJoin* p_ptndJoin = _pCard_PtndJoin(pCard_child);

      // todo: ...
      if (! p_ptndJoin->bMayFollowAfter(p_subSet)) {
        pDeck->vAppendMain(new Card_Note("**DOESN'T FOLLOW**"_Stz));
        p_permutation->Y_vSet(P_Invalid);
        break;
      }
      p_subSet->vAppend(pCard_child);
    }
    p_subSet->vRemoveAll();
  }

  DELETE_CARD(p_subSet);

  delete pPermGen;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
