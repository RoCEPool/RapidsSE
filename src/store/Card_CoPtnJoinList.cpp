/*============================================================================*/
/* Card_CoPtnJoinList.cpp -- Co-Partition List                                    */
/*============================================================================*/

#include "Card_CoPtnJoinList.hpp"

#include "Card_CoPtnJoin.hpp"
#include "Card_CmpPred.hpp"
#include "Card_SubSet.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CoPtnJoinList, CK_COPTNJOINLIST)
__aNewFunction(Card_CoPtnJoinList, CN_CARD)
__vDeleteFunction(Card_CoPtnJoinList, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoinList::Card_CoPtnJoinList(void)
: Card(CK_COPTNJOINLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoinList::~Card_CoPtnJoinList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoinList* Card_CoPtnJoinList::pExtract(Card_CoPtnPredList* p_coPtnPredList)
{
  _static_

  Card_CoPtnJoinList* p_coPtnJoinList = new Card_CoPtnJoinList;

  i8 nChildren = p_coPtnPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CoPtnPred* p_coPtnPred =
        _pCard_CoPtnPred(p_coPtnPredList->pGetChild(i_Child));

    Card_SubSet* p_subSet = p_coPtnPred->pDupSubCards();

//    if (! (G.p_coPtnJoin = T_pCoPtnJoinList_Find_coPtn_for(G.p_coPtnJoinList, G.p_coPtnPred)) ) {
      Card_CoPtnJoin* p_coPtnJoin = new Card_CoPtnJoin;
      p_coPtnJoinList->vAppend(p_coPtnJoin);
//    }

    p_coPtnJoin->vTransferChildren(p_subSet);
    DELETE_CARD(p_subSet);
  }


  return p_coPtnJoinList;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
