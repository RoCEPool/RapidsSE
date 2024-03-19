/*============================================================================*/
/* Card_Bounds.cpp -- Join Predicate                                        */
/*============================================================================*/

#include "Card_Bound.hpp"
#include "Card_Bounds.hpp"
#include "CardIter.hpp"
#include "Card_RlnConj.hpp"

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

  vDumpShort(__func__,"BOUNDS"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert tblCol to stmCol                                                   */
/*----------------------------------------------------------------------------*/
void Card_Bounds::vX_tblCol_To_expCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  Kid xBounds(this);
  while (xBounds.bEach(CK_BOUND)) {
    pCI->G.p_bound = xBounds.p_bound();
    pCI->G.p_bound->vX_tblCol_To_expCol(pCI);
  }

  pCI->G.p_bound = nullptr;
}

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
    xCI_bounds.vBackstep();
  }

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Bound* Card_Bounds::pFindChosenBound(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xBounds(this);
  while (xBounds.bEach(CK_BOUND)) {
    if (xBounds.pChild()->pSymbol()->Y_bHas(P_ChosenBound))
      return _pCard_Bound(xBounds.pChild());
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Bounds::vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xBounds(this); xBounds.bEach(CK_BOUND); /**/)
    xBounds.p_bound()->vApplyCast(pCI, /*??*/p_Dic_expr);
}

/*----------------------------------------------------------------------------*/
/* does this bounds have a bound logically equivalent to given compare?       */
/*----------------------------------------------------------------------------*/
bool Card_Bounds::bHasEquivalentBound(Card_RlnConj* p_rlnConj)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xBounds(this);
  while (xBounds.bEach(CK_BOUND)) {
    if (xBounds.p_bound()->bEquivalent(p_rlnConj))
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

  for (Kid xBounds(this); xBounds.bEach(CK_BOUND); /**/) {
    if (xBounds.p_bound()->bUsesSqyRsl())
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

  vValidate_mayHave(CK_BOUND,CK_NONE);
  //vValidate_mustHave(CK_BOUND,CK_NONE);

  (void)Card::eValidate_parentEither(pCI, CK_INDEX, CK_NONE);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/