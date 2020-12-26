/*============================================================================*/
/* ClassFn.hpp -- description of a class support function                     */
/*============================================================================*/
#pragma once

#include "eCF.h"

namespace BASE
{
  class ClassFn
  {
  public:

    fpCLASSFN* m_fpInspectFn;  // array of inspect function

  public:
    ClassFn(eCN eClass, eCF eFunction, fpCLASSFN* fpClassFn, i8);
  
    bool bCallClassFn(eCF eFunction, Object* pObject);

    static bool bInspect(Object* pObject);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
