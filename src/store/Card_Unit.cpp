/*============================================================================*/
/* Card_Unit.cpp -- Abstract Unit                                             */
/*============================================================================*/

#include "Card_Unit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Card_Unit)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Unit::Card_Unit(eCK eKind, eCN eClass, Stz* szCardNm)
: Card(eKind,eClass, szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_fpDeleteThis = &Card_Unit::vDeleteThis_;

  _checkInitialised_(CN_CARD_PROJECTPRED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Unit::Card_Unit(eCK eKind, eCN eClass, Stz* szCardNm,
                     Stz* szProperty, i8 iValue)
: Card(eKind,eClass, szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_fpDeleteThis = &Card_Unit::vDeleteThis_;

  vAddProperty(szProperty,iValue);
  
  _checkInitialised_(CN_CARD_PROJECTPRED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Unit::Card_Unit(eCK eKind, eCN eClass, Stz* szCardNm,
                     Stz* szProperty, Object* pValue)
: Card(eKind,eClass, szCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_fpDeleteThis = &Card_Unit::vDeleteThis_;

  vAddProperty(szProperty,pValue);

  _checkInitialised_(CN_CARD_PROJECTPRED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Unit::Card_Unit(eCK eKind, eCN eClass, Stz* szCardNm,
                     Stz* szProperty, Stz* szValue)
: Card(eKind,eClass, szCardNm, szProperty, szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_fpDeleteThis = &Card_Unit::vDeleteThis_;

  _checkInitialised_(CN_CARD_PROJECTPRED);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Unit::~Card_Unit(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Card_Unit::vDeleteThis_(Card* pCard)
{
  _static_

  delete (Card_Unit*)pCard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
