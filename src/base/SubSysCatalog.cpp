/*============================================================================*/
/* SubSysCatalog.cpp -- the subsystem catalog                                 */
/*============================================================================*/

#include "BASE.h"

SubSysCatalog gxSubSysCatalog;

SubSysFn __SubSysCatalog_Descrive
   ("Describe"_Stz,(fpSELFFN*)SubSysCatalog::vDescribe,
                    ""_Stz,
                    nullptr);

SubSysFn __SubSysCatalog_SubSys
   ("SubSys"_Stz,(fpSELFFN*)SubSysCatalog::vSubSystems,
                  ""_Stz,
                  nullptr);

SubSysFn __SubSysCatalog_SubSystems
   ("SubSystems"_Stz,(fpSELFFN*)SubSysCatalog::vSubSystems,
                      ""_Stz,
                      nullptr);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SubSysCatalog::SubSysCatalog(void)
{
  _nonLocalStatic_ // memory is automatically zero'd
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* list subsystems                                                            */
/*----------------------------------------------------------------------------*/
void SubSysCatalog::vSubSystems(SelfFn* aSelfFn, TokenVec* pTokenVec)
{
  _static_
  UNUSED(aSelfFn);
  UNUSED(pTokenVec);

  gxSubSysCatalog.vList();
}

/*----------------------------------------------------------------------------*/
/* describe susbsystems recursively                                           */
/*----------------------------------------------------------------------------*/
void SubSysCatalog::vDescribe(SelfFn* aSelfFn, TokenVec* pTokenVec)
{
  _static_
  UNUSED(aSelfFn);
  UNUSED(pTokenVec);

  gxSubSysCatalog.vDescribeRecursively("AttachMap"_Stz,0);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
