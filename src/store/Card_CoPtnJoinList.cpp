/*============================================================================*/
/* Card_CoPtnJoinList.cpp -- Co-Partition List                                    */
/*============================================================================*/

#include "Card_CoPtnJoinList.hpp"

#include "Card_CoPtnJoin.hpp"
#include "Card_CmpPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CoPtnJoinList, CK_COPTNJOINLIST)
__aNewFunction(Card_CoPtnJoinList, CN_CARD)
__vDeleteFunction(Card_CoPtnJoinList, CN_CARD)

__bIsCard(STORE, Card_CoPtnJoinList, CK_COPTNJOINLIST)

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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoin* Card_CoPtnJoinList::pFind_coPtn_for(Card_CoPtnPred* p_CoPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(pGetChild(i_Child));

    if (p_coPtnJoin->bCoPtnWith(p_CoPtnPred))
      return p_coPtnJoin;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CoPtnJoinList::bHasEqPredCmp(Card_CmpPred* p_predCmp_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren_coPtn = this->nChildCards();
  for (i8 i_Child_coPtn = 0; i_Child_coPtn < nChildren_coPtn; i_Child_coPtn++) {
    Card_CoPtnJoin* p_coPtnJoin = _pCard_CoPtnJoin(this->pGetChild(i_Child_coPtn));

    i8 nChildren_predCmp = p_coPtnJoin->nChildCards();
    for (i8 i_Child_predCmp = 0; i_Child_predCmp < nChildren_predCmp; i_Child_predCmp++) {
      Card_CmpPred *p_predCmp = _pCard_CmpPred(p_coPtnJoin->pGetChild(i_Child_predCmp));


      if (p_predCmp->bIsEq(p_predCmp_arg))
        return true;
    }
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
