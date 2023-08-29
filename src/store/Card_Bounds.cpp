/*============================================================================*/
/* Card_Bounds.cpp -- Join Predicate                                        */
/*============================================================================*/

#include "Card_Bound.hpp"
#include "Card_Bounds.hpp"
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
: Card_symbol(CK_BOUNDS, C_UNKNOWN)
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
i8 Card_Bounds::nCullBound(Stz* szWithMark)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nCulled = 0;

  Kid xCI_bounds(this);
  while (xCI_bounds.bEach()) {
    if (! xCI_bounds.pChild()->bMark1(szWithMark))
      continue;

    vDeleteChild(xCI_bounds.pChild());
    nCulled++;
    xCI_bounds.vRewind();
  }

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Bound* Card_Bounds::pFindChosenBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xCI(this);
  while (xCI.bEach(CK_BOUND)) {
    if (xCI.pChild()->pSymbol()->Y_bHas(P_ChosenBound))
      return _pCard_Bound(xCI.pChild());
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bounds::vEnsureTypes_For_stmOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_BOUND); /**/)
    xKid.p_bound()->vEnsureTypes_For_stmOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* does this bounds have a bound logically equivalent to given compare?       */
/*----------------------------------------------------------------------------*/
bool Card_Bounds::bHasEquivalentBound(Card_CmpPred* p_cmpPred)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xBounds(this);
  while (xBounds.bEach(CK_BOUND)) {
    if (xBounds.p_bound()->bEquivalent(p_cmpPred))
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Bounds::bUsesSqyRsl(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_BOUND); /**/) {
    if (xKid.p_bound()->bUsesSqyRsl())
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bounds::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/