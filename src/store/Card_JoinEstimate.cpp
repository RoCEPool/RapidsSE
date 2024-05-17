/*============================================================================*/
/* Card_JoinEstimate.cpp -- Estimate cost for a JoinNext of a Query               */
/*============================================================================*/

#include "Card_JoinEstimate.hpp"

#include "Card_Predicate.hpp"
#include "Card_RlnConj.hpp"
#include "Card_CostNetwork.hpp"
#include "Card_CostUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"
#include "CardIter.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_JoinEstimate, CK_JOINESTIMATE)
__aNewFunction(Card_JoinEstimate, CN_CARD)
__vDeleteFunction(Card_JoinEstimate, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinEstimate::Card_JoinEstimate(void)
: Card_symbol(CK_JOINESTIMATE, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinEstimate::~Card_JoinEstimate(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinEstimate::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
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
