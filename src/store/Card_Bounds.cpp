/*============================================================================*/
/* Card_Bounds.cpp -- Join Predicate                                        */
/*============================================================================*/

#include "Card_Bound.hpp"
#include "Card_Bounds.hpp"
#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_Index.hpp"
#include "Card_Table.hpp"
#include "Card_TablePtn.hpp"
#include "Card_Term.hpp"
#include "Card_PRED.hpp"
#include "CardIter.hpp"
#include "Card_CmpPred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Bounds, CK_BOUNDS)
__aNewFunction(Card_Bounds, CN_CARD)
__vDeleteFunction(Card_Bounds, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Bounds::Card_Bounds(void)
: Card(CK_BOUNDS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Bounds::~Card_Bounds(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bounds::vCullBound(char cWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI_bounds(this);
  while (xCI_bounds.G_bEachMain()) {
    if (xCI_bounds.G.p_MainCard->cMark1() != cWithMark)
      continue;

    vRemoveChild(xCI_bounds.G.p_MainCard);
    DELETE_CARD(xCI_bounds.G.p_MainCard);
    xCI_bounds.vRewind();
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Bound* Card_Bounds::pFindChosenBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_BOUND)) {
    if (xCI.G.p_bound->pSymbol()->Y_bHas(P_ChosenBound))
      return xCI.G.p_bound;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this bounds have a bound logically equivalent to given compare?       */
/*----------------------------------------------------------------------------*/
bool Card_Bounds::bHasEquivalentBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_BOUND)) {
    if (xCI.G.p_bound->bEquivalent(p_cmpPred))
      return true;
  }

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
