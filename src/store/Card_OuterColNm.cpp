/*============================================================================*/
/* Card_OuterColNm.cpp -- Outer Column Name                                   */
/*============================================================================*/

#include "Card_OuterColNm.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_OuterColNm, CK_OUTERCOLNM)
__aNewFunction(Card_OuterColNm, CN_CARD)
__vDeleteFunction(Card_OuterColNm, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OuterColNm::Card_OuterColNm(void)
: Card_symbol(CK_OUTERCOLNM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OuterColNm::~Card_OuterColNm(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
