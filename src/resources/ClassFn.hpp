/*============================================================================*/
/* ClassFn.hpp -- description of a class support function                     */
/*============================================================================*/
#pragma once

#include "eCF.h"

namespace Resources
{
  class ClassFn
  {
  public:

    pCLASSFN* m_fpInspectFn;  // array of inspect function

  public:
    ClassFn(eCF eKind, pCLASSFN* fpClassFn, i8);
  
    static bool bInspect(Object* pObject);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
