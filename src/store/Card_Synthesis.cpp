/*============================================================================*/
/* Card_Synthesis.cpp -- Synthesis Info                                       */
/*============================================================================*/

#include "Card_Synthesis.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Synthesis, CK_SYNTHESIS)
__aNewFunction(Card_Synthesis, CN_CARD)
__vDeleteFunction(Card_Synthesis, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Synthesis::Card_Synthesis(void)
: Card_symbol(CK_SYNTHESIS, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Synthesis::~Card_Synthesis(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Synthesis::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
