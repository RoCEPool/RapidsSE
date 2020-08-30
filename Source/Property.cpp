/*============================================================================*/
/* Property.cpp - Property                                                    */
/*============================================================================*/

#include "Facts.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Facts,Property,CN_PROPERTY)
__aNewFunction(Property,CN_PROPERTY)
__vDeleteFunction(Property,CN_PROPERTY)
__aFooterFunction(Property)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Property::Property(void)
: Object(CN_PROPERTY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROPERTY,_FOOTER_);
  
  m_eEg = EG_NONE;
  m_iEn = 0;
  
  m_aVoid = nullptr;
  
  _checkInitialised_(CN_PROPERTY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PROPERTY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Property::~Property(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PROPERTY,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
