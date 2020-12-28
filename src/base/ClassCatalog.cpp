/*============================================================================*/
/* ClasstCatalog.cpp -- the catalog of class functions                        */
/*============================================================================*/

#include "BASE.h"

#include "eCF.h"

ClassCatalog gxClassCatalog;

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
ClassCatalog::ClassCatalog(void)
{
  m_aDefaults = new ClassFn(CN_NONE,
                            CF_INSPECT,
                            reinterpret_cast<fpCLASSFN*>(&ClassFn::bInspect),
                            C_VARGS_END);
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (! m_fpaClassFn[i_Class])
      vSetClassFn((eCN)i_Class,m_aDefaults);
  }
}


/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
bool ClassCatalog::bCallClassFn(eCF eFunction, Object* pObject)
{
  eCN eClass = pObject->eClass();
  
  bool b = m_fpaClassFn[eClass]->bCallClassFn(eFunction,pObject);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* find prompt function given command                                         */
/*----------------------------------------------------------------------------*/
ClassFn* ClassCatalog::pFindFunction(eCN eClass, i8 iKind)
{
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void ClassCatalog::vListFunctions(void)
{
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void ClassCatalog::vSetClassFn(eCN eClass, ClassFn* aClassFN)
{
  m_fpaClassFn[eClass] = aClassFN;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
