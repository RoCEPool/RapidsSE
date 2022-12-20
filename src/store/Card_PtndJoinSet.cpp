/*============================================================================*/
/* Card_JoinPtnList.cpp -- Cross-Partition List                              */
/*============================================================================*/

#include "Card_PtndJoinSet.hpp"

#include "Card_CoPtnJoinList.hpp"
#include "Card_SubSet.hpp"

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


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
