/*============================================================================*/
/* Card_CrossPtnJoinList.cpp -- Cross-Partition List                              */
/*============================================================================*/

#include "Card_CrossPtnJoinList.hpp"

#include "Card_CmpPred.hpp"
#include "Card_CoPtnJoinList.hpp"
#include "Card_CrossPtnJoin.hpp"
#include "Card_JoinPredList.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CrossPtnJoinList, CK_CROSSPTNJOINLIST)
__aNewFunction(Card_CrossPtnJoinList, CN_CARD)
__vDeleteFunction(Card_CrossPtnJoinList, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CrossPtnJoinList::Card_CrossPtnJoinList(void)
: Card(CK_CROSSPTNJOINLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CrossPtnJoinList::~Card_CrossPtnJoinList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CrossPtnJoinList* Card_CrossPtnJoinList::pExtract(Card_JoinPredList* p_joinPredList,
                                                       Card_CoPtnJoinList* p_coPtnJoinList)
{
  _static_

  Card_CrossPtnJoinList* p_crossPtnJoinList = new Card_CrossPtnJoinList;

  i8 nChildren = p_joinPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CmpPred* p_cmpPred =
        _pCard_CmpPred(p_joinPredList->pGetChild(i_Child));

    // todo: ...
    if (p_coPtnJoinList->bHasEqPredCmp(p_cmpPred))
      continue;

    Card_CrossPtnJoin* p_crossPtnJoin = new Card_CrossPtnJoin;
    p_crossPtnJoin->vAppend(p_cmpPred->pDupCardAndSubCards());
    p_crossPtnJoinList->vAppend(p_crossPtnJoin);
  }

  return p_crossPtnJoinList;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
