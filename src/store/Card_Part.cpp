/*============================================================================*/
/* Card_Part.cpp -- Abstrc Particate                                          */
/*============================================================================*/

#include "Card_Part.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Card_Part)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Part::Card_Part(eCK eKind, eCN eClass, Stz* m_sCardNm)
: Card(eKind,eClass,m_sCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_fpDeleteThis = &Card_Part::vDeleteThis_;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Part::Card_Part(eCK eKind, eCN eClass, Stz* m_sCardNm,
                     Stz* szProperty, i8 iValue)
: Card(eKind,eClass,m_sCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_fpDeleteThis = &Card_Part::vDeleteThis_;
  
  vAddProperty(szProperty,iValue);

  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Part::Card_Part(eCK eKind, eCN eClass, Stz* m_sCardNm,
                     Stz* szProperty, Object* pValue)
: Card(eKind,eClass,m_sCardNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_fpDeleteThis = &Card_Part::vDeleteThis_;
  
  vAddProperty(szProperty,pValue);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Part::Card_Part(eCK eKind, eCN eClass, Stz* m_sCardNm,
                     Stz* szProperty, Stz* szValue)
: Card(eKind,eClass,m_sCardNm,szProperty,szValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
      
  m_fpDeleteThis = &Card_Part::vDeleteThis_;
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Part::~Card_Part(void) noexcept(false)
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
void Card_Part::vDeleteThis_(Card* pCard)
{
  _static_

  delete (Card_Part*)pCard;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
