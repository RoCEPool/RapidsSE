/*============================================================================*/
/* SubSysFn.cpp -- description of a subsystem                                 */
/*============================================================================*/

#include "Resources.h"

extern SubSysCatalog gxSubSysCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubSysFn::SubSysFn(Stz* szSubSysNm, fpSUBSYSFN* fpSubSysFn, i8 iArg)
: SelfFn(szSubSysNm,(fpSELFFN*)fpSubSysFn,iArg)
{
  
  m_szSubSysNm = szSubSysNm;
  m_fpSubSysFn = fpSubSysFn;
  
  gxSubSysCatalog.vAddSubSysFn(this);
}

/*----------------------------------------------------------------------------*/
/* subsystem function                                                         */
/*----------------------------------------------------------------------------*/
void SubSysFn::vDoSubSysFn(void)
{
  _static_
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
