/*============================================================================*/
/* GizmoFn.cpp -- Gizmo Command Function                                      */
/*============================================================================*/

#include "Base.h"

extern GizmoCatalog gxGizmoCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GizmoFn::GizmoFn(Stz* szGizmoNm, fpSELFFN* fpGizmoFn, Stz* szDescription, ...)
: SelfFn(szGizmoNm,(fpSELFFN*)fpGizmoFn,szDescription)
{
  gxGizmoCatalog.vAddFn(this);
  
  va_list xVarArgs;
  va_start(xVarArgs,szDescription);
  vAddSeeAlso(xVarArgs);
  va_end(xVarArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
