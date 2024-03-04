/*============================================================================*/
/* Card_From.cpp -- From                                                      */
/*============================================================================*/

#include "Card_From.hpp"
#include "Card_Table.hpp"

#include "Table.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_From, CK_o_FROM)
__aNewFunction(Card_From,CN_CARD)
__vDeleteFunction(Card_From,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_From::Card_From(void)
: Card_OP(CK_o_FROM, 1)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_From::~Card_From(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_From::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_s_TABLE, CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);

  Card_OP::vValidate(pCI,iDepth,pParent);

  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
