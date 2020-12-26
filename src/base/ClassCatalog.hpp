/*============================================================================*/
/* ClassCatalog.hpp -- the catalog of class functions                         */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ClassCatalog
  {
  public:

    ClassFn* m_aDefaults;  // default set of functions

    ClassFn* m_fpaClassFn[CN_ENUMS];  // array of inspect function

  public:
    ClassCatalog(void);
  
    bool bCallClassFn(eCF eFunction, Object* pObject);
    ClassFn* pFindFunction(eCN eClass, i8 iKind);
  
    void vListFunctions(void);
  
    void vSetClassFn(eCN eClass, ClassFn* aClassFN);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
