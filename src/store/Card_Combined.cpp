/*============================================================================*/
/* Card_Combined.cpp -- Combine                                                */
/*============================================================================*/

#include "Card_Combined.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Combined, CK_COMBINED)
__aNewFunction(Card_Combined, CN_CARD)
__vDeleteFunction(Card_Combined, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Combined::Card_Combined(void)
: Card_OP(CK_COMBINED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  SYM(SF_NUMBER,(i8) 0);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Combined::~Card_Combined(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete the <absJoin>                                                       */
/*----------------------------------------------------------------------------*/
void Card_Combined::vDeleteAllCombined(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMain(CK_COMBINED)) {
    pCI->G_vRemoveDeleteMain(CK_COMBINED);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Combined::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
