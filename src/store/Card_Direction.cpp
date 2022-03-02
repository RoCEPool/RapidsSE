/*============================================================================*/
/* Card_SortDirn.cpp -- Sort Direction                                        */
/*============================================================================*/

#include "Card_Direction.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Direction,CK_DIRECTION)
__aNewFunction(Card_Direction,CN_CARD)
__vDeleteFunction(Card_Direction,CN_CARD)

__bIsCard(STORE,Card_Direction,CK_DIRECTION)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Direction::Card_Direction(Stz* szDirn)
: Card_SUB(CK_DIRECTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vDirn(szDirn);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Direction::Card_Direction(Column* pColumn)
: Card_SUB(CK_DIRECTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vDirn(aSD_short[SD_ASC]);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Direction::~Card_Direction(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
