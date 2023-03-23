/*============================================================================*/
/* LibInfo.hpp - Stateful information about compiling and using a dynamic lib */
/*============================================================================*/

#include "BASE.h"
#include "LibInfo.hpp"
#include "Text.hpp"

using namespace STORE;


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(STORE,LibInfo,CN_LIBINFO)
__aNewFunction(LibInfo,CN_LIBINFO)
__vDeleteFunction(LibInfo,CN_LIBINFO)
__aFooterFunction(LibInfo)

/*----------------------------------------------------------------------------*/
/* Constructor                                                                */
/*----------------------------------------------------------------------------*/
LibInfo::LibInfo()
: Object(CN_LIBINFO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);

  m_pvDynLib = nullptr;
  m_eErrCode = EC_NONE;
  m_pErrDetails = nullptr;

  m_iCompileTime    = 0;
  m_iDynLinkTime    = 0;
  m_iDynUnlinkTime  = 0;
  m_iDeleteTime     = 0;

  _checkInitialised_(CN_LIBINFO);
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* Destructor                                                                 */
/*----------------------------------------------------------------------------*/
LibInfo::~LibInfo()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  ASSERT(m_pvDynLib == nullptr);

  delete m_pErrDetails;
}

/*----------------------------------------------------------------------------*/
/*-- Public Functions---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Return the error code, or EC_NONE if there wasn't an error                 */
/*----------------------------------------------------------------------------*/
eEC LibInfo::eErrCode()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  return m_eErrCode;
}

/*----------------------------------------------------------------------------*/
/* Return any additional error details, or nullptr if there weren't any       */
/*----------------------------------------------------------------------------*/
Stz* LibInfo::szErrDetails()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  if (!m_pErrDetails) {
    return nullptr;
  }

  return m_pErrDetails->szGet();
}


/*----------------------------------------------------------------------------*/
/*-- Private Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Return any additional error details, or nullptr if there weren't any       */
/*----------------------------------------------------------------------------*/
void LibInfo::vClearErrors()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  m_eErrCode = EC_NONE;

  if (m_pErrDetails) {
    delete m_pErrDetails;
    m_pErrDetails = nullptr;
  }
}

/*----------------------------------------------------------------------------*/
/* Returns the name of the dynamic library being created (without extension)  */
/*----------------------------------------------------------------------------*/
Stz* LibInfo::szBaseName()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  return m_sBaseName.szStz();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/