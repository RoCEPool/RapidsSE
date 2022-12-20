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

    Card_CoPtnJoin* p_coPtnJoin = nullptr;
    if (! (p_coPtnJoin = p_coPtnJoinList->pFind(p_coPtnPred)) ) {
      p_coPtnJoin = new Card_CoPtnJoin;
      p_coPtnJoinList->vAppend(p_coPtnJoin);
    }

    p_coPtnJoin->vTransferChildren(p_subSet);
    DELETE_CARD(p_subSet);
  }


  return p_coPtnJoinList;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoin* Card_CoPtnJoinList::pFind(Card_CoPtnPred* p_coPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren =nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(pGetChild(i_Child));

    if (p_coPtnJoin->bIsCoPtnWith(p_coPtnPred))
      return p_coPtnJoin;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CoPtnJoinList::bHasEqPredCmp(Card_CmpPred* p_cmpPred_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren_coPtn = nChildCards();
  for (i8 i_Child_coPtn = 0; i_Child_coPtn < nChildren_coPtn; i_Child_coPtn++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(pGetChild(i_Child_coPtn));

    i8 nChildren_cmpPred = p_coPtnJoin->nChildCards();
    for (i8 i_Child_cmpPred = 0; i_Child_cmpPred < nChildren_cmpPred; i_Child_cmpPred++) {
      Card_CmpPred *p_cmpPred = _pCard_CmpPred(p_coPtnJoin->pGetChild(i_Child_cmpPred));

      if (p_cmpPred->bIsEq(p_cmpPred_arg))
        return true;
    }
  }

  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
