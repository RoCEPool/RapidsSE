/*============================================================================*/
/* LibInfo.hpp - Stateful information about compiling and using a dynamic lib */
/*============================================================================*/

#include "BASE.h"
#include "LibInfo.hpp"

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

  _checkInitialised_(CN_LIBINFO);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/* Destructor                                                                 */
/*----------------------------------------------------------------------------*/
LibInfo::~LibInfo()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIBINFO,_FOOTER_);

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/