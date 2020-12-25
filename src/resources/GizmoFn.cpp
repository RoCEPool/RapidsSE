/*============================================================================*/
/* GizmoFn.cpp -- Gizmo Command Function                                      */
/*============================================================================*/

#include "Resources.h"

extern GizmoCatalog gxGizmoCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GizmoFn::GizmoFn(Stz* szGizmoNm, fpSELFFN* fpGizmoFn, Stz* szDescription, i8 iArg)
: SelfFn(szGizmoNm,(fpSELFFN*)fpGizmoFn,szDescription,iArg)
{
  gxGizmoCatalog.vAddFn(this);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
