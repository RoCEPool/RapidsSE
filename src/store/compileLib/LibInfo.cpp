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
LibInfo::LibInfo(bool bCollectStats)
: Object(CN_LIBINFO)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);

  m_eCompilerType = CB_NONE;
  m_pvDynLib = nullptr;
  m_eErrCode = EC_NONE;

  m_bCollectStats   = bCollectStats;
  m_uCompileNanos    = 0;
  m_uDynLinkNanos    = 0;
  m_uSymAddrNanos    = 0;
  m_uDynUnlinkNanos  = 0;
  m_uDeleteNanos     = 0;

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
  return m_sErrDetails.szStz();
}

/*----------------------------------------------------------------------------*/
/* Return the time taken to compile the shared object, in nanoseconds         */
/*----------------------------------------------------------------------------*/
u8 LibInfo::uCompileNanos()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  return m_uCompileNanos;
}

/*----------------------------------------------------------------------------*/
/* Return the time taken to dynamically load the shared object, in nanoseconds*/
/*----------------------------------------------------------------------------*/
u8 LibInfo::uDynLinkNanos()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  return m_uDynLinkNanos;
}

/*----------------------------------------------------------------------------*/
/* Return the time taken to resolve a symbol to an address, in nanoseconds    */
/*----------------------------------------------------------------------------*/
u8 LibInfo::uSymAddrNanos()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  return m_uSymAddrNanos;
}

/*----------------------------------------------------------------------------*/
/* Return the time taken to dynamically unload the shared object, in nanos    */
/*----------------------------------------------------------------------------*/
u8 LibInfo::uDynUnlinkNanos()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  return m_uDynUnlinkNanos;
}

/*----------------------------------------------------------------------------*/
/* Return the time taken to delete all library src/log/binary files, in nanos */
/*----------------------------------------------------------------------------*/
u8 LibInfo::uDeleteNanos()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
  return m_uDeleteNanos;
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
  m_sErrDetails.vReset();
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