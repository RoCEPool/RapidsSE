/*============================================================================*/
/* Card_Property.cpp -- Property                                              */
/*============================================================================*/

#include "Card_Property.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Property,CK_PROPERTY)
__aNewFunction(Card_Property,CN_CARD)
__vDeleteFunction(Card_Property,CN_CARD)


__AssertFunction(STORE,Card_Property,CN_CARD)
__bIsCard(STORE,Card_Property,CK_PROPERTY)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Property::Card_Property(Stz* szAtom)
: Card(CK_PROPERTY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  vAtom(szAtom);
  
  m_bPrintNm = false;
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Property::~Card_Property(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
