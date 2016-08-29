/*============================================================================*/
/* Object.hpp -- Base Class for Objects in Buffer Pool                        */
/*============================================================================*/

#pragma once

#include "Values.h"

namespace Pool
{
  class Object : public mn,
                 public cn
  {
  private:
    
  public:
    Object(eCN eClass);
    Object(eCN eClass, pDUMPFUN* pDumpFun);
    
    void vInitMagic(void);
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
