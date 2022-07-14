/*============================================================================*/
/* LocalStr.cpp -- Local String Medium                                        */
/*============================================================================*/

#include "BASE.h"

#include "LocalStrMedium.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalStrMedium::LocalStrMedium(void)
: Object(CN_LOCALSTRMEDIUM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRMEDIUM,_FOOTER_);
  
  m_aLocal  = nullptr;
  m__dummy0 = C_UNKNOWN;

  MemLib::vZero(m_caArea,sizeof(m_caArea),CN_LOCALSTRMEDIUM);
  
  _checkInitialised_(CN_LOCALSTRMEDIUM);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRMEDIUM,_FOOTER_);}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalStrMedium::~LocalStrMedium(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRMEDIUM,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Operators ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* auto conversion operator : as LocalStrBig*                                 */
/*----------------------------------------------------------------------------*/
LocalStrMedium::operator LocalStrMedium*(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRMEDIUM,_FOOTER_);
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* get pointer to string area                                                 */
/*----------------------------------------------------------------------------*/
char* LocalStrMedium::azArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRMEDIUM,_FOOTER_);
  
  return m_caArea;
}

/*----------------------------------------------------------------------------*/
/* return a value                                                             */
/*----------------------------------------------------------------------------*/
TypeValueDscr* LocalStrMedium::aLocal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRMEDIUM,_FOOTER_);

  return m_aLocal;
}

/*----------------------------------------------------------------------------*/
/* assign a value                                                             */
/*----------------------------------------------------------------------------*/
void LocalStrMedium::vLocal(TypeValueDscr* aLocal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRMEDIUM,_FOOTER_);
  
  m_aLocal = aLocal;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
