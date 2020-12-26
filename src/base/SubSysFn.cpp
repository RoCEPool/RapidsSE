/*============================================================================*/
/* SubSysFn.cpp -- description of a subsystem                                 */
/*============================================================================*/

#include "Base.h"

extern SubSysCatalog gxSubSysCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubSysFn::SubSysFn(Stz* szSubSysNm, fpSELFFN* fpSubSysFn, Stz* szDescription, i8 iArg)
: SelfFn(szSubSysNm,(fpSELFFN*)fpSubSysFn,szDescription,iArg)
{
  gxSubSysCatalog.vAddFn(this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
