/*============================================================================*/
/* Card_StreamCol.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_StreamCol.hpp"
#include "Card_As.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_StreamCol,CK_STREAMCOL)
__aNewFunction(Card_StreamCol,CN_CARD)
__vDeleteFunction(Card_StreamCol,CN_CARD)

__bIsCard(STORE,Card_StreamCol,CK_STREAMCOL)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_StreamCol::Card_StreamCol(void)
: Card(CK_STREAMCOL)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_StreamCol::~Card_StreamCol(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get stream column name                                                     */
/*----------------------------------------------------------------------------*/
Stz* Card_StreamCol::szStreamColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_As* pCard_As = _pCard_As(pFindSubCard(CK_AS));

  Stz* szStreamColNm = pCard_As->szAs();

  return szStreamColNm;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
