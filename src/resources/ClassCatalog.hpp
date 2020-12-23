/*============================================================================*/
/* ClassCatalog.hpp -- the catalog of class functions                         */
/*============================================================================*/
#pragma once

namespace Resources
{
  class ClassCatalog
  {
  public:
    i8       m_nFunctions;          // number of  functions
    ClassFn* m_aaClassFn[CN_ENUMS];  // array of function descriptions

  public:
    ClassFn* pFindFunction(eCN eClass, i8 iKind);
    void vListFunctions(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
