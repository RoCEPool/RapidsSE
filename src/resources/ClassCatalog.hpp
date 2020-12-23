/*============================================================================*/
/* ClassCatalog.hpp -- the catalog of class functions                         */
/*============================================================================*/
#pragma once

namespace Resources
{
  class ClassCatalog
  {
  public:
    ClassFn* m_aaIClassFn[CN_ENUMS];  // array of inspect function

  public:
    ClassFn* pFindFunction(eCN eClass, i8 iKind);
    void vListFunctions(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
