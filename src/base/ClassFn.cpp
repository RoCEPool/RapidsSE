/*============================================================================*/
/* ClassFn.cpp -- description of a class support function                     */
/*============================================================================*/

#include "Resources.h"

#include "eCF.h"

extern ClassCatalog gxClassCatalog;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ClassFn::ClassFn(eCN eClass, eCF eFunction, fpCLASSFN* fpClassFn, i8)
{
  m_fpInspectFn = fpClassFn;
  
  gxClassCatalog.vSetClassFn(eClass,this);
}

/*----------------------------------------------------------------------------*/
/* default object inspector                                                   */
/*----------------------------------------------------------------------------*/
bool ClassFn::bCallClassFn(eCF eFunction, Object* pObject)
{
  _static_
  
  bool b = m_fpInspectFn(pObject);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* default object inspector                                                   */
/*----------------------------------------------------------------------------*/
bool ClassFn::bInspect(Object* pObject)
{
  _static_
  
  pObject->bInspectObject();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
