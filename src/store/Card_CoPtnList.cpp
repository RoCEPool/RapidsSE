/*============================================================================*/
/* Card_CoPtnList.cpp -- Co-Partition List                                    */
/*============================================================================*/

#include "Card_CoPtnList.hpp"

#include "Card_CoPtn.hpp"
#include "Card_PredCmp.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CoPtnList,CK_COPTNLIST)
__aNewFunction(Card_CoPtnList,CN_CARD)
__vDeleteFunction(Card_CoPtnList,CN_CARD)

__bIsCard(STORE,Card_CoPtnList,CK_COPTNLIST)
__pGetSubFunction_Card(STORE,CoPtnList,CK_COPTNLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CoPtnList::Card_CoPtnList(void)
: Card(CK_COPTNLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CoPtnList::~Card_CoPtnList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtn* Card_CoPtnList::pFind_coPtn_for(Card_CoPtnPred* p_CoPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card_CoPtn* p_coPtn = _pCard_CoPtn(pGetChild(i_Child));

    if (p_coPtn->bCoPtnWith(p_CoPtnPred))
      return p_coPtn;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CoPtnList::bHasEqPredCmp(Card_PredCmp* p_predCmp_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nChildren_coPtn = this->nChildCards();
  for (i8 i_Child_coPtn = 0; i_Child_coPtn < nChildren_coPtn; i_Child_coPtn++) {
    Card_CoPtn* p_coPtn = _pCard_CoPtn(this->pGetChild(i_Child_coPtn));

    i8 nChildren_predCmp = p_coPtn->nChildCards();
    for (i8 i_Child_predCmp = 0; i_Child_predCmp < nChildren_predCmp; i_Child_predCmp++) {
      Card_PredCmp *p_predCmp = _pCard_PredCmp(p_coPtn->pGetChild(i_Child_predCmp));


      if (p_predCmp->bIsEq(p_predCmp_arg))
        return true;
    }
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
