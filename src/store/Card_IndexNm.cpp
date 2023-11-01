/*============================================================================*/
/* Card_IndexNm.cpp -- Unique                                                  */
/*============================================================================*/

#include "Card_IndexNm.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_IndexNm,CK_INDEXNM)
__aNewFunction(Card_IndexNm,CN_CARD)
__vDeleteFunction(Card_IndexNm,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_IndexNm::Card_IndexNm(void)
: Card_symbol(CK_INDEXNM, 5)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_IndexNm::~Card_IndexNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_IndexNm::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card::vValidate(pCI,iDepth,pParent);
  vValidate_noChildren();
  vValidationPassed();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
