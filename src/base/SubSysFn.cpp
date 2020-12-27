/*============================================================================*/
/* SubSysFn.cpp -- description of a subsystem                                 */
/*============================================================================*/

#include "Base.h"

extern SubSysCatalog gxSubSysCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubSysFn::SubSysFn(Stz* szSubSysNm, fpSELFFN* fpSubSysFn, Stz* szDescription, ...)
: SelfFn(szSubSysNm,(fpSELFFN*)fpSubSysFn,szDescription)
{
  gxSubSysCatalog.vAddFn(this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
