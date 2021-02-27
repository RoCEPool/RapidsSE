/*============================================================================*/
/* Licence.cpp - software licence                                             */
/*============================================================================*/

#include "BASE.h"

#include "Licence.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Licence,CN_LICENCE)
__aNewFunction(Licence,CN_LICENCE)
__vDeleteFunction(Licence,CN_LICENCE)
__aFooterFunction(Licence)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Licence::Licence(void)
: Object(CN_CMDLINEARGS)
{
  _precondition_(_MAGIC_,CN_LICENCE,_FOOTER_);
  
  m_sDescription = C_NO_NAME;

  m_wCreationDate.vSetNull();
  m_wExpiryDate.vSetNull();

  m_nMaxMemory      = C_UNKNOWN;
  m_nMaxNodes       = C_UNKNOWN;
  m_nMaxPartitions  = C_UNKNOWN;
  m_nMaxDataBuffers = C_UNKNOWN;
  m_nMaxThreads     = C_UNKNOWN;

  _checkInitialised_(CN_LICENCE);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Licence::~Licence(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LICENCE,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
