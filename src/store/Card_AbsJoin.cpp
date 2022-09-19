/*============================================================================*/
/* Card_AbsJoin.cpp -- Avstract Join                                     */
/*============================================================================*/

#include "Card_AbsJoin.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_AbsJoin, CK_ABSJOIN)
__aNewFunction(Card_AbsJoin, CN_CARD)
__vDeleteFunction(Card_AbsJoin, CN_CARD)

__bIsCard(STORE, Card_AbsJoin, CK_ABSJOIN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_AbsJoin::Card_AbsJoin(void)
: Card_MAIN(CK_ABSJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  m_cMark2 = '0';

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_AbsJoin::~Card_AbsJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
