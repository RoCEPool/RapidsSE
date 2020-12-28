/*============================================================================*/
/* SubSysFn.cpp -- description of a subsystem                                 */
/*============================================================================*/

#include "BASE.h"

extern SubSysCatalog gxSubSysCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubSysFn::SubSysFn(Stz* szSubSysNm, fpSELFFN* fpSubSysFn, Stz* szDescription, ...)
: SelfFn(szSubSysNm,(fpSELFFN*)fpSubSysFn,szDescription)
{
  _nonLocalStatic_ // memory is automatically zero'd
  
  gxSubSysCatalog.vAddFn(this);
  
  va_list xVarArgs;
  va_start(xVarArgs,szDescription);
  vAddSeeAlso(xVarArgs);
  va_end(xVarArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
