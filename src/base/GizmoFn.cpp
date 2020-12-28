/*============================================================================*/
/* GizmoFn.cpp -- Gizmo Command Function                                      */
/*============================================================================*/

#include "BASE.h"

extern GizmoCatalog gxGizmoCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GizmoFn::GizmoFn(Stz* szGizmoNm, fpSELFFN* fpGizmoFn, Stz* szDescription, ...)
: SelfFn(szGizmoNm,(fpSELFFN*)fpGizmoFn,szDescription)
{
  _nonLocalStatic_ // memory is automatically zero'd
  
  gxGizmoCatalog.vAddFn(this);
  
  va_list xVarArgs;
  va_start(xVarArgs,szDescription);
  vAddSeeAlso(xVarArgs);
  va_end(xVarArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
