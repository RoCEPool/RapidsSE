/*============================================================================*/
/* ToolFn.cpp -- Tool Command Function                                        */
/*============================================================================*/

#include "Base.h"

extern ToolCatalog gxToolCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToolFn::ToolFn(Stz* szToolNm, fpSELFFN* fpToolFn, Stz* szDescription, ...)
: SelfFn(szToolNm,(fpSELFFN*)fpToolFn,szDescription)
{
  gxToolCatalog.vAddFn(this);
  
  va_list xVarArgs;
  va_start(xVarArgs,szDescription);
  vAddSeeAlso(xVarArgs);
  va_end(xVarArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
